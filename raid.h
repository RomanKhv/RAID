#pragma once
#include <vector>
#include <map>

/////////////////////////////////////////////////////////////////////////////

enum class ArtefactType
{
	None,

	Weapon,
	Helmet,
	Shield,
	Gloves,
	Chest,
	Boots,

	Ring,
	Necklace,
	Banner
};

enum class StatType
{
	Atk,
	Atk_P,
	HP,
	HP_P,
	Def,
	Def_P,

	CritRate,
	CritDmg,

	Spd,
	Acc,
	Res,
};

struct Stat
{
	StatType Type;
	int Value = 0;
};

enum class ArtSet
{
	HP,
	Atk,
	Def,
	CritRate,
	Acc,
	Speed,
	Res,
	Vamp,			//4
	Destroy,
	Retaliation,
	Fury,
	Curing,
	Reflex,
	Cursed,
	Toxic,
	Frost,
	Daze,
	Avenging,
	Shield,
	Stalwart,
	CritDmg,		//2
	Frenzy,			//4
	Regeneration,
	Relentless,
	Savage,
	Taunting,
	Cruel,			//2
	Immortal,
	DivOffence,
	DivCritRate,
	DivLife,
	DivSpeed,
};

/////////////////////////////////////////////////////////////////////////////

struct Artefact
{
	ArtefactType Type = ArtefactType::Weapon;
	ArtSet Set;
	int Stars = 1;

	Stat MainStat;
	std::vector<Stat> AddStats;
};

/////////////////////////////////////////////////////////////////////////////

struct ChampionStats
{
	int HP = 0;
	int Atk = 0;
	int Def = 0;
	int Spd = 0;
	int CRate = 0;
	int CDmg = 0;
	int Res = 0;
	int Acc = 0;

	ChampionStats() = default;
	ChampionStats(int hp, int atk, int def, int spd, int crate, int cdmg, int res, int acc);
};

struct ChampionArts
{
	std::map<ArtefactType, Artefact> Map;
};

struct Champion
{
	const ChampionStats BasicStats;
	ChampionStats BonusStats;

	Champion(ChampionStats basic) : BasicStats(basic) {}
};

/////////////////////////////////////////////////////////////////////////////

void ApplyBonus(ArtSet, Champion);

/////////////////////////////////////////////////////////////////////////////

namespace ChampionFactory
{
	Champion Gromoboy();
}
