#include "pch.h"
#include "raid.h"
#include <boost/algorithm/clamp.hpp>
#include "stl_ext.h"

/////////////////////////////////////////////////////////////////////////////

Artefact::Artefact( ArtType type, ArtSet set, int stars, int level, StatType mainstat, std::vector<Stat> addstats )
	:Type(type)
	,Set(set)
	,Stars(stars)
	,Level(level)
	,MainStat(mainstat)
	,AddStats(addstats)
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
		_ASSERTE( this->count( a.Type ) == 0 );
		this->operator[]( a.Type ) = a;
	}
}

/////////////////////////////////////////////////////////////////////////////

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
	member_summ<&ChampionStats::HP>( summ, *this, rhs );
	member_summ<&ChampionStats::Atk>( summ, *this, rhs );
	member_summ<&ChampionStats::Def>( summ, *this, rhs );
	member_summ<&ChampionStats::CRate>( summ, *this, rhs );
	member_summ<&ChampionStats::CDmg>( summ, *this, rhs );
	member_summ<&ChampionStats::Spd>( summ, *this, rhs );
	member_summ<&ChampionStats::Acc>( summ, *this, rhs );
	member_summ<&ChampionStats::Res>( summ, *this, rhs );
	return summ;
}

template <int ChampionStats::* member>
void member_fraction( ChampionStats& dest, const ChampionStats& ref, int factor_percent )
{
	dest.*member += ref.*member * factor_percent / 100;
}

/////////////////////////////////////////////////////////////////////////////

ChampionStats Champion::TotalStats() const
{
	return BasicStats + BonusStats;
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
		_ASSERTE( !"not implemented" );
		break;
	case ArtType::Necklace:
		_ASSERTE( !"not implemented" );
		break;
	case ArtType::Banner:
		_ASSERTE( !"not implemented" );
		break;
	}
	return valid_stats;
}

int StatValueForLevel( ArtType art, StatType stat, int starRank, int level )
{
	starRank = boost::algorithm::clamp( starRank, 1, 6 );
	level = boost::algorithm::clamp( level, 0, 16 );
	level = (level / 4) * 4;

	switch ( stat )
	{
		case StatType::Atk:
		case StatType::Def:
			{
				static const int values_Weapon_Shield[3][5] = {
					//      0   4    8   12   16
					/*4*/{ 16, 51,  85, 120, 190 },
					/*5*/{ 25, 64, 104, 143, 225 },
					/*6*/{ 35, 80, 125, 170, 265 }
				};
				return values_Weapon_Shield[starRank - 4][level / 4];
			}
		case StatType::HP:
			{
				static const int values_Helmet[3][5] = {
					//      0     4     8    12    16
					/*4*/{ 260,    0, 1286, 1800, 2840 },
					/*5*/{ 450, 1044, 1638, 2231, 3480 },
					/*6*/{ 600, 1277, 1954, 2631, 4080 }
				};
				return values_Helmet[starRank - 4][level / 4];
			}
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
				return values_Atk_HP_Def_CR_p[starRank - 4][level / 4];
			}
		case StatType::CDmg:
			{
				static const int values_CDmg[3][5] = {
					//      0  4  8 12  16
					/*4*/{  8, 0, 0, 0,  0 },
					/*5*/{ 10, 0, 0, 0, 65 },
					/*6*/{  0, 0, 0, 0,  0 },
				};
				return values_CDmg[starRank - 4][level / 4];
			}
		case StatType::Spd:
			{
				static const int values_Spd[3][5] = {
					//     0   4   8  12  16
					/*4*/{ 4, 10, 16,  0, 35 },
					/*5*/{ 5, 12, 19, 26, 40 },
					/*6*/{ 0, 00, 00, 00, 00 },
				};
				return values_Spd[starRank - 4][level / 4];
			}
		case StatType::Acc:
		case StatType::Res:
			{
				static const int values_Acc_Res[3][5] = {
					//      0   4   8  12  16
					/*4*/{  8, 0, 0, 0, 0 },
					/*5*/{ 12, 0, 0, 0, 78 },
					/*6*/{ 16, 0, 0, 0, 0 },
				};
				return values_Acc_Res[starRank - 4][level / 4];
			}
	}

	_ASSERTE( false );
	return 0;
}

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

#define ApplyStatBonus( bonusStats, basicStats, stat, factor )			bonusStats.stat += basicStats.stat * (factor) / 100;

void ApplyStat( const Stat& stat, Champion& ch )
{
	switch ( stat.Type )
	{
		case StatType::Atk:
			ch.BonusStats.Atk += stat.Value;
			return;
		case StatType::HP:
			ch.BonusStats.HP += stat.Value;
			return;
		case StatType::Def:
			ch.BonusStats.Def += stat.Value;
			return;
		case StatType::Atk_p:
			member_fraction<&ChampionStats::Atk>( ch.BonusStats, ch.BasicStats, stat.Value );
			return;
		case StatType::HP_p:
			member_fraction<&ChampionStats::HP>( ch.BonusStats, ch.BasicStats, stat.Value );
			return;
		case StatType::Def_p:
			member_fraction<&ChampionStats::Def>( ch.BonusStats, ch.BasicStats, stat.Value );
			return;
		case StatType::CRate:
			member_fraction<&ChampionStats::CRate>( ch.BonusStats, ch.BasicStats, stat.Value );
			return;
		case StatType::CDmg:
			member_fraction<&ChampionStats::CDmg>( ch.BonusStats, ch.BasicStats, stat.Value );
			return;
		case StatType::Spd:
			ch.BonusStats.Spd += stat.Value;
			return;
		case StatType::Acc:
			ch.BonusStats.Acc += stat.Value;
			return;
		case StatType::Res:
			ch.BonusStats.Res += stat.Value;
			return;
	}
	_ASSERTE( !"unreachable code" );
}

/////////////////////////////////////////////////////////////////////////////

const int DivHPCompensation = 2;

void ApplySetBonus( ArtSet set, Champion& ch )
{
	switch ( set )
	{
		case ArtSet::HP:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, HP, 15 );
			}
			break;
		case ArtSet::Immortal:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, HP, 15+3 );
			}
			break;
		case ArtSet::DivLife:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, HP, 15+DivHPCompensation );
			}
			break;
		case ArtSet::Atk:
		case ArtSet::Cruel:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, Atk, 15 );
			}
			break;
		case ArtSet::DivAtk:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, Atk, 15 );
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, HP, DivHPCompensation );
			}
			break;
		case ArtSet::Def:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, Def, 15 );
			}
			break;
		case ArtSet::CRate:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, CRate, 12 );
			}
			break;
		case ArtSet::DivCritRate:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, CRate, 12 );
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, HP, DivHPCompensation );
			}
			break;
		case ArtSet::CDmg:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, CDmg, 20 );
			}
			break;
		case ArtSet::Speed:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, Spd, 12 );
			}
			break;
		case ArtSet::DivSpeed:
			{
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, Spd, 12 );
				ApplyStatBonus( ch.BonusStats, ch.BasicStats, HP, DivHPCompensation );
			}
			break;
		case ArtSet::Res:
			{
				ch.BonusStats.Res += 40;
			}
			break;
		case ArtSet::Acc:
			{
				ch.BonusStats.Acc += 40;
			}
			break;
	}
}

void ApplySetsBonuses( const Equipment& eq, Champion& ch )
{
	std::map<ArtSet, int> n_arts_by_set;
	for ( auto e : eq )
	{
		if ( e.second.IsValid() )
			n_arts_by_set[e.second.Set] += 1;
	}
	for ( auto set : n_arts_by_set )
	{
		const int count = set.second / SetSize( set.first );
		for ( int i = 0; i < count; ++i )
			ApplySetBonus( set.first, ch );
	}
}

void ApplyArtBonus( const Artefact& art, Champion& ch, bool consider_max_level )
{
	if ( !art.IsValid() )
	{
		_ASSERTE( !"invalid art" );
		return;
	}

	ApplyStat( art.GetMainStat(consider_max_level), ch );

	for ( const Stat& stat : art.AddStats )
		ApplyStat( stat, ch );
}

void ApplyEquipment( const Equipment& eq, Champion& ch )
{
	ApplySetsBonuses( eq, ch );

	for ( auto e : eq )
	{
		if ( e.second.IsValid() )
		{
			ApplyArtBonus( e.second, ch );
		}
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
	for ( const auto& a : eq )
		eq_sets[a.second.Set] ++;

	for ( const auto rs : req_sets )
	{
		const int n_req_arts = SetSize( rs.first ) * rs.second;
		const int n_eq_arts = stl::get_value( eq_sets, rs.first, 0 );
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

Champion ChampionFactory::ColdHeart()
{
	return Champion({ 13710, 1376, 738,  94,  15, 57,  30, 0 });
}

Champion ChampionFactory::Kael()
{
	return Champion({ 13710, 1200, 914,  103,  15, 57,  30, 0 });
}

Champion ChampionFactory::Gromoboy()
{
	return Champion({ 15855, 727, 1443,  97,  15, 50,  30, 0 });
}

Champion ChampionFactory::Lekar()
{
	return Champion({ 16680, 859, 969,  101,  15, 50,  30, 0 });
}

Champion ChampionFactory::Yuliana()
{
	return Champion({ 15195, 1354, 870,  103,  15, 50,  30, 0 });
}

Champion ChampionFactory::Krisk()
{
	return Champion({ 18660, 727, 1465,  94,  15, 50,  30, 0 });
}

Champion ChampionFactory::Hatun()
{
	return Champion({ 15555, 971, 1146,  97,  15, 50,  30, 0 });
}
