#include "pch.h"
#include "raid.h"
#include <boost/algorithm/clamp.hpp>

/////////////////////////////////////////////////////////////////////////////

Artefact::Artefact( ArtType type, ArtSet set, int stars, int level, StatType mainstat, std::vector<Stat> addstats )
	:Type(type)
	,Set(set)
	,Stars(stars)
	,Level(level)
	,MainStat(mainstat)
	,AddStats(addstats)
{
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
		valid_stats = { StatType::Atk, StatType::Atk_P, StatType::HP, StatType::HP_P, StatType::Def, StatType::Def_P, StatType::CritRate, StatType::CritDmg };
		break;
	case ArtType::Chest:
		valid_stats = { StatType::Atk, StatType::Atk_P, StatType::HP, StatType::HP_P, StatType::Def, StatType::Def_P, StatType::Acc, StatType::Res };
		break;
	case ArtType::Boots:
		valid_stats = { StatType::Atk, StatType::Atk_P, StatType::HP, StatType::HP_P, StatType::Def, StatType::Def_P, StatType::Spd };
		break;
	case ArtType::Ring:
		assert( !"implemented" );
		break;
	case ArtType::Necklace:
		assert( !"implemented" );
		break;
	case ArtType::Banner:
		assert( !"implemented" );
		break;
	}
	return valid_stats;
}

/////////////////////////////////////////////////////////////////////////////

const int DivHPCompensation = 2;

#define ApplyStatBonus( bonusStats, basicStats, stat, factor )			bonusStats.stat += basicStats.stat * (100 + (factor)) / 100;

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
		case ArtSet::CritRate:
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
		case ArtSet::CritDmg:
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

int StatValueForLevel( ArtType art, StatType stat, int starRank, int level )
{
	starRank = boost::algorithm::clamp( starRank, 1, 6 );
	level = boost::algorithm::clamp( level, 0, 16 );

	switch ( stat )
	{
		case StatType::Atk:
		case StatType::Def:
			{
				const int values_Weapon_Shield[3][5] = {
					//      0   4    8   12   16
					/*4*/{ 16, 51,  85, 120, 190 },
					/*5*/{ 25, 64, 104, 143, 225 },
					/*6*/{ 35, 80, 125, 170, 265 }
				};
				return values_Weapon_Shield[starRank - 4][level / 4];
			}
		case StatType::HP:
			{
				const int values_Helmet[3][5] = {
					//      0     4     8    12    16
					/*4*/{ 260,    0, 1286, 1800, 2840 },
					/*5*/{ 450, 1044, 1638, 2231, 3480 },
					/*6*/{ 600, 1277, 1954, 2631, 4080 }
				};
				return values_Helmet[starRank - 4][level / 4];
			}
		case StatType::Atk_P:
		case StatType::HP_P:
		case StatType::Def_P:
		case StatType::CritRate:
			{
				const int values_Atk_HP_Def_CR_p[3][5] = {
					//      0   4   8  12  16
					/*4*/{  6, 13, 19, 26, 40 },
					/*5*/{  8, 16, 24, 33, 50 },
					/*6*/{ 10, 20, 29, 39, 60 },
				};
				return values_Atk_HP_Def_CR_p[starRank - 4][level / 4];
			}
		case StatType::CritDmg:
			{
				const int values_CDmg[3][5] = {
					//      0  4  8 12  16
					/*4*/{  8, 0, 0, 0,  0 },
					/*5*/{ 10, 0, 0, 0, 65 },
					/*6*/{  0, 0, 0, 0,  0 },
				};
				return values_CDmg[starRank - 4][level / 4];
			}
		case StatType::Spd:
			{
				const int values_Spd[3][5] = {
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
			}
	}

	assert( false );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

Champion ChampionFactory::Gromoboy()
{
	return Champion({ 15855, 727, 1443,  97,  15, 50,  30, 0 });
}
