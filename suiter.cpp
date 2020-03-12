#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

const std::map<StatType, int> ref_stat_values = {
	{ StatType::HP, 50000 },
	{ StatType::Atk, 4000 },
	{ StatType::Def, 4000 },
	{ StatType::CRate, 100 },
	{ StatType::CDmg, 100 },
	{ StatType::Spd, 150 },
	{ StatType::Acc, 150 },
	{ StatType::Res, 100 }
};

float EstimateEquipment( const ChampionStats& ch_stats, const MatchOptions& matching )
{
	float est = 0;
	est += (float)ch_stats.HP / ref_stat_values.at( StatType::HP );
	est += (float)ch_stats.Atk / ref_stat_values.at( StatType::Atk );
	est += (float)ch_stats.Def / ref_stat_values.at( StatType::Def );
	est += (float)ch_stats.CRate / ref_stat_values.at( StatType::CRate );
	est += (float)ch_stats.CDmg / ref_stat_values.at( StatType::CDmg );
	est += (float)ch_stats.Spd / ref_stat_values.at( StatType::Spd );
	est += (float)ch_stats.Acc / ref_stat_values.at( StatType::Acc );
	est += (float)ch_stats.Res / ref_stat_values.at( StatType::Res );
	return est;
}

void FindRealBestEquipment( Champion& ch, const MatchOptions& matching, Equipment& best_eq )
{
	FindBestEquipment( _MyArts, ch.BasicStats, matching, best_eq );
	ApplyEquipment( best_eq, ch, false );
}

void FindBestEquipment( const std::vector<Artefact>& inventory, const ChampionStats& basic_ch_stats, const MatchOptions& matching, Equipment& best_eq )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	for ( const Artefact& art : inventory )
	{
		_ASSERTE( art.Type != ArtType::None );
		if ( matching.IsSetAccepted( art.Set ) )
			arts_by_type[art.Type].push_back( art );
	}

	Equipment eq;
	float best_eq_estimation = 0;
	for ( const auto& at : arts_by_type )
	{
		for ( const Artefact& art : at.second )
		{
			eq[at.first] = art;

			if ( !matching.IsEqHasRequiredSets( eq ) )
				continue;

			Champion ch( basic_ch_stats );
			ApplyEquipment( eq, ch, true );

			const float est = EstimateEquipment( ch.TotalStats(), matching );
			if ( est > best_eq_estimation )
			{
				best_eq = eq;
				best_eq_estimation = est;
			}
		}
	}
}
