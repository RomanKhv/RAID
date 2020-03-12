#include "pch.h"
#include <boost/test/unit_test.hpp>
#include "raid.h"
#include "to_string.h"

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

BOOST_AUTO_TEST_CASE( test_Best )
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

	BOOST_TEST_MESSAGE( "\nGromoboy:" );
	for ( const auto& e : eq )
	{
		BOOST_TEST_MESSAGE( to_string( e.second ) );
	}
}
