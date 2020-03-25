#include "pch.h"
#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "suiter.h"
#include "to_string.h"

#if !defined _DEBUG || defined DEBUG_FIND_BEST
#define RUN_FIND
#endif

/////////////////////////////////////////////////////////////////////////////

const ChampionName Champion_to_suitup = ChampionName::Gromoboy;

#define DISPLAY_BEST_POOL

/////////////////////////////////////////////////////////////////////////////

void report_stats_and_eq( const ChampionStats& final_stats, const Equipment& eq, const Champion& target_champ, std::stringstream& log )
{
	BOOST_CHECK_LE( final_stats[StatType::CRate], 100 );

	{
		ChampionExt old_ch = target_champ;
		const Equipment current_eq = GetCurrentEquipmentFor( target_champ.Name );
		old_ch.ApplyEquipment( current_eq, false, MatchOptions::ConsiderMaxLevels );
		log << stats_progress( final_stats, old_ch.TotalStats() ) << '\n';
	}

	// equipment
	log << to_string( eq ) << '\n';
}

void FindAndReportBestForChampion( const ChampionName name, const MatchOptions& matching )
{
	if ( name != Champion_to_suitup )
		return;

	std::stringstream log;
	const char* name_string = to_string(name);
	log << "\n  " << name_string << ":\n";

#ifndef DISPLAY_BEST_POOL
	ChampionExt ch = ChampionFactory::Gromoboy();
	const Equipment eq = FindRealBestEquipment( ch, matching );
#ifndef DEBUG_FIND_BEST
	BOOST_CHECK_GE( eq.Size(), 6 );
#endif

	// Report: new stats
	const ChampionStats final_stats = ch.TotalStats();
	report_stats_and_eq( final_stats, eq, ChampionFactory::Gromoboy() );
#else
	const Champion ch = Champion::ByName( ChampionName::Gromoboy );

	std::vector<Equipment> best_eq_pool;
	FindRealBestEquipment2( ch, matching, best_eq_pool );
	BOOST_CHECK( !best_eq_pool.empty() );
	BOOST_CHECK_LE( best_eq_pool.size(), EqEstPool::DefaultSize );

	log << "Top " << EqEstPool::DefaultSize << " best eq:\n";
	for ( int i = 0; i < best_eq_pool.size(); ++i )
	{
		log << '#' << (i+1) << '\n';
		const Equipment& eq = best_eq_pool[i];

		ChampionStats arts_bonus_stats;
		ApplyEquipment( eq, ch.BasicStats, arts_bonus_stats, false, matching.ConsiderMaxLevels );
		const ChampionStats final_stats = ch.TotalStats( arts_bonus_stats );

		report_stats_and_eq( final_stats, eq, ch, log );
	}
#endif

	const std::string out_string = log.str();
	BOOST_TEST_MESSAGE( out_string );

	std::ofstream file;
	file.open( std::string("__") + name_string + "__.champ" );
	if ( file.is_open() )
	{
		file << out_string;
	}
}

/////////////////////////////////////////////////////////////////////////////

/*
- Always compile and run MatchOptions instanciation to verify input data in Debug configurations
*/

BOOST_AUTO_TEST_CASE( FindBest_Gromoboy )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Moderate },
			{ StatType::Atk, MatchOptions::ArtFactor::NotInterested },
			{ StatType::Def, MatchOptions::ArtFactor::Magor },
			{ StatType::CRate, MatchOptions::ArtFactor::Minor },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
		,true
		,{ {StatType::Spd,150}, {StatType::Acc,110} }
		,{ {StatType::CRate,60}, {StatType::CDmg,100} }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Gromoboy, matching );
#endif
}
