#pragma once
#include <vector>
#include <map>
#include <boost/container/static_vector.hpp>
#include "stl_ext.h"

/////////////////////////////////////////////////////////////////////////////

enum /*class*/ ArtType
{
	None = -1,

	Weapon,
	Helmet,
	Shield,
	Gloves,
	Chest,
	Boots,

	Ring,
	Necklace,
	Banner,
};

enum class StatType
{
	HP,
	Atk,
	Def,
	Spd,
	CRate,
	CDmg,
	Res,
	Acc,

	HP_p,
	Atk_p,
	Def_p,
};

struct Stat
{
	StatType Type;
	int Value;
};

enum class ArtSet
{
	None = -1,
	//2
	HP,
	Atk,
	Def,
	Speed,
	CRate,
	CDmg,
	Res,
	Acc,
	Cruel,			//Беспощадность
	Immortal,
	DivAtk,
	DivCritRate,
	DivLife,
	DivSpeed,
	//4
	_FourBegin,
	Vamp = _FourBegin,
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
	Beshenstvo,		//4 Frenzy
	Regeneration,
	Svirepost,		//Relentless
	Savage,			//Жестокость
	Taunting,		//Насмешка

	Count
};

enum class ChampionName
{
	none,
	Kael,
	Gromoboy,
	Krisk,
};

/////////////////////////////////////////////////////////////////////////////

struct Artefact
{
	ArtType Type = ArtType::None;
	ArtSet Set = ArtSet::None;
	int Stars = 0;
	int Level = 0;

	StatType MainStat = StatType::Atk;
	boost::container::static_vector<Stat,4> AddStats;

	ChampionName Owner = ChampionName::none;

	static constexpr int SetCount = static_cast<int>( ArtSet::Count );
	
	Artefact() = default;
	Artefact( ArtType, ArtSet, int stars, int level, StatType, std::vector<Stat>, ChampionName owner = ChampionName::none );
	bool Initialized() const { return Type != ArtType::None; }
	Stat GetMainStat( bool consider_max_level ) const;
};

/////////////////////////////////////////////////////////////////////////////

enum class Element {
	none = -1,
	Blue, Green, Red, Void,
	Count
};

/////////////////////////////////////////////////////////////////////////////

struct ChampionStats
{
	static const int Count = 8;
	union {
		struct {
			int HP;
			int Atk;
			int Def;
			int Spd;
			int CRate;
			int CDmg;
			int Res;
			int Acc;
		};
		int Values[Count];
	};

	ChampionStats() : HP(0), Atk(0), Def(0), Spd(0), CRate(0), CDmg(0), Res(0), Acc(0) {}
	ChampionStats(int hp, int atk, int def, int spd, int crate, int cdmg, int res, int acc);
	ChampionStats operator+( const ChampionStats& ) const;
	int operator[](StatType t) const { _ASSERTE( static_cast<int>(t) < Count ); return Values[static_cast<int>(t)]; }
	int& operator[](StatType t)      { _ASSERTE( static_cast<int>(t) < Count ); return Values[static_cast<int>(t)]; }
	int p_stat(StatType t) const { return Values[static_cast<int>(t)%Count]; }
	int& p_stat(StatType t)      { return Values[static_cast<int>(t)%Count]; }

	typedef const StatType StatList[Count];
	static const StatList TypeList;
};

struct Equipment
{
	static const int BasicSize = 6;
	static const int TotalSize = 9;
	Artefact Arts[TotalSize];

	Equipment() = default;
	Equipment( std::initializer_list<Artefact> il );
	const Artefact& operator[]( ArtType t ) const { return Arts[ static_cast<int>(t) ]; }
	Artefact& operator[]( ArtType t ) { return Arts[ static_cast<int>(t) ]; }
	bool Initialized( ArtType t ) const { return operator[](t).Initialized(); }
	void Clear();
	size_t Size() const;
};

struct Equipment2
{
	using art_ref = const Artefact*;
	art_ref Arts[Equipment::TotalSize] = {nullptr};

	Equipment2() = default;
	const Artefact& operator[]( ArtType t ) const {
		art_ref art = Arts[ static_cast<int>(t) ];
		if ( art )
			return *art;
		else {
			static const Artefact none_art;
			return none_art;
		}
	}
	bool Initialized( ArtType t ) const {
		art_ref art = Arts[ static_cast<int>(t) ];
		return art && art->Initialized();
	}
	void Clear();
	size_t Size() const;
};

struct Champion
{
	const ChampionStats BasicStats;
	ChampionStats ArtsBonusStats;
	const Element Elem;

	explicit Champion( const ChampionStats& basic, Element e = Element::none );
	bool IsReal() const;
	ChampionStats TotalStats( bool apply_hall_bonus = true ) const;
};

/////////////////////////////////////////////////////////////////////////////

std::vector<StatType> StatTypesForArt( ArtType );
int  StatValueForLevel( ArtType, StatType, int starRank, int level );
int  SetSize( ArtSet );
inline int SetSize_fast( ArtSet set ) { return (stl::enum_to_int(set) < stl::enum_to_int(ArtSet::_FourBegin)) ? 2 : 4; }
bool IsValidStatForArt( StatType, ArtType );
bool IsGoodStatForArt( StatType, ArtType );
void ApplyStat( const Stat&, const ChampionStats& basic_stats, ChampionStats& arts_bonus );
void ApplyStat( const Stat&, Champion& );
void ApplySetBonus( ArtSet, Champion&, bool compensation );
void ApplySetsBonuses( const Equipment&, Champion&, bool compensation );
void ApplyArtBonus( const Artefact&, Champion&, bool consider_max_level /*= false*/ );
void ApplyEquipment( const Equipment&, Champion&, bool estimating );
void ApplyHallBonus( const Champion&, ChampionStats& );

/////////////////////////////////////////////////////////////////////////////

struct Hall
{
	static const int ElementCount = 4;
	std::map<StatType, int> Table[ElementCount];

	Hall( std::map< Element, std::map<StatType, int> > );
};
extern const Hall _MyHall;

extern std::map<StatType, int> _MyLeage;

/////////////////////////////////////////////////////////////////////////////

struct MatchOptions
{
	enum class ArtFactor {
		NotInterested,
		Minor,
		Moderate,
		Magor,
		Max,
		MinCap,
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

Equipment FindRealBestEquipment( Champion&, const MatchOptions& );
void FindBestEquipment( const std::vector<Artefact>&, const Champion&, const MatchOptions&, Equipment& );

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

extern const std::vector<Artefact> _MyArts;
Equipment GetCurrentEquipmentFor( ChampionName );

/////////////////////////////////////////////////////////////////////////////
