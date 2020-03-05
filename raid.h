#pragma once
#include <vector>
#include <map>

/////////////////////////////////////////////////////////////////////////////

enum class ArtType
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
	Gibel,
	Mest,
	Fury,
	Curing,
	Reflex,
	Cursed,
	Toxic,
	Frost,
	//Daze,			//Ступор
	Immunitet,		//Avenging
	Shield,
	//Stalwart,
	CritDmg,		//2
	Beshenstvo,		//4 Frenzy
	Regeneration,
	Svirepost,		//Relentless
	Savage,			//Жестокость
	Taunting,		//Насмешка
	Cruel,			//2 Беспощадность
	Immortal,
	DivAtk,
	DivCritRate,
	DivLife,
	DivSpeed,
};

/////////////////////////////////////////////////////////////////////////////

struct Artefact
{
	ArtType Type = ArtType::Weapon;
	ArtSet Set;
	int Stars = 1;
	int Level = 0;

	StatType MainStat;
	std::vector<Stat> AddStats;
	
	Artefact() = default;
	Artefact( ArtType, ArtSet, int stars, int level, StatType, std::vector<Stat> );
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

using Equipment = std::map<ArtType, Artefact>;

struct ChampionArts
{
	Equipment Map;
};

struct Champion
{
	const ChampionStats BasicStats;
	ChampionStats BonusStats;

	Champion(ChampionStats basic) : BasicStats(basic) {}
};

/////////////////////////////////////////////////////////////////////////////

std::vector<StatType> StatTypesForArt( ArtType );
void ApplySetBonus( ArtSet, Champion& );
int  StatValueForLevel( ArtType, StatType, int starRank, int level );

/////////////////////////////////////////////////////////////////////////////

struct MatchOptions
{
	enum class ArtFactor {
		NotInterested,
		Minor,
		Magor,
		Max,
	};
	std::map<ArtType, ArtFactor> Factors;
	bool ConsiderMaxLevels = true;
};

void FindBestEquipment( Champion&, const MatchOptions&, Equipment& );

/////////////////////////////////////////////////////////////////////////////

namespace ChampionFactory
{
	Champion Gromoboy();
}

/////////////////////////////////////////////////////////////////////////////

extern std::vector<Artefact> _MyArts;