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
	Atk_p,
	HP,
	HP_p,
	Def,
	Def_p,

	CRate,
	CDmg,

	Spd,
	Acc,
	Res,
};

struct Stat
{
	StatType Type;
	int Value;
};

enum class ArtSet
{
	None,
	HP,
	Atk,
	Def,
	CRate,
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
	CDmg,		//2
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
	ArtSet Set = ArtSet::HP;
	int Stars = 1;
	int Level = 0;

	StatType MainStat = StatType::Atk;
	std::vector<Stat> AddStats;
	
	Artefact() = default;
	Artefact( ArtType, ArtSet, int stars, int level, StatType, std::vector<Stat> );
	bool IsValid() const { return Type != ArtType::None; }
	Stat GetMainStat( bool consider_max_level ) const;
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
	ChampionStats operator+( const ChampionStats& ) const;
};

struct Equipment
	: std::map<ArtType, Artefact>
{
	Equipment() = default;
	Equipment( std::initializer_list<Artefact> il );
};

struct ChampionArts
{
	Equipment Map;
};

struct Champion
{
	const ChampionStats BasicStats;
	ChampionStats BonusStats;

	Champion(const ChampionStats& basic) : BasicStats(basic) {}
	ChampionStats TotalStats() const;
};

/////////////////////////////////////////////////////////////////////////////

std::vector<StatType> StatTypesForArt( ArtType );
int  StatValueForLevel( ArtType, StatType, int starRank, int level );
int  SetSize( ArtSet );
bool IsValidStatForArt( StatType, ArtType );
bool IsGoodStatForArt( StatType, ArtType );
void ApplyStat( const Stat&, Champion& );
void ApplySetBonus( ArtSet, Champion& );
void ApplySetsBonuses( const Equipment&, Champion& );
void ApplyArtBonus( const Artefact&, Champion&, bool consider_max_level = false );
void ApplyEquipment( const Equipment&, Champion& );

/////////////////////////////////////////////////////////////////////////////

struct MatchOptions
{
	enum class ArtFactor {
		NotInterested,
		Minor,
		Moderate,
		Magor,
		MinCap,
		Max,
	};
	std::map<StatType, ArtFactor> Factors;

	std::vector<ArtSet> RequiedSets;
	std::vector<ArtSet> SetFilter;

	bool ConsiderMaxLevels = true;
	std::map<StatType,int> MinCap;	//TODO:
	std::map<ArtType,StatType> RequiredArtStats;	//TODO

	MatchOptions() = default;
	MatchOptions( std::map<StatType, ArtFactor>,
				  std::vector<ArtSet> req_filter = {},
				  std::vector<ArtSet> set_filter = {},
				  bool consider_max_lvl = true,
				  std::map<StatType,int> min_caps = {} );
	bool IsSetAccepted( ArtSet ) const;
	bool IsEqHasRequiredSets( const Equipment& ) const;
	//bool IsEqAccepted( const Equipment& ) const;
};

void FindRealBestEquipment( Champion&, const MatchOptions&, Equipment& );
void FindBestEquipment( const std::vector<Artefact>&, const ChampionStats& basic_stats, const MatchOptions&, Equipment& );

/////////////////////////////////////////////////////////////////////////////

namespace ChampionFactory
{
	Champion ColdHeart();
	Champion Gromoboy();
	Champion Kael();
	Champion Lekar();
	Champion Yuliana();
	Champion Krisk();
	Champion Hatun();
}

/////////////////////////////////////////////////////////////////////////////

extern std::vector<Artefact> _MyArts;

/////////////////////////////////////////////////////////////////////////////
