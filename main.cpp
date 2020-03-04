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
		ApplyBonus(ArtSet::HP, ch);
		BOOST_CHECK_EQUAL(ch.BonusStats.HP, ch.BasicStats.HP * 115 / 100);
	}
}