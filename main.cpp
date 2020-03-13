#include "pch.h"
#include <boost/test/unit_test.hpp>
#include "raid.h"

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
		ApplySetsBonuses( eq, ch, false );
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
		Artefact( ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, {} ),
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

BOOST_AUTO_TEST_CASE( test_Gromoboy )
{
	Champion ch = ChampionFactory::Gromoboy();
	Equipment eq;
	const std::vector<Artefact> current_eq = {
		Artefact( ArtType::Weapon, ArtSet::Def, 4, 12, StatType::Atk,   { {StatType::CRate,11}, {StatType::Acc,15+1}, {StatType::HP_p,4+1} } ),
		Artefact( ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP,    { {StatType::Def_p,9+1}, {StatType::Spd,4+1}, {StatType::Res,28+2}, {StatType::CRate,9} } ),
		Artefact( ArtType::Shield, ArtSet::Acc, 5, 16, StatType::Def,   { {StatType::HP_p,6+1}, {StatType::Def_p,5+2}, {StatType::CDmg,15}, {StatType::CRate,16} } ),
		Artefact( ArtType::Gloves, ArtSet::Def, 5, 16, StatType::Def_p, { {StatType::Spd,13+1}, {StatType::Acc,9+2}, {StatType::CRate,6}, {StatType::Res,8+2} } ),
		Artefact( ArtType::Chest,  ArtSet::Def, 5, 12, StatType::HP_p,  { {StatType::Def,43+10}, {StatType::CDmg,12}, {StatType::Res,10+2} } ),
		Artefact( ArtType::Boots,  ArtSet::Def, 4, 16, StatType::Spd,   { {StatType::Def_p,8+1}, {StatType::Atk,6+5}, {StatType::Res,9+2}, {StatType::Def,10+5} } ),
		Artefact( ArtType::Ring,   ArtSet::None, 5, 12, StatType::HP,   { {StatType::Def,46+5}, {StatType::Def_p,5+1}, {StatType::HP_p,5+1} } ),
	};
	FindBestEquipment( current_eq, ch.BasicStats, MatchOptions(), eq );
	ApplyEquipment( eq, ch, false );

	BOOST_CHECK_EQUAL( ch.BonusStats.HP, 13797 - 2 );
	BOOST_CHECK_EQUAL( ch.BonusStats.Atk, 131 );
	BOOST_CHECK_EQUAL( ch.BonusStats.Def, 1960 - 3 );
	BOOST_CHECK_EQUAL( ch.BonusStats.Spd, 54 );
	BOOST_CHECK_EQUAL( ch.BonusStats.CRate, 42 );
	BOOST_CHECK_EQUAL( ch.BonusStats.CDmg, 27 );
	BOOST_CHECK_EQUAL( ch.BonusStats.Res, 63 );
	BOOST_CHECK_EQUAL( ch.BonusStats.Acc, 67 );

	//const ChampionStats final_stats = ch.TotalStats();
	//BOOST_CHECK_EQUAL( final_stats.HP, 32506 );
	//BOOST_CHECK_EQUAL( final_stats.Atk, 989 );
	//BOOST_CHECK_EQUAL( final_stats.Def, 3742 );
	//BOOST_CHECK_EQUAL( final_stats.Spd, 151 );
	//BOOST_CHECK_EQUAL( final_stats.CRate, 62 );
	//BOOST_CHECK_EQUAL( final_stats.CDmg, 89 );
	//BOOST_CHECK_EQUAL( final_stats.Res, 98 );
	//BOOST_CHECK_EQUAL( final_stats.Acc, 103 );
}
