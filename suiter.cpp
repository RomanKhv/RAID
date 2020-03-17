#include "pch.h"
#include "raid.h"
#include "iterator.h"
#include "stl_ext.h"

/////////////////////////////////////////////////////////////////////////////

const std::map<StatType, int> ref_stat_values = {
	{ StatType::HP, 40000 },
	{ StatType::Atk, 3000 },
	{ StatType::Def, 3000 },
	{ StatType::CRate, 100 },
	{ StatType::CDmg, 100 },
	{ StatType::Spd, 150 },
	{ StatType::Res, 80 },
	{ StatType::Acc, 150 },
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
		const MatchOptions::ArtFactor f = stl::get_value_or( matching.Factors, st, MatchOptions::ArtFactor::NotInterested );
		if ( FloatEstimationFactor( f, fk ) )
		{
			est += fk * (float)ch_stats[st] / ref_stat_values.at( st );
		}
		else {
			switch ( f )
			{
				case MatchOptions::ArtFactor::MinCap:
					{
						_ASSERTE( stl::contains( matching.MinCap, st ) );
						const int min_stat_cap = matching.MinCap.at( st );
						if ( ch_stats[st] < min_stat_cap )
							return 0.f;
					}
			}
		}
	}
	return est;
}

Equipment FindRealBestEquipment( Champion& ch, const MatchOptions& matching )
{
	Equipment best_eq;
	FindBestEquipment( _MyArts, ch, matching, best_eq );
	ApplyEquipment( best_eq, ch, false );
	return best_eq;
}

void FindBestEquipment( const std::vector<Artefact>& inventory, const Champion& target_champ, const MatchOptions& matching, Equipment& best_eq )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	for ( const Artefact& art : inventory )
	{
		_ASSERTE( art.Type != ArtType::None );
		if ( matching.IsSetAccepted( art.Set ) )
			arts_by_type[art.Type].push_back( art );
	}

	float best_eq_estimation = 0;
	arts_by_type_iterator eq_i( arts_by_type );
	for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
	{
		const Equipment eq = eq_i.get();

		if ( !matching.IsEqHasRequiredSets( eq ) )
			continue;

		Champion ch( target_champ.BasicStats, target_champ.Elem );
		ApplyEquipment( eq, ch, true );

		const float est = EstimateEquipment( ch.TotalStats(), matching );
		if ( est > best_eq_estimation )
		{
			best_eq = eq;
			best_eq_estimation = est;
		}
	}
}
