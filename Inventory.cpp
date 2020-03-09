#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

std::vector<Artefact> _MyArts =
{
	// HP
	Artefact{ ArtType::Weapon, ArtSet::HP, 6, 16, StatType::Atk, { {StatType::CDmg,13}, {StatType::Spd,12}, {StatType::CRate,11}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 16, StatType::Atk, { {StatType::Acc,19}, {StatType::CDmg,13}, {StatType::HP_p,6},   {StatType::Atk_p,5} } },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 8, StatType::Atk, { {StatType::HP_p,10}, {StatType::Spd,4} } },

	Artefact{ ArtType::Helmet, ArtSet::HP, 6, 8, StatType::HP, { {StatType::Res,22}, {StatType::Def_p,5}, {StatType::CDmg,11} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 16, StatType::HP, { {StatType::Acc,27}, {StatType::HP_p,4}, {StatType::Atk,20}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 16, StatType::HP, { {StatType::CRate,11}, {StatType::Atk_p,10} } },

	Artefact{ ArtType::Shield, ArtSet::HP, 6, 8, StatType::Def, { {StatType::Spd,6}, {StatType::Acc,18}, {StatType::Res,21} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 8, StatType::Def, { {StatType::Spd,5}, {StatType::CRate,10}, {StatType::HP,594} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 8, StatType::Def, { {StatType::HP_p,9}, {StatType::CRate,10}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 8, StatType::Def, { {StatType::HP_p,5}, {StatType::CRate,17} } },

	Artefact{ ArtType::Gloves, ArtSet::HP, 5, 8, StatType::Def_p, { {StatType::Atk_p,10}, {StatType::CDmg,11}, {StatType::HP,163} } },
	Artefact{ ArtType::Gloves, ArtSet::HP, 5, 8, StatType::CRate, { {StatType::Atk_p,10}, {StatType::Def_p,11} } },

	Artefact{ ArtType::Chest, ArtSet::HP, 5, 12, StatType::Def_p, { {StatType::Atk_p,10}, {StatType::CRate,11}, {StatType::CDmg,10} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 8, StatType::HP_p, { {StatType::CRate,11}, {StatType::Spd,5} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 8, StatType::Def_p, { {StatType::HP,5}, {StatType::Spd,5} } },

	Artefact{ ArtType::Boots, ArtSet::HP, 5, 8, StatType::Def_p, { {StatType::CRate,12}, {StatType::CDmg,6}, {StatType::HP,209} } },
	Artefact{ ArtType::Boots, ArtSet::HP, 5, 8, StatType::Spd, { {StatType::Def_p,7+1}, {StatType::CRate,5}, {StatType::Acc,7}, {StatType::Def,8+5} } },

	// Atk
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 8, StatType::Atk, { {StatType::CDmg,6}, {StatType::HP_p,19}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,4}, {StatType::Spd,13}, {StatType::HP,442}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,10}, {StatType::HP_p,16}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,9}, {StatType::HP,161}, {StatType::Spd,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::Spd,13}, {StatType::Acc,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 4, 12, StatType::Atk, { {StatType::HP_p,15}, {StatType::CDmg,3}, {StatType::Spd,3} } },

	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 8, StatType::HP, { {StatType::Acc,23}, {StatType::Res,22}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Def,20}, {StatType::CRate,6}, {StatType::Atk_p,16} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Atk,31}, {StatType::CRate,6}, {StatType::Def_p,8} } },

	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 8, StatType::Def, { {StatType::Spd,10}, {StatType::Def_p,13}, {StatType::Acc,12} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP,136}, {StatType::Acc,8}, {StatType::CRate,11} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP_p,4}, {StatType::Acc,18} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::CRate,10}, {StatType::HP_p,10}, {StatType::CDmg,4} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::HP_p,10} } },

	Artefact{ ArtType::Gloves, ArtSet::Atk, 6, 8, StatType::HP_p, { {StatType::Spd,8}, {StatType::Def,33}, {StatType::Acc,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Atk_p, { {StatType::Spd,5}, {StatType::CDmg,11}, {StatType::Acc,17} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Def_p, { {StatType::Atk_p,11}, {StatType::Acc,9} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 0, StatType::CDmg, { {StatType::CRate,4} } },

	Artefact{ ArtType::Chest, ArtSet::Atk, 5, 0, StatType::HP_p, { {StatType::Atk,22}, {StatType::Atk_p,5}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Chest, ArtSet::Atk, 5, 4, StatType::HP_p, { {StatType::CRate,11} } },

	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::Atk_p,8}, {StatType::CDmg,5}, {StatType::HP,426} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::HP_p,10}, {StatType::CRate,5}, {StatType::Def,19} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::CRate,9}, {StatType::Def_p,6}, {StatType::CDmg,5} } },

	// Acc
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 12, StatType::Atk, { {StatType::Atk_p,15}, {StatType::CRate,5}, {StatType::Spd,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 8, StatType::Atk, { {StatType::Acc,18}, {StatType::CRate,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 4, 8, StatType::Atk, { {StatType::CDmg,7}, {StatType::HP_p,4}, {StatType::Atk_p,7} } },

	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 8, StatType::HP, { {StatType::Acc,28}, {StatType::Spd,4}, {StatType::Atk,15} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::HP_p,12}, {StatType::Atk_p,11}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 4, 8, StatType::HP, { {StatType::Def_p,4}, {StatType::CRate,8}, {StatType::Atk,18} } },

	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 8, StatType::Def, { {StatType::Def_p,11}, {StatType::HP,219}, {StatType::Res,21} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 4, 8, StatType::Def, { {StatType::Res,15}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 4, 8, StatType::Def, { {StatType::HP_p,4}, {StatType::Spd,11} } },

	Artefact{ ArtType::Gloves, ArtSet::Acc, 6, 12, StatType::Atk_p, { {StatType::CDmg,20}, {StatType::Spd,5}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Acc, 4, 8, StatType::CRate, { {StatType::Def_p,7}, {StatType::CDmg,7} } },

	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 8, StatType::Atk_p, { {StatType::Def_p,5}, {StatType::CDmg,15} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 8, StatType::Atk_p, { {StatType::HP_p,10}, {StatType::Def_p,10} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 4, 8, StatType::HP_p,  { {StatType::Atk,9}, {StatType::Atk_p,8}, {StatType::Acc,7}, {StatType::Def_p,8} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 4, 12, StatType::HP_p, {  {StatType::Atk_p,7}, {StatType::CRate,4}, {StatType::CDmg,4} } },

	Artefact{ ArtType::Boots, ArtSet::Acc, 5, 0, StatType::Atk_p, {  {StatType::HP,280}, {StatType::Acc,9} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 4, 8, StatType::Spd,   {  {StatType::CRate,4}, {StatType::HP_p,12} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 4, 0, StatType::Def_p, {  {StatType::CRate,3}, {StatType::Def,12} } },
};

/////////////////////////////////////////////////////////////////////////////
