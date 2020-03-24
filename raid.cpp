#include "pch.h"
#include "raid.h"
#include "suiter.h"
#include "stl_ext.h"

/////////////////////////////////////////////////////////////////////////////

bool check_stat_input_error( const Stat& stat )
{
	switch ( stat.Type )
	{
		case StatType::Atk:			return stat.Value >= 16;
		case StatType::HP:			return stat.Value >= 100;
		case StatType::Def:			return stat.Value >= 15;
		case StatType::Atk_p:		return stat.Value < 20;
		case StatType::HP_p:		return stat.Value <= 24;
		case StatType::Def_p:		return stat.Value < 20;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

Artefact::Artefact( ArtType type, ArtSet set, int stars, int level, StatType mainstat, std::vector<Stat> addstats,
					ChampionName owner)
	:Type(type)
	,Set(set)
	,Stars(stars)
	,Level(level)
	,AddStats( addstats.begin(), addstats.end() )
	,Owner(owner)
{
	_MainStat.Type = mainstat;
	_ASSERTE( Initialized() );
	_ASSERTE( IsValidStatForArt( mainstat, type ) );
	_ASSERTE( IsGoodStatForArt( mainstat, type ) );
	_ASSERTE( addstats.size() <= 4 );
#ifdef _DEBUG
	for ( const Stat& stat : AddStats )
		_ASSERTE( check_stat_input_error( stat ) );
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
		_MainStat.Value = StatValueForLevel_fast( Type, _MainStat.Type, Stars, consider_max_level ? 16 : Level );
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
	for ( ArtType at : Equipment::AllTypesArr )
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
	for ( ArtType at : Equipment::AllTypesArr )
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

/////////////////////////////////////////////////////////////////////////////

	typedef int stat_table_t[3][5];

				static const stat_table_t Atk_Def_general = {
					//      0   4    8   12   16
					/*4*/{ 16, 51,  85, 120, 190 },
					/*5*/{ 25, 64, 104, 143, 225 },
					/*6*/{ 35, 80, 125, 170, 265 }
				};
				static const stat_table_t Atk_Def_banner = {
					//      0  4  8  12  16
					/*4*/{ 24, 0, 0, 0, 285 },
					/*5*/{ 38, 0, 0, 0, 338 },
					/*6*/{ 53, 0, 0, 0, 0 }
				};
				static const stat_table_t HP_general = {
					//      0     4     8    12    16
					/*4*/{ 260,    0, 1286, 1800, 2840 },
					/*5*/{ 450, 1044, 1638, 2231, 3480 },
					/*6*/{ 600, 1277, 1954, 2631, 4080 }
				};
				static const stat_table_t HP_banner = {
					//      0   4  8  12   16
					/*4*/{ 390, 0, 0, 0, 4260 },
					/*5*/{ 675, 0, 0, 0, 5220 },
					/*6*/{ 900, 0, 0, 0, 0 }
				};
				static const stat_table_t Atk_HP_Def_percent_CR = {
					//      0   4   8  12  16
					/*4*/{  6, 13, 19, 26, 40 },
					/*5*/{  8, 16, 24, 33, 50 },
					/*6*/{ 10, 20, 29, 39, 60 },
				};
				static const stat_table_t CDmg_general = {
					//      0  4  8 12  16
					/*4*/{  8, 0, 0, 0,  0 },
					/*5*/{ 10, 0, 0, 0, 65 },
					/*6*/{  0, 0, 0, 0,  0 },
				};
				static const stat_table_t CDmg_necklace = {
					//     0  4  8 12  16
					/*4*/{ 4, 0, 0, 0, 25 },
					/*5*/{ 5, 0, 0, 0, 33 },
					/*6*/{ 6, 0, 0, 0, 0 },
				};
				static const stat_table_t Spd_all = {
					//     0   4   8  12  16
					/*4*/{ 4, 10, 16,  0, 35 },
					/*5*/{ 5, 12, 19, 26, 40 },
					/*6*/{ 0, 00, 00, 00, 00 },
				};
				static const stat_table_t Acc_Res = {
					//      0   4   8  12  16
					/*4*/{  8, 0, 0, 0, 64 },
					/*5*/{ 12, 0, 0, 0, 78 },
					/*6*/{ 16, 0, 0, 0, 96 },
				};

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
		case ArtSet::Res:
		case ArtSet::Acc:
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

/////////////////////////////////////////////////////////////////////////////

inline void apply_stat_absolute( StatType st, int value, ChampionStats& arts_bonus_stats )
{
	arts_bonus_stats[st] += value;
}

void ApplyStat( const Stat& stat, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats )
{
	_ASSERTE( stat.Value > 0 );
	switch ( stat.Type )
	{
		case StatType::HP_p:
		case StatType::Atk_p:
		case StatType::Def_p:
			arts_bonus_stats.p_stat(stat.Type) += basic_stats.p_stat(stat.Type) * stat.Value / 100;
			return;
		default:
			apply_stat_absolute( stat.Type, stat.Value, arts_bonus_stats );
			return;
	}
	//switch ( stat.Type )
	//{
	//	case StatType::Atk:
	//		arts_bonus_stats.Atk += stat.Value;
	//		return;
	//	case StatType::HP:
	//		arts_bonus_stats.HP += stat.Value;
	//		return;
	//	case StatType::Def:
	//		arts_bonus_stats.Def += stat.Value;
	//		return;
	//	case StatType::Atk_p:
	//		member_fraction<&ChampionStats::Atk>( arts_bonus_stats, basic_stats, stat.Value );
	//		return;
	//	case StatType::HP_p:
	//		member_fraction<&ChampionStats::HP>( arts_bonus_stats, basic_stats, stat.Value );
	//		return;
	//	case StatType::Def_p:
	//		member_fraction<&ChampionStats::Def>( arts_bonus_stats, basic_stats, stat.Value );
	//		return;
	//	case StatType::CRate:
	//		arts_bonus_stats.CRate += stat.Value;
	//		return;
	//	case StatType::CDmg:
	//		arts_bonus_stats.CDmg += stat.Value;
	//		return;
	//	case StatType::Spd:
	//		arts_bonus_stats.Spd += stat.Value;
	//		return;
	//	case StatType::Acc:
	//		arts_bonus_stats.Acc += stat.Value;
	//		return;
	//	case StatType::Res:
	//		arts_bonus_stats.Res += stat.Value;
	//		return;
	//}
	_ASSERTE( !"unreachable code" );
}

void ApplyStat( const Stat& stat, ChampionExt& ch )
{
	ApplyStat( stat, ch.BasicStats, ch.ArtsBonusStats );
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
		case ArtSet::Atk:
		case ArtSet::Cruel:
			{
				add_percent_bonus<&ChampionStats::Atk>( arts_bonus_stats, basic_stats, 15 );
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
		case ArtSet::CDmg:
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
		case ArtSet::Res:
			{
				arts_bonus_stats.Res += 40;
			}
			break;
		case ArtSet::Acc:
			{
				arts_bonus_stats.Acc += 40;
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

void ApplyArtBonus( const Artefact& art, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool consider_max_level )
{
	if ( !art.Initialized() )
	{
		_ASSERTE( !"invalid art" );
		return;
	}

	ApplyStat( art.GetMainStat(consider_max_level), basic_stats, arts_bonus_stats );

	for ( const Stat& stat : art.AddStats )
		ApplyStat( stat, basic_stats, arts_bonus_stats );
}

void ApplyEquipment( const Equipment& eq, const ChampionStats& basic_stats, ChampionStats& art_bonus_stats, bool estimating, bool consider_max_level )
{
	ApplySetsBonuses( eq, basic_stats, art_bonus_stats, estimating );

	for ( const Artefact& art : eq.Arts )
	{
		if ( art.Initialized() )
		{
			ApplyArtBonus( art, basic_stats, art_bonus_stats, estimating || consider_max_level );
		}
	}
}

void ApplyEquipment( const EquipmentRef& eq, const ChampionStats& basic_stats, ChampionStats& arts_bonus_stats, bool estimating, bool consider_max_level )
{
	ApplySetsBonuses( eq, basic_stats, arts_bonus_stats, estimating );

	for ( const Artefact* art : eq.Arts )
	{
		if ( art )
		{
			_ASSERTE( art->Initialized() );
			ApplyArtBonus( *art, basic_stats, arts_bonus_stats, estimating || consider_max_level );
		}
	}
}

void ApplyHallBonus( const Champion& ch, ChampionStats& stats )
{
	const Hall::table_row_t& hall_bonus = _MyHall.Table[ stl::enum_to_int(ch.Elem) ];

	for ( StatType st : { StatType::HP_p, StatType::Atk_p, StatType::Def_p, StatType::CDmg, StatType::Res, StatType::Acc } )
	{
		const int b = hall_bonus[stl::enum_to_int( st )];
		ApplyStat( { st, b }, ch.BasicStats, stats );
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

MatchOptions::MatchOptions( std::map<StatType, ArtFactor> factors, std::vector<ArtSet> req_filter, std::set<ArtSet> set_filter,
							bool consider_max_lvl, std::map<StatType, int> min_caps, std::map<StatType, int> max_caps )
	:RequiedSets( std::move( req_filter ) )
	,SetFilter( std::move( set_filter ) )
	//,ConsiderMaxLevels( consider_max_lvl )
{
	_ASSERTE( ConsiderMaxLevels == consider_max_lvl );
	for ( const auto& p : factors )
	{
		IsValidStatForChampion( p.first );
		Factors[stl::enum_to_int( p.first )] = p.second;
	}
	for ( const auto& p : min_caps )
	{
		IsValidStatForChampion( p.first );
		_ASSERTE( Factor(p.first) == ArtFactor::NotInterested );
		Factors[stl::enum_to_int( p.first )] = ArtFactor::MinCap;
		MinCap[stl::enum_to_int( p.first )] = p.second;
	}
	for ( const auto& p : max_caps )
	{
		IsValidStatForChampion( p.first );
		//_ASSERTE( Factor(p.first) == ArtFactor::NotInterested );
		//Factors[stl::enum_to_int( p.first )] = ArtFactor::MaxCap;		"max cap" is additional limitation
		MaxCap[stl::enum_to_int( p.first )] = p.second;
	}
}

bool MatchOptions::IsSetAccepted( ArtSet set ) const
{
	if ( SetFilter.empty() )
		return true;	//accept all

	if ( set == ArtSet::None )
		return true;	//ring/necklace/banner

	return SetFilter.count( set ) > 0;
}

bool MatchOptions::IsArtAccepted( const Artefact& art, ChampionName ch_name ) const
{
	if ( art.Owner != ChampionName::none &&
		 art.Owner != ch_name )
		return false;

	if ( !IsSetAccepted( art.Set ) )
		return false;

	return true;
}

bool MatchOptions::IsEqHasRequiredSets( const EquipmentRef& eq ) const
{
	if ( RequiedSets.empty() )
		return true;

	std::map<ArtSet,int> req_sets;
	int total_req_art_count = 0;
	for ( ArtSet set : RequiedSets )
	{
		req_sets[set] ++;
		total_req_art_count += SetSize_fast( set );
	}
	_ASSERTE( total_req_art_count <= 6 );
	if ( total_req_art_count > 6 )
		return false;

	std::map<ArtSet,int> eq_sets;
	for ( size_t iat = 0; iat < Equipment::BasicSize; ++iat )
	{
		if ( const Artefact* art = eq.Arts[iat] )
		{
			_ASSERTE( art->IsBasic() );
			eq_sets[art->Set] ++;
		}
	}

	for ( const auto& rs : req_sets )
	{
		const int n_req_arts = SetSize_fast( rs.first ) * rs.second;
		const int n_eq_arts = stl::get_value_or( eq_sets, rs.first, 0 );
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

Champion ChampionFactory::ColdHeart()
{
	return Champion( { 13710, 1376, 738,  94,  15, 57,  30, 0 }, Element::Void, ChampionName::ColdHeart );
}

Champion ChampionFactory::Kael()
{
	return Champion( { 13710, 1200, 914,  103,  15, 57,  30, 0 }, Element::Blue, ChampionName::Kael );
}

Champion ChampionFactory::Gromoboy()
{
	return Champion( { 15855, 727, 1443,  97,  15, 50,  30, 0 }, Element::Void, ChampionName::Gromoboy );
}

Champion ChampionFactory::Lekar()
{
	return Champion( { 16680, 859, 969,  101,  15, 50,  30, 0 }, Element::Blue, ChampionName::Lekar );
}

Champion ChampionFactory::Yuliana()
{
	return Champion( { 15195, 1354, 870,  103,  15, 50,  30, 0 }, Element::Blue, ChampionName::Yuliana );
}

Champion ChampionFactory::Krisk()
{
	return Champion( { 18660, 727, 1465,  94,  15, 50,  30, 0 }, Element::Void, ChampionName::Krisk );
}

Champion ChampionFactory::Hatun()
{
	return Champion( { 15555, 971, 1146,  97,  15, 50,  30, 0 }, Element::Green, ChampionName::Hatun );
}
