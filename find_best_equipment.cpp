#include "pch.h"
#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "suiter.h"
#include "to_string.h"

#if !defined _DEBUG || defined DEBUG_FIND_BEST
#define RUN_FIND
#endif

/////////////////////////////////////////////////////////////////////////////

extern const ChampionName Champion_to_suitup = ChampionName::Gala;

#define DISPLAY_BEST_POOL

#define MINOR_SETS ArtSet::Gibel,ArtSet::Mest,ArtSet::Fury,ArtSet::Curing,ArtSet::Reflex,ArtSet::Cursed,ArtSet::Toxic,ArtSet::Frost,ArtSet::Daze,ArtSet::Immunitet,ArtSet::Vozmezdie/*,ArtSet::Shield*/,ArtSet::Doblest,ArtSet::Beshenstvo,ArtSet::Regeneration,ArtSet::Svirepost,ArtSet::Savage,ArtSet::Taunting

#define MINOR_CHAMPIONS ChampionName::Gala, ChampionName::Sohaty

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
	ChampionExt ch = ChampionFactory::();
	const Equipment eq = FindRealBestEquipment( ch, matching );
#ifndef DEBUG_FIND_BEST
	BOOST_CHECK_GE( eq.Size(), 6 );
#endif

	// Report: new stats
	const ChampionStats final_stats = ch.TotalStats();
	report_stats_and_eq( final_stats, eq, ChampionFactory::() );
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

	boost::filesystem::path file_name = std::string("__") + name_string + "__.champ";
	std::ofstream file;
	file.open( file_name.c_str() );
	if ( file.is_open() )
	{
		file << out_string;
	}
	BOOST_TEST_MESSAGE( (boost::format("%s\n") % (boost::filesystem::current_path()/file_name).string()).str() );
#endif
}

/////////////////////////////////////////////////////////////////////////////

/*
- Always compile and run MatchOptions instantiation to verify input data in Debug configurations
*/

BOOST_AUTO_TEST_CASE( FindBest_Alura )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 30000, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Atk, { 2000, MatchOptions::StatInfluence::Major, 3000 } },
			{ StatType::Def, { 1800, MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { 80, MatchOptions::StatInfluence::Minor, 95 } },
			//{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 170 } },
			{ StatType::Acc, { 190 } },
		}
		,{ ArtSet::Vamp }
	);
	matching.AllowSets( { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::Zhivuchest } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Alura, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Arbitr )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 50000, MatchOptions::StatInfluence::Minor } },
			//{ StatType::Atk, { 2500, MatchOptions::StatInfluence::Major, 3000 } },
			{ StatType::Def, { 2700, MatchOptions::StatInfluence::Minor } },
			//{ StatType::CRate, { 80, MatchOptions::StatInfluence::Minor, 95 } },
			//{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 285, MatchOptions::StatInfluence::Max } },
			{ StatType::Acc, { 170 } },
		}
		,{  }
		,{  }
		,{ ChampionName::Hatun, ChampionName::Lekar/*, ChampionName::Mavzolejnik*/, ChampionName::Hangar, MINOR_CHAMPIONS }
	);
	matching.AllowSets( { ArtSet::HP, ArtSet::DivLife, ArtSet::Immortal, ArtSet::Def, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::Zhivuchest } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Arbitr, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_BlackKnight )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Major } },
			{ StatType::Atk, { MatchOptions::StatInfluence::Major } },
			{ StatType::Def, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { 60, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 140 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::DivLife, MINOR_SETS }
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::BlackKnight, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_ColdHeart )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 32000, MatchOptions::StatInfluence::Major } },
			{ StatType::Atk, { 2500, MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { 1600, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { 70, MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 170 } },
			{ StatType::Acc, { 190 } },
		}
		,{ ArtSet::Vamp }
	);
	matching.AllowSets( { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::Zhivuchest } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::ColdHeart, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Fein )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 30000, MatchOptions::StatInfluence::Major } },
			{ StatType::Def, { 2800, MatchOptions::StatInfluence::Max } },
			{ StatType::Spd, { 180 } },
			{ StatType::Acc, { 220 } },
		}
		,{ ArtSet::Vamp }
	);
	matching.AllowSets( { ArtSet::HP, ArtSet::DivLife, ArtSet::Immortal, ArtSet::Def, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::Zhivuchest } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Fein, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Foly )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 32000, MatchOptions::StatInfluence::Major } },
			{ StatType::Atk, { 3100, MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { 1900, MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { 90, MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { 180, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd, { 170 } },
			{ StatType::Acc, { 190 } },
		}
		,{ ArtSet::Vamp }
	);
	matching.AllowSets( { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::CritDmg, ArtSet::Zhivuchest } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Foly, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Gala )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 32000, MatchOptions::StatInfluence::Minor } },
			{ StatType::Atk, { 3000, MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { 1900, MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { 85, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CDmg, { 130, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd, { 165 } },
		}
	);
	matching.AllowSets( { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, /*ArtSet::Speed, ArtSet::DivSpeed,*/ ArtSet::CRate, ArtSet::DivCritRate, ArtSet::CritDmg, ArtSet::Immortal } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Gala, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Gorgorab )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { 31000, MatchOptions::StatInfluence::Major } },
			{ StatType::Def,  { 2800, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd,  { 260, MatchOptions::StatInfluence::Max } },
		}
		,{ ArtSet::Speed, ArtSet::Speed }
		,{ ArtSet::Atk, ArtSet::CRate, ArtSet::CritDmg, ArtSet::Cruel, ArtSet::DivAtk, ArtSet::DivCritRate, MINOR_SETS }
		,{ ChampionName::Hatun, ChampionName::Lekar, ChampionName::Mavzolejnik, MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Gorgorab, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_GornyKorol )
{
	MatchOptions matching(
		{
			{ StatType::HP,   { 60000, MatchOptions::StatInfluence::Max } },
			{ StatType::Atk,  { 2500, MatchOptions::StatInfluence::Major } },
			{ StatType::Def,  { 2000, MatchOptions::StatInfluence::Minor } },
			//{ StatType::CRate,  { 70, MatchOptions::StatInfluence::Minor } },
			//{ StatType::CDmg,  { 100, MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd,  { MatchOptions::StatInfluence::Modrt } },
		}
		,{ ArtSet::Shield, ArtSet::Immortal }
		,{}
		,{ ChampionName::Hangar, MINOR_CHAMPIONS }
	);
	matching.AllowSets( {} );
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::GornyKorol, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Gromoboy )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ 60, MatchOptions::StatInfluence::Minor, 75 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor, 100 } },
			{ StatType::Spd,  { 160 } },
			{ StatType::Acc,  { 160 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, MINOR_SETS }
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Gromoboy, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Guard )
{
	MatchOptions matching(
		{
			{ StatType::HP,   { 32000, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { 1700, MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { 80, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CDmg,  { 120, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd,  { 170, MatchOptions::StatInfluence::Minor } },
			{ StatType::Acc,  { 200, MatchOptions::StatInfluence::Minor } },
		}
		,{ /*ArtSet::Vamp*/ }
		,{}
		,{ MINOR_CHAMPIONS }
	);
	matching.AllowSets( { ArtSet::Cruel, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::CritDmg, ArtSet::Zhivuchest, ArtSet::Immortal } );
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Guard, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Hangar )
{
	MatchOptions matching(
		{
			{ StatType::HP,   { 60000, MatchOptions::StatInfluence::Max } },
			{ StatType::Def,  { 2000, MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { 70, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CDmg,  { 100, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd,  { 150, MatchOptions::StatInfluence::Minor } },
			{ StatType::Acc,  { 150, MatchOptions::StatInfluence::Minor } },
		}
		,{ ArtSet::Shield, ArtSet::Immortal }
		,{}
		,{ MINOR_CHAMPIONS }
	);
	matching.AllowSets( {} );
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Hangar, matching );
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
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Kael, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Killian )
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
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Killian, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Krisk )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { 50000, MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { 3000, MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { 60, MatchOptions::StatInfluence::Modrt, 80 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 160 } },
			{ StatType::Acc, { 130 } },
			//{ StatType::Res, { 100 } },
		}
		,{ ArtSet::Vamp }
		//,{ ArtSet::Immortal }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Krisk, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Lekar )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 32000, MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Def, { 2800, MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { 85, MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 200, MatchOptions::StatInfluence::Modrt } },
		}
		,{ /*ArtSet::Speed*/ }
		,{  }
		,{ MINOR_CHAMPIONS }
	);
	matching.AllowSets( { ArtSet::HP, ArtSet::DivLife, ArtSet::Immortal, ArtSet::Def, ArtSet::Zhivuchest, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Rastoropnost, ArtSet::CRate } );
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Lekar, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Mashalled )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 30000, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Atk, { 3000, MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { 2000, MatchOptions::StatInfluence::Minor } },
			{ StatType::CRate, { 80, MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { 100, MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 170 } },
			{ StatType::Acc, { 200 } },
		}
		,{  }
		,{  }
		,{ MINOR_CHAMPIONS }
	);
	matching.AllowSets( { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::CritDmg, ArtSet::Rastoropnost } );
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Mashalled, matching );
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
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Mavzolejnik, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Razen )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def, { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate, { 60, MatchOptions::StatInfluence::Major, 80 } },
			{ StatType::CDmg, { 90, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Acc, { 170, MatchOptions::StatInfluence::Minor } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, MINOR_SETS }
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Razen, matching );
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
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Rotos, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_SteelSkull )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Def, { MatchOptions::StatInfluence::Max } },
			//{ StatType::CRate, { MatchOptions::StatInfluence::Minor } },
			//{ StatType::CDmg, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Acc, { 180 } },
		}
		//,{ ArtSet::Immortal, ArtSet::Immortal }
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::DivLife }
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::SteelSkull, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Sohaty )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 33000, MatchOptions::StatInfluence::Minor } },
			{ StatType::Atk, { 2200, MatchOptions::StatInfluence::Major } },
			{ StatType::Def, { 2000, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { 80, MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { 110, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 170, MatchOptions::StatInfluence::Minor } },
			{ StatType::Acc, { 190 } },
		}
		,{ ArtSet::Vamp }
	);
	matching.AllowSets( { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::CritDmg, ArtSet::Speed, ArtSet::Acc, ArtSet::Rastoropnost } );
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Sohaty, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Straholud_no_accuracy )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { MatchOptions::StatInfluence::Max } },
			{ StatType::Def, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { 90, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CDmg, { 100, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Acc, { 190 } },
		}
		,{ ArtSet::Immortal }
	);
	matching.AllowSets( { ArtSet::HP, ArtSet::Immortal, ArtSet::DivLife, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::CritDmg/*, ArtSet::Acc, ArtSet::Rastoropnost*/ } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Straholud, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_Tyrel )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def,  { 3000, MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ 60, MatchOptions::StatInfluence::Modrt, 80 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Minor, 100 } },
			{ StatType::Spd, { 160, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Acc, { 180-10 } }
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
		,{ ChampionName::Gromoboy, MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Tyrel, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_VelikiyGalek )
{
	MatchOptions matching(
		{
			{ StatType::HP,  { 33000, MatchOptions::StatInfluence::Minor, 35000 } },
			{ StatType::Atk, { 2100, MatchOptions::StatInfluence::Modrt } },
			{ StatType::Def, { 2000, MatchOptions::StatInfluence::Modrt } },
			{ StatType::CRate, { 95, MatchOptions::StatInfluence::Minor } },
			{ StatType::CDmg, { 100, MatchOptions::StatInfluence::Minor } },
			{ StatType::Spd, { 170, MatchOptions::StatInfluence::Minor, 180 } },	//shouldn't be too fast on Spider
			{ StatType::Acc, { 210 } },
		}
		//,{ ArtSet::Immortal }
		//,{ ArtSet::Def, ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, MINOR_SETS }
	);
	matching.AllowSets( { ArtSet::HP, ArtSet::DivLife, ArtSet::Immortal, ArtSet::Speed, ArtSet::DivSpeed, ArtSet::Acc, ArtSet::Rastoropnost, ArtSet::CRate, ArtSet::DivCritRate, ArtSet::Def, ArtSet::Zhivuchest } );

	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::VGalek, matching );
}

BOOST_AUTO_TEST_CASE( FindBest_VisirOvelis )
{
	const MatchOptions matching(
		{
			{ StatType::HP,   { 35000, MatchOptions::StatInfluence::Modrt } },
			//{ StatType::Atk, { MatchOptions::StatInfluence::Minor } },
			{ StatType::Def,  { 2700, MatchOptions::StatInfluence::Major } },
			{ StatType::Spd,  { 180 } },
			{ StatType::Acc,  { 230 } },
		}
		,{ ArtSet::Vamp }
		,{ ArtSet::DivLife, ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel, ArtSet::CritDmg, ArtSet::DivCritRate, ArtSet::CritDmg, MINOR_SETS }
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::VisirOvelis, matching );
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
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Voitelnica, matching );
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
		,{ MINOR_CHAMPIONS }
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
		,{ MINOR_CHAMPIONS }
	);
	BOOST_CHECK( matching.IsInputOK() );
	FindAndReportBestForChampion( ChampionName::Zargala, matching );
}
