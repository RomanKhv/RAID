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

const ChampionName Champion_to_suitup = ChampionName::Gorgorab;

#define DISPLAY_BEST_POOL

#define MINOR_SETS ArtSet::Gibel,ArtSet::Mest,ArtSet::Fury,ArtSet::Curing,ArtSet::Reflex,ArtSet::Cursed,ArtSet::Toxic,ArtSet::Frost,ArtSet::Daze,ArtSet::Immunitet,ArtSet::Vozmezdie/*,ArtSet::Shield*/,ArtSet::Doblest,ArtSet::Beshenstvo,ArtSet::Regeneration,ArtSet::Svirepost,ArtSet::Savage,ArtSet::Taunting

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
	log << to_string( eq, false );
	//log << to_string( eq, true );
	log << '\n';
}

void FindAndReportBestForChampion( const ChampionName name, const MatchOptions& matching )
{
#ifdef RUN_FIND
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
#endif
}

/////////////////////////////////////////////////////////////////////////////

/*
- Always compile and run MatchOptions instanciation to verify input data in Debug configurations
*/

BOOST_AUTO_TEST_CASE( FindBest_Alura )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Atk, { MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 140 } },
			{ StatType::Acc, { 80 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::HP, ArtSet::DivLife }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Alura, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Gorgorab )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Major } },
			{ StatType::Def,  { 2200, MatchOptions::StatInfluence::Max } },
			{ StatType::Spd,  { 230, MatchOptions::StatInfluence::Max } },
		}
		,{ ArtSet::Speed, ArtSet::Speed }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Gorgorab, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Gromoboy )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ 50, MatchOptions::StatInfluence::Minor, 60 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor, 100 } },
			{ StatType::Spd,  { 160 } },
			{ StatType::Acc,  { 140 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Gromoboy, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Kael )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Atk, { MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { MatchOptions::StatInfluence::Modrt, 60 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 155 } },
			{ StatType::Acc, { 115 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Def }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Kael, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Krisk )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { 60, MatchOptions::StatInfluence::Modrt, 70 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 160 } },
			{ StatType::Acc, { 130 } },
			{ StatType::Res, { 100 } },
		}
		//,{ ArtSet::Vamp }
		,{ ArtSet::Immortal }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Krisk, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Lekar )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatFactorMode::Minor } },
			{ StatType::Def, { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { 90 } },
			{ StatType::Spd, { 170 } },
		}
		,{ ArtSet::Speed }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Lekar, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Mavzolejnik )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Major } },
			{ StatType::Atk, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Def, { 2300, MatchOptions::StatInfluence::Max } },
			//{ StatType::Res, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 180 } },
		}
		,{ ArtSet::Immortal }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Mavzolejnik, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Rotos )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Atk, { MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { 80, MatchOptions::StatInfluence::Major, 95 } },
			{ StatType::CDmg, { 90, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Acc, { 100, MatchOptions::StatInfluence::Minor } },
		}
		,{  }
		,{ ArtSet::Vamp, ArtSet::Immortal, MINOR_SETS }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Rotos, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_SteelSkull )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatFactorMode::Minor } },
			{ StatType::Def, { MatchOptions::StatInfluence::Max } },
			//{ StatType::CRate, { MatchOptions::StatFactorMode::Minor } },
			//{ StatType::CDmg, { MatchOptions::StatFactorMode::Minor } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Acc, { 180 } },
		}
		,{ ArtSet::Immortal, ArtSet::Immortal }
		//,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::SteelSkull, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Tyrel )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ MatchOptions::StatInfluence::Modrt, 50 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor, 60 } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Acc, { 180 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Tyrel, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_VisirOvelis )
{
	const MatchOptions matching(
		//{
		//	{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
		//	//{ StatType::Atk, { MatchOptions::StatFactorMode::Minor } },
		//	{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
		//	{ StatType::CRate, { MatchOptions::StatInfluence::Minor } },
		//	{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
		//	{ StatType::Spd,  { 150 } },
		//	{ StatType::Acc,  { 120 } },
		//}
		//,{ ArtSet::Immortal }
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatFactorMode::Minor } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::Spd,  { 160 } },
			{ StatType::Acc,  { 180 } },
		}
		,{ ArtSet::Immortal, ArtSet::Immortal }
		//,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, MINOR_SETS }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::VisirOvelis, matching );
	/*
Weapon: [Immortal] 5* (16)       { {CRate,10}, {Acc,26}, {HP_p,7}, {HP,864}, }
Helmet: [Immortal] 5* (16)       { {Spd,9}, {Res,10}, {Acc,29}, {HP_p,10}, }
Shield: [Immortal] 5* (8)        { {Def_p,15}, {Spd,4}, }
Gloves: [HP]       6* (16) Def_p { {Atk_p,6}, {Acc,42}, {HP,518}, {CDmg,12}, }
Chest:  [HP]       5* (16) Def_p { {Atk_p,11}, {CRate,11}, {CDmg,10}, {Res,10}, }
Boots:  [Immortal] 5* (16) Spd   { {Acc,18}, {HP_p,10}, {CRate,11}, {Atk,37}, }
Ring:   []         5* (12) HP    { {HP_p,6}, {Def_p,14}, {Atk,22}, }
Necklace:[]        5* (0)  Atk   { {Acc,9}, }
Banner: []         5* (8)  Acc   { {Def,43}, {HP_p,12}, {Spd,6}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_Voitelnica )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Minor } },
			{ StatType::Atk, { MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { MatchOptions::StatInfluence::Modrt, 70 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd,  { 160 } },
			{ StatType::Acc,  { 90 } },
		}
		,{  }
		,{ ArtSet::Vamp, ArtSet::Def, ArtSet::HP, ArtSet::Immortal }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Voitelnica, matching );
	/*
	Artefact{ ArtType::Weapon, ArtSet::CRate, 5, 16, StatType::Atk, { {StatType::Acc,20,2}, {StatType::CRate,17}, {StatType::Res,18,1}, {StatType::CDmg,5} }, ChampionName::Voitelnica },
	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 16, StatType::HP, { {StatType::Atk_p,10,2}, {StatType::CDmg,11}, {StatType::HP_p,5,1}, {StatType::CRate,5} }, ChampionName::Voitelnica },
	Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 16, StatType::Def, { {StatType::Acc,20,1}, {StatType::Spd,9,1}, {StatType::Def_p,5,1}, {StatType::CDmg,6} }, ChampionName::Voitelnica ),
	Artefact{ ArtType::Gloves, ArtSet::Acc, 6, 16, StatType::Atk_p, { {StatType::CRate,20}, {StatType::Spd,5,2}, {StatType::HP_p,5,1}, {StatType::Def_p,7,1} }, ChampionName::Voitelnica },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 16, StatType::Atk_p, { {StatType::Def_p,5,1}, {StatType::CDmg,15}, {StatType::Acc,10,2}, {StatType::Res,9} }, ChampionName::Voitelnica },
	Artefact( ArtType::Boots, ArtSet::DivSpeed, 5, 16, StatType::Spd, { {StatType::Atk_p,14,2}, {StatType::Atk,15,12}, {StatType::CRate,9} }, ChampionName::Voitelnica ),
1>Weapon: [CRate]    6* (16)       { {Acc,22}, {CRate,17}, {Res,19}, {CDmg,5}, }
1>Helmet: [CRate]    5* (12)       { {Atk_p,10}, {CDmg,11}, {HP_p,5}, }
1>Shield: [DivSpeed] 5* (12)       { {Acc,20}, {Spd,9}, {Def_p,5}, }
1>Gloves: [Acc]      6* (12) Atk_p { {CRate,20}, {Spd,5}, {HP_p,5}, }
1>Chest:  [Acc]      5* (12) Atk_p { {Def_p,5}, {CDmg,15}, {Acc,10}, }
1>Boots:  [DivSpeed] 5* (12) Spd   { {Atk_p,14}, {Atk,20}, {CRate,9}, }
1>Ring:   []         5* (8)  HP    { {Atk_p,16}, {HP_p,5}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_Yuliana )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Minor } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ 80 } },
			{ StatType::Spd,  { 165 } },
			{ StatType::Acc,  { 180 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::Cruel, ArtSet::DivAtk }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Yuliana, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Zargala )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Minor } },
			{ StatType::Atk,  { MatchOptions::StatInfluence::Max } },
			//{ StatType::Def,  { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { 1800 } },
			{ StatType::CRate,{ 80 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd,  { 165 } },
			{ StatType::Acc,  { 120 } },
		}
		,{  }
		,{ ArtSet::Vamp, ArtSet::Def, ArtSet::HP, ArtSet::Immortal, ArtSet::DivLife }
	);
	BOOST_CHECK( matching.IsInputOK() );

	FindAndReportBestForChampion( ChampionName::Zargala, matching );
}
