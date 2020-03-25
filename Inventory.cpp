#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

const Hall _MyHall({
	{ Element::Blue,  { {StatType::HP_p,2}, {StatType::Atk_p,2}, {StatType::Def_p,2}, {StatType::CDmg,2}, {StatType::Res,5}, {StatType::Acc,30} }, },
	{ Element::Green, { {StatType::HP_p,2}, {StatType::Atk_p,0}, {StatType::Def_p,2}, {StatType::CDmg,0}, {StatType::Res,0}, {StatType::Acc,10} }, },
	{ Element::Red,   { {StatType::HP_p,2}, {StatType::Atk_p,2}, {StatType::Def_p,2}, {StatType::CDmg,2}, {StatType::Res,0}, {StatType::Acc,10} }, },
	{ Element::Void,  { {StatType::HP_p,2}, {StatType::Atk_p,2}, {StatType::Def_p,3}, {StatType::CDmg,2}, {StatType::Res,5}, {StatType::Acc,30} }, },
});

//std::map<StatType, int> _MyLeage

/////////////////////////////////////////////////////////////////////////////

const std::vector<Artefact> _MyArts =
{
	// HP
	Artefact{ ArtType::Weapon, ArtSet::HP, 6, 16, StatType::Atk, { {StatType::CDmg,13}, {StatType::Spd,12}, {StatType::CRate,11}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 16, StatType::Atk, { {StatType::Acc,19}, {StatType::CDmg,13}, {StatType::HP_p,6},   {StatType::Atk_p,5} } },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, { {StatType::CDmg,10}, {StatType::CRate,9} } },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, { {StatType::HP_p,10}, {StatType::Spd,4} } },

	Artefact{ ArtType::Helmet, ArtSet::HP, 6, 12, StatType::HP, { {StatType::Spd,23}, {StatType::HP_p,7}, {StatType::Res,11}, {StatType::Atk_p,6} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 6, 8, StatType::HP, { {StatType::Res,22}, {StatType::Def_p,5}, {StatType::CDmg,11} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 16, StatType::HP, { {StatType::Acc,27}, {StatType::HP_p,4}, {StatType::Atk,20}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 16, StatType::HP, { {StatType::CRate,11}, {StatType::Atk_p,10} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 8, StatType::HP, { {StatType::Acc,21}, {StatType::CDmg,9} } },

	Artefact{ ArtType::Shield, ArtSet::HP, 6, 12, StatType::Def, { {StatType::CRate,7}, {StatType::Acc,22}, {StatType::Def_p,16}, {StatType::Res,11} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 6, 8, StatType::Def, { {StatType::Spd,6}, {StatType::Acc,18}, {StatType::Res,21} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 8, StatType::Def, { {StatType::Spd,5}, {StatType::CRate,10}, {StatType::HP,594} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 12, StatType::Def, { {StatType::HP_p,14}, {StatType::CRate,10}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 8, StatType::Def, { {StatType::HP_p,5}, {StatType::CRate,17} } },

	Artefact{ ArtType::Gloves, ArtSet::HP, 6, 12, StatType::Def_p, { {StatType::Atk_p,5}, {StatType::Acc,42}, {StatType::HP,343}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Gloves, ArtSet::HP, 5, 12, StatType::Def_p, { {StatType::Atk_p,10}, {StatType::CDmg,15}, {StatType::HP,163} } },
	Artefact{ ArtType::Gloves, ArtSet::HP, 5, 12, StatType::CRate, { {StatType::Atk_p,10}, {StatType::Def_p,11}, {StatType::Spd,5} } },

	Artefact{ ArtType::Chest, ArtSet::HP, 6, 8, StatType::Atk_p, { {StatType::CDmg,6}, {StatType::Acc,11}, {StatType::Spd,11}, {StatType::Atk,44} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 16, StatType::Def_p, { {StatType::Atk_p,10}, {StatType::CRate,11}, {StatType::CDmg,10}, {StatType::Res,8} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 12, StatType::Atk_p, { {StatType::Spd,8}, {StatType::HP_p,9}, {StatType::Def_p,9} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 8, StatType::HP_p, { {StatType::CRate,11}, {StatType::Spd,5} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 13, StatType::Def_p, { {StatType::HP_p,5}, {StatType::Spd,5}, {StatType::Atk_p,6} } },

	Artefact{ ArtType::Boots, ArtSet::HP, 6, 1, StatType::Atk_p, { {StatType::HP_p,7}, {StatType::Def_p,5}, {StatType::HP,453}, {StatType::Spd,6} } },
	Artefact{ ArtType::Boots, ArtSet::HP, 6, 4, StatType::Def_p, { {StatType::CRate,12}, {StatType::CDmg,6}, {StatType::HP,209} } },
	Artefact{ ArtType::Boots, ArtSet::HP, 4, 16, StatType::Spd, { {StatType::Def_p,7+1}, {StatType::CRate,5}, {StatType::Acc,7}, {StatType::Def,8+7} } },

#ifndef DEBUG_FIND_BEST
	// Atk
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 8, StatType::Atk, { {StatType::CRate,6}, {StatType::Acc,22}, {StatType::Res,11}, {StatType::HP,1012} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 12, StatType::Atk, { {StatType::CDmg,6}, {StatType::HP_p,24}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,4}, {StatType::Spd,13}, {StatType::HP,442}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,10}, {StatType::HP_p,16}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,9}, {StatType::HP,161}, {StatType::Spd,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 12, StatType::Atk, { {StatType::Spd,13}, {StatType::Acc,8}, {StatType::CRate,5} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 4, 12, StatType::Atk, { {StatType::HP_p,15}, {StatType::CDmg,3}, {StatType::Spd,3} } },

	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 12, StatType::HP, { {StatType::HP_p,6}, {StatType::Def_p,18}, {StatType::CRate,13}, {StatType::Acc,12} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 8, StatType::HP, { {StatType::Acc,23}, {StatType::Res,22}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Def,20}, {StatType::CRate,6}, {StatType::Atk_p,16} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Atk,31}, {StatType::CRate,6}, {StatType::Def_p,8} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 12, StatType::HP, { {StatType::Def_p,12}, {StatType::CRate,12}, {StatType::Res,8} } },

	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 8, StatType::Def, { {StatType::Def_p,14}, {StatType::HP,344}, {StatType::CRate,7}, {StatType::Res,21} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 8, StatType::Def, { {StatType::Res,21}, {StatType::HP_p,11}, {StatType::Spd,6}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 12, StatType::Def, { {StatType::Spd,15}, {StatType::Def_p,13}, {StatType::Acc,12} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP,136}, {StatType::Acc,8}, {StatType::CRate,11} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP_p,4}, {StatType::Acc,18} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::CRate,10}, {StatType::HP_p,10}, {StatType::CDmg,4} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::HP_p,10} } },

	Artefact{ ArtType::Gloves, ArtSet::Atk, 6, 8, StatType::CDmg, { {StatType::Spd,11}, {StatType::Res,12}, {StatType::Acc,10}, {StatType::CRate,12} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 6, 8, StatType::HP_p, { {StatType::Spd,8}, {StatType::Def,33}, {StatType::Acc,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Atk_p, { {StatType::Spd,5}, {StatType::CDmg,11}, {StatType::Acc,17} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Def_p, { {StatType::Atk_p,11}, {StatType::Acc,9} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 0, StatType::CDmg, { {StatType::CRate,4} } },

	Artefact{ ArtType::Chest, ArtSet::Atk, 6, 8, StatType::HP_p, { {StatType::Spd,6}, {StatType::Def_p,11}, {StatType::CRate,6}, {StatType::HP,379} } },
	Artefact{ ArtType::Chest, ArtSet::Atk, 5, 8, StatType::HP_p, { {StatType::Atk,22}, {StatType::Atk_p,9}, {StatType::Def_p,10} } },

	Artefact{ ArtType::Boots, ArtSet::Atk, 6, 1, StatType::HP_p, { {StatType::Def_p,7}, {StatType::CDmg,6}, {StatType::Acc,11}, {StatType::CRate,5} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 6, 1, StatType::Atk_p, { {StatType::Def,30}, {StatType::Spd,6}, {StatType::Acc,11}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::Atk_p,8}, {StatType::CDmg,5}, {StatType::HP,426} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::HP_p,10}, {StatType::CRate,5}, {StatType::Def,19} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::CRate,9}, {StatType::Def_p,6}, {StatType::CDmg,5} } },

	// Def
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::CRate,8}, {StatType::HP_p,10}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::Acc,17}, {StatType::Res,19} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::CDmg,10}, {StatType::Spd,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::CRate,11}, {StatType::Atk_p,12} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 4, 12, StatType::Atk, { {StatType::Atk_p,11}, {StatType::CRate,6}, {StatType::Atk_p,6} } },

	Artefact{ ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, { {StatType::Def_p,11}, {StatType::Atk_p,10} } },
	Artefact{ ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, { {StatType::CRate,5}, {StatType::Acc,26} } },
	Artefact{ ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, { {StatType::Atk,21}, {StatType::HP_p,16} } },

	Artefact{ ArtType::Shield, ArtSet::Def, 5, 12, StatType::Def, { {StatType::Spd,10}, {StatType::Def_p,9}, {StatType::CDmg,10}, {StatType::CRate,6} } },
	Artefact{ ArtType::Shield, ArtSet::Def, 5, 12, StatType::Def, { {StatType::CRate,6}, {StatType::Res,20+2}, {StatType::HP_p,16+1} } },
	Artefact{ ArtType::Shield, ArtSet::Def, 5, 8, StatType::Def, { {StatType::HP_p,6}, {StatType::Def_p,13} } },
	Artefact{ ArtType::Shield, ArtSet::Def, 5, 8, StatType::Def, { {StatType::Acc,18}, {StatType::CRate,5} } },

	Artefact{ ArtType::Gloves, ArtSet::Def, 5, 8, StatType::HP_p, { {StatType::CRate,10}, {StatType::CDmg,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Def, 4, 10, StatType::Def_p, { {StatType::CDmg,5}, {StatType::Acc,23}, {StatType::HP,278+125} } },
	Artefact{ ArtType::Gloves, ArtSet::Def, 4, 8, StatType::CRate, { {StatType::Spd,7}, {StatType::Def_p,7} } },
	Artefact{ ArtType::Gloves, ArtSet::Def, 4, 1, StatType::CRate, { {StatType::Atk_p,4} } },

	Artefact{ ArtType::Chest, ArtSet::Def, 5, 8, StatType::HP_p, { {StatType::Def_p,9}, {StatType::Res,10} } },
	Artefact{ ArtType::Chest, ArtSet::Def, 5, 8, StatType::Def_p, { {StatType::Def,30}, {StatType::HP,372} } },

	Artefact{ ArtType::Boots, ArtSet::Def, 5, 13, StatType::Spd, { {StatType::Atk,25}, {StatType::Acc,9}, {StatType::Def,17} } },

	Artefact{ ArtType::Weapon, ArtSet::Def, 4, 12, StatType::Atk, { {StatType::CRate,11}, {StatType::Acc,15+1}, {StatType::HP_p,4+1} }, ChampionName::Gromoboy },
	Artefact{ ArtType::Gloves, ArtSet::Def, 5, 16, StatType::Def_p, { {StatType::Spd,13+1}, {StatType::Acc,9+2}, {StatType::CRate,6}, {StatType::Res,8+2} }, ChampionName::Gromoboy },
	Artefact{ ArtType::Chest, ArtSet::Def, 5, 12, StatType::HP_p, { {StatType::Def,43+10}, {StatType::CDmg,12}, {StatType::Res,10+2} }, ChampionName::Gromoboy },
	Artefact{ ArtType::Boots, ArtSet::Def, 4, 16, StatType::Spd, { {StatType::Def_p,8+1}, {StatType::Atk,6+10}, {StatType::Res,9+2}, {StatType::Def,10+5} }, ChampionName::Gromoboy },

	// CRate
	Artefact{ ArtType::Weapon, ArtSet::CRate, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::HP_p,10}, {StatType::Res,9} } },
	Artefact{ ArtType::Weapon, ArtSet::CRate, 4, 1, StatType::Atk, { {StatType::CRate,4}, {StatType::Spd,4} } },

	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::Acc,26}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 12, StatType::HP, { {StatType::Atk_p,10}, {StatType::CDmg,11}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 12, StatType::HP, { {StatType::CDmg,11}, {StatType::CRate,11}, {StatType::Spd,4} } },
	Artefact{ ArtType::Helmet, ArtSet::CRate, 4, 12, StatType::HP, { {StatType::Atk_p,8}, {StatType::CDmg,9}, {StatType::Acc,15} } },

	Artefact{ ArtType::Shield, ArtSet::CRate, 5, 12, StatType::Def, { {StatType::Def_p,9}, {StatType::Spd,8}, {StatType::CRate,6} } },
	Artefact{ ArtType::Shield, ArtSet::CRate, 4, 4, StatType::Def, { {StatType::HP_p,8}, {StatType::Def_p,3} } },
	Artefact{ ArtType::Shield, ArtSet::CRate, 4, 4, StatType::Def, { {StatType::HP_p,5}, {StatType::CRate,4} } },

	Artefact{ ArtType::Gloves, ArtSet::CRate, 5, 12, StatType::CRate, { {StatType::CDmg,8}, {StatType::Spd,9}, {StatType::Acc,19} } },
	Artefact{ ArtType::Gloves, ArtSet::CRate, 5, 12, StatType::CRate, { {StatType::CDmg,6}, {StatType::Atk_p,14}, {StatType::Atk,16} } },
#endif

	// Acc
	Artefact{ ArtType::Weapon, ArtSet::Acc, 6, 8, StatType::Atk, { {StatType::Atk_p,12}, {StatType::Res,21} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 12, StatType::Atk, { {StatType::Atk_p,15}, {StatType::CRate,5}, {StatType::Spd,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 12, StatType::Atk, { {StatType::Acc,18}, {StatType::CRate,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 4, 8, StatType::Atk, { {StatType::CDmg,7}, {StatType::HP_p,4}, {StatType::Atk_p,7} } },

	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP, { {StatType::Def_p,9+1}, {StatType::Spd,4+1}, {StatType::Res,28+2}, {StatType::CRate,9} }, ChampionName::Gromoboy },
#ifndef DEBUG_FIND_BEST
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 8, StatType::HP, { {StatType::Acc,28}, {StatType::Spd,4}, {StatType::Atk,15+5} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 8, StatType::HP, { {StatType::Spd,8}, {StatType::Res,9}, {StatType::HP_p,11} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::HP_p,12}, {StatType::Atk_p,11}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Spd,9}, {StatType::CRate,10} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 4, 8, StatType::HP, { {StatType::Def_p,4}, {StatType::CRate,8}, {StatType::Atk,18} } },

#endif
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 16, StatType::Def, { {StatType::HP_p,6+1}, {StatType::Def_p,5+2}, {StatType::CDmg,15}, {StatType::CRate,16} }, ChampionName::Gromoboy },
#ifndef DEBUG_FIND_BEST
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 8, StatType::Def, { {StatType::Def_p,11}, {StatType::HP,219}, {StatType::Res,21} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 8, StatType::Def, { {StatType::Acc,19}, {StatType::Res,18}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 12, StatType::Def, { {StatType::Acc,20}, {StatType::Def_p,5}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 4, 8, StatType::Def, { {StatType::Res,15}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 4, 8, StatType::Def, { {StatType::HP_p,4}, {StatType::Spd,11} } },

	Artefact{ ArtType::Gloves, ArtSet::Acc, 6, 12, StatType::Atk_p, { {StatType::CDmg,20}, {StatType::Spd,5}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Acc, 5, 12, StatType::Atk_p, { {StatType::CDmg,10}, {StatType::Def_p,9} } },
	Artefact{ ArtType::Gloves, ArtSet::Acc, 4, 1, StatType::Atk_p, { {StatType::CRate,4}, {StatType::Spd,4}, {StatType::HP_p,4} } },
	Artefact{ ArtType::Gloves, ArtSet::Acc, 4, 8, StatType::CRate, { {StatType::Def_p,7}, {StatType::CDmg,7} } },

	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 12, StatType::Atk_p, { {StatType::Def_p,5}, {StatType::CDmg,15}, {StatType::Acc,10} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 12, StatType::Atk_p, { {StatType::HP_p,10}, {StatType::Def_p,10}, {StatType::Spd,4} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 4, 8, StatType::HP_p,  { {StatType::Atk,9+7}, {StatType::Atk_p,8}, {StatType::Acc,7}, {StatType::Def_p,8} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 4, 12, StatType::HP_p, { {StatType::Atk_p,7}, {StatType::CRate,4}, {StatType::CDmg,4} } },

	Artefact{ ArtType::Boots, ArtSet::Acc, 6, 8, StatType::Def_p, { {StatType::Atk_p,13}, {StatType::CRate,13}, {StatType::Atk,20} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 5, 0, StatType::Atk_p, { {StatType::HP,280}, {StatType::Acc,9} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 5, 8, StatType::Spd,   { {StatType::CRate,11}, {StatType::Atk,31} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 4, 8, StatType::Spd,   { {StatType::CRate,4}, {StatType::HP_p,12} } },

	// Spd
	Artefact{ ArtType::Weapon, ArtSet::Speed, 6, 12, StatType::Atk, { {StatType::HP_p,6}, {StatType::Spd,12}, {StatType::HP,816}, {StatType::Acc,21} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 6, 12, StatType::Atk, { {StatType::Atk_p,19}, {StatType::Acc,21}, {StatType::HP,307} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Res,10}, {StatType::CRate,8}, {StatType::Atk_p,15}, {StatType::Spd,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::Acc,32}, {StatType::HP_p,5+1}, {StatType::CDmg,11}, {StatType::Atk_p,5} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Acc,17}, {StatType::Atk_p,5}, {StatType::Res,31} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Spd,9}, {StatType::HP,257}, {StatType::CDmg,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Res,19}, {StatType::Acc,10}, {StatType::CDmg,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::Acc,17}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::CRate,11}, {StatType::HP_p,8} },												ChampionName::Lekar },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Spd,8}, {StatType::HP,268} } },

	Artefact{ ArtType::Helmet, ArtSet::Speed, 6, 12, StatType::HP, { {StatType::CDmg,11}, {StatType::Def_p,11}, {StatType::HP_p,14} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 6, 8, StatType::HP, { {StatType::Def,26}, {StatType::CRate,6}, {StatType::CDmg,19}, {StatType::Res,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 13, StatType::HP, { {StatType::Atk_p,11}, {StatType::HP_p,4}, {StatType::Def,44}, {StatType::Res,10} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::Atk,45}, {StatType::Res,11}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 12, StatType::HP, { {StatType::HP_p,6}, {StatType::CDmg,17}, {StatType::Atk,19} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::Atk,16}, {StatType::HP_p,14} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::Acc,19}, {StatType::Atk_p,11} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 12, StatType::HP, { {StatType::Atk_p,16}, {StatType::Acc,8}, {StatType::CDmg,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::CDmg,4}, {StatType::Res,10} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::HP_p,11}, {StatType::Acc,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 12, StatType::HP, { {StatType::Def_p,4}, {StatType::Spd,9}, {StatType::Acc,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 12, StatType::HP, { {StatType::Atk_p,4}, {StatType::Def_p,14}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 8, StatType::HP, { {StatType::Atk_p,7}, {StatType::Spd,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 8, StatType::HP, { {StatType::Acc,17}, {StatType::Def_p,9} } },

	Artefact{ ArtType::Shield, ArtSet::Speed, 6, 8, StatType::Def, { {StatType::Res,11}, {StatType::HP_p,5}, {StatType::CRate,6}, {StatType::CDmg,17} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 6, 12, StatType::Def, { {StatType::CDmg,19}, {StatType::CRate,7}, {StatType::Def_p,12} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::HP,197}, {StatType::CDmg,10}, {StatType::Spd,9} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Res,8}, {StatType::Acc,18}, {StatType::CDmg,9} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 12, StatType::Def, { {StatType::HP_p,10}, {StatType::Acc,10}, {StatType::Def_p,16} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 12, StatType::Def, { {StatType::Acc,17}, {StatType::HP_p,10}, {StatType::Res,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::CDmg,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Spd,8}, {StatType::CDmg,9} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::CRate,10}, {StatType::HP_p,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::Acc,11} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 8, StatType::Def, { {StatType::CDmg,8}, {StatType::Spd,3}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 12, StatType::Def, { {StatType::Def_p,13}, {StatType::Acc,6}, {StatType::Res,8} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 8, StatType::Def, { {StatType::CDmg,4}, {StatType::Spd,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 8, StatType::Def, { {StatType::Spd,6}, {StatType::Acc,13} } },

	Artefact{ ArtType::Gloves, ArtSet::Speed, 6, 1, StatType::CDmg, { {StatType::Def_p,6}, {StatType::CRate,6}, {StatType::Def,18}, {StatType::HP,220} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 6, 12, StatType::CRate, { {StatType::Acc,29}, {StatType::HP,301}, {StatType::Atk,50}, {StatType::Def_p,7} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 1, StatType::CDmg, { {StatType::Spd,4}, {StatType::Def,20}, {StatType::Res,11}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::Def_p, { {StatType::Def,30}, {StatType::HP_p,5}, {StatType::Atk_p,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::CRate, { {StatType::Atk,21}, {StatType::Def,20}, {StatType::Spd,14} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::Atk_p, { {StatType::Acc,27}, {StatType::Spd,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::CRate, { {StatType::Spd,5}, {StatType::Def,54} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 4, 8, StatType::HP_p, { {StatType::Acc,14}, {StatType::CRate,7} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 4, 8, StatType::Atk_p, { {StatType::Def_p,12}, {StatType::CDmg,3} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 4, 8, StatType::Def_p, { {StatType::Atk_p,5}, {StatType::Spd,3} } },

	//Artefact{ ArtType::Chest, ArtSet::Speed, 6, 1, StatType::Res, { {StatType::CRate,6}, {StatType::Atk_p,7}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 8, StatType::HP_p, { {StatType::HP,155}, {StatType::CDmg,6}, {StatType::Acc,29} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 8, StatType::Def_p, { {StatType::Res,21}, {StatType::Atk,22} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 1, StatType::Atk_p, {  } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 16, StatType::Def_p, { {StatType::Acc,14}, {StatType::Def,20}, {StatType::HP_p,5}, {StatType::Atk,12+5} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 10, StatType::Def_p, { {StatType::Atk,23}, {StatType::HP_p,9} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, { {StatType::HP_p,5}, {StatType::Spd,10} } },

	Artefact{ ArtType::Boots, ArtSet::Speed, 6, 1, StatType::Def_p, { {StatType::Def,29}, {StatType::HP,400}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 12, StatType::Spd, { {StatType::Def_p,9}, {StatType::Acc,19}, {StatType::Res,19} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 4, StatType::Atk_p, { {StatType::Def_p,6}, {StatType::Acc,20}, {StatType::Spd,5} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, { {StatType::Atk,37}, {StatType::CRate,11} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 1, StatType::HP_p, { {StatType::Atk,14+4}, {StatType::Spd,5} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 4, 16, StatType::Spd, { {StatType::CDmg,5}, {StatType::Res,9+2}, {StatType::Atk,12+5}, {StatType::Def,13+5} } },

	// Vamp
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::CDmg,6}, {StatType::CRate,15}, {StatType::Atk_p,5}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CRate,8}, {StatType::Atk_p,10}, {StatType::Spd,5} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CRate,14}, {StatType::Acc,9}, {StatType::HP,157} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CDmg,14}, {StatType::CRate,6}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 8, StatType::Atk, { {StatType::CDmg,6}, {StatType::Spd,12} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::Atk_p,15}, {StatType::Spd,4}, {StatType::CRate,5} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CRate,10}, {StatType::Spd,4}, {StatType::HP_p,6} } },

	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 8, StatType::HP, { {StatType::Def_p,8}, {StatType::Spd,5}, {StatType::Atk,34} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Spd,15}, {StatType::Def_p,6}, {StatType::CDmg,4}, {StatType::Res,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::Atk_p,5}, {StatType::Acc,31}, {StatType::HP_p,4} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::CRate,6}, {StatType::HP_p,14}, {StatType::Atk,21} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::Acc,19}, {StatType::Spd,4} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::Def_p,4}, {StatType::CDmg,7}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::CRate,4}, {StatType::Def_p,13}, {StatType::Spd,4} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::Spd,6}, {StatType::CRate,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::CRate,8}, {StatType::HP_p,8} } },
#endif

	Artefact{ ArtType::Shield, ArtSet::Vamp, 6, 12, StatType::Def, { {StatType::Acc,11}, {StatType::CRate,19}, {StatType::HP,423} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 8, StatType::Def, { {StatType::CRate,9}, {StatType::HP,185} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::HP,232} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 4, 8, StatType::Def, { {StatType::HP_p,4}, {StatType::CRate,8}, {StatType::HP,280} } },

	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 8, StatType::CRate, { {StatType::CDmg,12}, {StatType::Acc,10}, {StatType::Def,31} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 8, StatType::Def_p, { {StatType::CDmg,10}, {StatType::Def,35} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 8, StatType::Def_p, { {StatType::Def,21}, {StatType::HP_p,17} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 12, StatType::CRate, { {StatType::CDmg,9}, {StatType::Acc,15}, {StatType::Def_p,4} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 8, StatType::Def_p, { {StatType::HP_p,9}, {StatType::Spd,4} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 8, StatType::CRate, { {StatType::Atk_p,7}, {StatType::CDmg,5} } },

	//Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 0, StatType::Res, { {StatType::Acc,9}, {StatType::CDmg,4}, {StatType::CRate,4} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::CDmg,11}, {StatType::HP_p,4}, {StatType::Spd,4}, {StatType::Atk_p,4} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Atk_p,10}, {StatType::CRate,6}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 8, StatType::Def_p, { {StatType::Spd,9}, {StatType::Def,16} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 4, 8, StatType::Atk_p, { {StatType::Atk,19}, {StatType::Acc,15} } },

	Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 12, StatType::Spd, { {StatType::Acc,19}, {StatType::HP_p,10}, {StatType::CRate,4} } },
	Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 8, StatType::Spd, { {StatType::Res,18}, {StatType::Acc,20} } },
	Artefact{ ArtType::Boots, ArtSet::Vamp, 4, 8, StatType::Spd, { {StatType::HP,266}, {StatType::CDmg,8} } },

#ifndef DEBUG_FIND_BEST
	// Cruel
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 1, StatType::Atk, { {StatType::Atk_p,5}, {StatType::Res,8}, {StatType::Acc,10}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 1, StatType::Atk, { {StatType::HP_p,6}, {StatType::Spd,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 4, 8, StatType::Atk, { {StatType::Acc,8}, {StatType::Atk_p,3}, {StatType::HP,417}, {StatType::HP_p,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 4, 8, StatType::Atk, { {StatType::Acc,16}, {StatType::CRate,5}, {StatType::HP,244}, {StatType::Res,15} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 4, 8, StatType::Atk, { {StatType::HP,271}, {StatType::CRate,4}, {StatType::CDmg,13} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 4, 8, StatType::Atk, { {StatType::HP_p,13}, {StatType::HP,159}, {StatType::Acc,7} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 4, 8, StatType::Atk, { {StatType::CRate,7}, {StatType::Atk_p,9}, {StatType::CDmg,4} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 4, 8, StatType::Atk, { {StatType::Atk_p,7}, {StatType::Acc,15} } ),

	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 8, StatType::HP, { {StatType::Res,9}, {StatType::Def_p,11}, {StatType::Atk,42}, {StatType::HP_p,4} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 8, StatType::HP, { {StatType::Res,8}, {StatType::CDmg,15}, {StatType::Atk_p,6}, {StatType::Def_p,6} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 8, StatType::HP, { {StatType::CDmg,10}, {StatType::Spd,9} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 4, 12, StatType::HP, { {StatType::Acc,9}, {StatType::Def_p,4}, {StatType::CRate,9}, {StatType::HP_p,11} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 4, 12, StatType::HP, { {StatType::Spd,4}, {StatType::CDmg,8}, {StatType::CRate,9}, {StatType::HP_p,8} } ),

	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 8, StatType::Def, { {StatType::HP_p,5}, {StatType::CDmg,10}, {StatType::Res,9}, {StatType::Acc,19} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::Acc,18}, {StatType::CDmg,16}, {StatType::Spd,4} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::CDmg,10}, {StatType::Def_p,11}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 4, 8, StatType::Def, { {StatType::Spd,4}, {StatType::HP_p,12}, {StatType::Def_p,4}, {StatType::HP,278} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 4, 1, StatType::Def, { {StatType::Def_p,4}, {StatType::Acc,7} } ),

	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 8, StatType::Def_p, { {StatType::Res,10}, {StatType::Spd,14} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 8, StatType::CRate, { {StatType::CDmg,9}, {StatType::Atk,22} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 4, 8, StatType::Atk_p, { {StatType::CRate,4}, {StatType::Spd,7}, {StatType::HP_p,6} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 4, 8, StatType::Def_p, { {StatType::Atk_p,7}, {StatType::Acc,7} } ),

	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 8, StatType::HP_p, { {StatType::Acc,9}, {StatType::Def,18}, {StatType::CDmg,15} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 4, 8, StatType::Def_p, { {StatType::Acc,7}, {StatType::CDmg,5}, {StatType::CRate,10}, {StatType::Atk,14+5} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 4, 8, StatType::HP_p, { {StatType::Atk_p,8}, {StatType::Spd,7}, {StatType::Def_p,4} } ),

	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 1, StatType::Atk_p, { {StatType::Atk,17}, {StatType::HP,216}, {StatType::HP_p,5}, {StatType::Spd,5} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 8, StatType::Atk_p, { {StatType::CDmg,14}, {StatType::Def,23} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 4, 1, StatType::Def_p, { {StatType::Atk,15+5}, {StatType::Def,14+4}, {StatType::Atk_p,4}, {StatType::CDmg,4} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 4, 1, StatType::Def_p, { {StatType::CDmg,4}, {StatType::CRate,4} } ),

	// Immortal
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::CRate,10}, {StatType::Acc,18}, {StatType::HP_p,5}, {StatType::HP,689} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::Atk_p,4}, {StatType::Spd,9}, {StatType::HP_p,15} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 8, StatType::Atk, { {StatType::Acc,17}, {StatType::Res,9}, {StatType::CDmg,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 8, StatType::Atk, { {StatType::HP_p,10}, {StatType::CDmg,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 4, 12, StatType::Atk, { {StatType::HP_p,4}, {StatType::Acc,16}, {StatType::Spd,8}, {StatType::Atk_p,8} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 4, 8, StatType::Atk, { {StatType::HP_p,7}, {StatType::Acc,6}, {StatType::Spd,7} } ),

	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::Acc,35}, {StatType::CDmg,5}, {StatType::Def_p,5} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::Def_p,8}, {StatType::CRate,9}, {StatType::Acc,20} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 4, 8, StatType::HP, { {StatType::CDmg,9}, {StatType::Spd,3}, {StatType::HP_p,8} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 4, 8, StatType::HP, { {StatType::Acc,13}, {StatType::CDmg,9} } ),

	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Spd,9}, {StatType::Acc,10}, {StatType::CDmg,16} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 8, StatType::Def, { {StatType::Res,19}, {StatType::Def_p,4}, {StatType::Spd,9} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Def_p,11}, {StatType::CRate,11}, {StatType::Res,11} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Acc,19}, {StatType::CDmg,10}, {StatType::HP,221} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 1, StatType::Def, { {StatType::CDmg,5}, {StatType::Acc,10} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 9, StatType::Def, { {StatType::CRate,5}, {StatType::Acc,8} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 4, 8, StatType::Def, { {StatType::CRate,8}, {StatType::Def_p,4}, {StatType::Spd,4}, {StatType::Acc,16} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 4, 12, StatType::Def, { {StatType::CDmg,4}, {StatType::Def_p,9+1}, {StatType::Acc,22} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 4, 8, StatType::Def, { {StatType::Def_p,5}, {StatType::CRate,3}, {StatType::CDmg,13} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 4, 1, StatType::Def, { {StatType::HP_p,5}, {StatType::CDmg,4}, {StatType::Def_p,4} } ),

	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 16, StatType::Atk_p, { {StatType::CRate,5}, {StatType::Acc,16}, {StatType::HP_p,17}, {StatType::Res,10} } ),
#endif
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 8, StatType::CRate, { {StatType::HP_p,15}, {StatType::Atk_p,6}, {StatType::Def,18} } ),
#ifndef DEBUG_FIND_BEST
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 8, StatType::Def_p, { {StatType::CRate,11}, {StatType::Res,8} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 4, 16, StatType::Def_p, { {StatType::Spd,6+1}, {StatType::CRate,4}, {StatType::Atk,18}, {StatType::CDmg,13} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 4, 12, StatType::Def_p, { {StatType::Spd,3+1}, {StatType::Res,15+2}, {StatType::HP_p,14+1} } ),

	Artefact( ArtType::Boots, ArtSet::Immortal, 4, 8, StatType::Spd, { {StatType::Atk,27}, {StatType::Acc,8} } ),
	Artefact( ArtType::Boots, ArtSet::Immortal, 5, 8, StatType::Def_p, { {StatType::CDmg,11}, {StatType::HP_p,5} } ),

	// DivAtk
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 6, 16, StatType::Atk, { {StatType::HP,860}, {StatType::HP_p,12}, {StatType::Atk_p,10}, {StatType::Acc,12+1} } ),
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 5, 12, StatType::Atk, { {StatType::Atk_p,5}, {StatType::HP_p,10}, {StatType::CDmg,16} } ),
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 5, 8, StatType::Atk, { {StatType::Res,29}, {StatType::Acc,9}, {StatType::Atk_p,5} } ),

	Artefact( ArtType::Helmet, ArtSet::DivAtk, 6, 8, StatType::HP, { {StatType::Res,9}, {StatType::HP_p,12}, {StatType::Atk_p,11} } ),
	Artefact( ArtType::Helmet, ArtSet::DivAtk, 5, 12, StatType::HP, { {StatType::CDmg,15}, {StatType::CRate,5}, {StatType::Atk_p,10} } ),

	Artefact( ArtType::Shield, ArtSet::DivAtk, 6, 12, StatType::Def, { {StatType::Spd,5}, {StatType::Res,19}, {StatType::CRate,13}, {StatType::Def_p,11} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::Acc,9}, {StatType::HP_p,11}, {StatType::CDmg,16} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::Def_p,15}, {StatType::Acc,18}, {StatType::HP_p,4} } ),

	Artefact( ArtType::Gloves, ArtSet::DivAtk, 6, 8, StatType::CRate, { {StatType::Def_p,13}, {StatType::HP,483}, {StatType::Res,22} } ),
	Artefact( ArtType::Gloves, ArtSet::DivAtk, 5, 8, StatType::Atk_p, { {StatType::Atk,46}, {StatType::CRate,4}, {StatType::Def_p,6} } ),
	Artefact( ArtType::Gloves, ArtSet::DivAtk, 5, 8, StatType::CRate, { {StatType::Atk,40}, {StatType::CDmg,5}, {StatType::Def_p,11} } ),

	Artefact( ArtType::Chest, ArtSet::DivAtk, 6, 8, StatType::HP_p, { {StatType::CRate,13}, {StatType::Atk_p,6}, {StatType::Atk,52} } ),

	Artefact( ArtType::Boots, ArtSet::DivAtk, 6, 8, StatType::Atk_p, { {StatType::CDmg,11}, {StatType::Atk,20}, {StatType::HP,836} } ),

	// DivCritRate
	Artefact( ArtType::Weapon, ArtSet::DivCritRate, 4, 1, StatType::Atk, { {StatType::Acc,3}, {StatType::HP,180}, {StatType::CRate,4} } ),

	Artefact( ArtType::Helmet, ArtSet::DivCritRate, 5, 9, StatType::HP, { {StatType::CDmg,12}, {StatType::Spd,9} } ),
	Artefact( ArtType::Helmet, ArtSet::DivCritRate, 4, 1, StatType::HP, { {StatType::CDmg,5}, {StatType::Atk_p,5} } ),

	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::Acc,19}, {StatType::Def_p,5}, {StatType::HP_p,16} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::CDmg,15}, {StatType::HP_p,4}, {StatType::Spd,5} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::Spd,10}, {StatType::HP_p,5}, {StatType::CRate,4} } ),

	Artefact( ArtType::Gloves, ArtSet::DivCritRate, 4, 8, StatType::HP_p, { {StatType::CRate,8}, {StatType::Def_p,4} } ),

	Artefact( ArtType::Boots, ArtSet::DivCritRate, 4, 8, StatType::Spd, { {StatType::Res,15}, {StatType::CRate,3} } ),

	// DivLife
	Artefact( ArtType::Weapon, ArtSet::DivLife, 6, 12, StatType::Atk, { {StatType::HP_p,12}, {StatType::Atk_p,19}, {StatType::Spd,5}, {StatType::CDmg,6} } ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 6, 16, StatType::Atk, { {StatType::HP_p,11}, {StatType::CDmg,6}, {StatType::Atk_p,17}, {StatType::Acc,11} } ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 5, 12, StatType::Atk, { {StatType::Spd,4}, {StatType::Acc,21}, {StatType::HP_p,15} } ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 5, 8, StatType::Atk, { {StatType::Res,10}, {StatType::CDmg,4}, {StatType::Atk_p,14} } ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 4, 12, StatType::Atk, { {StatType::CRate,4}, {StatType::HP_p,9+1}, {StatType::Atk_p,12+1} } ),

	Artefact( ArtType::Helmet, ArtSet::DivLife, 6, 12, StatType::HP, { {StatType::Spd,16}, {StatType::Atk_p,7}, {StatType::CDmg,13} } ),
	Artefact( ArtType::Helmet, ArtSet::DivLife, 5, 8, StatType::HP, { {StatType::Atk,30}, {StatType::Spd,9}, {StatType::Def_p,5} } ),

	Artefact( ArtType::Shield, ArtSet::DivLife, 6, 8, StatType::Def, { {StatType::HP_p,11}, {StatType::Def_p,5}, {StatType::HP,800}, {StatType::Res,10} } ),

	Artefact( ArtType::Gloves, ArtSet::DivLife, 6, 12, StatType::Def_p, { {StatType::HP_p,11}, {StatType::Def,27}, {StatType::Acc,10}, {StatType::CRate,18} } ),
	Artefact( ArtType::Gloves, ArtSet::DivLife, 5, 8, StatType::HP_p, { {StatType::Acc,10}, {StatType::CDmg,14}, {StatType::Atk,13+5} } ),

	Artefact( ArtType::Chest, ArtSet::DivLife, 6, 12, StatType::Def_p, { {StatType::Spd,6}, {StatType::Def,29}, {StatType::HP_p,7}, {StatType::Acc,41} } ),

	Artefact( ArtType::Boots, ArtSet::DivLife, 6, 16, StatType::Spd, { {StatType::Atk,27}, {StatType::Atk_p,13}, {StatType::Def_p,12}, {StatType::Acc,31} } ),
	Artefact( ArtType::Boots, ArtSet::DivLife, 5, 8, StatType::HP_p, { {StatType::Def_p,10}, {StatType::HP,252}, {StatType::Spd,10} } ),

	// DivSpeed
	Artefact( ArtType::Weapon, ArtSet::DivSpeed, 6, 8, StatType::Atk, { {StatType::HP,226}, {StatType::HP_p,13}, {StatType::Res,10}, {StatType::Acc,20} } ),
	Artefact( ArtType::Weapon, ArtSet::DivSpeed, 6, 8, StatType::Atk, { {StatType::CRate,6}, {StatType::Spd,11}, {StatType::HP,1062} } ),

	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 6, 8, StatType::HP, { {StatType::Res,21}, {StatType::CDmg,12}, {StatType::CRate,5}, {StatType::Atk,26} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 6, 16, StatType::HP, { {StatType::Atk_p,13}, {StatType::HP_p,19}, {StatType::CRate,6}, {StatType::Atk,22} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 5, 8, StatType::HP, { {StatType::Def,16}, {StatType::Def_p,10}, {StatType::HP_p,9} } ),

	Artefact( ArtType::Shield, ArtSet::DivSpeed, 6, 8, StatType::Def, { {StatType::Acc,10}, {StatType::HP,651}, {StatType::Spd,5}, {StatType::CDmg,11} } ),
	Artefact( ArtType::Shield, ArtSet::DivSpeed, 6, 8, StatType::Def, { {StatType::HP,606}, {StatType::HP_p,5}, {StatType::CDmg,13} } ),
	Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 12, StatType::Def, { {StatType::Acc,20}, {StatType::Spd,9}, {StatType::Def_p,5} } ),

	Artefact( ArtType::Gloves, ArtSet::DivSpeed, 5, 12, StatType::HP_p, { {StatType::Def_p,11}, {StatType::Def,33}, {StatType::Atk,21} } ),

	Artefact( ArtType::Boots, ArtSet::DivSpeed, 5, 12, StatType::Spd, { {StatType::Atk_p,14}, {StatType::Atk,15+5}, {StatType::CRate,9} } ),
#endif

	Artefact( ArtType::Gloves, ArtSet::Gibel, 5, 8, StatType::CRate, { {StatType::HP_p,5}, {StatType::Atk_p,15} } ),

	Artefact( ArtType::Boots, ArtSet::Fury, 4, 8, StatType::Spd, { {StatType::Acc,15}, {StatType::Atk_p,4} } ),
	Artefact( ArtType::Boots, ArtSet::Reflex, 4, 8, StatType::Spd, { {StatType::HP_p,9}, {StatType::CDmg,8} } ),
	Artefact( ArtType::Boots, ArtSet::Cursed, 5, 8, StatType::Spd, { {StatType::Def_p,10}/*, {StatType::HP,380}*/ } ),

	Artefact( ArtType::Gloves, ArtSet::Daze, 5, 8, StatType::Def_p, { {StatType::Res,8}, {StatType::CDmg,6}, {StatType::Atk_p,6}, {StatType::CRate,14} } ),

	Artefact( ArtType::Gloves, ArtSet::Immunitet, 5, 8, StatType::HP_p, { {StatType::CRate,14}, {StatType::CDmg,5} } ),

	Artefact( ArtType::Gloves, ArtSet::Vozmezdie, 5, 8, StatType::HP_p, { {StatType::Def_p,4}, {StatType::CRate,15} } ),
	Artefact( ArtType::Boots, ArtSet::Vozmezdie, 5, 8, StatType::Spd, { {StatType::CRate,9}, {StatType::Def_p,9}, {StatType::Atk_p,4} } ),

	Artefact( ArtType::Gloves, ArtSet::Doblest, 6, 8, StatType::Def_p, { {StatType::HP_p,13}, {StatType::Spd,11} } ),
	Artefact( ArtType::Boots, ArtSet::Doblest, 5, 8, StatType::Spd, { {StatType::Def_p,6}, {StatType::CRate,4}, {StatType::HP,980} } ),

	// Rings
	Artefact( ArtType::Ring,  ArtSet::None, 5, 12, StatType::HP, { {StatType::Def,46+5}, {StatType::Def_p,5+1}, {StatType::HP_p,5+1} }, ChampionName::Gromoboy ),
};

/////////////////////////////////////////////////////////////////////////////
