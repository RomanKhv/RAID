#include "pch.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "to_string.h"

#if !defined _DEBUG || defined DEBUG_SMALL_INVENTORY

/////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE( FindBest_Gromoboy )
{
	BOOST_TEST_MESSAGE( "\n  Gromoboy:" );

	const MatchOptions matching(
		{
			{ StatType::HP,  MatchOptions::ArtFactor::Moderate },
			{ StatType::Atk, MatchOptions::ArtFactor::NotInterested },
			{ StatType::Def, MatchOptions::ArtFactor::Magor },
			{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
			{ StatType::CDmg, MatchOptions::ArtFactor::Minor },
			{ StatType::Spd, MatchOptions::ArtFactor::MinCap },
			{ StatType::Acc, MatchOptions::ArtFactor::MinCap },
		}
		,{}
		,{ /*ArtSet::HP, ArtSet::Def, ArtSet::Acc, ArtSet::Speed, ArtSet::Immortal, ArtSet::DivLife, ArtSet::DivSpeed*/ }
		,true
		,{ {StatType::Spd,100}, {StatType::Acc,120} }
	);
	Champion ch = ChampionFactory::Gromoboy();
	const Equipment eq = FindRealBestEquipment( ch, matching );
#ifndef DEBUG_SMALL_INVENTORY
	BOOST_CHECK_EQUAL( eq.Size(), 6 );
#endif

	// Report: new stats
	const ChampionStats final_stats = ch.TotalStats();
	{
		Champion old_ch = ChampionFactory::Gromoboy();
		const Equipment current_eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
		ApplyEquipment( current_eq, old_ch, false, matching.ConsiderMaxLevels );
		BOOST_TEST_MESSAGE( stats_progress( final_stats, old_ch.TotalStats() ) );
	}

	// Report: equipment
	BOOST_TEST_MESSAGE( to_string( eq ) );
}

#endif