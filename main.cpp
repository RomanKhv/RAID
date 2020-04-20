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

#ifdef _DEBUG
template <typename IMAP>
int index_map_iterator<IMAP>::_instances_counter = 0;
#endif

enum class Enum { e0, e1, e2, Count };

BOOST_AUTO_TEST_CASE( test_index_map )
{
	{
		enum_index_map<ArtSet, ArtSet::count, int> sets;
		sets[ArtSet::HP] = 1;
		BOOST_CHECK_EQUAL( sets._Arr[0], 1 );
		sets[ArtSet::Atk] = 2;
		BOOST_CHECK_EQUAL( sets._Arr[1], 2 );
	}
	{
		index_map<ArtSet, stl::enum_to_<size_t>( ArtSet::count ), int> sets;
		sets[ArtSet::HP] = 1;
		BOOST_CHECK_EQUAL( sets._Arr[0], 1 );
		sets[ArtSet::Atk] = 2;
		BOOST_CHECK_EQUAL( sets._Arr[1], 2 );
	}
	{
		using map_t = enum_index_map<Enum, Enum::Count, int>;
		map_t m;
		BOOST_CHECK( m.begin() != m.end() );
		BOOST_CHECK( m.size() == 3 );
		BOOST_CHECK( ++(++(++m.begin())) == m.end() );
		BOOST_CHECK_EQUAL( m._Arr[0], 0 );
		BOOST_CHECK_EQUAL( m._Arr[1], 0 );
		BOOST_CHECK_EQUAL( m._Arr[2], 0 );

		m[Enum::e1] = 1;
		BOOST_CHECK( m.begin().key() == Enum::e0 );
		BOOST_CHECK( m.begin().value() == 0 );

		auto i = m.begin();
		BOOST_CHECK( i.key() == Enum::e0 );
		BOOST_CHECK_EQUAL( i.value(), 0 );
		++i;
		BOOST_CHECK( i.key() == Enum::e1 );
		BOOST_CHECK_EQUAL( i.value(), 1 );
		++i;
		BOOST_CHECK( i.key() == Enum::e2 );
		BOOST_CHECK_EQUAL( i.value(), 0 );
		++i;
		BOOST_CHECK( i == m.end() );

#ifdef INDEX_MAP_READY_FOR_RANGE_FOR
		int counter = 0;
#ifdef _DEBUG
		index_map_iterator<map_t>::_instances_counter = 0;
#endif
		for ( const auto i : m )
			counter++;
#ifdef _DEBUG
		 BOOST_CHECK_EQUAL( index_map_iterator<map_t>::_instances_counter, 2 );
#endif
		BOOST_CHECK_EQUAL( counter, 3 );
#endif
	}
}

BOOST_AUTO_TEST_CASE( test_enum_range_iterator )
{
//	{
//		int counter = 0;
//		for ( Enum e : make_enum_range<Enum>( Enum::e0, Enum::Count ) )
//			counter++;
//		BOOST_CHECK_EQUAL( counter, 3 );
//	}
	{
		enum_iterator<Enum> i( Enum::e0, Enum::Count );
		BOOST_CHECK( !i.finished() );
		BOOST_CHECK( *i == Enum::e0 );
		++i;
		BOOST_CHECK( !i.finished() );
		BOOST_CHECK( *i == Enum::e1 );
		++i;
		BOOST_CHECK( !i.finished() );
		++i;
		BOOST_CHECK( i.finished() );
		++i;
		BOOST_CHECK( i.finished() );
	}
	{
		int counter = 0;
		for ( enum_iterator<Enum> i( Enum::e0, Enum::Count ); !i.finished(); ++i )
			counter++;
		BOOST_CHECK_EQUAL( counter, 3 );
	}
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

		BOOST_CHECK( Stat::IsBasic( StatType::HP ) );
		BOOST_CHECK( Stat::IsBasic( StatType::Atk ) );
		BOOST_CHECK( Stat::IsBasic( StatType::Def ) );
		BOOST_CHECK( Stat::IsBasic( StatType::Spd ) );
		BOOST_CHECK( Stat::IsBasic( StatType::CRate ) );
		BOOST_CHECK( Stat::IsBasic( StatType::CDmg ) );
		BOOST_CHECK( Stat::IsBasic( StatType::Res ) );
		BOOST_CHECK( Stat::IsBasic( StatType::Acc ) );
		BOOST_CHECK( !Stat::IsBasic( StatType::HP_p ) );
		BOOST_CHECK( !Stat::IsBasic( StatType::Atk_p ) );
		BOOST_CHECK( !Stat::IsBasic( StatType::Def_p ) );

		BOOST_CHECK_EQUAL( stats.basic_from_p( StatType::HP_p ), 1 );
		BOOST_CHECK_EQUAL( stats.basic_from_p( StatType::Atk_p ), 2 );
		BOOST_CHECK_EQUAL( stats.basic_from_p( StatType::Def_p ), 3 );
	}

	for ( int s = 0; s < stl::enum_to_int( ArtSet::count ); ++s )
	{
		const ArtSet set = static_cast<ArtSet>(s);
		BOOST_CHECK_EQUAL( SetSize_fast(set), debug::SetSize(set) );
	}

	{
		const MatchOptions m;
		for ( StatType st : ChampionStats::TypeList )
		{
			BOOST_CHECK( m.Factor(st).Mode == MatchOptions::StatInfluence::NotInterested );
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
	CHECK_SET_BONUS( ArtSet::Resist, Res, 40 );
	CHECK_SET_BONUS( ArtSet::Acc, Acc, 40 );
}

BOOST_AUTO_TEST_CASE( test_ApplyStat )
{
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::HP,100 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Atk,100 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Def,100 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Spd,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Spd, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::CRate,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CRate, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::CDmg,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CDmg, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Res,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Res, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Acc,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Acc, 10 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::HP_p,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 1000 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Atk_p,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 100 );
	}
	{
		ChampionExt ch = TestChamp;
		ch.ApplyStat( { StatType::Def_p,10 } );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 100 );
	}
}

BOOST_AUTO_TEST_CASE( test_EqSetBonuses )
{
	{
		ChampionExt ch = Champion::ByName( ChampionName::Gromoboy );
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
				for ( StatType stat : debug::StatTypesForArt( art ) )
				{
					_ASSERTE( debug::StatValueForLevel( art, stat, stars, level ) == StatValueForLevel_fast( art, stat, stars, level ) );
					BOOST_CHECK_EQUAL( debug::StatValueForLevel( art, stat, stars, level ), StatValueForLevel_fast( art, stat, stars, level ) );
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

BOOST_AUTO_TEST_CASE( find_SetsRestrictions )
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

		BOOST_CHECK( MatchOptions( {}, {}, { ArtSet::Atk } ).IsArtAccepted( eq[ArtType::Weapon], ChampionName::Gromoboy ) );
		BOOST_CHECK( !MatchOptions( {}, {}, { ArtSet::Atk } ).IsArtAccepted( eq[ArtType::Shield], ChampionName::Gromoboy ) );
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

	BOOST_CHECK( MatchOptions( {}, {}, { ArtSet::Atk } ).IsSetAccepted( ArtSet::HP ) );
	BOOST_CHECK( !MatchOptions( {}, {}, { ArtSet::Atk } ).IsSetAccepted( ArtSet::Atk ) );
	BOOST_CHECK( !MatchOptions( {}, {}, { ArtSet::Atk, ArtSet::Def } ).IsSetAccepted( ArtSet::Atk ) );
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

BOOST_AUTO_TEST_CASE( test_EstimationBasics )
{
	{
		const MatchOptions matching(
			{
				{ StatType::Res, { MatchOptions::StatInfluence::Max } },
			}
		);
		ChampionStats ch;
		ch.Res = 70;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.7f );
		ch.Res = 100;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.f );
		ch.Res = 120;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.2f );
	}
	{
		const MatchOptions matching(
			{
				{ StatType::Acc, { 120 } },
			}
		);
		ChampionStats ch;
		ch.Acc = 110;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0 );
		ch.Acc = 115;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.5f );
		ch.Acc = 120;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.f );
		ch.Acc = 150;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.f );
		ch.Acc = 200;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.f );
	}
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

		pool.join( EqEst( 5, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 5 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 2 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 1 );

		pool.join( EqEst( 4, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 5 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 4 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 2 );

		pool.join( EqEst( 3, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 5 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 4 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 3 );

		pool.join( EqEst( 0.5, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 5 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 4 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 3 );

		pool.join( EqEst( 6, {} ) );
		BOOST_CHECK_EQUAL( pool._Arr.size(), 3 );
		BOOST_CHECK_EQUAL( pool._Arr[0]._Est, 6 );
		BOOST_CHECK_EQUAL( pool._Arr[1]._Est, 5 );
		BOOST_CHECK_EQUAL( pool._Arr[2]._Est, 4 );
	}
}

const std::map<StatType, MatchOptions::StatFactor> All_Stats_Moderate = {
	{ StatType::HP,  {MatchOptions::StatInfluence::Modrt} },
	{ StatType::Atk, {MatchOptions::StatInfluence::Modrt} },
	{ StatType::Def, {MatchOptions::StatInfluence::Modrt} },
	{ StatType::Spd, {MatchOptions::StatInfluence::Modrt} },
	{ StatType::CRate, {MatchOptions::StatInfluence::Modrt} },
	{ StatType::CDmg, {MatchOptions::StatInfluence::Modrt} },
	{ StatType::Res, {MatchOptions::StatInfluence::Modrt} },
	{ StatType::Acc, {MatchOptions::StatInfluence::Modrt} }
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

BOOST_AUTO_TEST_CASE( test_TuneCoefs )
{
	//CRate penalty on excess +1 shouldn't be stronger than bonus of +1000 HP
	const ChampionName name = ChampionName::Zargala;
	const MatchOptions matching(
		{
			{ StatType::HP,   { MatchOptions::StatInfluence::Minor } },
			{ StatType::Atk,  { MatchOptions::StatInfluence::Max } },
			{ StatType::CRate,{ 80 } },
			{ StatType::CDmg, { MatchOptions::StatInfluence::Modrt } },
			{ StatType::Spd,  { 165 } },
			{ StatType::Acc,  { 120 } },
		}
	);

	float est_current, est_new;
	ChampionStats stats_current, stats_new;
	{	// current
		const Equipment eq = {
			Artefact{ ArtType::Weapon, ArtSet::CRate, 5, 16, StatType::Atk, { {StatType::Acc,20,2}, {StatType::CRate,17}, {StatType::Res,18,1}, {StatType::CDmg,5} } },
			Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 16, StatType::HP, { {StatType::Atk_p,10,2}, {StatType::CDmg,11}, {StatType::HP_p,5,1}, {StatType::CRate,5} } },
			Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 16, StatType::Def, { {StatType::Acc,20,1}, {StatType::Spd,9,1}, {StatType::Def_p,5,1}, {StatType::CDmg,6} } ),
			Artefact{ ArtType::Gloves, ArtSet::Acc, 6, 16, StatType::Atk_p, { {StatType::CRate,20}, {StatType::Spd,5,2}, {StatType::HP_p,5,1}, {StatType::Def_p,7,1} } },
			Artefact{ ArtType::Chest, ArtSet::Acc, 5, 16, StatType::Atk_p, { {StatType::Def_p,5,1}, {StatType::CDmg,15}, {StatType::Acc,10,2}, {StatType::Res,9} } },
			Artefact( ArtType::Boots, ArtSet::DivSpeed, 5, 16, StatType::Spd, { {StatType::Atk_p,14,2}, {StatType::Atk,15,12}, {StatType::CRate,9} } ),
			Artefact{ ArtType::Ring, ArtSet::None, 5, 8, StatType::HP, { {StatType::Atk_p,16,1}, {StatType::HP_p,5,1} } },
			Artefact{ ArtType::Necklace, ArtSet::None, 4, 12, StatType::Atk, { {StatType::Acc,24}, {StatType::HP,522}, {StatType::CDmg,5} } },
		};
		ChampionExt ch = Champion::ByName( name );
		ApplyEquipment( eq, ch.BasicStats, ch.ArtsBonusStats, true, true );
		stats_current = ch.TotalStats();
		est_current = EstimateEquipment( stats_current, matching );
	}
	{	// new
		const Equipment eq = {
			Artefact( ArtType::Weapon, ArtSet::DivAtk, 6, 16, StatType::Atk, { {StatType::HP,860}, {StatType::HP_p,12}, {StatType::Atk_p,10}, {StatType::Acc,12,1} } ),
			Artefact( ArtType::Helmet, ArtSet::DivAtk, 5, 16, StatType::HP, { {StatType::CDmg,15}, {StatType::CRate,5}, {StatType::Atk_p,10,1}, {StatType::Def_p,5,1} } ),
			Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 16, StatType::Def, { {StatType::Acc,20,1}, {StatType::Spd,9,1}, {StatType::Def_p,5,1}, {StatType::CDmg,6} } ),
			Artefact{ ArtType::Gloves, ArtSet::Acc, 6, 16, StatType::Atk_p, { {StatType::CRate,20}, {StatType::Spd,5,2}, {StatType::HP_p,5,1}, {StatType::Def_p,7,1} } },
			Artefact{ ArtType::Chest, ArtSet::Acc, 5, 16, StatType::Atk_p, { {StatType::Def_p,5,1}, {StatType::CDmg,15}, {StatType::Acc,10,2}, {StatType::Res,9} } },
			Artefact( ArtType::Boots, ArtSet::DivSpeed, 5, 16, StatType::Spd, { {StatType::Atk_p,14,2}, {StatType::Atk,15,12}, {StatType::CRate,9} } ),
			Artefact{ ArtType::Ring, ArtSet::None, 5, 8, StatType::HP, { {StatType::Atk_p,16,1}, {StatType::HP_p,5,1} } },
			Artefact{ ArtType::Necklace, ArtSet::None, 4, 12, StatType::Atk, { {StatType::Acc,24}, {StatType::HP,522}, {StatType::CDmg,5} } },
		};
		ChampionExt ch = Champion::ByName( name );
		ApplyEquipment( eq, ch.BasicStats, ch.ArtsBonusStats, true, true );
		stats_new = ch.TotalStats();
		est_new = EstimateEquipment( stats_new, matching );
	}
	//BOOST_TEST_MESSAGE( est_current << " vs " << est_new );
	//BOOST_CHECK_LT( est_current, est_new );
}

BOOST_AUTO_TEST_CASE( test_champ_relations )
{
	const ChampionStats Hatun = GetCurrentFinalStatsFor( ChampionName::Hatun );
	const ChampionStats Voitelnica = GetCurrentFinalStatsFor( ChampionName::Voitelnica );
	const ChampionStats Zargala = GetCurrentFinalStatsFor( ChampionName::Zargala );
	const ChampionStats Kael = GetCurrentFinalStatsFor( ChampionName::Kael );
	BOOST_CHECK_GT( Hatun.Spd, Voitelnica.Spd );
	//BOOST_CHECK_GT( Voitelnica.Spd, Kael.Spd );
	BOOST_CHECK_GT( Zargala.Spd, Kael.Spd );
}

//#ifndef DEBUG_FIND_BEST

BOOST_AUTO_TEST_CASE( test_CurrentStats )
{
	{
		ChampionExt ch = Champion::ByName( ChampionName::Gromoboy );
		const Equipment eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
		ApplyEquipment( eq, ch.BasicStats, ch.ArtsBonusStats, false, false );

		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 15207 - 2 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 305 - 1 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 2295 - 2 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Spd, 51 + 1 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CRate, 47 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CDmg, 6 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Res, 42 );
		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Acc, 90 );

		ChampionStats hall_stats;
		ApplyHallBonus( ch, hall_stats );
		BOOST_CHECK_EQUAL( hall_stats.HP, 317 );
		BOOST_CHECK_EQUAL( hall_stats.Atk, 15 - 1 );
		BOOST_CHECK_EQUAL( hall_stats.Def, 43 );
		BOOST_CHECK_EQUAL( hall_stats.Spd, 0 );
		BOOST_CHECK_EQUAL( hall_stats.CRate, 0 );
		BOOST_CHECK_EQUAL( hall_stats.CDmg, 2 );
		BOOST_CHECK_EQUAL( hall_stats.Res, 5 );
		BOOST_CHECK_EQUAL( hall_stats.Acc, 40 );
	}
	{
		const ChampionStats stats = GetCurrentArtsStatsFor( ChampionName::Krisk );
		BOOST_CHECK_EQUAL( stats.HP, 20184 - 2 );
		BOOST_CHECK_EQUAL( stats.Atk, 266 - 1 );
		BOOST_CHECK_EQUAL( stats.Def, 1868 + 2 );
		BOOST_CHECK_EQUAL( stats.Spd, 45 );
		BOOST_CHECK_EQUAL( stats.CRate, 51 );
		BOOST_CHECK_EQUAL( stats.CDmg, 15 );
		BOOST_CHECK_EQUAL( stats.Res, 30 );
		BOOST_CHECK_EQUAL( stats.Acc, 102 );
	}
	{
		const ChampionStats stats = GetCurrentArtsStatsFor( ChampionName::Lekar );
		//BOOST_CHECK_EQUAL( stats.HP, 12543-429 - 2 );
		//BOOST_CHECK_EQUAL( stats.Atk, 503-15 - 2 );
		//BOOST_CHECK_EQUAL( stats.Def, 1332-25 - 1 );
		//BOOST_CHECK_EQUAL( stats.Spd, 70-1 );
		//BOOST_CHECK_EQUAL( stats.CRate, 84 );
		//BOOST_CHECK_EQUAL( stats.CDmg, 15-4 );
		//BOOST_CHECK_EQUAL( stats.Res, 8 );
		//BOOST_CHECK_EQUAL( stats.Acc, 36-9 );
	}
	{
		const ChampionStats stats = GetCurrentArtsStatsFor( ChampionName::SteelSkull );
		//BOOST_CHECK_EQUAL( stats.HP, 10209 - 2 );
		//BOOST_CHECK_EQUAL( stats.Atk, 479 - 1 );
		//BOOST_CHECK_EQUAL( stats.Def, 1057 - 2 );
		//BOOST_CHECK_EQUAL( stats.Spd, 70 );
		//BOOST_CHECK_EQUAL( stats.CRate, 65 );
		//BOOST_CHECK_EQUAL( stats.CDmg, 5 );
		//BOOST_CHECK_EQUAL( stats.Res, 24 );
		//BOOST_CHECK_EQUAL( stats.Acc, 147 );
	}
	{
		const ChampionStats stats = GetCurrentArtsStatsFor( ChampionName::Yuliana );
		//BOOST_CHECK_EQUAL( stats.HP, 10209 - 2 );
		//BOOST_CHECK_EQUAL( stats.Atk, 479 - 1 );
		//BOOST_CHECK_EQUAL( stats.Def, 1057 - 2 );
		//BOOST_CHECK_EQUAL( stats.Spd, 70 );
		//BOOST_CHECK_EQUAL( stats.CRate, 65 );
		//BOOST_CHECK_EQUAL( stats.CDmg, 5 );
		//BOOST_CHECK_EQUAL( stats.Res, 24 );
		//BOOST_CHECK_EQUAL( stats.Acc, 147 );
	}
}
//#endif

#endif
