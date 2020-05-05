#pragma once

#include "raid.h"

std::string to_string( const StatType& );
std::string to_string( const ArtType& );
std::string to_string( const ArtSet& );
std::string to_string( const Artefact&, bool format_as_code = false );
std::string to_string( const ChampionStats& );
const char* to_string( const ChampionName& );
std::string to_string( const Equipment&, bool format_as_code = false );
std::string to_string( const EquipmentRef&, bool format_as_code = false );
std::string stats_progress( const ChampionStats& new_stats, const ChampionStats& prev_stats );
