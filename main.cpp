#include "pch.h"
#include <boost/test/unit_test.hpp>
#include "raid.h"
#include "iterator.h"
#include "iterator2.h"
#include "suiter.h"
#include "to_string.h"

#ifndef DEBUG_FIND_BEST
#define GENERAL_TESTS
#endif

/////////////////////////////////////////////////////////////////////////////

using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef GENERAL_TESTS

BOOST_AUTO_TEST_CASE( test_basics )
{
	BOOST_CHECK( !Artefact().Initialized() );

	{
		const ChampionStats stats( 1, 2, 3, 4, 5, 6, 7, 8 );
		BOOST_CHECK( stats[StatType::HP] == 1 );
		BOOST_CHECK( stats[StatType::Atk] == 2 );
		BOOST_CHECK( stats[StatType::Def] == 3 );
		BOOST_CHECK( stats[StatType::Spd] == 4 );
		BOOST_CHECK( stats[StatType::CRate] == 5 );
		BOOST_CHECK( stats[StatType::CDmg] == 6 );
		BOOST_CHECK( stats[StatType::Res] == 7 );
		BOOST_CHECK( stats[StatType::Acc] == 8 );
	}

	//for ( ArtSet set = static_cast<ArtSet>(0); stl::enum_to_int( set ) < stl::enum_to_int( ArtSet::Count ); ++static_cast<int&>(set) )
	for ( int s = 0; s < stl::enum_to_int( ArtSet::Count ); ++s )
	{
		const ArtSet set = static_cast<ArtSet>(s);
		BOOST_CHECK_EQUAL( SetSize_fast(set), SetSize(set) );
	}

	{
		const MatchOptions m;
		for ( StatType st : ChampionStats::TypeList )
		{
			BOOST_CHECK( m.Factor(st) == MatchOptions::ArtFactor::NotInterested );
		}
	}
}

static const Champion TestChamp( { 10000, 1000, 1000,  100,  100, 100,  0, 0 }, Element::Void );

#define CHECK_SET_BONUS( set, stat, bonus_value ) { \
		ChampionExt ch = TestChamp; \
		ApplySetBonus( set, ch.BasicStats, ch.ArtsBonusStats, false ); \
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.stat, bonus_value ); }

BOOST_AUTO_TEST_CASE( test_SetBonus )
{
	CHECK_SET_BONUS( ArtSet::HP,       HP, 1500 );
	CHECK_SET_BONUS( ArtSet::Immortal, HP, 1500 );
	CHECK_SET_BONUS( ArtSet::DivLife,  HP, 1500 );
	CHECK_SET_BONUS( ArtSet::Atk,    Atk, 150 );
	CHECK_SET_BONUS( ArtSet::Cruel,  Atk, 150 );
	CHECK_SET_BONUS( ArtSet::DivAtk, Atk, 150 );
	CHECK_SET_BONUS( ArtSet::Def,    Def, 150 );
	CHECK_SET_BONUS( ArtSet::Speed,    Spd, 12 );
	CHECK_SET_BONUS( ArtSet::DivSpeed, Spd, 12 );
	CHECK_SET_BONUS( ArtSet::Res, Res, 40 );
	CHECK_SET_BONUS( ArtSet::Acc, Acc, 40 );
}

BOOST_AUTO_TEST_CASE( test_ApplyStat )
{
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::HP,100 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Atk,100 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Def,100 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Spd,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Spd, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::CRate,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CRate, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::CDmg,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CDmg, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Res,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Res, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Acc,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Acc, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::HP_p,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 1000 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Atk_p,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ApplyStat( { StatType::Def_p,10 }, ch );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 100 );
	}
}

BOOST_AUTO_TEST_CASE( test_EqSetBonuses )
{
	{
		ChampionExt ch = ChampionFactory::Gromoboy();
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, {} ),
			Artefact( ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, {} ),
			Artefact( ArtType::Shield, ArtSet::Atk, 4, 8, StatType::Def, {} ),
			Artefact( ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Boots, ArtSet::Speed, 6, 8, StatType::Spd, {} ),
		};
		ApplySetsBonuses( eq, ch.BasicStats, ch.ArtsBonusStats, false );
		BOOST_CHECK( ch.ArtsBonusStats.HP > 0 );
		BOOST_CHECK( ch.ArtsBonusStats.Spd > 0 );
		BOOST_CHECK( ch.ArtsBonusStats.Atk == 0 );
		BOOST_CHECK( ch.ArtsBonusStats.Def == 0 );
	}
}

BOOST_AUTO_TEST_CASE( test_StatValuesByLevel )
{
	BOOST_CHECK( MatchOptions::ConsiderMaxLevels );	//now check only 16th level

//	//for ( ArtType art : { ArtType::Weapon, ArtType::Helmet, ArtType::Shield, ArtType::Gloves, ArtType::Chest, ArtType::Boots } )
//	for ( ArtType art : Equipment::AllTypesArr )
//		for ( int stars = 4; stars <= 6; ++stars )
//			for ( int level : { /*0, 4, 8, 12,*/ 16 } )
//				for ( StatType stat : StatTypesForArt( art ) )
//				{
//					BOOST_CHECK( StatValueForLevel( art, stat, stars, level ) > 0 );
//				}

	for ( ArtType art : Equipment::AllTypesArr )
		for ( int stars = 4; stars <= 6; ++stars )
			for ( int level : { /*0, 4, 8, 12,*/ 16 } )
				for ( StatType stat : StatTypesForArt( art ) )
				{
					_ASSERTE( StatValueForLevel( art, stat, stars, level ) == StatValueForLevel_fast( art, stat, stars, level ) );
					BOOST_CHECK_EQUAL( StatValueForLevel( art, stat, stars, level ), StatValueForLevel_fast( art, stat, stars, level ) );
				}
}

EquipmentRef convert( const Equipment& ref_eq )
{
	EquipmentRef eq;
	for ( ArtType at : Equipment::AllTypesArr )
	{
		const Artefact& ref_art = ref_eq[at];
		if ( ref_art.Initialized() )
			eq.Arts[at] = &ref_art;
		else
			eq.Arts[at] = nullptr;
	}
	return eq;
}

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
		BOOST_CHECK( MatchOptions( {}, {} ).IsEqHasRequiredSets( convert(eq) ) );
		BOOST_CHECK( MatchOptions( {}, { ArtSet::HP, ArtSet::Speed } ).IsEqHasRequiredSets( convert(eq) ) );
		BOOST_CHECK( !MatchOptions( {}, { ArtSet::HP, ArtSet::Atk } ).IsEqHasRequiredSets( convert(eq) ) );
	}
	{
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, {} ),
			Artefact( ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, {} ),
			Artefact( ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, {} ),
			Artefact( ArtType::Chest, ArtSet::HP, 5, 8, StatType::Def_p, {} ),
			Artefact( ArtType::Boots, ArtSet::HP, 5, 8, StatType::Spd, {} ),
		};
		BOOST_CHECK( MatchOptions( {}, { ArtSet::HP, ArtSet::HP } ).IsEqHasRequiredSets( convert(eq) ) );
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
		BOOST_CHECK( MatchOptions( {}, { ArtSet::Vamp } ).IsEqHasRequiredSets( convert(eq) ) );
		BOOST_CHECK( MatchOptions( {}, { ArtSet::HP, ArtSet::Vamp } ).IsEqHasRequiredSets( convert(eq) ) );
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
		BOOST_CHECK_EQUAL( i.get().Size(), 0 );

		i.begin();
		BOOST_CHECK( !i.finished() );
		Equipment eq = i.get();
		BOOST_CHECK_EQUAL( eq.Size(), 2 );
		BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 1 );
		BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 3 );

		i.next();
		BOOST_CHECK( !i.finished() );
		eq = i.get();
		BOOST_CHECK_EQUAL( eq.Size(), 2 );
		BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 1 );
		BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 4 );

		i.next();
		BOOST_CHECK( !i.finished() );
		eq = i.get();
		BOOST_CHECK_EQUAL( eq.Size(), 2 );
		BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 2 );
		BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 3 );

		i.next();
		BOOST_CHECK( !i.finished() );
		eq = i.get();
		BOOST_CHECK_EQUAL( eq.Size(), 2 );
		BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 2 );
		BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 4 );

		i.next();
		BOOST_CHECK( i.finished() );
		BOOST_CHECK_EQUAL( i.get().Size(), 0 );
	}
}

BOOST_AUTO_TEST_CASE( test_Iterator2 )
{
	{
		std::map<ArtType, std::vector<Artefact>> arts_by_type;
		arts_by_type_iterator2 i( arts_by_type, 0 );
		BOOST_CHECK( i.finished() );
		BOOST_CHECK_EQUAL( i.get().Size(), 0 );
		i.begin();
		BOOST_CHECK( i.finished() );
	}
	{
		std::map<ArtType, std::vector<Artefact>> arts_by_type = {
			{ ArtType::Weapon, { {ArtType::Weapon,{},1,{},StatType::Atk,{}}, {ArtType::Weapon,{},2,{},StatType::Atk,{}} } },
			{ ArtType::Shield, { {ArtType::Shield,{},3,{},StatType::Def,{}}, {ArtType::Shield,{},4,{},StatType::Def,{}} } },
		};

		int n_iterations = 0;
		for ( size_t ri = 0; ri < arts_by_type.cbegin()->second.size(); ++ri )
		{
			arts_by_type_iterator2 i( arts_by_type, ri );
			BOOST_CHECK( i.finished() );

			for ( i.begin(); !i.finished(); i.next() )
			{
				n_iterations++;
				Equipment eq = i.get();
				BOOST_CHECK_EQUAL( eq.Size(), 2 );
			}
		}
		BOOST_CHECK_EQUAL( n_iterations, 4 );

		{
			arts_by_type_iterator2 i( arts_by_type, 0 );
			BOOST_CHECK( i.finished() );

			i.begin();
			BOOST_CHECK( !i.finished() );
			Equipment eq = i.get();
			BOOST_CHECK_EQUAL( eq.Size(), 2 );
			BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 1 );
			BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 3 );

			i.next();
			BOOST_CHECK( !i.finished() );
			eq = i.get();
			BOOST_CHECK_EQUAL( eq.Size(), 2 );
			BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 1 );
			BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 4 );

			i.next();
			BOOST_CHECK( i.finished() );
			BOOST_CHECK_EQUAL( i.get().Size(), 0 );
		}
		{
			arts_by_type_iterator2 i( arts_by_type, 1 );
			BOOST_CHECK( i.finished() );

			i.begin();
			BOOST_CHECK( !i.finished() );
			Equipment eq = i.get();
			BOOST_CHECK_EQUAL( eq.Size(), 2 );
			BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 2 );
			BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 3 );

			i.next();
			BOOST_CHECK( !i.finished() );
			eq = i.get();
			BOOST_CHECK_EQUAL( eq.Size(), 2 );
			BOOST_CHECK_EQUAL( eq[ArtType::Weapon].Stars, 2 );
			BOOST_CHECK_EQUAL( eq[ArtType::Shield].Stars, 4 );

			i.next();
			BOOST_CHECK( i.finished() );
			BOOST_CHECK_EQUAL( i.get().Size(), 0 );
		}
	}
}

BOOST_AUTO_TEST_CASE( test_Estimation_FloatFactor )
{
	const int min_acc_cap = 120;
	const int w = 20;
	float fk;
	BOOST_CHECK( !EstimateMinCap( 100, min_acc_cap, w, fk ) );
	BOOST_CHECK( !EstimateMinCap( 110, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 0 );
	BOOST_CHECK( EstimateMinCap( 120, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 1.f );
	BOOST_CHECK( EstimateMinCap( 131, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 1.f );
	BOOST_CHECK( EstimateMinCap( 140, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 1.f );
	BOOST_CHECK( EstimateMinCap( 150, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 0.5f );
	BOOST_CHECK( EstimateMinCap( 160, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 0.5f );
	BOOST_CHECK( EstimateMinCap( 200, min_acc_cap, w, fk ) );			BOOST_CHECK_EQUAL( fk, 0.5f );
}

BOOST_AUTO_TEST_CASE( test_join )
{
	const std::vector<Artefact> inventory = {
		Artefact( ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, {} ),
		Artefact( ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, {} ),
		Artefact( ArtType::Shield, ArtSet::Atk, 4, 8, StatType::Def, {} ),
		Artefact( ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, {} ),
		Artefact( ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, {} ),
	};
	{
		EqEst est1( 1, {} );
		EqEst est2( 2, {} );

		est2._join( est1 );
		BOOST_CHECK_EQUAL( est2._Est, 2 );
		est1._join( est2 );
		BOOST_CHECK_EQUAL( est1._Est, 2 );
	}
	{
		EqEstPool pool( 3 );
		pool.join( EqEst( 2, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 1 );

		pool.join( EqEst( 1, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 2 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 2 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 1 );

		pool.join( EqEst( 4, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 4 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 2 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 1 );

		pool.join( EqEst( 3, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 4 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 3 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 2 );

		pool.join( EqEst( 0.5, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 4 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 3 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 2 );
	}
}

const std::map<StatType, MatchOptions::ArtFactor> All_Stats_Moderate = {
	{ StatType::HP,  MatchOptions::ArtFactor::Moderate },
	{ StatType::Atk, MatchOptions::ArtFactor::Moderate },
	{ StatType::Def, MatchOptions::ArtFactor::Moderate },
	{ StatType::Spd, MatchOptions::ArtFactor::Moderate },
	{ StatType::CRate, MatchOptions::ArtFactor::Moderate },
	{ StatType::CDmg, MatchOptions::ArtFactor::Moderate },
	{ StatType::Res, MatchOptions::ArtFactor::Moderate },
	{ StatType::Acc, MatchOptions::ArtFactor::Moderate }
};

BOOST_AUTO_TEST_CASE( test_Best )
{
	const std::vector<Artefact> inventory = {
		Artefact( ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, {} ),
		Artefact( ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, {} ),
		Artefact( ArtType::Shield, ArtSet::Atk, 4, 8, StatType::Def, {} ),
		Artefact( ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, {} ),
		Artefact( ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, {} ),
	};
	{
		Champion ch = TestChamp;
		Equipment eq;
		FindBestEquipment( inventory, ch, MatchOptions( All_Stats_Moderate ), eq );
		BOOST_CHECK_EQUAL( eq.Size(), 5 );
	}
	{
		Champion ch = TestChamp;
		Equipment eq;
		FindBestEquipment( inventory, ch, MatchOptions( All_Stats_Moderate, {ArtSet::Speed} ), eq );
		BOOST_CHECK_EQUAL( eq.Size(), 5 );
	}
	{
		Champion ch = TestChamp;
		Equipment eq;
		FindBestEquipment( inventory, ch, MatchOptions( All_Stats_Moderate, {ArtSet::Atk} ), eq );
		BOOST_CHECK_EQUAL( eq.Size(), 0 );
	}
}

//#ifndef DEBUG_FIND_BEST

BOOST_AUTO_TEST_CASE( test_Gromoboy )
{
	ChampionExt ch = ChampionFactory::Gromoboy();
	const Equipment eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
	ApplyEquipment( eq, ch.BasicStats, ch.ArtsBonusStats, false, false );

	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 13797 - 2 );
	BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 136 );
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
	BOOST_CHECK_EQUAL( hall_stats.Acc, 30 );

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
//#endif

#endif