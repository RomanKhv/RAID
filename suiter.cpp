#include "pch.h"
#include "raid.h"
#include "iterator.h"
#include "iterator2.h"
#include "stl_ext.h"
#include "profiler.h"

#define USE_TBB
#ifdef USE_TBB
struct IUnknown; // Workaround for "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-
#include "tbb/enumerable_thread_specific.h"
#include "tbb/parallel_for_each.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#endif

/////////////////////////////////////////////////////////////////////////////

//const std::map<StatType, int> ref_stat_values = {
const int ref_stat_values[ChampionStats::Count] = {
	/*StatType::HP*/ 40000,
	/*StatType::Atk*/ 3000,
	/*StatType::Def*/ 3000,
	/*StatType::CRate*/ 100,
	/*StatType::CDmg*/ 100,
	/*StatType::Spd*/ 150,
	/*StatType::Res*/ 80,
	/*StatType::Acc*/ 150,
};

bool FloatEstimationFactor( MatchOptions::ArtFactor f, float& fk )
{
	switch ( f )
	{
		case MatchOptions::ArtFactor::NotInterested:
			fk = 0;
			return false;
		case MatchOptions::ArtFactor::Minor:
			fk = 0.25f;
			return true;
		case MatchOptions::ArtFactor::Moderate:
			fk = 0.5f;
			return true;
		case MatchOptions::ArtFactor::Magor:
			fk = 0.75f;
			return true;
		case MatchOptions::ArtFactor::Max:
			fk = 1;
			return true;
	}
	return false;
}

float EstimateEquipment( const ChampionStats& ch_stats, const MatchOptions& matching )
{
	float est = 0;
	for ( StatType st : ChampionStats::TypeList )
	{
		float fk = 0;
		const MatchOptions::ArtFactor f = matching.Factors[ stl::enum_to_int(st) ];
		if ( FloatEstimationFactor( f, fk ) )
		{
			est += fk * (float)ch_stats[st] / ref_stat_values[ stl::enum_to_int(st) ];
		}
		else {
			switch ( f )
			{
				case MatchOptions::ArtFactor::MinCap:
					{
						const int min_stat_cap = matching.MinCap[ stl::enum_to_int(st) ];
						if ( ch_stats[st] < min_stat_cap )
							return 0.f;
					}
			}
		}
	}
	return est;
}

struct EqEst
{
	float _Est = 0;
	EquipmentRef _Eq;

	void join( const EqEst& rhs )
	{
		if ( rhs._Est > _Est )
		{
			_Est = rhs._Est;
			_Eq = rhs._Eq;
		}
	}
};

void ProcessCombination( const EquipmentRef& eq, const Champion& target_champ, const MatchOptions& matching, EqEst& best_combination )
{
	if ( !matching.RequiedSets.empty() &&
		!matching.IsEqHasRequiredSets( eq ) )
		return;

	ChampionStats art_bonus_stats;
	ApplyEquipment( eq, target_champ.BasicStats, art_bonus_stats, true );

	const float est = EstimateEquipment( target_champ.TotalStats( art_bonus_stats ), matching );

	if ( est > best_combination._Est )
	{
		best_combination._Est = est;
		best_combination._Eq = eq;
	}
}

void FindBestEquipment( const std::map<ArtType, std::vector<Artefact>>& arts_by_type, const Champion& target_champ, const MatchOptions& matching, Equipment& best_eq )
{
	scope_profile_time prof_time( "FindBestEquipment" );
	std::cout << arts_by_type_iterator::n_combinations(arts_by_type) << " combinations\n";

	EqEst best;

#ifdef USE_TBB
	best =
	tbb::parallel_reduce( tbb::blocked_range<size_t>( 0, arts_by_type.cbegin()->second.size() ), EqEst(), 
		[&arts_by_type, &target_champ, &matching]( const tbb::blocked_range<size_t>& r, EqEst better )
		{
			for ( size_t root_index = r.begin(); root_index < r.end(); ++root_index )
			{
				arts_by_type_iterator2 eq_i( arts_by_type, root_index );
				EquipmentRef eq;
				for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
				{
					eq_i.get( eq );
					_ASSERTE( eq.CheckValidMapping() );

					ProcessCombination( eq, target_champ, matching, better );
				}
			}
			return better;
		},
		[]( EqEst lhs, const EqEst& rhs )
		{
			lhs.join( rhs );
			return lhs;
		}
	);
#else
	arts_by_type_iterator eq_i( arts_by_type );
	EquipmentRef eq;
	for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
	{
		eq_i.get( eq );
		_ASSERTE( eq.CheckValidMapping() );

		ProcessCombination( eq, target_champ, matching, best );
	}
#endif

	for ( ArtType at : Equipment::AllTypesArr )
		best_eq[at] = best._Eq[at];
}

void SeparateInventory( const std::vector<Artefact>& inventory, const MatchOptions& matching, ChampionName ch_name,
						std::map<ArtType, std::vector<Artefact>>& arts_by_type )
{
	for ( const Artefact& art : inventory )
	{
		_ASSERTE( art.Initialized() );
		if ( matching.IsArtAccepted( art, ch_name ) )
			arts_by_type[art.Type].push_back( art );
	}
}

void FindBestEquipment( const std::vector<Artefact>& inventory, const Champion& ch, const MatchOptions& matching, Equipment& best_eq )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	SeparateInventory( inventory, matching, ch.Name, arts_by_type );
	FindBestEquipment( arts_by_type, ch, matching, best_eq );
}

Equipment FindRealBestEquipment( Champion& ch, const MatchOptions& matching )
{
	scope_profile_time prof_time( "FindRealBestEquipment" );
	Equipment best_eq;

	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	SeparateInventory( _MyArts, matching, ch.Name, arts_by_type );
	FindBestEquipment( arts_by_type, ch, matching, best_eq );

	ApplyEquipment( best_eq, ch, false );

	return best_eq;
}
