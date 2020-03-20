#include "pch.h"
#include "to_string.h"

#define CASE_RETURN_STRING( et, ei )	case et::ei: return #ei;

void suppress_last_symbol( std::stringstream& ss, char substitution = ' ' )
{
	ss.seekp( -1, std::ios_base::end );
	ss << substitution;
}

std::string to_string( const StatType& t )
{
	switch ( t )
	{
		CASE_RETURN_STRING( StatType, Atk );
		CASE_RETURN_STRING( StatType, Atk_p );
		CASE_RETURN_STRING( StatType, HP );
		CASE_RETURN_STRING( StatType, HP_p );
		CASE_RETURN_STRING( StatType, Def );
		CASE_RETURN_STRING( StatType, Def_p );
		CASE_RETURN_STRING( StatType, CRate );
		CASE_RETURN_STRING( StatType, CDmg );
		CASE_RETURN_STRING( StatType, Spd );
		CASE_RETURN_STRING( StatType, Acc );
		CASE_RETURN_STRING( StatType, Res );
	};
	_ASSERT(false);
	return "";
}

std::string to_string( const ArtType& t )
{
	switch ( t )
	{
		CASE_RETURN_STRING( ArtType, None );
		CASE_RETURN_STRING( ArtType, Weapon );
		CASE_RETURN_STRING( ArtType, Helmet );
		CASE_RETURN_STRING( ArtType, Shield );
		CASE_RETURN_STRING( ArtType, Gloves );
		CASE_RETURN_STRING( ArtType, Chest );
		CASE_RETURN_STRING( ArtType, Boots );
		CASE_RETURN_STRING( ArtType, Ring );
		CASE_RETURN_STRING( ArtType, Necklace );
		CASE_RETURN_STRING( ArtType, Banner );
	}
	_ASSERT( false );
	return "";
}

std::string to_string( const ArtSet& set )
{
	switch ( set )
	{
		CASE_RETURN_STRING( ArtSet, HP );
		CASE_RETURN_STRING( ArtSet, Atk );
		CASE_RETURN_STRING( ArtSet, Def );
		CASE_RETURN_STRING( ArtSet, CRate );
		CASE_RETURN_STRING( ArtSet, CDmg );
		CASE_RETURN_STRING( ArtSet, Acc );
		CASE_RETURN_STRING( ArtSet, Speed );
		CASE_RETURN_STRING( ArtSet, Vamp );
		CASE_RETURN_STRING( ArtSet, Res );
		CASE_RETURN_STRING( ArtSet, Gibel );
		CASE_RETURN_STRING( ArtSet, Mest );
		CASE_RETURN_STRING( ArtSet, Fury );
		CASE_RETURN_STRING( ArtSet, Curing );
		CASE_RETURN_STRING( ArtSet, Reflex );
		CASE_RETURN_STRING( ArtSet, Cursed );
		CASE_RETURN_STRING( ArtSet, Toxic );
		CASE_RETURN_STRING( ArtSet, Frost );
		CASE_RETURN_STRING( ArtSet, Immunitet );
		CASE_RETURN_STRING( ArtSet, Shield );
		CASE_RETURN_STRING( ArtSet, Beshenstvo );
		CASE_RETURN_STRING( ArtSet, Regeneration );
		CASE_RETURN_STRING( ArtSet, Svirepost );
		CASE_RETURN_STRING( ArtSet, Savage );
		CASE_RETURN_STRING( ArtSet, Taunting );
		CASE_RETURN_STRING( ArtSet, Cruel );
		CASE_RETURN_STRING( ArtSet, Immortal );
		CASE_RETURN_STRING( ArtSet, DivAtk );
		CASE_RETURN_STRING( ArtSet, DivCritRate );
		CASE_RETURN_STRING( ArtSet, DivLife );
		CASE_RETURN_STRING( ArtSet, DivSpeed );
	}
	_ASSERTE( !"not yet supported" );
	return "";
}

std::string to_string( const Artefact& art )
{
	std::stringstream ss;
	ss << to_string(art.Type) << ":\t[" << to_string(art.Set) << "]\t";
	ss << art.Stars << "* (" << art.Level << ") ";
	switch ( art.Type )
	{
		case ArtType::Weapon:
		case ArtType::Helmet:
		case ArtType::Shield:
			ss << "\t\t\t";
			break;
		default:
			ss << "\t" << to_string( art.MainStat ) << "\t";
			break;
	};
	if ( !art.AddStats.empty() )
	{
		ss << "{ ";
		for ( const Stat& s : art.AddStats )
		{
			ss << "{" << to_string(s.Type) << "," << s.Value << "}, ";
		}
		ss << "}";
	}
	return ss.str();
}

std::string to_string( const ChampionStats& stats )
{
	std::stringstream ss;
	ss << to_string(StatType::HP) << ":\t " << stats.HP << "\n";
	ss << to_string(StatType::Atk) << ":\t " << stats.Atk << "\n";
	ss << to_string(StatType::Def) << ":\t " << stats.Def << "\n";
	ss << to_string(StatType::Spd) << ":\t " << stats.Spd << "\n";
	ss << to_string(StatType::CRate) << ": " << stats.CRate << "\n";
	ss << to_string(StatType::CDmg) << ":\t " << stats.CDmg << "\n";
	ss << to_string(StatType::Res) << ":\t " << stats.Res << "\n";
	ss << to_string(StatType::Acc) << ":\t " << stats.Acc /*<< "\n"*/;
	return ss.str();
}

std::string to_string( const Equipment& eq )
{
	std::stringstream ss;
	for ( const Artefact& art : eq.Arts )
	{
		if ( art.Initialized() )
		{
			ss << to_string( art ) << "\n";
		}
	}
	//suppress_last_symbol( ss );
	return ss.str();
}

std::string to_string( const EquipmentRef& eq )
{
	std::stringstream ss;
	for ( const Artefact* art : eq.Arts )
	{
		if ( art && art->Initialized() )
		{
			ss << to_string( *art ) << "\n";
		}
	}
	//suppress_last_symbol( ss );
	return ss.str();
}

std::string stats_progress( const ChampionStats& new_stats, const ChampionStats& prev_stats )
{
	std::stringstream ss;
	for ( StatType st : ChampionStats::TypeList )
	{
		ss << to_string(st) << ":\t" << new_stats[st];
		const int delta = new_stats[st] - prev_stats[st];
		if ( delta > 0 )
			ss << ":\t(+";
		else
			ss << ":\t(";
		ss << delta << ")\n";
	}
	return ss.str();
}
