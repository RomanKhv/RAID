#include "pch.h"
#include "raid.h"
#include "suiter.h"
#include "stl_ext.h"
#include "to_string.h"

/////////////////////////////////////////////////////////////////////////////

bool check_stat_input_error( const Stat& stat )
{
	switch ( stat.Type )
	{
		case StatType::HP:			return stat.BaseValue >= 100;
		case StatType::Atk:			return stat.Value >= 16;
		case StatType::Def:			return stat.Value >= 15;
		case StatType::Atk_p:		return stat.BaseValue <= 24;
		case StatType::HP_p:		return stat.BaseValue <= 24;
		case StatType::Def_p:		return stat.BaseValue <= 22;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

Artefact::Artefact( ArtType type, ArtSet set, int stars, int level, StatType mainstat, std::vector<Stat> addstats,
					ChampionName owner, const char* comment )
	:Type(type)
	,Set(set)
	,Stars(stars)
	,Level(level)
	,AddStats( addstats.begin(), addstats.end() )
	,Owner(owner)
{
	_MainStat.Type = mainstat;
	if ( comment )
		Comment = comment;

	_ASSERTE( Initialized() );
	_ASSERTE( debug::IsValidStatForArt( mainstat, type ) );
	_ASSERTE( debug::IsGoodStatForArt( mainstat, type ) );
	_ASSERTE( addstats.size() <= 4 );
#ifdef _DEBUG
	for ( const Stat& stat : AddStats )
		_ASSERTE( check_stat_input_error( stat ) );
	to_string( Set );
#endif
}

void Artefact::Reset()
{
	*this = Artefact();
}

const Stat& Artefact::GetMainStat( bool consider_max_level ) const
{
	_ASSERTE( Initialized() );
	if ( !_MainStat.Initialized() )
	{
		_MainStat.Value = _MainStat.BaseValue = StatValueForLevel_fast( Type, _MainStat.Type, Stars, consider_max_level ? 16 : Level );
	}
#ifdef _DEBUG
	else {
		_ASSERTE( _MainStat.Value == StatValueForLevel_fast( Type, _MainStat.Type, Stars, consider_max_level ? 16 : Level ) );
	}
#endif
	return _MainStat;
}

/////////////////////////////////////////////////////////////////////////////

//const ArtType Equipment::BasicTypesArr[Equipment::BasicSize] = {
//	ArtType::Weapon,  ArtType::Helmet,   ArtType::Shield,
//	ArtType::Gloves,  ArtType::Chest,    ArtType::Boots
//};
const ArtType Equipment::AllTypesArr[Equipment::TotalSize] = {
	ArtType::Weapon,  ArtType::Helmet,   ArtType::Shield,
	ArtType::Gloves,  ArtType::Chest,    ArtType::Boots,
	ArtType::Ring,    ArtType::Necklace, ArtType::Banner
};

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

void EquipmentRef::Clear()
{
	for ( art_ref& art : Arts )
		art = nullptr;
}

size_t EquipmentRef::Size() const
{
	size_t n_arts = 0;
	for ( const art_ref& art : Arts )
	{
		if ( art )
		{
			_ASSERTE( Initialized( art->Type ) );
			_ASSERTE( art->Initialized() );
			n_arts++;
		}
	}
	return n_arts;
}

bool EquipmentRef::CheckValidMapping() const
{
	for ( const ArtType at : Equipment::AllTypesArr )
	{
		_ASSERTE( stl::is_valid_enum_as_index( at ) );
		art_ref art = Arts[at];
		if ( art && art->Type!=at )
			return false;
	}
	return true;
}

Equipment EquipmentRef::as_basic() const
{
	Equipment eq;
	for ( const ArtType at : Equipment::AllTypesArr )
	{
		if ( const Artefact* art = Arts[at] )
			eq[at] = *art;
		else
			eq[at].Reset();
	}
	return eq;
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
void add_percent_bonus( ChampionStats& dest, const ChampionStats& ref, int factor_percent )
{
	dest.*member += ref.*member * factor_percent / 100;
}

/////////////////////////////////////////////////////////////////////////////

inline Champion::Champion( const ChampionStats& basic, Element e, ChampionName name )
	: BasicStats( basic )
	, Elem( e )
	, Name( name )
{
}

bool Champion::IsReal() const
{
	_ASSERTE( (Name==ChampionName::none) || (Elem!=Element::none) );
	return Elem != Element::none;
}

ChampionStats Champion::TotalStats( const ChampionStats& arts_bonus_stats, bool apply_hall_bonus ) const
{
	ChampionStats stats = BasicStats + arts_bonus_stats;

	if ( apply_hall_bonus )
	{
		_ASSERTE( IsReal() );
		ApplyHallBonus( *this, stats );
	}

	return stats;
}

ChampionExt::ChampionExt( const Champion& b )
	:Champion(b)
{
}

void ChampionExt::ApplyEquipment( const Equipment& eq, bool estimating, bool consider_max_level )
{
	::ApplyEquipment( eq, BasicStats, ArtsBonusStats, estimating, consider_max_level );
}

ChampionStats ChampionExt::TotalStats( bool apply_hall_bonus ) const
{
	return Champion::TotalStats( ArtsBonusStats, apply_hall_bonus );
}

/////////////////////////////////////////////////////////////////////////////

namespace debug {

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

}//debug

/////////////////////////////////////////////////////////////////////////////

	typedef int stat_table_t[3][5];

				static const stat_table_t Atk_Def_general = {
					//      0   4    8   12   16
					/*4*/{ 16, 51,  85, 120, 190 },
					/*5*/{ 25, 64, 104, 143, 225 },
					/*6*/{ 35, 80, 125, 170, 265 }
				};
				static const stat_table_t Atk_Def_banner = {
					//      0   4    8   12   16
					/*4*/{ 24,  0, 128, 180, 285 },
					/*5*/{ 38, 96, 156, 215, 338 },
					/*6*/{ 53,  0,   0, 255, 398 }
				};
				static const stat_table_t HP_general = {
					//      0     4     8    12    16
					/*4*/{ 260,  773, 1286, 1800, 2840 },
					/*5*/{ 450, 1044, 1638, 2231, 3480 },
					/*6*/{ 600, 1277, 1954, 2631, 4080 }
				};
				static const stat_table_t HP_banner = {
					//      0     4     8    12    16
					/*4*/{ 390,    0,    0,    0, 4260 },
					/*5*/{ 675, 1566, 2457, 3347, 5220 },
					/*6*/{ 900,    0,    0, 3947, 6120 }
				};
				static const stat_table_t Atk_HP_Def_percent_CR = {
					//      0   4   8  12  16
					/*4*/{  6, 13, 19, 26, 40 },
					/*5*/{  8, 16, 24, 33, 50 },
					/*6*/{ 10, 20, 29, 39, 60 },
				};
				static const stat_table_t CDmg_general = {
					//      0  4  8   12  16
					/*4*/{  8, 0,  0,  0,  0 },
					/*5*/{ 10, 0, 31, 42, 65 },
					/*6*/{ 12, 0, 38,  0, 80 },
				};
				static const stat_table_t CDmg_necklace = {
					//     0   4   8  12  16
					/*4*/{ 4,  8, 12, 16, 25 },
					/*5*/{ 5, 11, 16, 21, 33 },
					/*6*/{ 6,  0, 19, 26, 40 },
				};
				static const stat_table_t Spd_all = {
					//     0   4   8  12  16
					/*4*/{ 4, 10, 16, 22, 35 },
					/*5*/{ 5, 12, 19, 26, 40 },
					/*6*/{ 6, 14, 21, 29, 45 },
				};
				static const stat_table_t Acc_Res = {
					//      0   4   8  12  16
					/*4*/{  8,  0, 30, 41, 64 },
					/*5*/{ 12, 25, 38, 50, 78 },
					/*6*/{ 16,  0,  0, 62, 96 },
				};

namespace debug {

int StatValueForLevel( ArtType art, StatType stat, int starRank, int level )
{
	const stat_table_t* stat_table = nullptr;

	switch ( stat )
	{
		case StatType::Atk:
		case StatType::Def:
			if ( art != ArtType::Banner )
			{
				stat_table = &Atk_Def_general;
			}
			else {
				stat_table = &Atk_Def_banner;
			}
			break;

		case StatType::HP:
			if ( art != ArtType::Banner )
			{
				stat_table = &HP_general;
			}
			else {
				stat_table = &HP_banner;
			}
			break;

		case StatType::Atk_p:
		case StatType::HP_p:
		case StatType::Def_p:
		case StatType::CRate:
			{
				stat_table = &Atk_HP_Def_percent_CR;
			}
			break;

		case StatType::CDmg:
			if ( art != ArtType::Necklace )
			{
				stat_table = &CDmg_general;
			}
			else {
				stat_table = &CDmg_necklace;
			}
			break;

		case StatType::Spd:
			{
				stat_table = &Spd_all;
			}
			break;

		case StatType::Acc:
		case StatType::Res:
			{
				stat_table = &Acc_Res;
			}
			break;
	}

	//starRank = boost::algorithm::clamp( starRank, 1, 6 );
	//level = boost::algorithm::clamp( level, 0, 16 );
	_ASSERTE( 4 <= starRank && starRank <= 6 );
	_ASSERTE( 0 <= level && level <= 16 );

	const int value = (*stat_table)[starRank - 4][level / 4];
	//_ASSERTE( value > 0 );
	return value;
}
}//debug

int StatValueForLevel_fast( ArtType art, StatType stat, int starRank, int level )
{
	static const stat_table_t* stat_art_map[ChampionStats::Count + 3][2] = {
		/*StatType::HP*/   { &HP_general,      &HP_banner },
		/*StatType::Atk*/  { &Atk_Def_general, &Atk_Def_banner },
		/*StatType::Def*/  { &Atk_Def_general, &Atk_Def_banner },
		/*StatType::Spd*/  { &Spd_all,         &Spd_all },
		/*StatType::CRate*/{ &Atk_HP_Def_percent_CR, &Atk_HP_Def_percent_CR },
		/*StatType::CDmg*/ { &CDmg_general,    &CDmg_necklace },
		/*StatType::Res*/  { &Acc_Res,         &Acc_Res },
		/*StatType::Acc*/  { &Acc_Res,         &Acc_Res },
		/*StatType::HP_p*/ { &Atk_HP_Def_percent_CR, &Atk_HP_Def_percent_CR },
		/*StatType::Atk_p*/{ &Atk_HP_Def_percent_CR, &Atk_HP_Def_percent_CR },
		/*StatType::Def_p*/{ &Atk_HP_Def_percent_CR, &Atk_HP_Def_percent_CR },
	};
	const int second_index = stl::enum_to_int(art) /
		( (stat==StatType::CDmg) ? stl::enum_to_int(ArtType::Necklace) : stl::enum_to_int(ArtType::Banner));
	_ASSERTE( 0 <= second_index && second_index <= 1 );
	const stat_table_t* stat_table = stat_art_map[stl::enum_to_int(stat)][second_index];

	//starRank = boost::algorithm::clamp( starRank, 1, 6 );
	//level = boost::algorithm::clamp( level, 0, 16 );
	_ASSERTE( 4 <= starRank && starRank <= 6 );
	_ASSERTE( 0 <= level && level <= 16 );

	const int value = (*stat_table)[starRank - 4][level / 4];
	//_ASSERTE( value > 0 );
	return value;
}

/////////////////////////////////////////////////////////////////////////////
namespace debug {

int SetSize( ArtSet set )
{
	switch ( set )
	{
		case ArtSet::HP:
		case ArtSet::Atk:
		case ArtSet::Def:
		case ArtSet::CRate:
		case ArtSet::CritDmg:
		case ArtSet::Speed:
		case ArtSet::Resist:
		case ArtSet::Acc:
		case ArtSet::Cruel:
		case ArtSet::Immortal:
		case ArtSet::DivAtk:
		case ArtSet::DivCritRate:
		case ArtSet::DivLife:
		case ArtSet::DivSpeed:
		case ArtSet::Zhivuchest:
		case ArtSet::Rastoropnost:
			return 2;
	}
	return 4;
}

bool IsValidStatForArt( StatType stat, ArtType art )
{
	const std::vector<StatType> valid_stats = StatTypesForArt( art );
	return std::find( valid_stats.begin(), valid_stats.end(), stat ) != valid_stats.end();
}

bool IsValidStatForChampion( StatType st )
{
	return st!=StatType::HP_p && st!=StatType::Atk_p && st!=StatType::Def_p;
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

}//debug
/////////////////////////////////////////////////////////////////////////////

void ApplyStat( const Stat& stat, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool estimating )
{
	_ASSERTE( stat.Value > 0 && stat.BaseValue > 0 );
	if ( Stat::IsBasic( stat.Type ) )	// == IsAdditive
	{
		arts_bonus_stats[stat.Type] += stat.get_val( estimating );
	}
	else {
		arts_bonus_stats.basic_from_p(stat.Type) += basic_stats.basic_from_p(stat.Type) * stat.get_val(estimating) / 100;
	}
	//switch ( stat.Type )
	//{
	//	case StatType::HP_p:
	//	case StatType::Atk_p:
	//	case StatType::Def_p:
	//		arts_bonus_stats.p_stat(stat.Type) += basic_stats.p_stat(stat.Type) * stat.Value / 100;
	//		return;
	//	default:
	//		arts_bonus_stats[stat.Type] += stat.Value;
	//		return;
	//}
	//_ASSERTE( !"unreachable code" );
}

void ChampionExt::ApplyStat( const Stat& stat, bool estimating )
{
	::ApplyStat( stat, BasicStats, ArtsBonusStats, estimating );
}

/////////////////////////////////////////////////////////////////////////////

const int DivHPCompensation = 2;

void ApplySetBonus( ArtSet set, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool compensation )
{
	switch ( set )
	{
		case ArtSet::HP:
			{
				add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, 15 );
			}
			break;
		case ArtSet::Immortal:
			{
				add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, 15 + (compensation ? 3 : 0) );
			}
			break;
		case ArtSet::DivLife:
			{
				add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, 15 + (compensation ? DivHPCompensation : 0) );
			}
			break;
		case ArtSet::Zhivuchest:
			{
				add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, 10 );
				add_percent_bonus<&ChampionStats::Def>( arts_bonus_stats, basic_stats, 10 );
			}
			break;
		case ArtSet::Atk:
			{
				add_percent_bonus<&ChampionStats::Atk>( arts_bonus_stats, basic_stats, 15 );
			}
			break;
		case ArtSet::Cruel:
			{
				add_percent_bonus<&ChampionStats::Atk>( arts_bonus_stats, basic_stats, 15 + (compensation ? 3 : 0) );
			}
			break;
		case ArtSet::DivAtk:
			{
				add_percent_bonus<&ChampionStats::Atk>( arts_bonus_stats, basic_stats, 15 );
				if ( compensation )
					add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, DivHPCompensation );
			}
			break;
		case ArtSet::Def:
			{
				add_percent_bonus<&ChampionStats::Def>( arts_bonus_stats, basic_stats, 15 );
			}
			break;
		case ArtSet::CRate:
			{
				arts_bonus_stats.CRate += 12;
			}
			break;
		case ArtSet::DivCritRate:
			{
				arts_bonus_stats.CRate += 12;
				if ( compensation )
					add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, DivHPCompensation );
			}
			break;
		case ArtSet::CritDmg:
			{
				arts_bonus_stats.CDmg += 20;
			}
			break;
		case ArtSet::Speed:
			{
				add_percent_bonus<&ChampionStats::Spd>( arts_bonus_stats, basic_stats, 12 );
			}
			break;
		case ArtSet::DivSpeed:
			{
				add_percent_bonus<&ChampionStats::Spd>( arts_bonus_stats, basic_stats, 12 );
				if ( compensation )
					add_percent_bonus<&ChampionStats::HP>( arts_bonus_stats, basic_stats, DivHPCompensation );
			}
			break;
		case ArtSet::Resist:
			{
				arts_bonus_stats.Res += 40;
			}
			break;
		case ArtSet::Acc:
			{
				arts_bonus_stats.Acc += 40;
			}
			break;
		case ArtSet::Rastoropnost:
			{
				arts_bonus_stats.Acc += 40;
				add_percent_bonus<&ChampionStats::Spd>( arts_bonus_stats, basic_stats, 5 );
			}
			break;
	}
}

void ApplySetsBonuses( const Equipment& eq, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool compensation )
{
	int n_arts_by_set[Artefact::SetCount] = {0};
	for ( size_t iat = 0; iat < Equipment::BasicSize; ++iat )
	{
		const Artefact& art = eq.Arts[iat];
		if ( art.Initialized() )
		{
			_ASSERTE( art.IsBasic() );
			n_arts_by_set[stl::enum_to_int( art.Set )]++;
		}
	}
	for ( int set = 0; set < Artefact::SetCount; ++set )
	{
		if ( !n_arts_by_set[set] )
			continue;
		const int count = n_arts_by_set[set] / SetSize_fast( static_cast<ArtSet>(set) );
		for ( int i = 0; i < count; ++i )
			ApplySetBonus( static_cast<ArtSet>(set), basic_stats, art_bonus_stats, compensation );
	}
}

void ApplySetsBonuses( const EquipmentRef& eq, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool compensation )
{
	int n_arts_by_set[Artefact::SetCount] = {0};
	for ( size_t iat = 0; iat < Equipment::BasicSize; ++iat )
	{
		if ( const Artefact* art = eq.Arts[iat] )
		{
			_ASSERTE( art->Initialized() );
			_ASSERTE( art->IsBasic() );
			n_arts_by_set[stl::enum_to_int( art->Set )]++;
		}
	}
	for ( int set = 0; set < Artefact::SetCount; ++set )
	{
		if ( !n_arts_by_set[set] )
			continue;
		const int count = n_arts_by_set[set] / SetSize_fast( static_cast<ArtSet>(set) );
		for ( int i = 0; i < count; ++i )
			ApplySetBonus( static_cast<ArtSet>(set), basic_stats, arts_bonus_stats, compensation );
	}
}

void ApplyArtBonus( const Artefact& art, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool estimating, bool consider_max_level )
{
	if ( !art.Initialized() )
	{
		_ASSERTE( !"invalid art" );
		return;
	}
	if ( !MatchOptions::ConsiderMaxLevelsForNonBasicArts && !art.IsBasic() )
		consider_max_level = false;

	ApplyStat( art.GetMainStat(consider_max_level), basic_stats, arts_bonus_stats, false );

	for ( const Stat& stat : art.AddStats )
		ApplyStat( stat, basic_stats, arts_bonus_stats, estimating );
}

void ApplyEquipment( const Equipment& eq, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool estimating, bool consider_max_level )
{
	ApplySetsBonuses( eq, basic_stats, art_bonus_stats, estimating );

	for ( const Artefact& art : eq.Arts )
	{
		if ( art.Initialized() )
		{
			ApplyArtBonus( art, basic_stats, art_bonus_stats, estimating, estimating || consider_max_level );
		}
	}
}

void ApplyEquipment( const EquipmentRef& eq, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool estimating, bool consider_max_level )
{
	ApplySetsBonuses( eq, basic_stats, arts_bonus_stats, estimating );

	for ( const Artefact* const art : eq.Arts )
	{
		if ( art )
		{
			_ASSERTE( art->Initialized() );
			ApplyArtBonus( *art, basic_stats, arts_bonus_stats, estimating, estimating || consider_max_level );
		}
	}
}

void ApplyHallBonus( const Champion& ch, ChampionStats& stats )
{
	const Hall::table_row_t& hall_bonus = _MyHall.Table[ stl::enum_to_int(ch.Elem) ];

	for ( const StatType st : { StatType::HP_p, StatType::Atk_p, StatType::Def_p, StatType::CDmg, StatType::Res, StatType::Acc } )
	{
		const int b = hall_bonus[stl::enum_to_int( st )];
		if ( b > 0 )
			ApplyStat( { st, b }, ch.BasicStats, stats, false );
	}
}

/////////////////////////////////////////////////////////////////////////////

Hall::Hall( std::map<Element, std::map<StatType, int>> m )
{
	for ( auto& e : Table )
		for ( auto& b : e )
			b = 0;
	for ( const auto& e : m )
		for ( const auto& s : e.second )
		{
			Table[stl::enum_to_int(e.first)][stl::enum_to_int(s.first)] = s.second;
		}
}

/////////////////////////////////////////////////////////////////////////////

MatchOptions::MatchOptions( std::map<StatType, StatFactor> factors, std::vector<ArtSet> req_filter, std::set<ArtSet> exclusion_filter,
							std::set<ChampionName> providers, ArtTier art_tier_cap )
	//,ConsiderMaxLevels( consider_max_lvl )
	//:ArtTierCap( art_tier_cap )
{
	for ( ArtSet set : req_filter )
	{
		RequiedSets[set]++;
	}
	for ( ArtSet set : exclusion_filter )
	{
		ExcludedSets[set] = true;
	}
	for ( ChampionName name : providers )
	{
		Undressable[name] = true;
	}
	for ( const auto& p : factors )
	{
		const auto& f = Factors[stl::enum_to_int( p.first )] = p.second;
		_ASSERTE( debug::IsValidStatForChampion( p.first ) );
		_ASSERTE( f.HasMinCap() || f.Mode!=StatInfluence::NotInterested );
		_ASSERTE( !f.HasMinCap() || !f.HasMaxCap() || f.MinCap<p.second.MaxCap );
	}
	StatOnArt[ArtType::Boots] = StatType::Spd;

	static bool inventory_logged = false;
	if ( !inventory_logged ) {
		std::printf( "Inventory: %I64u items\n", _MyArts.size() );
		inventory_logged = true;
	}
}

void MatchOptions::AllowSets( std::set<ArtSet> sets )
{
	//std::fill( ExcludedSets.begin(), ExcludedSets.end(), true );
	for ( bool& f : ExcludedSets )
		f = true;
	for ( ArtSet set : sets )
		ExcludedSets[set] = false;
	for ( const auto& p : RequiedSets )
		ExcludedSets[p.first] = false;
}

void MatchOptions::RequireSpeedBoots( bool speed_boots )
{
	if ( speed_boots )
		StatOnArt[ArtType::Boots] = StatType::Spd;
	else
		StatOnArt[ArtType::Boots] = boost::none;
}

bool MatchOptions::IsInputOK() const
{
	int total_req_art_count = 0;
	for ( const auto& set : RequiedSets )
	{
		total_req_art_count += SetSize_fast( set.first );
	}
	_ASSERTE( total_req_art_count <= 6 );
	if ( total_req_art_count > 6 )
		return false;

	return true;
}

bool MatchOptions::IsSetAccepted( ArtSet set ) const
{
	if ( set == ArtSet::None )
		return true;	//ring/necklace/banner

	return !ExcludedSets[set];
}

bool MatchOptions::IsArtAccepted( const Artefact& art, ChampionName ch_name ) const
{
	if ( art.Owner != ChampionName::none &&
		 art.Owner != ch_name && 
		 !Undressable[art.Owner] )
		return false;

	if ( !art.IsBasic() )
	{
		return art.Owner == ch_name;
	}

	if ( !IsSetAccepted( art.Set ) )
		return false;

	//if ( !IsArtAcceptedByTier( art ) )
	//	return false;

	if ( StatOnArt[art.Type].has_value() )
	{
		if ( art.MainStatType() != StatOnArt[art.Type] )
			return false;
	}

	return true;
}

//bool MatchOptions::IsArtAcceptedByTier( const Artefact& art ) const
//{
//	const ArtTier at = GetArtTier( art );
//
//	return stl::enum_to_int(at) >= stl::enum_to_int(ArtTierCap);
//}

bool MatchOptions::IsEqHasRequiredSets( const EquipmentRef& eq ) const
{
	if ( RequiedSets.empty() )
		return true;

	enum_index_map<ArtSet,ArtSet::count,int> eq_sets;
	for ( size_t iat = 0; iat < Equipment::BasicSize; ++iat )
	{
		if ( const Artefact* art = eq.Arts[iat] )
		{
			_ASSERTE( art->IsBasic() );
			eq_sets[art->Set] ++;
		}
	}

	for ( const auto set : RequiedSets )
	{
		_ASSERTE( set.second > 0 );
		const int n_req_arts = SetSize_fast( set.first ) * set.second;
		const int n_eq_arts = eq_sets[set.first];
		if ( n_eq_arts < n_req_arts )
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////

ChampionStats FinalStats( const Champion& ch, const Equipment& eq )
{
	ChampionStats arts_bonus_stats;
	ApplyEquipment( eq, ch.BasicStats, arts_bonus_stats, false, MatchOptions::ConsiderMaxLevels );
	return ch.TotalStats( arts_bonus_stats );
}

Equipment GetCurrentEquipmentFor( ChampionName name )
{
	_ASSERTE( name != ChampionName::none );
	Equipment eq;
	for ( const Artefact& art : _MyArts )
	{
		_ASSERTE( art.Initialized() );
		if ( art.Owner == name )
			eq[art.Type] = art;
	}
	return eq;
}

Champion Champion::ByName( ChampionName name )
{
	switch ( name )
	{
		case ChampionName::Alura:
			return Champion( { 14040, 1575, 793,  96,  15+5, 60+10,  30, 0 }, Element::Blue, name );
			break;
		case ChampionName::Arbitr:
			return Champion( { 21135, 1068, 1101,  110,  15, 50,  30, 10 }, Element::Void, name );
			break;
		case ChampionName::BlackKnight:
			return Champion( { 22470, 914, 1167,  100,  15, 57,  50, 0 }, Element::Red, name );
			break;
		case ChampionName::ColdHeart:
			return Champion( { 13710, 1376, 738,  94,  15+5, 57+10,  30, 0 }, Element::Void, name );
			break;
		case ChampionName::Fein:
			return Champion( { 13710, 1663, 727,  99,  15, 57,  30, 0 }, Element::Green, name );
			break;
		case ChampionName::Foly:
			return Champion( { 15030, 1476, 1101,  105,  15+5, 63+10,  30, 0 }, Element::Blue, name );
			break;
		case ChampionName::Gala:
			return Champion( { 18165, 1432, 661,  97,  15+5, 60+10,  60, 0 }, Element::Void, name );
			break;
		case ChampionName::Gorgorab:
			return Champion( { 17670, 1057, 1068,  97,  15, 50,  30, 0 }, Element::Blue, name );
			break;
		case ChampionName::GornyKorol:
			return Champion( { 31050, 1432, 1112,  85,  15, 63,  90, 0 }, Element::Red, name );
			break;
		case ChampionName::Gromoboy:
			return Champion( { 15855, 727, 1443,  97,  15+5, 50,  30, 0 }, Element::Void, name );
			break;
		case ChampionName::Guard:
			return Champion( { 15030, 1531, 771,  93,  15+5, 50+10,  30, 0 }, Element::Blue, name );
			break;
		case ChampionName::Hangar:
			return Champion( { 21975, 826, 1288+75,  98,  15+5, 50+10,  45, 10 }, Element::Void, name );
			break;
		case ChampionName::Hatun:
			return Champion( { 15690, 980, 1156,  97,  15, 50,  30, 0 }, Element::Green, name );
			break;
		case ChampionName::Kael:
			return Champion( { 13710, 1200, 914,  103,  15, 57,  30, 0 }, Element::Blue, name );
			break;
		case ChampionName::Killian:
			return Champion( { 13215, 1432, 1266,  98,  15, 63,  30, 10 }, Element::Blue, name );
			break;
		case ChampionName::Krisk:
			return Champion( { 19485, 760, 1520,  94,  15, 50,  50, 10+10+50 }, Element::Void, name );
			break;
		case ChampionName::Lekar:
			return Champion( { 17175, 881, 1002,  106,  15+5, 50+10,  30, 0 }, Element::Blue, name );
			break;
		case ChampionName::Mashalled:
			return Champion( { 17835, 1454, 936,  103,  15+5, 63,  30, 0 }, Element::Green, name );
			break;
		case ChampionName::Mavzolejnik:
			return Champion( { 19485, 1057, 947,  104,  15, 50,  30, 0 }, Element::Red, name );
			break;
		case ChampionName::Razen:
			return Champion( { 18330, 1046, 1310,  91,  15, 50,  50, 0 }, Element::Red, name );
			break;
		case ChampionName::Rotos:
			return Champion( { 11895, 1520, 1266,  90,  15+5, 63+10,  40, 0 }, Element::Blue, name );
			break;
		case ChampionName::Skilla:
			return Champion( { 191980, 859, 1387+70,  95,  15+5, 63+10,  40, 0 }, Element::Blue, name );
			break;
		case ChampionName::Sohaty:
			return Champion( { 20970, 859, 1046,  107,  15, 50,  30, 0 }, Element::Green, name );
			break;
		case ChampionName::SteelSkull:
			return Champion( { 16020, 1277, 958,  111,  15+5, 50+10,  30, 0 }, Element::Green, name );
			break;
		case ChampionName::Straholud:
			return Champion( { 22965, 958, 815,  95,  15+5, 50+10,  45, 0 }, Element::Blue, name );
			break;
		case ChampionName::Tyrel:
			return Champion( { 16185, 881, 1343,  95,  15+5, 50+10,  45, 0 }, Element::Blue, name );
			break;
		case ChampionName::VGalek:
			return Champion( { 15195, 1332, 958,  98,  15+5, 60+10,  30, 0+10 }, Element::Blue, name );
			break;
		case ChampionName::VisirOvelis:
			return Champion( { 16350, 1476, 1013,  101,  15, 63,  40, 0+10 }, Element::Red, name );
			break;
		case ChampionName::Voitelnica:
			return Champion( { 10440, 907, 534,  97,  15+5, 50,  30, 0 }, Element::Red, name );
			break;
		case ChampionName::Yuliana:
			return Champion( { 15360, 1398, 881,  103,  15+5, 60,  30, 0+10 }, Element::Blue, name );
			break;
		case ChampionName::Zargala:
			return Champion( { 16380, 1374, 779,  103,  15+5, 60+10,  30, 0+10+16 }, Element::Red, name );
			break;
	}
	_ASSERTE( !"not yet supported" );
	return Champion( { 0, 0, 0,  0,  0, 0,  0, 0 }, Element::Blue, name );
}

ChampionStats GetCurrentArtsStatsFor( ChampionName name )
{
	ChampionStats arts_bonus_stats;
	const Champion ch = Champion::ByName( name );
	const Equipment current_eq = GetCurrentEquipmentFor( name );
	ApplyEquipment( current_eq, ch.BasicStats, arts_bonus_stats, false, false );
	return arts_bonus_stats;
}

ChampionStats GetCurrentFinalStatsFor( ChampionName name )
{
	const Champion ch = Champion::ByName( name );
	return ch.TotalStats( GetCurrentArtsStatsFor( name ) );
}

#ifdef _DEBUG
template <typename IMAP>
int index_map_iterator<IMAP>::_instances_counter = 0;
#endif

