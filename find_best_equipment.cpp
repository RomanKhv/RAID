#include "pch.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "suiter.h"
#include "to_string.h"

#if !defined _DEBUG || defined DEBUG_FIND_BEST
#define RUN_FIND
#endif

#define DISPLAY_BEST_POOL

/////////////////////////////////////////////////////////////////////////////

void report_stats_and_eq( const ChampionStats& final_stats, const Equipment& eq, const Champion& target_champ )
{
	BOOST_CHECK_LE( final_stats[StatType::CRate], 100 );

	{
		ChampionExt old_ch = target_champ;
		const Equipment current_eq = GetCurrentEquipmentFor( target_champ.Name );
		old_ch.ApplyEquipment( current_eq, false, MatchOptions::ConsiderMaxLevels );
		BOOST_TEST_MESSAGE( stats_progress( final_stats, old_ch.TotalStats() ) );
	}

	// equipment
	BOOST_TEST_MESSAGE( to_string( eq ) );
}

void FindAndReportBestForChampion( const ChampionName name, const MatchOptions& matching )
{
	BOOST_TEST_MESSAGE( "\n  Gromoboy:" );

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
	const Champion ch = ChampionFactory::Gromoboy();

	std::vector<Equipment> best_eq_pool;
	FindRealBestEquipment2( ch, matching, best_eq_pool );
	BOOST_CHECK( !best_eq_pool.empty() );
	BOOST_CHECK_LE( best_eq_pool.size(), EqEstPool::DefaultSize );

	BOOST_TEST_MESSAGE( "Top " << EqEstPool::DefaultSize << " best eq:" );
	for ( int i = 0; i < best_eq_pool.size(); ++i )
	{
		BOOST_TEST_MESSAGE( "#" << (i+1) );
		const Equipment& eq = best_eq_pool[i];

		ChampionStats arts_bonus_stats;
		ApplyEquipment( eq, ch.BasicStats, arts_bonus_stats, false, matching.ConsiderMaxLevels );
		const ChampionStats final_stats = ch.TotalStats( arts_bonus_stats );

		report_stats_and_eq( final_stats, eq, ch );
	}
#endif
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
