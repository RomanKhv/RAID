#include "pch.h"
#include <boost/test/unit_test.hpp>
#include "raid.h"
#include "iterator.h"

/////////////////////////////////////////////////////////////////////////////

using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( test_SetBonus )
{
	{
		Champion ch = ChampionFactory::Gromoboy();
		ApplySetBonus( ArtSet::HP, ch, false );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, int(ch.BasicStats.HP * 0.15) );
	}
}

BOOST_AUTO_TEST_CASE( test_EqSetBonuses )
{
	{
		Champion ch = ChampionFactory::Gromoboy();
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, {} ),
			Artefact( ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, {} ),
			Artefact( ArtType::Shield, ArtSet::Atk, 4, 8, StatType::Def, {} ),
			Artefact( ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Boots, ArtSet::Speed, 6, 8, StatType::Spd, {} ),
		};
		ApplySetsBonuses( eq, ch, false );
		BOOST_CHECK( ch.ArtsBonusStats.HP > 0 );
		BOOST_CHECK( ch.ArtsBonusStats.Spd > 0 );
		BOOST_CHECK( ch.ArtsBonusStats.Atk == 0 );
		BOOST_CHECK( ch.ArtsBonusStats.Def == 0 );
	}
}

//BOOST_AUTO_TEST_CASE( test_StatValuesByLevel )
//{
//	for ( ArtType art : { ArtType::Weapon, ArtType::Helmet, ArtType::Shield, ArtType::Gloves, ArtType::Chest, ArtType::Boots } )
//		for ( int stars = 4; stars <= 6; ++stars )
//			for ( int level : { 0, 4, 8, 12, 16 } )
//				for ( StatType stat : StatTypesForArt( art ) )
//				{
//					BOOST_CHECK( StatValueForLevel( art, stat, stars, level ) > 0 );
//				}
//}

BOOST_AUTO_TEST_CASE( find_RequiredSets )
{
	{
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, {} ),
			Artefact( ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, {} ),
			Artefact( ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, {} ),
			Artefact( ArtType::Chest, ArtSet::Speed, 5, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, {} ),
		};
		BOOST_CHECK( MatchOptions( {}, {} ).IsEqHasRequiredSets( eq ) );
		BOOST_CHECK( MatchOptions( {}, { ArtSet::HP, ArtSet::Speed } ).IsEqHasRequiredSets( eq ) );
		BOOST_CHECK( !MatchOptions( {}, { ArtSet::HP, ArtSet::Atk } ).IsEqHasRequiredSets( eq ) );
	}
	{
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, {} ),
			Artefact( ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, {} ),
			Artefact( ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, {} ),
			Artefact( ArtType::Chest, ArtSet::HP, 5, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Boots, ArtSet::HP, 5, 8, StatType::Spd, {} ),
		};
		BOOST_CHECK( MatchOptions( {}, { ArtSet::HP, ArtSet::HP } ).IsEqHasRequiredSets( eq ) );
	}
	{
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, {} ),
			Artefact( ArtType::Helmet, ArtSet::Vamp, 5, 8, StatType::HP, {} ),
			Artefact( ArtType::Shield, ArtSet::Vamp, 5, 8, StatType::Def, {} ),
			Artefact( ArtType::Gloves, ArtSet::Vamp, 5, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Chest, ArtSet::HP, 5, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Boots, ArtSet::Vamp, 5, 8, StatType::Spd, {} ),
		};
		BOOST_CHECK( MatchOptions( {}, { ArtSet::Vamp } ).IsEqHasRequiredSets( eq ) );
		BOOST_CHECK( MatchOptions( {}, { ArtSet::HP, ArtSet::Vamp } ).IsEqHasRequiredSets( eq ) );
	}
}

BOOST_AUTO_TEST_CASE( test_Iterator )
{
	{
		std::map<ArtType, std::vector<Artefact>> arts_by_type;
		arts_by_type_iterator i( arts_by_type );
		BOOST_CHECK( i.finished() );
		i.begin();
		BOOST_CHECK( i.finished() );
	}
	{
		std::map<ArtType, std::vector<Artefact>> arts_by_type = {
			{ ArtType::Weapon, { {ArtType::Weapon,{},1,{},StatType::Atk,{}}, {ArtType::Weapon,{},2,{},StatType::Atk,{}} } },
			{ ArtType::Shield, { {ArtType::Shield,{},3,{},StatType::Def,{}}, {ArtType::Shield,{},4,{},StatType::Def,{}} } },
		};
		arts_by_type_iterator i( arts_by_type );
		BOOST_CHECK( i.finished() );
		BOOST_CHECK_EQUAL( i.get().size(), 0 );

		i.begin();
		BOOST_CHECK( !i.finished() );
		Equipment eq = i.get();
		BOOST_CHECK_EQUAL( eq.size(), 2 );
		BOOST_CHECK_EQUAL( eq.cbegin()->second.Stars, 1 );
		BOOST_CHECK_EQUAL( eq.crbegin()->second.Stars, 3 );

		i.next();
		BOOST_CHECK( !i.finished() );
		eq = i.get();
		BOOST_CHECK_EQUAL( eq.size(), 2 );
		BOOST_CHECK_EQUAL( eq.cbegin()->second.Stars, 1 );
		BOOST_CHECK_EQUAL( eq.crbegin()->second.Stars, 4 );

		i.next();
		BOOST_CHECK( !i.finished() );
		eq = i.get();
		BOOST_CHECK_EQUAL( eq.size(), 2 );
		BOOST_CHECK_EQUAL( eq.cbegin()->second.Stars, 2 );
		BOOST_CHECK_EQUAL( eq.crbegin()->second.Stars, 3 );

		i.next();
		BOOST_CHECK( !i.finished() );
		eq = i.get();
		BOOST_CHECK_EQUAL( eq.size(), 2 );
		BOOST_CHECK_EQUAL( eq.cbegin()->second.Stars, 2 );
		BOOST_CHECK_EQUAL( eq.crbegin()->second.Stars, 4 );

		i.next();
		BOOST_CHECK( i.finished() );
		BOOST_CHECK_EQUAL( i.get().size(), 0 );
	}
}

BOOST_AUTO_TEST_CASE( test_Best )
{
	const std::vector<Artefact> inventory = {
		Artefact( ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, {} ),
		Artefact( ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, {} ),
		Artefact( ArtType::Shield, ArtSet::Atk, 4, 8, StatType::Def, {} ),
		Artefact( ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, {} ),
		Artefact( ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, {} ),
	};
	Champion ch = ChampionFactory::Gromoboy();
	{
		Equipment eq;
		FindBestEquipment( inventory, ch, MatchOptions(), eq );
		//TODO: BOOST_CHECK_EQUAL( eq.size(), 5 );
	}
	{
		Equipment eq;
		FindBestEquipment( inventory, ch, MatchOptions( {}, {ArtSet::HP} ), eq );
		//TODO: BOOST_CHECK_EQUAL( eq.size(), 1 );
	}
}

BOOST_AUTO_TEST_CASE( test_Gromoboy )
{
	Champion ch = ChampionFactory::Gromoboy();
	const Equipment eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
	ApplyEquipment( eq, ch, false );

	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 13797 - 2 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 131 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 1960 - 3 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Spd, 54 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CRate, 42 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CDmg, 27 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Res, 63 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Acc, 67 );

	ChampionStats hall_stats;
	ApplyHallBonus( ch, hall_stats );
	BOOST_CHECK_EQUAL( hall_stats.HP, 317 );
	BOOST_CHECK_EQUAL( hall_stats.Atk, 15 - 1 );
	BOOST_CHECK_EQUAL( hall_stats.Def, 43 );
	BOOST_CHECK_EQUAL( hall_stats.Spd, 0 );
	BOOST_CHECK_EQUAL( hall_stats.CRate, 0 );
	BOOST_CHECK_EQUAL( hall_stats.CDmg, 2 );
	BOOST_CHECK_EQUAL( hall_stats.Res, 5 );
	BOOST_CHECK_EQUAL( hall_stats.Acc, 20 );

	//const ChampionStats final_stats = ch.TotalStats( true );
	//BOOST_CHECK_EQUAL( final_stats.HP, 32506 );
	//BOOST_CHECK_EQUAL( final_stats.Atk, 989 );
	//BOOST_CHECK_EQUAL( final_stats.Def, 3742 );
	//BOOST_CHECK_EQUAL( final_stats.Spd, 151 );
	//BOOST_CHECK_EQUAL( final_stats.CRate, 62 );
	//BOOST_CHECK_EQUAL( final_stats.CDmg, 89 );
	//BOOST_CHECK_EQUAL( final_stats.Res, 98 );
	//BOOST_CHECK_EQUAL( final_stats.Acc, 103 );
}
