#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

Artefact::Artefact( ArtefactType type, ArtSet set, int stars, int level, StatType mainstat, std::vector<Stat> addstats )
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

void ApplySetBonus( ArtSet set, Champion& ch )
{
	switch ( set )
	{
		case ArtSet::HP:
		case ArtSet::Immortal:
		case ArtSet::DivLife:
			{
				ch.BonusStats.HP += ch.BasicStats.HP * 115 / 100;
			}
			break;
		case ArtSet::Atk:
		case ArtSet::Cruel:
		case ArtSet::DivAtk:
			{
				ch.BonusStats.Atk += ch.BasicStats.Atk * 115 / 100;
			}
			break;
		case ArtSet::Def:
			{
				ch.BonusStats.Def += ch.BasicStats.Def * 115 / 100;
			}
			break;
		case ArtSet::CritRate:
		case ArtSet::DivCritRate:
			{
				ch.BonusStats.CRate += ch.BasicStats.CRate * 112 / 100;
			}
			break;
		case ArtSet::Acc:
			{
				ch.BonusStats.CRate += 40;
			}
			break;
		case ArtSet::Speed:
		case ArtSet::DivSpeed:
			{
				ch.BonusStats.Spd += ch.BasicStats.Spd * 112 / 100;
			}
			break;
		case ArtSet::Res:
			{
				ch.BonusStats.Res += 40;
			}
			break;
		case ArtSet::CritDmg:
			{
				ch.BonusStats.CDmg += ch.BasicStats.CDmg * 120 / 100;
			}
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////

Champion ChampionFactory::Gromoboy()
{
	return Champion({ 15855, 727, 1443,  97,  15, 50,  30, 0 });
}
