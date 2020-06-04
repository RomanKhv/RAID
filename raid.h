#pragma once
#include <vector>
#include <map>
#include <set>
#include <boost/container/static_vector.hpp>
#include "stl_ext.h"

/////////////////////////////////////////////////////////////////////////////

enum /*class*/ ArtType
{
	None = -1,	//-1 is good for finding algorytm errors

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

/////////////////////////////////////////////////////////////////////////////

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
	int Value = 0;		//= BaseValue + ExtValue
	int BaseValue = 0;
	int ExtValue = 0;

	Stat() = default;
	Stat( StatType t, int val ) : Type(t), BaseValue(val), Value(val) {}
	Stat( StatType t, int base, int add ) : Type(t), BaseValue(base), ExtValue(add), Value(base+add) {}

	bool Initialized() const { return Value > 0; }
	int  get_val(bool estimation) const { return estimation ? BaseValue : Value; }

	static const int TypeCount = 8 + 3;
	static bool IsBasic( const StatType st ) { return stl::enum_to_int(st) <= stl::enum_to_int(StatType::Acc); }
};

/////////////////////////////////////////////////////////////////////////////

enum class ArtSet
{
	None = -1,
	//2
	HP,
	Atk,
	Def,
	Speed,
	CRate,
	CritDmg,
	Resist,
	Acc,
	Cruel,			//Беспощадность
	Immortal,
	DivAtk,
	DivCritRate,
	DivLife,
	DivSpeed,
	//4
	Vamp,
	Gibel,
	Mest,
	Fury,
	Curing,
	Reflex,
	Cursed,
	Toxic,
	Frost,
	Daze,			//Ступор
	Immunitet,		//Avenging
	Vozmezdie,
	Shield,
	Doblest,
	Beshenstvo,		//4 Frenzy
	Regeneration,
	Svirepost,		//Relentless
	Savage,			//Жестокость
	Taunting,		//Насмешка
	Revenge,		//Месть

	count,

	Bannerets,
	SupremeElves,
	HolyOrden,
	Barbarians,
	Ogrins,
	Snakes,
	Werewolfs,
	Orks,
	Demons,
	Deads,
	DarkElves,
	Renegates,
	Dworves,

	_FourBegin = Vamp
};

enum class ChampionName
{
	none = -1,

	Alura,
	ColdHeart,
	Gorgorab,
	Gromoboy,
	Hangar,
	Hatun,
	Kael,
	Krisk,
	Lekar,
	Mavzolejnik,
	Rotos,
	SteelSkull,
	Tyrel,
	VisirOvelis,
	Voitelnica,
	Yuliana,
	Zargala,

	count
};

/////////////////////////////////////////////////////////////////////////////

struct Artefact
{
	ArtType Type = ArtType::None;
	ArtSet Set = ArtSet::None;
	int Stars = 0;
	int Level = 0;

	boost::container::static_vector<Stat,4> AddStats;

	ChampionName Owner = ChampionName::none;
	std::string Comment;
	
	Artefact() = default;
	Artefact( ArtType, ArtSet, int stars, int level, StatType, std::vector<Stat>, ChampionName owner = ChampionName::none, const char* comment = nullptr );
	void Reset();
	bool Initialized() const { return Stars > 0; }
	bool IsBasic() const { _ASSERTE(Initialized()); return stl::enum_to_int(Type) <= stl::enum_to_int(ArtType::Boots); }
	StatType MainStatType() const { return _MainStat.Type; }
	const Stat& GetMainStat( bool consider_max_level ) const;

	static constexpr int SetCount = static_cast<int>( ArtSet::count );
private:
	mutable Stat _MainStat;
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
	using values_t = int[Count];
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
		values_t Values;
	};

	ChampionStats() : HP(0), Atk(0), Def(0), Spd(0), CRate(0), CDmg(0), Res(0), Acc(0) {}
	ChampionStats(int hp, int atk, int def, int spd, int crate, int cdmg, int res, int acc);
	ChampionStats operator+( const ChampionStats& ) const;
	int operator[](StatType t) const { _ASSERTE( static_cast<int>(t) < Count ); return Values[static_cast<int>(t)]; }
	int& operator[](StatType t)      { _ASSERTE( static_cast<int>(t) < Count ); return Values[static_cast<int>(t)]; }
	int basic_from_p(StatType t) const { return Values[static_cast<int>(t)-Count]; }
	int& basic_from_p(StatType t)      { return Values[static_cast<int>(t)-Count]; }

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
	size_t Size() const;
	const Artefact& operator[]( ArtType t ) const { _ASSERTE( stl::is_valid_enum_as_index(t) ); return Arts[ stl::enum_to_int(t) ]; }
	Artefact& operator[]( ArtType t )             { _ASSERTE( stl::is_valid_enum_as_index(t) ); return Arts[ stl::enum_to_int(t) ]; }
	bool Initialized( ArtType t ) const           { _ASSERTE( stl::is_valid_enum_as_index(t) ); return operator[](t).Initialized(); }
	void Clear();

	//static const ArtType BasicTypesArr[BasicSize];
	static const ArtType AllTypesArr[TotalSize];
};

struct EquipmentRef
{
	using art_ref = const Artefact*;
	art_ref Arts[Equipment::TotalSize] = {nullptr};

	EquipmentRef() = default;

	const Artefact& operator[]( ArtType t ) const {
		_ASSERTE( stl::is_valid_enum_as_index(t) );
		const art_ref art = Arts[ static_cast<int>(t) ];
		if ( art )
			return *art;
		else {
			static const Artefact none_art;
			return none_art;
		}
	}
	bool Initialized( ArtType t ) const {
		_ASSERTE( stl::is_valid_enum_as_index(t) );
		art_ref art = Arts[ static_cast<int>(t) ];
		return art && art->Initialized();
	}
	size_t Size() const;
	void Clear();
	bool CheckValidMapping() const;

	Equipment as_basic() const;
};

/////////////////////////////////////////////////////////////////////////////

struct Champion
{
	const ChampionStats BasicStats;
	const Element Elem;
	const ChampionName Name = ChampionName::none;

	explicit Champion( const ChampionStats& basic, Element e = Element::none, ChampionName name = ChampionName::none );
	bool IsReal() const;
	ChampionStats TotalStats( const ChampionStats& art_bonus_stats, bool apply_hall_bonus = true ) const;

	static Champion ByName( ChampionName );
};

struct ChampionExt
	: public Champion
{
	ChampionStats ArtsBonusStats;

	ChampionExt( const Champion& );
	void ApplyStat( const Stat&, bool estimating = false );
	void ApplyEquipment( const Equipment&, bool estimating, bool consider_max_level );
	ChampionStats TotalStats( bool apply_hall_bonus = true ) const;
};

/////////////////////////////////////////////////////////////////////////////

// straight-forward implementations
namespace debug
{
std::vector<StatType> StatTypesForArt( ArtType );
int  StatValueForLevel( ArtType, StatType, int starRank, int level );
int  SetSize( ArtSet );
bool IsValidStatForArt( StatType, ArtType );
bool IsValidStatForChampion( StatType );
bool IsGoodStatForArt( StatType, ArtType );
}

// optimized implementations
int  StatValueForLevel_fast( ArtType, StatType, int starRank, int level );
inline int SetSize_fast( ArtSet set ) { return (stl::enum_to_int(set) < stl::enum_to_int(ArtSet::_FourBegin)) ? 2 : 4; }

void ApplyStat( const Stat&, const ChampionStats& basic_stats, ChampionStats& arts_bonus, bool estimating );
void ApplySetBonus( ArtSet, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool compensation );
void ApplySetsBonuses( const Equipment&, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool compensation );
void ApplyArtBonus( const Artefact&, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool estimating, bool consider_max_level );
void ApplyEquipment( const Equipment&, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool estimating, bool consider_max_level );
void ApplyEquipment( const EquipmentRef&, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool estimating, bool consider_max_level );
void ApplyHallBonus( const Champion&, ChampionStats& );

/////////////////////////////////////////////////////////////////////////////

struct Hall
{
	static const int ElementCount = 4;
	using table_row_t = int[Stat::TypeCount];
	table_row_t Table[ElementCount];

	Hall( std::map< Element, std::map<StatType, int> > );
};
extern const Hall _MyHall;

//extern std::map<StatType, int> _MyLeage;

/////////////////////////////////////////////////////////////////////////////

extern const std::vector<Artefact> _MyArts;
Equipment GetCurrentEquipmentFor( ChampionName );
ChampionStats GetCurrentArtsStatsFor( ChampionName );
ChampionStats GetCurrentFinalStatsFor( ChampionName );

/////////////////////////////////////////////////////////////////////////////
