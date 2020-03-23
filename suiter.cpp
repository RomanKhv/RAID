#include "pch.h"
#include "suiter.h"
#include "iterator.h"
#include "iterator2.h"
#include "stl_ext.h"
#include "profiler.h"
#include "to_string.h"

#ifndef DEBUG_FIND_BEST
	#define USE_TBB
#endif
#ifdef USE_TBB
struct IUnknown; // Workaround for "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#endif

/////////////////////////////////////////////////////////////////////////////

const ChampionStats::values_t Ref_Stat_Values = {
	/*StatType::HP*/ 40000,
	/*StatType::Atk*/ 3500,
	/*StatType::Def*/ 3500,
	/*StatType::Spd*/ 150,
	/*StatType::CRate*/ 100,
	/*StatType::CDmg*/ 150,
	/*StatType::Res*/ 80,
	/*StatType::Acc*/ 150,
};

const ChampionStats::values_t Max_Stat_Caps = {
	/*StatType::HP*/   0,
	/*StatType::Atk*/  3500,
	/*StatType::Def*/  3500,
	/*StatType::Spd*/  0,
	/*StatType::CRate*/ 100,
	/*StatType::CDmg*/ 0,
	/*StatType::Res*/  0,
	/*StatType::Acc*/  0,
};

const ChampionStats::values_t Stat_Tolerance = {
	/*StatType::HP*/   0,
	/*StatType::Atk*/  400,
	/*StatType::Def*/  400,
	/*StatType::Spd*/  10,
	/*StatType::CRate*/ 10,
	/*StatType::CDmg*/ 20,
	/*StatType::Res*/  0,
	/*StatType::Acc*/  0,
};

const float fK_ignore = 0;
const float fK_minor = 0.25f;
const float fK_moderate = 0.5f;
const float fK_magor = 0.75f;
const float fK_max = 1;

inline float FactorK( MatchOptions::ArtFactor f )
{
	static const float fks[5] = {
		fK_ignore, fK_minor, fK_moderate, fK_magor, fK_max
	};
	return fks[ stl::enum_to_int(f) ];
}

bool FloatEstimationFactor( MatchOptions::ArtFactor f, float& fk )
{
	if ( stl::enum_to_int(f) <= 4 )
	{
		fk = FactorK( f );
		return true;
	}
	else
		return false;
}

float linerp( const int x, const int x1, const float y1, const int x2, const float y2 )
{
	const float a = float(y2 - y1) / (x2 - x1);
	const float b = float(x2*y1 - x1*y2) / (x2 - x1);
	return a * x + b;
}

bool EstimateMinCap( const int value, const int ref_value, const int width, float& e )
{
	if ( value <= (ref_value - width/2) )
	{
		e = fK_ignore;
		return false;
	}
	if ( value < ref_value )
		e = linerp( value, ref_value - width / 2, fK_ignore, ref_value, fK_max );
	else
	if ( value < (ref_value + width) )
		e = fK_max;
	else
	if ( value < (ref_value + 3*width/2) )
		e = linerp( value, ref_value+width, fK_max, ref_value+3*width/2, fK_moderate );
	else
		e = fK_moderate;
	return true;
}

int GetActualMaxCap( StatType st, const MatchOptions& matching )
{
	const int global_max_stat_cap = Max_Stat_Caps[stl::enum_to_int( st )];
	const int champ_max_stat_cap = matching.MaxCap[stl::enum_to_int( st )];
	if ( champ_max_stat_cap || global_max_stat_cap )
	{
		if ( champ_max_stat_cap && global_max_stat_cap )
		{
			_ASSERTE( champ_max_stat_cap < global_max_stat_cap );
			return std::min( champ_max_stat_cap, global_max_stat_cap );
		}
		else {
			if ( champ_max_stat_cap )
			{
				_ASSERTE( !global_max_stat_cap );
				return champ_max_stat_cap;
			}
			else {
				return global_max_stat_cap;
			}
		}
	}
	else
		return 0;
}

float EstimateEquipment( const ChampionStats& ch_stats, const MatchOptions& matching )
{
	float est = 0;

	for ( StatType st : ChampionStats::TypeList )
	{
		const int min_stat_cap = matching.MinCap[ stl::enum_to_int(st) ];
		if ( min_stat_cap <= 0 )
			continue;	//not requested
		_ASSERTE( matching.Factor(st) == MatchOptions::ArtFactor::MinCap );

		float e = 0;
		if ( !EstimateMinCap( ch_stats[st], min_stat_cap, 20, e ) )
			return 0.f;	//too small => not accepted
		est += e;
	}

	for ( StatType st : ChampionStats::TypeList )
	{
		const MatchOptions::ArtFactor f = matching.Factor( st );
		float fk = 0;
		if ( FloatEstimationFactor( f, fk ) )
		{
			const int stat_value = ch_stats[st];
			const int ref_stat_value = Ref_Stat_Values[ stl::enum_to_int(st) ];
			const int max_stat_cap   = GetActualMaxCap( st, matching );

			float e = 0;
			if ( max_stat_cap && stat_value > max_stat_cap )
			{
				const int tol = Stat_Tolerance[stl::enum_to_int( st )];
				const int excessive_threshold = ( tol > 0 ) ?
					max_stat_cap + tol :
					Max_Stat_Caps[ stl::enum_to_int(st) ] * 110 / 100;
				if ( stat_value > excessive_threshold )
					continue;	//stat got too high, give 0 estimation (or "return 0;" ?)
				else {
					e = linerp( stat_value, max_stat_cap, 1, excessive_threshold, 0 );
				}
			}
			else
				e = (float)stat_value / ref_stat_value;

			est += fk * e;
		}
		else {
			switch ( f )
			{
				case MatchOptions::ArtFactor::MinCap:
					{
						_ASSERTE( matching.MinCap[ stl::enum_to_int(st) ] > 0 );
					}
			}
		}
	}
	return est;
}

/////////////////////////////////////////////////////////////////////////////

EqEstPool::EqEstPool()
	:_Arr( Size )
{
}

void EqEstPool::join( const EqEst& rhs )
{
	_ASSERTE( _Arr.capacity() == Size );
	_ASSERTE( rhs.Initialized() );

	bool inserted = false;
	for ( auto i = _Arr.begin(); i != _Arr.end(); ++i )
	{
		if ( rhs._Est > i->_Est )
		{
			if ( _Arr.full() )
				_Arr.pop_back();
			_Arr.insert( i, rhs );
			inserted = true;
			break;
		}
	}
	if ( !inserted && !_Arr.full() )
		_Arr.push_back( rhs );
}

void EqEstPool::join( const EqEstPool& rhs )
{
	_ASSERTE( _Arr.capacity() == Size );

	for ( const EqEst& est : rhs._Arr )
		join( est );
}

/////////////////////////////////////////////////////////////////////////////

bool ProcessCombination( const EquipmentRef& eq, const Champion& target_champ, const MatchOptions& matching, EqEst& best_combination )
{
	if ( !matching.RequiedSets.empty() &&
		!matching.IsEqHasRequiredSets( eq ) )
		return false;

	ChampionStats arts_bonus_stats;
	ApplyEquipment( eq, target_champ.BasicStats, arts_bonus_stats, true, matching.ConsiderMaxLevels );

	const float est = EstimateEquipment( target_champ.TotalStats( arts_bonus_stats ), matching );

	if ( est > best_combination._Est )
	{
		best_combination._Est = est;
		best_combination._Eq = eq;

#ifdef DEBUG_FIND_BEST
		//std::cout << "\nBetter: (" << est << ")\n" << to_string( eq ) << "\n";
		//std::cout << "\nBetter: (" << est << ")\n" << to_string( arts_bonus_stats ) << "\n";
#endif
		return true;
	}
	return false;
}

void FindBestEquipment( const std::map<ArtType, std::vector<Artefact>>& arts_by_type, const Champion& target_champ, const MatchOptions& matching, EqEst& best )
{
	std::cout << arts_by_type_iterator::n_combinations(arts_by_type) << " combinations\n";

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
			lhs._join( rhs );
			return lhs;
		}
	);
#else
#ifdef NDEBUG
#error TBB is disabled
#endif
	arts_by_type_iterator eq_i( arts_by_type );
	EquipmentRef eq;
	for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
	{
		eq_i.get( eq );
		_ASSERTE( eq.CheckValidMapping() );

		ProcessCombination( eq, target_champ, matching, best );
	}
#endif
}

void FindBestEquipment2( const std::map<ArtType, std::vector<Artefact>>& arts_by_type, const Champion& target_champ, const MatchOptions& matching, EqEstPool& best )
{
	std::cout << arts_by_type_iterator::n_combinations(arts_by_type) << " combinations\n";

#ifdef USE_TBB
	best =
	tbb::parallel_reduce( tbb::blocked_range<size_t>( 0, arts_by_type.cbegin()->second.size() ), EqEstPool(),
		[&arts_by_type, &target_champ, &matching]( const tbb::blocked_range<size_t>& r, EqEstPool better )
		{
			for ( size_t root_index = r.begin(); root_index < r.end(); ++root_index )
			{
				arts_by_type_iterator2 eq_i( arts_by_type, root_index );
				EquipmentRef eq;
				for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
				{
					eq_i.get( eq );
					_ASSERTE( eq.CheckValidMapping() );

					EqEst est;
					if ( ProcessCombination( eq, target_champ, matching, est ) )
						better.join( est );
				}
			}
			return better;
		},
		[]( EqEstPool lhs, const EqEstPool& rhs )
		{
			lhs.join( rhs );
			return lhs;
		}
	);
#else
#ifdef NDEBUG
#error TBB is disabled
#endif
	arts_by_type_iterator eq_i( arts_by_type );
	EquipmentRef eq;
	for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
	{
		eq_i.get( eq );
		_ASSERTE( eq.CheckValidMapping() );

		EqEst est;
		if ( ProcessCombination( eq, target_champ, matching, est ) )
			best.join( est );
	}
#endif
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

	EqEst best;
	FindBestEquipment( arts_by_type, ch, matching, best );

	best_eq = best._Eq.as_basic();
}

void FindBestEquipment2( const std::vector<Artefact>& inventory, const Champion& ch, const MatchOptions& matching, std::vector<Equipment>& eqs )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	SeparateInventory( inventory, matching, ch.Name, arts_by_type );

	EqEstPool best;
	FindBestEquipment2( arts_by_type, ch, matching, best );

	for ( const EqEst& rhs : best._Arr )
	{
		eqs.emplace_back( rhs._Eq.as_basic() );
	}
}

Equipment FindRealBestEquipment( ChampionExt& ch, const MatchOptions& matching )
{
	scope_profile_time prof_time( "FindRealBestEquipment" );

	Equipment best_eq;
	FindBestEquipment( _MyArts, ch, matching, best_eq );

	ApplyEquipment( best_eq, ch.BasicStats, ch.ArtsBonusStats, false, matching.ConsiderMaxLevels );

	return best_eq;
}

void FindRealBestEquipment2( const Champion& ch, const MatchOptions& matching, std::vector<Equipment>& eqs )
{
	scope_profile_time prof_time( "FindRealBestEquipment" );

	FindBestEquipment2( _MyArts, ch, matching, eqs );
}
