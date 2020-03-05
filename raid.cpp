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

	switch ( art )
	{
		case ArtType::Weapon:
		{
			const int values[3][17] = {
				//      0   1           4                8                  12                  16
				/*4*/{ 16, 25, 33, 42, 51, 59,  0,  0,  85,  94,   0,   0, 120, 129,   0,   0, 190 },
				/*5*/{ 25, 35, 45, 55, 64, 74, 84, 94, 104, 114, 124, 134, 143, 153, 164, 174, 225 },
				/*6*/{ 35, 46,  0,  0,  0,  0,  0,  0, 125, 136,   0,   0,   0,   0,   0,   0, 265 },
			};
			return values[starRank - 4][level];
		}
		break;
	case ArtType::Helmet:
		break;
	case ArtType::Shield:
		break;
	case ArtType::Gloves:
		break;
	case ArtType::Chest:
		break;
	case ArtType::Boots:
		break;
	case ArtType::Ring:
		break;
	case ArtType::Necklace:
		break;
	case ArtType::Banner:
		break;
	}
	assert( false );
	return 0;
}

void FindBestEquipment( Champion& ch, const MatchOptions& matching, Equipment& eq )
{
}

/////////////////////////////////////////////////////////////////////////////

Champion ChampionFactory::Gromoboy()
{
	return Champion({ 15855, 727, 1443,  97,  15, 50,  30, 0 });
}
