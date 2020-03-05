#include "pch.h"
#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	return nullptr;
}

#include "raid.h"

BOOST_AUTO_TEST_CASE( test_SetBonus )
{
	{
		Champion ch = ChampionFactory::Gromoboy();
		ApplySetBonus(ArtSet::HP, ch);
		BOOST_CHECK_EQUAL(ch.BonusStats.HP, ch.BasicStats.HP * 115 / 100);
	}
}

BOOST_AUTO_TEST_CASE( test_StatValuesByLevel )
{
	for ( ArtType art : { ArtType::Weapon, ArtType::Helmet, ArtType::Shield, ArtType::Gloves, ArtType::Chest, ArtType::Boots } )
		for ( int stars = 4; stars <= 6; ++stars )
			for ( int level : { 0, 1, 4, 8, 12, 16 } )
				for ( StatType stat : StatTypesForArt( art ) )
				{
					BOOST_CHECK( StatValueForLevel( art, stat, stars, level ) > 0 );
				}
}

BOOST_AUTO_TEST_CASE( find_Best )
{
	{
		Champion ch = ChampionFactory::Gromoboy();
		Equipment eq;
		FindBestEquipment( ch, MatchOptions(), eq );
		BOOST_CHECK( !eq.empty() );
	}
}
