#include "pch.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "to_string.h"

#if !defined _DEBUG || defined DEBUG_FIND_BEST

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
		}
		,{}
		,{ /*ArtSet::HP, ArtSet::Def, ArtSet::Acc, ArtSet::Speed, ArtSet::Immortal, ArtSet::DivLife, ArtSet::DivSpeed*/ }
		,true
		,{ {StatType::Spd,100}, {StatType::Acc,120} }
	);
	ChampionExt ch = ChampionFactory::Gromoboy();
	const Equipment eq = FindRealBestEquipment( ch, matching );
#ifndef DEBUG_FIND_BEST
	BOOST_CHECK_GE( eq.Size(), 6 );
#endif

	// Report: new stats
	const ChampionStats final_stats = ch.TotalStats();
	{
		ChampionExt old_ch = ChampionFactory::Gromoboy();
		const Equipment current_eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
		old_ch.ApplyEquipment( current_eq, false, matching.ConsiderMaxLevels );
		BOOST_TEST_MESSAGE( stats_progress( final_stats, old_ch.TotalStats() ) );
	}

	BOOST_CHECK_LE( final_stats[StatType::CRate], 100 );

	// Report: equipment
	BOOST_TEST_MESSAGE( to_string( eq ) );
}

#endif