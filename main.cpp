#include "pch.h"
#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////

#include "raid.h"

BOOST_AUTO_TEST_CASE( test_SetBonus )
{
	{
		Champion ch = ChampionFactory::Gromoboy();
		ApplySetBonus( ArtSet::HP, ch );
		BOOST_CHECK_EQUAL( ch.BonusStats.HP, int(ch.BasicStats.HP * 0.15) );
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
		ApplySetsBonuses( eq, ch );
		BOOST_CHECK( ch.BonusStats.HP > 0 );
		BOOST_CHECK( ch.BonusStats.Spd > 0 );
		BOOST_CHECK( ch.BonusStats.Atk == 0 );
		BOOST_CHECK( ch.BonusStats.Def == 0 );
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

BOOST_AUTO_TEST_CASE( find_Best )
{
	const std::vector<Artefact> inventory = {
		Artefact( ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, {} ),
		Artefact( ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, {} ),
		Artefact( ArtType::Shield, ArtSet::Atk, 4, 8, StatType::Def, {} ),
		Artefact( ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, {} ),
		Artefact( ArtType::Boots, ArtSet::Speed, 6, 8, StatType::Spd, {} ),
	};
	Champion ch = ChampionFactory::Gromoboy();
	{
		Equipment eq;
		FindBestEquipment( inventory, ch.BasicStats, MatchOptions(), eq );
		BOOST_CHECK_EQUAL( eq.size(), 5 );
	}
	{
		Equipment eq;
		FindBestEquipment( inventory, ch.BasicStats, MatchOptions( {}, {ArtSet::HP} ), eq );
		BOOST_CHECK_EQUAL( eq.size(), 1 );
	}
}
