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
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 0, StatType::HP, { {StatType::Acc,11}, {StatType::CDmg,4} } },

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
	Artefact{ ArtType::Boots, ArtSet::HP, 5, 8, StatType::Spd, { {StatType::Def_p,7}, {StatType::CRate,5}, {StatType::Acc,7}, {StatType::Def,8+5} } },

	// Atk
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 0, StatType::Atk, { {StatType::HP,255}, {StatType::HP_p,5}, {StatType::Res,10}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 8, StatType::Atk, { {StatType::CDmg,6}, {StatType::HP_p,19}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,4}, {StatType::Spd,13}, {StatType::HP,442}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,10}, {StatType::HP_p,16}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,9}, {StatType::HP,161}, {StatType::Spd,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::Spd,13}, {StatType::Acc,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 4, 12, StatType::Atk, { {StatType::HP_p,15}, {StatType::CDmg,3}, {StatType::Spd,3} } },

	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 0, StatType::HP, { {StatType::Atk_p,6}, {StatType::Def,24}, {StatType::Res,10}, {StatType::Acc,11} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 8, StatType::HP, { {StatType::Acc,23}, {StatType::Res,22}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Def,20}, {StatType::CRate,6}, {StatType::Atk_p,16} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Atk,31}, {StatType::CRate,6}, {StatType::Def_p,8} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 0, StatType::HP, { {StatType::Def_p,6}, {StatType::CRate,6} } },

	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 0, StatType::Def, { {StatType::Def_p,7}, {StatType::HP,344}, {StatType::CRate,7}, {StatType::Res,11} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 8, StatType::Def, { {StatType::Spd,10}, {StatType::Def_p,13}, {StatType::Acc,12} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP,136}, {StatType::Acc,8}, {StatType::CRate,11} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP_p,4}, {StatType::Acc,18} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::CRate,10}, {StatType::HP_p,10}, {StatType::CDmg,4} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::HP_p,10} } },

	Artefact{ ArtType::Gloves, ArtSet::Atk, 6, 8, StatType::HP_p, { {StatType::Spd,8}, {StatType::Def,33}, {StatType::Acc,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Atk_p, { {StatType::Spd,5}, {StatType::CDmg,11}, {StatType::Acc,17} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Def_p, { {StatType::Atk_p,11}, {StatType::Acc,9} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 0, StatType::CDmg, { {StatType::CRate,4} } },

	Artefact{ ArtType::Chest, ArtSet::Atk, 6, 0, StatType::HP_p, { {StatType::Spd,6}, {StatType::Def_p,6}, {StatType::CRate,6}, {StatType::HP,402} } },
	Artefact{ ArtType::Chest, ArtSet::Atk, 5, 0, StatType::HP_p, { {StatType::Atk,22}, {StatType::Atk_p,5}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Chest, ArtSet::Atk, 5, 4, StatType::HP_p, { {StatType::CRate,11} } },

	Artefact{ ArtType::Boots, ArtSet::Atk, 6, 0, StatType::Atk_p, { {StatType::Def,30}, {StatType::Spd,6}, {StatType::Acc,11}, {StatType::Def_p,6} } },
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
	Artefact{ ArtType::Gloves, ArtSet::Acc, 4, 1, StatType::Atk_p, { {StatType::CRate,4}, {StatType::Spd,4}, {StatType::HP_p,4} } },
	Artefact{ ArtType::Gloves, ArtSet::Acc, 4, 8, StatType::CRate, { {StatType::Def_p,7}, {StatType::CDmg,7} } },

	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 8, StatType::Atk_p, { {StatType::Def_p,5}, {StatType::CDmg,15} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 8, StatType::Atk_p, { {StatType::HP_p,10}, {StatType::Def_p,10} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 4, 8, StatType::HP_p,  { {StatType::Atk,9}, {StatType::Atk_p,8}, {StatType::Acc,7}, {StatType::Def_p,8} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 4, 12, StatType::HP_p, { {StatType::Atk_p,7}, {StatType::CRate,4}, {StatType::CDmg,4} } },

	Artefact{ ArtType::Boots, ArtSet::Acc, 5, 0, StatType::Atk_p, { {StatType::HP,280}, {StatType::Acc,9} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 4, 8, StatType::Spd,   { {StatType::CRate,4}, {StatType::HP_p,12} } },

	// Spd
	Artefact{ ArtType::Weapon, ArtSet::Speed, 6, 8, StatType::Atk, { {StatType::Atk_p,13}, {StatType::Acc,21}, {StatType::HP,307} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Res,10}, {StatType::CRate,8}, {StatType::Atk_p,15}, {StatType::Spd,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Acc,17}, {StatType::Atk_p,5}, {StatType::Res,20} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Spd,9}, {StatType::HP,257}, {StatType::CDmg,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Res,19}, {StatType::Acc,10}, {StatType::CDmg,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 1, StatType::Atk, { {StatType::CRate,5}, {StatType::HP_p,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::Spd,8}, {StatType::HP,268} } },

	Artefact{ ArtType::Helmet, ArtSet::Speed, 6, 12, StatType::HP, { {StatType::CDmg,11}, {StatType::Def_p,11}, {StatType::HP_p,14} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 13, StatType::HP, { {StatType::Atk_p,11}, {StatType::HP_p,4}, {StatType::Def,44}, {StatType::Res,10} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::Atk,45}, {StatType::Res,11}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 12, StatType::HP, { {StatType::HP_p,6}, {StatType::CDmg,17}, {StatType::Atk,19} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::Atk,16}, {StatType::HP_p,14} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::Acc,19}, {StatType::Atk_p,11} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 8, StatType::HP, { {StatType::HP_p,10}, {StatType::CDmg,4} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 12, StatType::HP, { {StatType::Def_p,4}, {StatType::Spd,9}, {StatType::Acc,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 12, StatType::HP, { {StatType::Atk_p,4}, {StatType::Def_p,14}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 8, StatType::HP, { {StatType::Atk_p,7}, {StatType::Spd,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 4, 8, StatType::HP, { {StatType::Acc,17}, {StatType::Def_p,9} } },

	Artefact{ ArtType::Shield, ArtSet::Speed, 6, 12, StatType::Def, { {StatType::CDmg,19}, {StatType::CRate,7}, {StatType::Def_p,12} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::HP,197}, {StatType::CDmg,10}, {StatType::Spd,9} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Res,8}, {StatType::Acc,18}, {StatType::CDmg,9} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::HP_p,10}, {StatType::Acc,10}, {StatType::Def_p,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 12, StatType::Def, { {StatType::Acc,17}, {StatType::HP_p,10}, {StatType::Res,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::CDmg,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 1, StatType::Def, { {StatType::Spd,4}, {StatType::CDmg,4} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::Acc,11} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 8, StatType::Def, { {StatType::CDmg,8}, {StatType::Spd,3}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 12, StatType::Def, { {StatType::Def_p,13}, {StatType::Acc,6}, {StatType::Res,8} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 8, StatType::Def, { {StatType::CDmg,4}, {StatType::Spd,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 8, StatType::Def, { {StatType::Spd,6}, {StatType::Acc,13} } },

	Artefact{ ArtType::Gloves, ArtSet::Speed, 6, 12, StatType::CRate, { {StatType::Acc,29}, {StatType::HP,301}, {StatType::Atk,50}, {StatType::Def_p,7} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 1, StatType::CDmg, { {StatType::Spd,4}, {StatType::Def,20}, {StatType::Res,11}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::Def_p, { {StatType::Def,30}, {StatType::HP_p,5}, {StatType::Atk_p,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::CRate, { {StatType::Atk,21}, {StatType::Def,20}, {StatType::Spd,14} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::Atk_p, { {StatType::Acc,27}, {StatType::Spd,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::CRate, { {StatType::Spd,5}, {StatType::Def,54} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 4, 8, StatType::HP_p, { {StatType::Acc,14}, {StatType::CRate,7} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 4, 8, StatType::Atk_p, { {StatType::Def_p,12}, {StatType::CDmg,3} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 4, 8, StatType::Def_p, { {StatType::Atk_p,5}, {StatType::Spd,3} } },

	Artefact{ ArtType::Chest, ArtSet::Speed, 6, 1, StatType::Res, { {StatType::CRate,6}, {StatType::Atk_p,7}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 8, StatType::HP_p, { {StatType::HP,155}, {StatType::CDmg,6}, {StatType::Acc,29} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 8, StatType::Def_p, { {StatType::Res,21}, {StatType::Atk,22} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 1, StatType::Atk_p, {  } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 16, StatType::Def_p, { {StatType::Acc,14}, {StatType::Def,20}, {StatType::HP_p,5}, {StatType::Atk,12} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 10, StatType::Def_p, { {StatType::Atk,23}, {StatType::HP_p,9} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 8, StatType::Def_p, { {StatType::HP_p,5}, {StatType::Spd,10} } },

	Artefact{ ArtType::Boots, ArtSet::Speed, 6, 1, StatType::Def_p, { {StatType::Def,29}, {StatType::HP,400}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 12, StatType::Spd, { {StatType::Def_p,9}, {StatType::Acc,19}, {StatType::Res,19} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 4, StatType::Atk_p, { {StatType::Def_p,6}, {StatType::Acc,20}, {StatType::Spd,5} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Spd, { {StatType::Atk,37}, {StatType::CRate,11} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 1, StatType::HP_p, { {StatType::Atk,14}, {StatType::Spd,5} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 4, 16, StatType::Spd, { {StatType::CDmg,5}, {StatType::Res,9}, {StatType::Atk,12}, {StatType::Def,13} } },

	// Vamp
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CRate,8}, {StatType::Atk_p,10}, {StatType::Spd,5} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CRate,14}, {StatType::Acc,9}, {StatType::HP,157} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CDmg,14}, {StatType::CRate,6}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 8, StatType::Atk, { {StatType::Atk_p,15}, {StatType::Spd,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 1, StatType::Atk, { {StatType::CDmg,6}, {StatType::CRate,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 8, StatType::Atk, { {StatType::CRate,10}, {StatType::Spd,4} } },

	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Spd,15}, {StatType::Def_p,6}, {StatType::CDmg,4}, {StatType::Res,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 8, StatType::HP, { {StatType::Atk_p,5}, {StatType::Acc,31} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 8, StatType::HP, { {StatType::CRate,6}, {StatType::HP_p,14} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::Def_p,4}, {StatType::CDmg,7}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::CRate,4}, {StatType::Def_p,13}, {StatType::Spd,4} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::Spd,6}, {StatType::CRate,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 8, StatType::HP, { {StatType::CRate,8}, {StatType::HP_p,8} } },

	Artefact{ ArtType::Shield, ArtSet::Vamp, 6, 8, StatType::Def, { {StatType::Acc,11}, {StatType::CRate,19} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 8, StatType::Def, { {StatType::CRate,9}, {StatType::HP,185} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::HP,232} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 4, 8, StatType::Def, { {StatType::HP_p,4}, {StatType::CRate,8}, {StatType::HP,280} } },

	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 8, StatType::Def_p, { {StatType::CDmg,10}, {StatType::Def,35} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 8, StatType::CRate, { {StatType::CDmg,9}, {StatType::Acc,15} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 8, StatType::Def_p, { {StatType::HP_p,9}, {StatType::Spd,4} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 8, StatType::CRate, { {StatType::Atk_p,7}, {StatType::CDmg,5} } },

	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 0, StatType::Res, { {StatType::Acc,9}, {StatType::CDmg,4}, {StatType::CRate,4} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::CDmg,11}, {StatType::HP_p,4}, {StatType::Spd,4}, {StatType::Atk_p,4} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 8, StatType::HP_p, { {StatType::Atk_p,10}, {StatType::CRate,6} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 8, StatType::Def_p, { {StatType::Spd,9}, {StatType::Def,16} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 4, 8, StatType::Atk_p, { {StatType::Atk,19}, {StatType::Acc,15} } },

	Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 8, StatType::Spd, { {StatType::Res,18}, {StatType::Acc,20} } },
	Artefact{ ArtType::Boots, ArtSet::Vamp, 4, 8, StatType::Spd, { {StatType::HP,266}, {StatType::CDmg,8} } },
};

/////////////////////////////////////////////////////////////////////////////
