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
			{ StatType::Atk, MatchOptions::ArtFactor::NotInterested },
			{ StatType::Def, MatchOptions::ArtFactor::Magor },
			{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
			{ StatType::Spd, MatchOptions::ArtFactor::MinCap },
			{ StatType::Acc, MatchOptions::ArtFactor::MinCap },
		},
		{}, {}, true,
		{ {StatType::Spd,100}, {StatType::Acc,120} }
	);
	Champion ch = ChampionFactory::Gromoboy();
	const Equipment eq = FindRealBestEquipment( ch, matching );
	BOOST_CHECK_EQUAL( eq.size(), 6 );

	BOOST_TEST_MESSAGE( "\n  Gromoboy:" );

	// Report: new stats
	const ChampionStats final_stats = ch.TotalStats();
	//BOOST_TEST_MESSAGE( to_string( final_stats ) );
	{
		Champion old_ch = ChampionFactory::Gromoboy();
		const Equipment current_eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
		ApplyEquipment( current_eq, old_ch, false );
		BOOST_TEST_MESSAGE( stats_progress( final_stats, old_ch.TotalStats() ) );
	}

	// Report: equipment
	for ( const auto& e : eq )
	{
		BOOST_TEST_MESSAGE( to_string( e.second ) );
	}
}
