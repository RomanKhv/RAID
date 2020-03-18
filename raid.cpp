#include "pch.h"
#include "raid.h"
#include <boost/algorithm/clamp.hpp>
#include "stl_ext.h"

/////////////////////////////////////////////////////////////////////////////

Artefact::Artefact( ArtType type, ArtSet set, int stars, int level, StatType mainstat, std::vector<Stat> addstats,
					ChampionName owner)
	:Type(type)
	,Set(set)
	,Stars(stars)
	,Level(level)
	,MainStat(mainstat)
	,AddStats( addstats.begin(), addstats.end() )
	,Owner(owner)
{
	_ASSERTE( IsValidStatForArt( mainstat, type ) );
	_ASSERTE( IsGoodStatForArt( mainstat, type ) );
}

Stat Artefact::GetMainStat( bool consider_max_level ) const
{
	return {
		MainStat,
		StatValueForLevel( Type, MainStat, Stars, consider_max_level ? 16 : Level )
	};
}

/////////////////////////////////////////////////////////////////////////////

Equipment::Equipment( std::initializer_list<Artefact> il )
{
	for ( const Artefact& a : il )
	{
		_ASSERTE( !Initialized( a.Type ) );
		this->operator[]( a.Type ) = a;
	}
}

void Equipment::Clear()
{
	for ( Artefact& art : Arts )
		art = Artefact();
}

size_t Equipment::Size() const
{
	size_t n_arts = 0;
	for ( const Artefact& art : Arts )
		if ( art.Initialized() )
			n_arts++;
	return n_arts;
}

/////////////////////////////////////////////////////////////////////////////

const ChampionStats::StatList ChampionStats::TypeList = {
	StatType::HP, StatType::Atk, StatType::Def, StatType::Spd, StatType::CRate, StatType::CDmg, StatType::Res, StatType::Acc
};

ChampionStats::ChampionStats( int hp, int atk, int def, int spd, int crate, int cdmg, int res, int acc )
	:HP(hp)
	,Atk(atk)
	,Def(def)
	,Spd(spd)
	,CRate(crate)
	,CDmg(cdmg)
	,Res(res)
	,Acc(acc)
{
}

template <int ChampionStats::* member>
void member_summ( ChampionStats& dest, const ChampionStats& lhs, const ChampionStats& rhs )
{
	dest.*member = lhs.*member + rhs.*member;
}

ChampionStats ChampionStats::operator+( const ChampionStats& rhs ) const
{
	ChampionStats summ;
	for ( int i=0; i<Count; ++i )
		summ.Values[i] = Values[i] + rhs.Values[i];
	//member_summ<&ChampionStats::HP>( summ, *this, rhs );
	//member_summ<&ChampionStats::Atk>( summ, *this, rhs );
	//member_summ<&ChampionStats::Def>( summ, *this, rhs );
	//member_summ<&ChampionStats::CRate>( summ, *this, rhs );
	//member_summ<&ChampionStats::CDmg>( summ, *this, rhs );
	//member_summ<&ChampionStats::Spd>( summ, *this, rhs );
	//member_summ<&ChampionStats::Acc>( summ, *this, rhs );
	//member_summ<&ChampionStats::Res>( summ, *this, rhs );
	return summ;
}

//int ChampionStats::operator[]( StatType stat ) const
//{
//	switch ( stat )
//	{
//		case StatType::HP:
//		case StatType::HP_p:
//			return HP;
//		case StatType::Atk:
//		case StatType::Atk_p:
//			return Atk;
//		case StatType::Def:
//		case StatType::Def_p:
//			return Def;
//		case StatType::Spd:
//			return Spd;
//		case StatType::CRate:
//			return CRate;
//		case StatType::CDmg:
//			return CDmg;
//		case StatType::Res:
//			return Res;
//		case StatType::Acc:
//			return Acc;
//	}
//	_ASSERT(false);
//	return 0;
//}

template <int ChampionStats::* member>
void member_fraction( ChampionStats& dest, const ChampionStats& ref, int factor_percent )
{
	dest.*member += ref.*member * factor_percent / 100;
}

/////////////////////////////////////////////////////////////////////////////

inline Champion::Champion( const ChampionStats& basic, Element e )
	: BasicStats( basic )
	, Elem( e )
{
}

bool Champion::IsReal() const
{
	return Elem != Element::none;
}

ChampionStats Champion::TotalStats(bool apply_hall_bonus) const
{
	ChampionStats stats = BasicStats + ArtsBonusStats;

	if ( apply_hall_bonus )
	{
		_ASSERTE( IsReal() );
		ApplyHallBonus( *this, stats );
	}

	return stats;
}

/////////////////////////////////////////////////////////////////////////////

std::vector<StatType> StatTypesForArt( ArtType art )
{
	std::vector<StatType> valid_stats;
	switch ( art )
	{
	case ArtType::Weapon:
		valid_stats = { StatType::Atk };
		break;
	case ArtType::Helmet:
		valid_stats = { StatType::HP };
		break;
	case ArtType::Shield:
		valid_stats = { StatType::Def };
		break;
	case ArtType::Gloves:
		valid_stats = { StatType::Atk, StatType::Atk_p, StatType::HP, StatType::HP_p, StatType::Def, StatType::Def_p, StatType::CRate, StatType::CDmg };
		break;
	case ArtType::Chest:
		valid_stats = { StatType::Atk, StatType::Atk_p, StatType::HP, StatType::HP_p, StatType::Def, StatType::Def_p, StatType::Acc, StatType::Res };
		break;
	case ArtType::Boots:
		valid_stats = { StatType::Atk, StatType::Atk_p, StatType::HP, StatType::HP_p, StatType::Def, StatType::Def_p, StatType::Spd };
		break;
	case ArtType::Ring:
		valid_stats = { StatType::HP, StatType::Atk, StatType::Def };
		break;
	case ArtType::Necklace:
		valid_stats = { StatType::CDmg, StatType::HP, StatType::Atk, StatType::Def };
		break;
	case ArtType::Banner:
		valid_stats = { StatType::Acc, StatType::Res, StatType::HP, StatType::Atk, StatType::Def };
		break;
	}
	return valid_stats;
}

/////////////////////////////////////////////////////////////////////////////

int StatValueForLevel( ArtType art, StatType stat, int starRank, int level )
{
	typedef int table_t[3][5];
	const table_t* stat_table = nullptr;

	switch ( stat )
	{
		case StatType::Atk:
		case StatType::Def:
			if ( art != ArtType::Banner )
			{
				static const table_t weapon_shield = {
					//      0   4    8   12   16
					/*4*/{ 16, 51,  85, 120, 190 },
					/*5*/{ 25, 64, 104, 143, 225 },
					/*6*/{ 35, 80, 125, 170, 265 }
				};
				stat_table = &weapon_shield;
			}
			else {
				static const int banner[3][5] = {
					//      0   4    8   12   16
					/*4*/{ 24, 0,  0, 0, 0 },
					/*5*/{ 38, 0, 0, 0, 0 },
					/*6*/{ 53, 0, 0, 0, 0 }
				};
				stat_table = &banner;
			}
			break;

		case StatType::HP:
			if ( art != ArtType::Banner )
			{
				static const int helmet[3][5] = {
					//      0     4     8    12    16
					/*4*/{ 260,    0, 1286, 1800, 2840 },
					/*5*/{ 450, 1044, 1638, 2231, 3480 },
					/*6*/{ 600, 1277, 1954, 2631, 4080 }
				};
				stat_table = &helmet;
			}
			else {
				static const int banner[3][5] = {
					//      0     4     8    12    16
					/*4*/{ 390,    0, 0, 0, 0 },
					/*5*/{ 675, 0, 0, 0, 0 },
					/*6*/{ 900, 0, 0, 0, 0 }
				};
				stat_table = &banner;
			}
			break;

		case StatType::Atk_p:
		case StatType::HP_p:
		case StatType::Def_p:
		case StatType::CRate:
			{
				static const int values_Atk_HP_Def_CR_p[3][5] = {
					//      0   4   8  12  16
					/*4*/{  6, 13, 19, 26, 40 },
					/*5*/{  8, 16, 24, 33, 50 },
					/*6*/{ 10, 20, 29, 39, 60 },
				};
				stat_table = &values_Atk_HP_Def_CR_p;
			}
			break;

		case StatType::CDmg:
			if ( art != ArtType::Necklace )
			{
				static const int values_CDmg[3][5] = {
					//      0  4  8 12  16
					/*4*/{  8, 0, 0, 0,  0 },
					/*5*/{ 10, 0, 0, 0, 65 },
					/*6*/{  0, 0, 0, 0,  0 },
				};
				stat_table = &values_CDmg;
			}
			else {
				static const int necklace[3][5] = {
					//     0  4  8 12  16
					/*4*/{ 4, 0, 0, 0, 0 },
					/*5*/{ 5, 0, 0, 0, 0 },
					/*6*/{ 6, 0, 0, 0, 0 },
				};
				stat_table = &necklace;
			}
			break;

		case StatType::Spd:
			{
				static const int values_Spd[3][5] = {
					//     0   4   8  12  16
					/*4*/{ 4, 10, 16,  0, 35 },
					/*5*/{ 5, 12, 19, 26, 40 },
					/*6*/{ 0, 00, 00, 00, 00 },
				};
				stat_table = &values_Spd;
			}
			break;

		case StatType::Acc:
		case StatType::Res:
			{
				static const table_t values_Acc_Res = {
					//      0   4   8  12  16
					/*4*/{  8, 0, 0, 0, 60 },
					/*5*/{ 12, 0, 0, 0, 78 },
					/*6*/{ 16, 0, 0, 0, 96 },
				};
				stat_table = &values_Acc_Res;
			}
			break;
	}

	//starRank = boost::algorithm::clamp( starRank, 1, 6 );
	//level = boost::algorithm::clamp( level, 0, 16 );
	_ASSERTE( 4 <= starRank && starRank <= 6 );
	_ASSERTE( 0 <= level && level <= 16 );

	const int value = (*stat_table)[starRank - 4][level / 4];
	_ASSERTE( value > 0 );
	return value;
}

/////////////////////////////////////////////////////////////////////////////

int SetSize( ArtSet set )
{
	switch ( set )
	{
		case ArtSet::HP:
		case ArtSet::Atk:
		case ArtSet::Def:
		case ArtSet::CRate:
		case ArtSet::CDmg:
		case ArtSet::Speed:
		case ArtSet::Res:
		case ArtSet::Acc:
		case ArtSet::Cruel:
		case ArtSet::Immortal:
		case ArtSet::DivAtk:
		case ArtSet::DivCritRate:
		case ArtSet::DivLife:
		case ArtSet::DivSpeed:
			return 2;
	}
	return 4;
}

bool IsValidStatForArt( StatType stat, ArtType art )
{
	const std::vector<StatType> valid_stats = StatTypesForArt( art );
	return std::find( valid_stats.begin(), valid_stats.end(), stat ) != valid_stats.end();
}

bool IsGoodStatForArt( StatType stat, ArtType art )
{
	switch ( art )
	{
		case ArtType::Gloves:
		case ArtType::Chest:
		case ArtType::Boots:
		{
			switch ( stat )
			{
				case StatType::Atk:
				case StatType::HP:
				case StatType::Def:
					return false;
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

#define ApplyStatBonus( bonusStats, basicStats, stat, factor )			bonusStats.stat += basicStats.stat * (factor) / 100;

void ApplyStat( const Stat& stat, const ChampionStats& basic_stats, ChampionStats& arts_bonus )
{
	switch ( stat.Type )
	{
		case StatType::HP_p:
		case StatType::Atk_p:
		case StatType::Def_p:
			arts_bonus.p_stat(stat.Type) = basic_stats.p_stat(stat.Type) * stat.Value / 100;
			return;
		default:
			arts_bonus[stat.Type] += stat.Value;
			return;
	}
	//switch ( stat.Type )
	//{
	//	case StatType::Atk:
	//		arts_bonus.Atk += stat.Value;
	//		return;
	//	case StatType::HP:
	//		arts_bonus.HP += stat.Value;
	//		return;
	//	case StatType::Def:
	//		arts_bonus.Def += stat.Value;
	//		return;
	//	case StatType::Atk_p:
	//		member_fraction<&ChampionStats::Atk>( arts_bonus, basic_stats, stat.Value );
	//		return;
	//	case StatType::HP_p:
	//		member_fraction<&ChampionStats::HP>( arts_bonus, basic_stats, stat.Value );
	//		return;
	//	case StatType::Def_p:
	//		member_fraction<&ChampionStats::Def>( arts_bonus, basic_stats, stat.Value );
	//		return;
	//	case StatType::CRate:
	//		arts_bonus.CRate += stat.Value;
	//		return;
	//	case StatType::CDmg:
	//		arts_bonus.CDmg += stat.Value;
	//		return;
	//	case StatType::Spd:
	//		arts_bonus.Spd += stat.Value;
	//		return;
	//	case StatType::Acc:
	//		arts_bonus.Acc += stat.Value;
	//		return;
	//	case StatType::Res:
	//		arts_bonus.Res += stat.Value;
	//		return;
	//}
	_ASSERTE( !"unreachable code" );
}

void ApplyStat( const Stat& stat, Champion& ch )
{
	ApplyStat( stat, ch.BasicStats, ch.ArtsBonusStats );
}

/////////////////////////////////////////////////////////////////////////////

const int DivHPCompensation = 2;

void ApplySetBonus( ArtSet set, Champion& ch, bool compensation )
{
	switch ( set )
	{
		case ArtSet::HP:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, HP, 15 );
			}
			break;
		case ArtSet::Immortal:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, HP, 15 + (compensation ? 3 : 0) );
			}
			break;
		case ArtSet::DivLife:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, HP, 15 + (compensation ? DivHPCompensation : 0) );
			}
			break;
		case ArtSet::Atk:
		case ArtSet::Cruel:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, Atk, 15 );
			}
			break;
		case ArtSet::DivAtk:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, Atk, 15 );
				if ( compensation )
					ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, HP, DivHPCompensation );
			}
			break;
		case ArtSet::Def:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, Def, 15 );
			}
			break;
		case ArtSet::CRate:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, CRate, 12 );
			}
			break;
		case ArtSet::DivCritRate:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, CRate, 12 );
				if ( compensation )
					ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, HP, DivHPCompensation );
			}
			break;
		case ArtSet::CDmg:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, CDmg, 20 );
			}
			break;
		case ArtSet::Speed:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, Spd, 12 );
			}
			break;
		case ArtSet::DivSpeed:
			{
				ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, Spd, 12 );
				if ( compensation )
					ApplyStatBonus( ch.ArtsBonusStats, ch.BasicStats, HP, DivHPCompensation );
			}
			break;
		case ArtSet::Res:
			{
				ch.ArtsBonusStats.Res += 40;
			}
			break;
		case ArtSet::Acc:
			{
				ch.ArtsBonusStats.Acc += 40;
			}
			break;
	}
}

void ApplySetsBonuses( const Equipment& eq, Champion& ch, bool compensation )
{
	int n_arts_by_set[Artefact::SetCount] = {0};
	for ( const Artefact& art : eq.Arts )
	{
		if ( art.Initialized() )
			n_arts_by_set[stl::enum_to_int(art.Set)]++;
	}
	for ( int set = 0; set < Artefact::SetCount; ++set )
	{
		const int count = n_arts_by_set[set] / SetSize( static_cast<ArtSet>(set) );
		for ( int i = 0; i < count; ++i )
			ApplySetBonus( static_cast<ArtSet>(set), ch, compensation );
	}
}

void ApplyArtBonus( const Artefact& art, Champion& ch, bool consider_max_level )
{
	if ( !art.Initialized() )
	{
		_ASSERTE( !"invalid art" );
		return;
	}

	ApplyStat( art.GetMainStat(consider_max_level), ch );

	for ( const Stat& stat : art.AddStats )
		ApplyStat( stat, ch );
}

void ApplyEquipment( const Equipment& eq, Champion& ch, bool estimating )
{
	ApplySetsBonuses( eq, ch, estimating );

	for ( const Artefact& art : eq.Arts )
	{
		if ( art.Initialized() )
		{
			ApplyArtBonus( art, ch, estimating );
		}
	}
}

void ApplyHallBonus( const Champion& ch, ChampionStats& stats )
{
	const std::map<StatType, int>& hall_bonus = _MyHall.Table[ stl::enum_to_int(ch.Elem) ];

	for ( StatType st : { StatType::HP_p, StatType::Atk_p, StatType::Def_p, StatType::CDmg, StatType::Res, StatType::Acc } )
	{
		ApplyStat( { st, hall_bonus.at(st) }, ch.BasicStats, stats );
	}
}

/////////////////////////////////////////////////////////////////////////////

Hall::Hall( std::map<Element, std::map<StatType, int>> m )
{
	for ( const auto& e : m )
		for ( const auto& s : e.second )
		{
			Table[stl::enum_to_int(e.first)][s.first] = s.second;
		}
}

/////////////////////////////////////////////////////////////////////////////

MatchOptions::MatchOptions( std::map<StatType, ArtFactor> factors, std::vector<ArtSet> req_filter, std::vector<ArtSet> set_filter,
							bool consider_max_lvl, std::map<StatType, int> min_caps )
	:Factors( std::move( factors ) )
	,RequiedSets( std::move( req_filter ) )
	,SetFilter( std::move( set_filter ) )
	,ConsiderMaxLevels( consider_max_lvl )
	,MinCap( std::move( min_caps ) )
{
}

bool MatchOptions::IsSetAccepted( ArtSet set ) const
{
	if ( SetFilter.empty() )
		return true;	//accept all

	return stl::contains( SetFilter, set );
}

bool MatchOptions::IsEqHasRequiredSets( const Equipment& eq ) const
{
	if ( RequiedSets.empty() )
		return true;

	std::map<ArtSet,int> req_sets;
	int total_req_art_count = 0;
	for ( ArtSet set : RequiedSets )
	{
		req_sets[set] ++;
		total_req_art_count += SetSize( set );
	}
	_ASSERTE( total_req_art_count <= 6 );
	if ( total_req_art_count > 6 )
		return false;

	std::map<ArtSet,int> eq_sets;
	for ( const Artefact& art : eq.Arts )
		eq_sets[art.Set] ++;

	for ( const auto& rs : req_sets )
	{
		const int n_req_arts = SetSize( rs.first ) * rs.second;
		const int n_eq_arts = stl::get_value_or( eq_sets, rs.first, 0 );
		if ( n_eq_arts < n_req_arts )
			return false;
	}

	return true;
}

//bool MatchOptions::IsEqAccepted( const Equipment& eq ) const
//{
//	for ( const auto a : eq )
//	{
//		if ( !IsSetAccepted( a.second.Set ) )
//			return false;
//	}
//	return IsEqHasRequiredSets( eq );
//}

/////////////////////////////////////////////////////////////////////////////

Equipment GetCurrentEquipmentFor( ChampionName name )
{
	Equipment eq;
	for ( const Artefact& art : _MyArts )
	{
		if ( art.Owner == name )
			eq[art.Type] = art;
	}
	return eq;
}

Champion ChampionFactory::ColdHeart()
{
	return Champion( { 13710, 1376, 738,  94,  15, 57,  30, 0 }, Element::Void );
}

Champion ChampionFactory::Kael()
{
	return Champion( { 13710, 1200, 914,  103,  15, 57,  30, 0 }, Element::Blue );
}

Champion ChampionFactory::Gromoboy()
{
	return Champion( { 15855, 727, 1443,  97,  15, 50,  30, 0 }, Element::Void );
}

Champion ChampionFactory::Lekar()
{
	return Champion( { 16680, 859, 969,  101,  15, 50,  30, 0 }, Element::Blue );
}

Champion ChampionFactory::Yuliana()
{
	return Champion( { 15195, 1354, 870,  103,  15, 50,  30, 0 }, Element::Blue );
}

Champion ChampionFactory::Krisk()
{
	return Champion( { 18660, 727, 1465,  94,  15, 50,  30, 0 }, Element::Void );
}

Champion ChampionFactory::Hatun()
{
	return Champion( { 15555, 971, 1146,  97,  15, 50,  30, 0 }, Element::Green );
}
