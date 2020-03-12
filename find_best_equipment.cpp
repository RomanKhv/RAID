#include "pch.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "to_string.h"

/////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( FindBest_Gromoboy )
{
	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Moderate },
			//{ StatType::Atk, MatchOptions::ArtFactor::NotInterested },
			{ StatType::Def, MatchOptions::ArtFactor::Magor },
			{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
			{ StatType::Spd, MatchOptions::ArtFactor::MinCap },
			{ StatType::Acc, MatchOptions::ArtFactor::MinCap },
		},
		{}
	);
	Champion ch = ChampionFactory::Gromoboy();
	Equipment eq;
	FindRealBestEquipment( ch, matching, eq );
	BOOST_CHECK_EQUAL( eq.size(), 6 );

	BOOST_TEST_MESSAGE( "\n  Gromoboy:" );
	const ChampionStats final_stats = ch.TotalStats();
	BOOST_TEST_MESSAGE( to_string( final_stats ) );
	for ( const auto& e : eq )
	{
		BOOST_TEST_MESSAGE( to_string( e.second ) );
	}
}
