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

const ChampionName Champion_to_suitup = ChampionName::Yuliana;

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

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Alura, matching );
#endif
}

BOOST_AUTO_TEST_CASE( FindBest_Gromoboy )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Major } },
			{ StatType::CRate,{ MatchOptions::StatInfluence::Minor, 60 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor, 100 } },
			{ StatType::Spd,  { 150 } },
			{ StatType::Acc,  { 110 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Gromoboy, matching );
#endif
	/*
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

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Kael, matching );
#endif
}

BOOST_AUTO_TEST_CASE( FindBest_Krisk )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Major } },
			{ StatType::Def, { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { MatchOptions::StatInfluence::Modrt, 60 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 140 } },
			{ StatType::Acc, { 120 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Krisk, matching );
#endif
	/*
Weapon: [Vamp]     5* (12)       { {Spd,5}, {Acc,33}, {HP,458}, }
Helmet: [Vamp]     5* (16)       { {Def_p,6}, {Acc,28}, {CRate,6}, {Res,11}, }
Shield: [Acc]      5* (16)       { {HP_p,7}, {Def_p,7}, {CDmg,15}, {CRate,16}, }
Gloves: [DivLife]  6* (12) Def_p { {HP_p,11}, {Def,27}, {Acc,10}, {CRate,18}, }
Chest:  [Vamp]     5* (16) HP_p  { {Atk_p,10}, {CRate,6}, {Def_p,5}, {HP,351}, }
Boots:  [Vamp]     5* (8)  Spd   { {Res,18}, {Acc,20}, }
Necklace:[]        5* (0)  HP    { {Def,16}, {Acc,8}, {Atk,21}, }
	*/
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

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Lekar, matching );
#endif
	/*
Weapon: [Speed]    5* (16)       { {CRate,11}, {HP_p,9}, {Spd,5}, {Res,8}, }
Helmet: [HP]       6* (16)       { {Spd,29}, {HP_p,8}, {Res,13}, {Atk_p,7}, }
Shield: [HP]       6* (12)       { {CRate,7}, {Acc,22}, {Def_p,16}, {Res,11}, }
Gloves: [Speed]    5* (8)  CRate { {Atk,21}, {Def,20}, {Spd,14}, }
Chest:  [HP]       5* (13) Def_p { {HP_p,5}, {Spd,5}, {Atk_p,6}, }
Boots:  [HP]       6* (4)  Def_p { {CRate,12}, {CDmg,6}, {HP,209}, }
Ring:   []         5* (8)  Atk   { {Def,62}, {Atk_p,6}, {Def_p,7}, }
	*/
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
			{ StatType::Spd, { 150 } },
			{ StatType::Acc, { 160 } },
		}
		,{ /*ArtSet::Vamp ArtSet::Immortal*/ }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::SteelSkull, matching );
#endif
}

BOOST_AUTO_TEST_CASE( FindBest_Tyrel )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatFactorMode::Minor } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ MatchOptions::StatInfluence::Modrt, 50 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor, 60 } },
			{ StatType::Spd,  { 150 } },
			{ StatType::Acc,  { 130 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Tyrel, matching );
#endif
	/*
Weapon: [Def]      4* (12)       { {CRate,11}, {Acc,16}, {HP_p,5}, }
Helmet: [Vamp]     5* (16)       { {Atk_p,5}, {Acc,31}, {HP_p,4}, {Def,17}, }
Shield: [Vamp]     5* (8)        { {CRate,9}, {HP,185}, }
Gloves: [Def]      5* (16) Def_p { {Spd,14}, {Acc,11}, {CRate,6}, {Res,10}, }
Chest:  [Vamp]     5* (16) Def_p { {CDmg,9}, {CRate,10}, {Res,11}, {HP,464}, }
Boots:  [Vamp]     5* (12) Spd   { {Acc,29}, {Atk,22}, {HP,137}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_VisirOvelis )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatFactorMode::Minor } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd,  { 150 } },
			{ StatType::Acc,  { 120 } },
		}
		,{ /*ArtSet::Vamp*/ ArtSet::Immortal }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::VisirOvelis, matching );
#endif
	/*
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

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Voitelnica, matching );
#endif
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
			{ StatType::Spd,  { 160 } },
			{ StatType::Acc,  { 180 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::Cruel, ArtSet::DivAtk }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Yuliana, matching );
#endif
	/*
Weapon: [Vamp]     5* (12)       { {HP_p,5}, {Atk_p,5}, {Acc,35}, }
Helmet: [Vamp]     5* (16)       { {HP_p,10}, {Acc,19}, {Spd,4}, {Def_p,5}, }
Shield: [Speed]    5* (12)       { {Acc,17}, {HP_p,10}, {Res,10}, }
Gloves: [Speed]    6* (12) CRate { {Acc,29}, {HP,301}, {Atk,50}, {Def_p,7}, }
Chest:  [Vamp]     5* (8)  Def_p { {Spd,9}, {Def,16}, }
Boots:  [Vamp]     4* (8)  Spd   { {Atk_p,4}, {Acc,25}, }
Ring:   []         5* (16) HP    { {Def_p,13}, {HP_p,7}, {Atk_p,7}, {Def,27}, }
	*/
}

BOOST_AUTO_TEST_CASE( FindBest_Zargala )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Minor } },
			{ StatType::Atk,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ 80 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd,  { 165 } },
			{ StatType::Acc,  { 120 } },
		}
		,{  }
		,{ ArtSet::Vamp, ArtSet::Def, ArtSet::HP, ArtSet::Immortal, ArtSet::DivLife }
	);
	BOOST_CHECK( matching.IsInputOK() );

#ifdef RUN_FIND
	FindAndReportBestForChampion( ChampionName::Zargala, matching );
#endif
	/*
	*/
}
