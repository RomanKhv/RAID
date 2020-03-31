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

const ChampionName Champion_to_suitup = ChampionName::Kael;

#define DISPLAY_BEST_POOL

/////////////////////////////////////////////////////////////////////////////

void report_stats_and_eq( const ChampionStats& final_stats, const Equipment& eq, const Champion& target_champ, std::stringstream& log )
{
	BOOST_CHECK_LE( final_stats[StatType::CRate], 100 );

	{
		ChampionExt old_ch = target_champ;
		const Equipment current_eq = GetCurrentEquipmentFor( target_champ.Name );
		old_ch.ApplyEquipment( current_eq, false, MatchOptions::ConsiderMaxLevels );
		const ChampionStats old_stats = old_ch.TotalStats();
		log << stats_progress( final_stats, old_stats ) << '\n';
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
	const Champion ch = Champion::ByName( name );

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
			{ StatType::Def, MatchOptions::ArtFactor::Major },
			{ StatType::CRate, MatchOptions::ArtFactor::Minor },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
		,{ {StatType::Spd,150}, {StatType::Acc,110} }
		,{ {StatType::CRate,60}, {StatType::CDmg,100} }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Gromoboy, matching );
#endif
	/*
HP:    31005 (-2906)
Atk:   1031 (+84)
Def:   3524 (+81)
Spd:   152 (+1)
CRate: 63 (+6)
CDmg:  58 (-21)
Res:   35 (-63)
Acc:   110 (+13)
Weapon: [Vamp]     5* (16)       { {CDmg,6}, {CRate,15}, {Atk_p,5}, {Acc,9}, }
Helmet: [Acc]      5* (12)       { {Spd,9}, {CRate,10}, }
Shield: [Vamp]     6* (12)       { {Acc,11}, {CRate,19}, {HP,423}, }
Gloves: [Vamp]     5* (8)  Def_p { {Def,21}, {HP_p,17}, }
Chest:  [DivLife]  6* (16) Def_p { {Spd,6}, {Def,29}, {HP_p,14}, {Acc,41}, }
Boots:  [Vamp]     5* (16) Spd   { {Acc,19}, {HP_p,10}, {CRate,4}, {Atk_p,4}, }
Ring:   []         5* (12) HP    { {Def,51}, {Def_p,6}, {HP_p,6}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_Kael )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Moderate },
			{ StatType::Atk, MatchOptions::ArtFactor::Max },
			{ StatType::Def, MatchOptions::ArtFactor::Moderate },
			{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Def }
		,{ {StatType::Spd,155}, {StatType::Acc,115} }
		,{ {StatType::CRate,60}/*, {StatType::CDmg,100}*/ }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Kael, matching );
#endif
}

BOOST_AUTO_TEST_CASE( FindBest_Krisk )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Major },
			{ StatType::Def, MatchOptions::ArtFactor::Max },
			{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
		,{ {StatType::Spd,140}, {StatType::Acc,120} }
		,{ {StatType::CRate,60}/*, {StatType::CDmg,100}*/ }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Krisk, matching );
#endif
	/*
HP:    40050 (+9241)
Atk:   1071 (+55)
Def:   2958 (-47)
Spd:   139 (0)
CRate: 61 (-14)
CDmg:  67 (-11)
Res:   64 (-5)
Acc:   129 (+19)
Weapon: [Vamp]     5* (12)       { {Spd,5}, {Acc,33}, {HP,458}, }
Helmet: [Vamp]     5* (16)       { {Def_p,6}, {Acc,28}, {CRate,6}, {Res,11}, }
Shield: [Acc]      5* (16)       { {HP_p,7}, {Def_p,7}, {CDmg,15}, {CRate,16}, }
Gloves: [DivLife]  6* (12) Def_p { {HP_p,11}, {Def,27}, {Acc,10}, {CRate,18}, }
Chest:  [Vamp]     5* (16) HP_p  { {Atk_p,10}, {CRate,6}, {Def_p,5}, {HP,351}, }
Boots:  [Vamp]     5* (8)  Spd   { {Res,18}, {Acc,20}, }
Necklace:[]        5* (0)  HP    { {Def,16}, {Acc,8}, {Atk,21}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_VisirOvelis )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Moderate },
			//{ StatType::Atk, MatchOptions::ArtFactor::Minor },
			{ StatType::Def, MatchOptions::ArtFactor::Max },
			{ StatType::CRate, MatchOptions::ArtFactor::Minor },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
		}
		,{ /*ArtSet::Vamp*/ ArtSet::Immortal }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
		,{ {StatType::Spd,150}, {StatType::Acc,120} }
		//,{ {StatType::CRate,60}, {StatType::CDmg,100} }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::VisirOvelis, matching );
#endif
	/*
HP:    26935 (+11375)
Atk:   1766 (+421)
Def:   2553 (+1571)
Spd:   153 (+52)
CRate: 36 (+21)
CDmg:  73 (+21)
Res:   57 (+27)
Acc:   134 (+124)
Weapon: [Immortal] 5* (12)       { {CRate,10}, {Acc,18}, {HP_p,5}, {HP,689}, }
Helmet: [Immortal] 5* (12)       { {Acc,35}, {CDmg,5}, {Def_p,5}, }
Shield: [Speed]    5* (12)       { {HP_p,10}, {Acc,10}, {Def_p,16}, }
Gloves: [HP]       6* (12) Def_p { {Atk_p,5}, {Acc,42}, {HP,343}, {CDmg,6}, }
Chest:  [HP]       5* (16) Def_p { {Atk_p,10}, {CRate,11}, {CDmg,10}, {Res,8}, }
Boots:  [Speed]    5* (12) Spd   { {Def_p,9}, {Acc,19}, {Res,19}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_Voitelnica )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Minor },
			{ StatType::Atk, MatchOptions::ArtFactor::Max },
			{ StatType::Def, MatchOptions::ArtFactor::Minor },
			//{ StatType::Spd, MatchOptions::ArtFactor::Max },
			{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
			{ StatType::CDmg, MatchOptions::ArtFactor::Moderate },
			//{ StatType::Acc, MatchOptions::ArtFactor::Major },
		}
		,{  }
		,{ ArtSet::Vamp, ArtSet::Def, ArtSet::HP, ArtSet::Immortal }
		,{ {StatType::Spd,160}, {StatType::Acc,90} }
		,{ {StatType::CRate,70}/*, {StatType::CDmg,90}*/ }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Voitelnica, matching );
#endif
	/*
1>HP:    19174 (+411)
1>Atk:   2568 (+350)
1>Def:   821 (-138)
1>Spd:   162 (+3)
1>CRate: 73 (+9)
1>CDmg:  83 (+23)
1>Res:   49 (-37)
1>Acc:   102 (+17)
1>Weapon: [CRate]    6* (16)       { {Acc,22}, {CRate,17}, {Res,19}, {CDmg,5}, }
1>Helmet: [CRate]    5* (12)       { {Atk_p,10}, {CDmg,11}, {HP_p,5}, }
1>Shield: [DivSpeed] 5* (12)       { {Acc,20}, {Spd,9}, {Def_p,5}, }
1>Gloves: [Acc]      6* (12) Atk_p { {CRate,20}, {Spd,5}, {HP_p,5}, }
1>Chest:  [Acc]      5* (12) Atk_p { {Def_p,5}, {CDmg,15}, {Acc,10}, }

1>Boots:  [DivSpeed] 5* (12) Spd   { {Atk_p,14}, {Atk,20}, {CRate,9}, }
1>Ring:   []         5* (8)  HP    { {Atk_p,16}, {HP_p,5}, }
	*/
}
