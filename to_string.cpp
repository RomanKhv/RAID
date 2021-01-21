#include "pch.h"
#include "to_string.h"
#include <boost/format.hpp>

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
		CASE_RETURN_STRING( ArtSet, CritDmg );
		CASE_RETURN_STRING( ArtSet, Acc );
		CASE_RETURN_STRING( ArtSet, Speed );
		CASE_RETURN_STRING( ArtSet, Vamp );
		CASE_RETURN_STRING( ArtSet, Resist );
		CASE_RETURN_STRING( ArtSet, Gibel );
		CASE_RETURN_STRING( ArtSet, Mest );
		CASE_RETURN_STRING( ArtSet, Fury );
		CASE_RETURN_STRING( ArtSet, Curing );
		CASE_RETURN_STRING( ArtSet, Reflex );
		CASE_RETURN_STRING( ArtSet, Cursed );
		CASE_RETURN_STRING( ArtSet, Toxic );
		CASE_RETURN_STRING( ArtSet, Frost );
		CASE_RETURN_STRING( ArtSet, Daze );
		CASE_RETURN_STRING( ArtSet, Immunitet );
		CASE_RETURN_STRING( ArtSet, Revenge );
		CASE_RETURN_STRING( ArtSet, Vozmezdie );
		CASE_RETURN_STRING( ArtSet, Shield );
		CASE_RETURN_STRING( ArtSet, Doblest );
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
		CASE_RETURN_STRING( ArtSet, Zhivuchest );
		CASE_RETURN_STRING( ArtSet, Rastoropnost );

		CASE_RETURN_STRING( ArtSet, Bannerets );
		CASE_RETURN_STRING( ArtSet, SupremeElves );
		CASE_RETURN_STRING( ArtSet, HolyOrden );
		CASE_RETURN_STRING( ArtSet, Barbarians );
		CASE_RETURN_STRING( ArtSet, Ogrins );
		CASE_RETURN_STRING( ArtSet, Snakes );
		CASE_RETURN_STRING( ArtSet, Werewolfs );
		CASE_RETURN_STRING( ArtSet, Orks );
		CASE_RETURN_STRING( ArtSet, Demons );
		CASE_RETURN_STRING( ArtSet, Deads );
		CASE_RETURN_STRING( ArtSet, DarkElves );
		CASE_RETURN_STRING( ArtSet, Renegates );
		CASE_RETURN_STRING( ArtSet, Dworves );
		case ArtSet::None: return {};
	}
	_ASSERTE( !"not yet supported" );
	return "";
}

std::string to_string( const Artefact& art, bool format_as_code )
{
	std::stringstream ss;
	if ( !format_as_code )
	{
		ss.width( 8 );	ss << std::left << (to_string( art.Type ) + ":");
		ss.width( 11 );	ss << (boost::format( "[%s]" ) % to_string( art.Set )).str();
		ss << art.Stars << "* (";
		ss.width( 4 ); ss << (boost::format( "%d)" ) % art.Level).str();
		ss.width( 6 ); ss << std::left;
		switch ( art.Type )
		{
			case ArtType::Weapon:
			case ArtType::Helmet:
			case ArtType::Shield:
				ss << "";
				break;
			default:
				ss << to_string( art.MainStatType() );
				break;
		};
		if ( !art.AddStats.empty() )
		{
			ss << "{ ";
			for ( const Stat& s : art.AddStats )
			{
				ss << "{" << to_string( s.Type ) << "," << s.Value << "}, ";
			}
			ss << "}";
		}
		if ( art.Owner != ChampionName::none )
			ss << "   " << to_string( art.Owner );
		if ( !art.Comment.empty() )
			ss << " " << art.Comment;
	}
	else {
		ss << "Artefact( ArtType::" << to_string( art.Type );
		ss << ", ArtSet::" << to_string( art.Set );
		ss << ", " << art.Stars << ", " << art.Level;
		ss << ", StatType::" << to_string( art.MainStatType() );
		ss << ", { ";
		for ( const Stat& s : art.AddStats )
		{
			ss << "{StatType::" << to_string( s.Type ) << "," << s.BaseValue << "}, ";
		}
		if ( art.Owner != ChampionName::none )
		{
			ss << ", {ChampionName::" << to_string( art.Owner );
		}
		ss << "} ),";
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

std::string to_string( const Equipment& eq, bool format_as_code )
{
	std::stringstream ss;
	for ( const Artefact& art : eq.Arts )
	{
		if ( art.Initialized() )
		{
			ss << to_string( art, format_as_code ) << "\n";
		}
	}
	//suppress_last_symbol( ss );
	return ss.str();
}

std::string to_string( const EquipmentRef& eq, bool format_as_code )
{
	std::stringstream ss;
	for ( const Artefact* art : eq.Arts )
	{
		if ( art && art->Initialized() )
		{
			ss << to_string( *art, format_as_code ) << "\n";
		}
	}
	//suppress_last_symbol( ss );
	return ss.str();
}

const char* to_string( const ChampionName& name )
{
	switch ( name )
	{
		CASE_RETURN_STRING( ChampionName, Alura );
		CASE_RETURN_STRING( ChampionName, Arbitr );
		CASE_RETURN_STRING( ChampionName, BlackKnight );
		CASE_RETURN_STRING( ChampionName, ColdHeart );
		CASE_RETURN_STRING( ChampionName, Fein );
		CASE_RETURN_STRING( ChampionName, Foly );
		CASE_RETURN_STRING( ChampionName, Gala );
		CASE_RETURN_STRING( ChampionName, Gorgorab );
		CASE_RETURN_STRING( ChampionName, GornyKorol );
		CASE_RETURN_STRING( ChampionName, Gromoboy );
		CASE_RETURN_STRING( ChampionName, Guard );
		CASE_RETURN_STRING( ChampionName, Hakkorn );
		CASE_RETURN_STRING( ChampionName, Hangar );
		CASE_RETURN_STRING( ChampionName, Hatun );
		CASE_RETURN_STRING( ChampionName, Kael );
		CASE_RETURN_STRING( ChampionName, Killian );
		CASE_RETURN_STRING( ChampionName, Krisk );
		CASE_RETURN_STRING( ChampionName, Lekar );
		CASE_RETURN_STRING( ChampionName, Mashalled );
		CASE_RETURN_STRING( ChampionName, Mavzolejnik );
		CASE_RETURN_STRING( ChampionName, Molly );
		CASE_RETURN_STRING( ChampionName, Razen );
		CASE_RETURN_STRING( ChampionName, Revoglas );
		CASE_RETURN_STRING( ChampionName, Rotos );
		CASE_RETURN_STRING( ChampionName, Skilla );
		CASE_RETURN_STRING( ChampionName, Sohaty );
		CASE_RETURN_STRING( ChampionName, SteelSkull );
		CASE_RETURN_STRING( ChampionName, Straholud );
		CASE_RETURN_STRING( ChampionName, Tyrel );
		CASE_RETURN_STRING( ChampionName, VGalek );
		CASE_RETURN_STRING( ChampionName, VisirOvelis );
		CASE_RETURN_STRING( ChampionName, Voitelnica );
		CASE_RETURN_STRING( ChampionName, Yuliana );
		CASE_RETURN_STRING( ChampionName, Zargala );
	}
	_ASSERTE( !"unreachable code" );
	return "";
}

std::string stats_progress( const ChampionStats& new_stats, const ChampionStats& prev_stats )
{
	std::stringstream ss;
	for ( StatType st : ChampionStats::TypeList )
	{
		ss.width( 7 );	ss << std::left << (to_string(st) + ":");
		ss << new_stats[st];
		const int delta = new_stats[st] - prev_stats[st];
		//ss << "\t(";
		ss << " (";
		if ( delta > 0 )
			ss << "+";
		ss << delta << ")\n";
	}
	suppress_last_symbol( ss );
	return ss.str();
}
