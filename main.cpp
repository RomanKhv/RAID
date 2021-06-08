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

		BOOST_CHECK( !Stat::IsP( StatType::HP ) );
		BOOST_CHECK( !Stat::IsP( StatType::Atk ) );
		BOOST_CHECK( !Stat::IsP( StatType::Def ) );
		BOOST_CHECK( !Stat::IsP( StatType::Spd ) );
		BOOST_CHECK( !Stat::IsP( StatType::CRate ) );
		BOOST_CHECK( !Stat::IsP( StatType::CDmg ) );
		BOOST_CHECK( !Stat::IsP( StatType::Res ) );
		BOOST_CHECK( !Stat::IsP( StatType::Acc ) );
		BOOST_CHECK( Stat::IsP( StatType::HP_p ) );
		BOOST_CHECK( Stat::IsP( StatType::Atk_p ) );
		BOOST_CHECK( Stat::IsP( StatType::Def_p ) );

		BOOST_CHECK( Stat::ToBasic( StatType::HP ) == StatType::HP );
		BOOST_CHECK( Stat::ToBasic( StatType::Atk ) == StatType::Atk );
		BOOST_CHECK( Stat::ToBasic( StatType::Def ) == StatType::Def );
		BOOST_CHECK( Stat::ToBasic( StatType::Spd ) == StatType::Spd );
		BOOST_CHECK( Stat::ToBasic( StatType::CRate ) == StatType::CRate );
		BOOST_CHECK( Stat::ToBasic( StatType::CDmg ) == StatType::CDmg );
		BOOST_CHECK( Stat::ToBasic( StatType::Res ) == StatType::Res );
		BOOST_CHECK( Stat::ToBasic( StatType::Acc ) == StatType::Acc );
		BOOST_CHECK( Stat::ToBasic( StatType::HP_p ) == StatType::HP );
		BOOST_CHECK( Stat::ToBasic( StatType::Atk_p ) == StatType::Atk );
		BOOST_CHECK( Stat::ToBasic( StatType::Def_p ) == StatType::Def );

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
	CHECK_SET_BONUS( ArtSet::Zhivuchest, HP, 1000 );
	CHECK_SET_BONUS( ArtSet::Zhivuchest, Def, 100 );
	CHECK_SET_BONUS( ArtSet::Atk,    Atk, 150 );
	CHECK_SET_BONUS( ArtSet::Cruel,  Atk, 150 );
	CHECK_SET_BONUS( ArtSet::DivAtk, Atk, 150 );
	CHECK_SET_BONUS( ArtSet::Def,    Def, 150 );
	CHECK_SET_BONUS( ArtSet::Speed,    Spd, 12 );
	CHECK_SET_BONUS( ArtSet::DivSpeed, Spd, 12 );
	CHECK_SET_BONUS( ArtSet::Resist, Res, 40 );
	CHECK_SET_BONUS( ArtSet::Acc, Acc, 40 );
	CHECK_SET_BONUS( ArtSet::Rastoropnost, Acc, 40 );
	CHECK_SET_BONUS( ArtSet::Rastoropnost, Spd, 5 );
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

BOOST_AUTO_TEST_CASE( test_InventoryCorrectness )
{
	const Champion ch = Champion::ByName( ChampionName::ColdHeart );
	enum_index_map<ChampionName,ChampionName::count,Equipment> eqs;
	for ( const Artefact& art : _MyArts )
	{
		ChampionStats stats;
		ApplyArtBonus( art, ch.BasicStats, stats, false, true, false );

		if ( art.Owner != ChampionName::none )
		{
			Artefact& ch_art = eqs[art.Owner][art.Type];
			BOOST_CHECK( !ch_art.Initialized() );
			ch_art = art;
		}
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

		{
			MatchOptions mo;
			mo.ForbiddenSets( { ArtSet::Atk } );
			BOOST_CHECK( mo.IsArtAccepted( eq[ArtType::Weapon], ChampionName::Gromoboy ) );
			BOOST_CHECK( !mo.IsArtAccepted( eq[ArtType::Shield], ChampionName::Gromoboy ) );
		}
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
	{
		MatchOptions mo;
		mo.ForbiddenSets( { ArtSet::Atk } );
		BOOST_CHECK( mo.IsSetAccepted( ArtSet::HP ) );
		BOOST_CHECK( !mo.IsSetAccepted( ArtSet::Atk ) );
		mo.ForbiddenSets( { ArtSet::Atk, ArtSet::Def } );
		BOOST_CHECK( !mo.IsSetAccepted( ArtSet::Atk ) );
	}
	const ChampionName ch = ChampionName::Juliana;
	{
		const MatchOptions mo(
			{
				{ StatType::HP,  { 30000, MatchOptions::StatInfluence::Modrt } },
				{ StatType::CRate,  { 90 } },
				{ StatType::Acc,  { 200 } },
			}
		);
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Shield, ArtSet::HP, 5, 8, StatType::Def, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Gloves, ArtSet::HP, 5, 8, StatType::CRate, {} ), ch ) );
		BOOST_CHECK( !mo.IsArtAccepted( Artefact( ArtType::Gloves, ArtSet::HP, 5, 8, StatType::CDmg, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Gloves, ArtSet::HP, 5, 8, StatType::HP_p, {} ), ch ) );
		BOOST_CHECK( !mo.IsArtAccepted( Artefact( ArtType::Gloves, ArtSet::HP, 5, 8, StatType::Atk_p, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Ring, ArtSet::HolyOrden, 5, 8, StatType::Atk, {}, ch ), ch ) );
	}
	{
		MatchOptions mo(
			{},
			{ ArtSet::Shield }
		);
		mo.AllowSets( { ArtSet::HP } );
		BOOST_CHECK( !mo.AreSetsRestrictedToRequired() );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::Shield, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( !mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Ring, ArtSet::HolyOrden, 5, 8, StatType::Atk, {}, ch ), ch ) );
	}
	{
		const MatchOptions mo(
			{},
			{ ArtSet::Shield, ArtSet::Immortal }
		);
		BOOST_CHECK( mo.AreSetsRestrictedToRequired() );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::Shield, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( !mo.IsArtAccepted( Artefact( ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, {} ), ch ) );
		BOOST_CHECK( mo.IsArtAccepted( Artefact( ArtType::Ring, ArtSet::HolyOrden, 5, 8, StatType::Atk, {}, ch ), ch ) );
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
	// min cap
	{
		ChampionStats ch;
		const MatchOptions matching(
#ifdef USE_TARGET_VALUES_AS_REF
			{
				{ StatType::CRate, { 50 } },
			}
		);
		ch.CRate = 70;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.0f );
		ch.CRate = 200;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.0f );
#else
			{
				{ StatType::Acc, { 120 } },
			}
		);
		ch.Acc = 110;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0 );
		ch.Acc = 115;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.399999619f );
		ch.Acc = 120;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.800000012f );
		ch.Acc = 150;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.800000012f );
		ch.Acc = 200;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.800000012f );
#endif
	}
	{
		const MatchOptions matching(
			{
				{ StatType::Acc, { 120, MatchOptions::StatInfluence::Modrt } },
			}
		);
		ChampionStats ch;
		ch.Acc = 110;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0 );
#ifdef USE_TARGET_VALUES_AS_REF
#else
		ch.Acc = 115;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.399999619f );
		ch.Acc = 120;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.800000012f );
		ch.Acc = 144;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.879999995f );
#endif
	}
	// Max Cap
	{
		const MatchOptions matching(
			{
				{ StatType::CRate, { MatchOptions::StatInfluence::Max } },
			}
		);
		ChampionStats ch;
		ch.CRate = 70;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.7f );
		ch.CRate = 100;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.f );
		ch.CRate = 120;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 1.f );
	}
	{
		const MatchOptions matching(
			{
				{ StatType::CRate, { MatchOptions::StatInfluence::Max, 80 } },
			}
		);
		ChampionStats ch;
		ch.CRate = 95;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.8f );
	}
	// Min + Max
	{
		const MatchOptions matching(
			{
				{ StatType::CRate, { 60, MatchOptions::StatInfluence::Modrt, 78 } },
			}
		);
		ChampionStats ch;
#ifdef USE_TARGET_VALUES_AS_REF
#else
		ch.CRate = 60;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.6f );
		ch.CRate = 78;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.69f );
		ch.CRate = 95;
		BOOST_CHECK_EQUAL( EstimateEquipment( ch, matching ), 0.69f );
#endif
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

#ifdef USE_TIERS
BOOST_AUTO_TEST_CASE( Test_TierFiltering )
{
	const MatchOptions m;
	const MatchOptions m1( {}, {}, {}, ArtTier::T1 );
	const MatchOptions m2( {}, {}, {}, ArtTier::T2 );
//	BOOST_CHECK( m1.ArtTierCap == ArtTier::T1 );
//	BOOST_CHECK( m2.ArtTierCap == ArtTier::T2 );
//	{
//		const Artefact art( ArtType::Boots, ArtSet::Vamp, 5, 0, StatType::Spd, {} );
//		BOOST_CHECK( GetArtTier(art) == ArtTier::T2 );
//	}
	{
		const Artefact art( ArtType::Helmet, ArtSet::Vamp, 6, 0, StatType::HP, { {StatType::Spd,17}, {StatType::Atk,20} } );
		BOOST_CHECK( GetArtTier(art) == ArtTier::T1 );
		BOOST_CHECK( m.IsArtAccepted( art, ChampionName::none ) );
		BOOST_CHECK( m1.IsArtAccepted( art, ChampionName::none ) );
		BOOST_CHECK( !m2.IsArtAccepted( art, ChampionName::none ) );
	}
	{
		const Artefact art( ArtType::Helmet, ArtSet::Vamp, 5, 0, StatType::HP, { {StatType::Spd,17}, {StatType::Atk,20} } );
		BOOST_CHECK( GetArtTier(art) == ArtTier::T2 );
		BOOST_CHECK( m.IsArtAccepted( art, ChampionName::none ) );
		BOOST_CHECK( m1.IsArtAccepted( art, ChampionName::none ) );
		BOOST_CHECK( m2.IsArtAccepted( art, ChampionName::none ) );
	}
//	{
//		const Artefact art( ArtType::Helmet, ArtSet::Vamp, 5, 0, StatType::HP, { {StatType::Spd,17}, {StatType::Atk,20} } );
//		BOOST_CHECK( GetArtTier(art) == ArtTier::T1 );
//		BOOST_CHECK( m1.IsArtAccepted( art, ChampionName::none ) );
//		BOOST_CHECK( !m2.IsArtAccepted( art, ChampionName::none ) );
//		//BOOST_CHECK( !m3.IsArtAccepted( art, ChampionName::none ) );
//	}
//	{
//		const Artefact art( ArtType::Helmet, ArtSet::Vamp, 5, 0, StatType::HP, { {StatType::Spd,15}, {StatType::Atk,20} } );
//		BOOST_CHECK( GetArtTier(art) == ArtTier::T2 );
//	}
//	{
//		const Artefact art( ArtType::Helmet, ArtSet::Vamp, 5, 0, StatType::HP, { {StatType::Def_p,5}, {StatType::Acc,26}, {StatType::CRate,6}, {StatType::Res,10} } );
//		BOOST_CHECK( GetArtTier(art) == ArtTier::T2 );
//		BOOST_CHECK( m1.IsArtAccepted( art, ChampionName::none ) );
//		BOOST_CHECK( m2.IsArtAccepted( art, ChampionName::none ) );
//		//BOOST_CHECK( !m3.IsArtAccepted( art, ChampionName::none ) );
//	}
//	{
//		const Artefact art( ArtType::Helmet, ArtSet::Vamp, 6, 0, StatType::HP, { {StatType::Def,20} } );
//		BOOST_CHECK( GetArtTier(art) == ArtTier::T2 );
//	}
//	//{
//	//	const Artefact art( ArtType::Helmet, ArtSet::Vamp, 5, 0, StatType::HP, { {StatType::Def,20}, {StatType::Def_p,5}, {StatType::CRate,6}, {StatType::Res,10} } );
//	//	BOOST_CHECK( GetArtTier(art) == ArtTier::T3 );
//	//	BOOST_CHECK( !m1.IsArtAccepted( art, ChampionName::none ) );	//too weak for Tier1
//	//	BOOST_CHECK( m2.IsArtAccepted( art, ChampionName::none ) );
//	//	BOOST_CHECK( m3.IsArtAccepted( art, ChampionName::none ) );
//	//}
//	//{
//	//	const Artefact art( ArtType::Boots, ArtSet::Vamp, 4, 0, StatType::Spd, { {StatType::Spd,15} } );
//	//	BOOST_CHECK( GetArtTier(art) == ArtTier::T3 );
//	//}
}
#endif

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
		Artefact( ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, {} ),
		Artefact( ArtType::Chest, ArtSet::Speed, 5, 8, StatType::Def_p, {} ),
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

//BOOST_AUTO_TEST_CASE( verify_Best )
//{
//	const std::vector<Artefact> inventory = {
//			Artefact( ArtType::Weapon, ArtSet::Speed, 6, 16, StatType::Atk, { {StatType::HP_p,6}, {StatType::Spd,12}, {StatType::HP,816}, {StatType::Acc,31}, } ),
//			Artefact( ArtType::Helmet, ArtSet::DivLife, 6, 12, StatType::HP, { {StatType::Spd,16}, {StatType::Atk_p,7}, {StatType::CDmg,13}, } ),
//			Artefact( ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::HP,197}, {StatType::CDmg,15}, {StatType::Spd,12}, {StatType::Acc,10}, } ),
//			Artefact( ArtType::Gloves, ArtSet::Def, 5, 16, StatType::Def_p, { {StatType::Spd,13}, {StatType::Acc,9}, {StatType::CRate,6}, {StatType::Res,8}, } ),
//			Artefact( ArtType::Chest, ArtSet::Speed, 5, 8, StatType::HP_p, { {StatType::HP,418}, {StatType::Spd,10}, } ),
//			Artefact( ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::Def_p,9}, {StatType::Acc,19}, {StatType::Res,19}, {StatType::Atk,16}, } ),
//			Artefact( ArtType::Ring, ArtSet::Deads, 4, 12, StatType::Def, { {StatType::HP_p,13}, {StatType::Atk_p,4}, {StatType::Def_p,4}, } ),
//			Artefact( ArtType::Banner, ArtSet::Deads, 5, 13, StatType::Def, { {StatType::Atk_p,16}, {StatType::Def_p,6}, {StatType::Spd,10}, } ),
//	};
//	{
//		Champion ch = Champion::ByName( ChampionName::Gorgorab );
//		const MatchOptions matching(
//			{
//				{ StatType::HP,   { 31000, MatchOptions::StatInfluence::Major } },
//				{ StatType::Def,  { 2800, MatchOptions::StatInfluence::Max } },
//				{ StatType::Spd,  { 240, MatchOptions::StatInfluence::Max } },
//			}
//			, { ArtSet::Speed, ArtSet::Speed }
//			, { ArtSet::Atk, ArtSet::DivAtk, ArtSet::Cruel }
//			, { ChampionName::Hatun, ChampionName::Lekar, ChampionName::Mavzolejnik }
//		);
//		Equipment eq;
//		FindBestEquipment( inventory, ch, matching, eq );
//		BOOST_CHECK_EQUAL( eq.Size(), 8 );
//
//		const ChampionStats final_stats = FinalStats( ch, eq );
//		BOOST_TEST_MESSAGE( to_string( final_stats ) );
//		BOOST_CHECK_GT( final_stats.Def, matching.Factor(StatType::Def).MinCap );
//	}
//}

#ifdef _DEBUG
/*
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
*/
#endif

BOOST_AUTO_TEST_CASE( test_champ_relations )
{
	const ChampionStats zaliv = GetCurrentFinalStatsFor( ChampionName::Gorgorab/*Hatun*/, true );
	const ChampionStats raskol = GetCurrentFinalStatsFor( ChampionName::Zargala, true );
	const ChampionStats dd = GetCurrentFinalStatsFor( ChampionName::Foly, true );
	const ChampionStats dd2 = GetCurrentFinalStatsFor( ChampionName::Rotos, true );
	BOOST_CHECK_GT( zaliv.Spd, raskol.Spd );
	BOOST_CHECK_GT( raskol.Spd, dd.Spd );
	BOOST_CHECK_GT( raskol.Spd, dd2.Spd );
}

//#ifndef DEBUG_FIND_BEST

//BOOST_AUTO_TEST_CASE( test_StatsCalculation )
//{
//	{
//		ChampionExt ch = Champion::ByName( ChampionName::Gromoboy );
//		const Equipment eq = GetCurrentEquipmentFor( ChampionName::Gromoboy );
//		ApplyEquipment( eq, ch.BasicStats, ch.ArtsBonusStats, false, true, false );
//
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.HP, 15207 - 2 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Atk, 305 - 1 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Def, 2295 - 2 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Spd, 51 + 1 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CRate, 47 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.CDmg, 6 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Res, 42 );
//		BOOST_CHECK_EQUAL( ch.ArtsBonusStats.Acc, 90 );
//
//		ChampionStats hall_stats;
//		ApplyHallBonus( ch, hall_stats );
//		BOOST_CHECK_EQUAL( hall_stats.HP, 317 );
//		BOOST_CHECK_EQUAL( hall_stats.Atk, 15 - 1 );
//		BOOST_CHECK_EQUAL( hall_stats.Def, 43 );
//		BOOST_CHECK_EQUAL( hall_stats.Spd, 0 );
//		BOOST_CHECK_EQUAL( hall_stats.CRate, 0 );
//		BOOST_CHECK_EQUAL( hall_stats.CDmg, 2 );
//		BOOST_CHECK_EQUAL( hall_stats.Res, 5 );
//		BOOST_CHECK_EQUAL( hall_stats.Acc, 40 );
//	}
//}
//#endif

#define BOOST_CHECK_INTERVAL( v, mn, mx ) \
		BOOST_CHECK_GE( v, mn ); \
		BOOST_CHECK_LE( v, mx );

#ifndef PROFILING
BOOST_AUTO_TEST_CASE( test_speed_interval )
{
	{
		ChampionExt ch = Champion::ByName( ChampionName::Fakhrakin );
		ApplyStat( {StatType::Spd,5,2}, ch.BasicStats, ch.ArtsBonusStats, true );
		const ChampionStats final_stats = ch.TotalStats();
		BOOST_CHECK( final_stats.SpdExt > 0 );
		BOOST_CHECK_EQUAL( final_stats.SpdExt, 2 );
		BOOST_CHECK( !final_stats.SuitsSpdInterval( 100, 106 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 100, 107 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 106, 108 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 107, 108 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 108, 109 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 105, 109 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 108, 120 ) );
		BOOST_CHECK( final_stats.SuitsSpdInterval( 109, 120 ) );
		BOOST_CHECK( !final_stats.SuitsSpdInterval( 110, 120 ) );
	}
	{
		const int min_spd = 180;
		const int max_spd = 185;
		MatchOptions matching(
			{
				{ StatType::HP,  { MatchOptions::StatInfluence::Max } },
				{ StatType::Spd, { min_spd, MatchOptions::StatInfluence::StrictInterval, max_spd } },
			}
			, { ArtSet::Immortal }
		);
		matching.AllowSets( { ArtSet::Immortal } );
		BOOST_CHECK( matching.IsInputOK() );

		ChampionExt ch = Champion::ByName( ChampionName::Fakhrakin );
		std::vector<Equipment> best_eq_pool;
		FindBestEquipment2( _MyArts, ch, matching, best_eq_pool );
		BOOST_CHECK( !best_eq_pool.empty() );
		BOOST_CHECK_LE( best_eq_pool.size(), EqEstPool::DefaultSize );
		ApplyEquipment( best_eq_pool.front(), ch.BasicStats, ch.ArtsBonusStats, false, true, true );
		const ChampionStats stats = ch.TotalStats();
		BOOST_CHECK( stats.SuitsSpdInterval( min_spd, max_spd ) );
	}
}
#endif

#ifdef _DEBUG
BOOST_AUTO_TEST_CASE( display_current_set )
{
	extern const ChampionName Champion_to_suitup;
	const ChampionName name = Champion_to_suitup;
	//const ChampionName name = ChampionName::Gromoboy;

	const Equipment eq = GetCurrentEquipmentFor( name );
	BOOST_TEST_MESSAGE( "\nCurrent equipment of " << to_string(name) << ":" );
	BOOST_TEST_MESSAGE( to_string( eq, false ) );
	const ChampionStats stats = GetCurrentFinalStatsFor( name, SuitUp::ConsiderGlyphsInReport );
	BOOST_TEST_MESSAGE( to_string( stats ) );
}
#endif

#endif
