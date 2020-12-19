#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

const Hall _MyHall({
	{ Element::Blue,  { {StatType::HP_p,4}, {StatType::Atk_p,3}, {StatType::Def_p,3}, {StatType::CDmg,8}, {StatType::Res,10}, {StatType::Acc,80} }, },
	{ Element::Green, { {StatType::HP_p,4}, {StatType::Atk_p,3}, {StatType::Def_p,3}, {StatType::CDmg,8}, {StatType::Res,5}, {StatType::Acc,50} }, },
	{ Element::Red,   { {StatType::HP_p,3}, {StatType::Atk_p,3}, {StatType::Def_p,3}, {StatType::CDmg,8}, {StatType::Res,5}, {StatType::Acc,40} }, },
	{ Element::Void,  { {StatType::HP_p,3}, {StatType::Atk_p,3}, {StatType::Def_p,3}, {StatType::CDmg,8}, {StatType::Res,10}, {StatType::Acc,50} }, },
});

/////////////////////////////////////////////////////////////////////////////

const std::vector<Artefact> _MyArts =
{
	// �����
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 16, StatType::Atk, { {StatType::Atk_p,7,1}, {StatType::HP_p,18,1}, {StatType::Spd,10}, {StatType::Res,11,3} }, ChampionName::Kael },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 4, 16, StatType::HP, { {StatType::Atk_p,11,1}, {StatType::HP_p,7,1}, {StatType::CRate,4}, {StatType::CDmg,5} }, ChampionName::Kael },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 4, 16, StatType::Def, { {StatType::CDmg,11}, {StatType::Acc,13,2}, {StatType::HP_p,4,1}, {StatType::CRate,4} }, ChampionName::Kael },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 16, StatType::CRate, { {StatType::Atk_p,15,2}, {StatType::Acc,11,4}, {StatType::Def_p,6,1}, {StatType::CDmg,5} }, ChampionName::Kael },
	Artefact{ ArtType::Chest,  ArtSet::Vamp, 5, 16, StatType::Atk_p, { {StatType::Acc,19,2}, {StatType::HP,202,125}, {StatType::HP_p,5,1}, {StatType::Spd,5} }, ChampionName::Kael },
	Artefact{ ArtType::Boots,  ArtSet::Atk, 4, 16, StatType::Spd, { {StatType::Def_p,7,1}, {StatType::CDmg,7}, {StatType::Res,15,3}, {StatType::Def,13,5} }, ChampionName::Kael },

	// Hatun
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::HP_p,10,1}, {StatType::Spd,5,1}, {StatType::Atk_p,5,1}, {StatType::CRate,5} }, ChampionName::Hatun },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::HP_p,6,1}, {StatType::CDmg,16}, {StatType::Spd,9,2}, {StatType::Def_p,5,1} }, ChampionName::Hatun },
	Artefact{ ArtType::Shield, ArtSet::Speed, 4, 12, StatType::Def, { {StatType::Def_p,12,1}, {StatType::HP_p,3,1}, {StatType::CRate,8,2} }, ChampionName::Hatun },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::HP_p, { {StatType::CDmg,5}, {StatType::Spd,14,1}, {StatType::HP,155}, {StatType::Res,9} }, ChampionName::Hatun },
	Artefact{ ArtType::Chest, ArtSet::Speed, 4, 16, StatType::Def_p, { {StatType::HP_p,10,1}, {StatType::Spd,4,1}, {StatType::Atk_p,4}, {StatType::Res,7,2} }, ChampionName::Hatun },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::Atk,28,5}, {StatType::HP_p,5,1}, {StatType::CDmg,5}, {StatType::Def_p,5,1} }, ChampionName::Hatun },


	// HP
	  Artefact{ ArtType::Weapon, ArtSet::HP, 6, 16, StatType::Atk, { {StatType::CDmg,13}, {StatType::Spd,12}, {StatType::CRate,11}, {StatType::Atk_p,7,2} }, ChampionName::Rotos },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 16, StatType::Atk, { {StatType::Acc,19}, {StatType::CDmg,13}, {StatType::HP_p,6}, {StatType::Atk_p,5} } },
	Artefact{ ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, { {StatType::Spd,13,1}, {StatType::Acc,11,2}, {StatType::HP,296,100} } },
	  Artefact{ ArtType::Weapon, ArtSet::HP, 5, 12, StatType::Atk, { {StatType::Acc,19,2}, {StatType::Spd,10,1}, {StatType::Atk_p,6,2} }, ChampionName::VisirOvelis },

	Artefact( ArtType::Helmet, ArtSet::HP, 6, 16, StatType::HP, { {StatType::Spd,29,3}, {StatType::HP_p,7,3}, {StatType::Res,11,4}, {StatType::Atk_p,6,2} } ),
	Artefact{ ArtType::Helmet, ArtSet::HP, 6, 8, StatType::HP, { {StatType::Res,22}, {StatType::Def_p,5}, {StatType::CDmg,11} } },
	Artefact{ ArtType::Helmet, ArtSet::HP, 5, 16, StatType::HP, { {StatType::Acc,27}, {StatType::HP_p,4}, {StatType::Atk,20}, {StatType::CDmg,6} } },

	Artefact{ ArtType::Shield, ArtSet::HP, 6, 16, StatType::Def, { {StatType::CRate,7}, {StatType::Acc,22}, {StatType::Def_p,16,1}, {StatType::Res,20,2} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 6, 12, StatType::Def, { {StatType::Spd,12}, {StatType::Acc,18}, {StatType::Res,21} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 12, StatType::Def, { {StatType::HP_p,14}, {StatType::CRate,10}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 16, StatType::Def, { {StatType::HP_p,5}, {StatType::CRate,17}, {StatType::Acc,9}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Shield, ArtSet::HP, 5, 12, StatType::Def, { {StatType::CRate,10}, {StatType::Spd,10}, {StatType::Acc,10} } },

	Artefact{ ArtType::Gloves, ArtSet::HP, 5, 12, StatType::Def_p, { {StatType::Atk_p,10}, {StatType::CDmg,15}, {StatType::HP,163} } },
	Artefact{ ArtType::Gloves, ArtSet::HP, 5, 12, StatType::CRate, { {StatType::Atk_p,10}, {StatType::Def_p,11}, {StatType::Spd,5} } },

	Artefact{ ArtType::Chest, ArtSet::HP, 5, 16, StatType::Def_p, { {StatType::Atk_p,10,1}, {StatType::CRate,11}, {StatType::CDmg,10}, {StatType::Res,8,2} } },
	  Artefact{ ArtType::Chest, ArtSet::HP, 6, 16, StatType::Atk_p, { {StatType::CDmg,17}, {StatType::Acc,11,1}, {StatType::Spd,11,1}, {StatType::Atk,44} }, ChampionName::Rotos },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 13, StatType::Atk_p, { {StatType::Spd,8}, {StatType::HP_p,9}, {StatType::Def_p,9} } },
	Artefact{ ArtType::Chest, ArtSet::HP, 5, 16, StatType::Def_p, { {StatType::HP_p,5,1}, {StatType::Spd,5,1}, {StatType::Atk_p,6,1}, {StatType::Atk,19,5} } },

	Artefact{ ArtType::Boots, ArtSet::HP, 5, 16, StatType::Spd, { {StatType::Def_p,10,1}, {StatType::Res,9,2}, {StatType::CRate,4}, {StatType::CDmg,5} } },
	Artefact{ ArtType::Boots, ArtSet::HP, 4, 16, StatType::Spd, { {StatType::Def_p,7,1}, {StatType::CRate,5}, {StatType::Acc,7}, {StatType::Def,8,7} } },

	  Artefact{ ArtType::Gloves, ArtSet::HP, 6, 16, StatType::Def_p, { {StatType::Atk_p,5,2}, {StatType::Acc,42,2}, {StatType::HP,343,175}, {StatType::CDmg,12} }, ChampionName::VisirOvelis },

#ifndef DEBUG_FIND_BEST
	// Atk
	Artefact{ ArtType::Weapon, ArtSet::Atk, 6, 12, StatType::Atk, { {StatType::CDmg,6}, {StatType::HP_p,24}, {StatType::Atk_p,7} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 16, StatType::Atk, { {StatType::CDmg,15}, {StatType::HP_p,5,2}, {StatType::CRate,9}, {StatType::HP,353,225} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 12, StatType::Atk, { {StatType::CRate,14}, {StatType::HP,161}, {StatType::Spd,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 8, StatType::Atk, { {StatType::CRate,10}, {StatType::HP_p,16}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Atk, 5, 12, StatType::Atk, { {StatType::Spd,13}, {StatType::Acc,8}, {StatType::CRate,5} } },

	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 12, StatType::HP, { {StatType::HP_p,6}, {StatType::Def_p,18}, {StatType::CRate,13}, {StatType::Acc,12} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 6, 8, StatType::HP, { {StatType::Acc,23}, {StatType::Res,22}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 12, StatType::HP, { {StatType::Def,20}, {StatType::CRate,6}, {StatType::Atk_p,22} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 8, StatType::HP, { {StatType::Atk,31}, {StatType::CRate,6}, {StatType::Def_p,8} } },
	Artefact{ ArtType::Helmet, ArtSet::Atk, 5, 12, StatType::HP, { {StatType::Def_p,12}, {StatType::CRate,12}, {StatType::Res,8} } },

	Artefact( ArtType::Shield, ArtSet::Atk, 6, 12, StatType::Def, { {StatType::Def_p,21,1}, {StatType::HP,344,275}, {StatType::CRate,7}, {StatType::Res,21,2} } ),
	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 12, StatType::Def, { {StatType::Res,32}, {StatType::HP_p,11}, {StatType::Spd,6}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 6, 13, StatType::Def, { {StatType::Spd,15,1}, {StatType::Def_p,13,1}, {StatType::Acc,12,2} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP,136}, {StatType::Acc,8}, {StatType::CRate,11} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Def_p,10}, {StatType::HP_p,4}, {StatType::Acc,18} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::CRate,10}, {StatType::HP_p,10}, {StatType::CDmg,4} } },
	Artefact{ ArtType::Shield, ArtSet::Atk, 5, 8, StatType::Def, { {StatType::Acc,20}, {StatType::HP_p,10} } },

	  Artefact{ ArtType::Gloves, ArtSet::Atk, 6, 16, StatType::CDmg, { {StatType::Spd,11}, {StatType::Res,23,3}, {StatType::Acc,10,2}, {StatType::CRate,19} }, ChampionName::Foly },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 12, StatType::HP_p, { {StatType::Spd,8}, {StatType::Def,33}, {StatType::Acc,19} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 12, StatType::Atk_p, { {StatType::Spd,5}, {StatType::CDmg,11}, {StatType::Acc,27} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 8, StatType::Def_p, { {StatType::Atk_p,11}, {StatType::Acc,9} } },
	Artefact{ ArtType::Gloves, ArtSet::Atk, 5, 0, StatType::CDmg, { {StatType::CRate,4} } },

	  Artefact( ArtType::Chest, ArtSet::Atk, 6, 16, StatType::HP_p, { {StatType::Spd,17,1}, {StatType::Def_p,11,1}, {StatType::CRate,6}, {StatType::HP,679,225} }, ChampionName::Foly ),
	Artefact{ ArtType::Chest, ArtSet::Atk, 5, 16, StatType::HP_p, { {StatType::Atk,22}, {StatType::Atk_p,9}, {StatType::Def_p,15}, {StatType::CDmg,5} } },

	Artefact{ ArtType::Boots, ArtSet::Atk, 6, 1, StatType::HP_p, { {StatType::Def_p,7}, {StatType::CDmg,6}, {StatType::Acc,11}, {StatType::CRate,5} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 6, 1, StatType::Atk_p, { {StatType::Def,30}, {StatType::Spd,6}, {StatType::Acc,11}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::Atk_p,8}, {StatType::CDmg,5}, {StatType::HP,426} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::HP_p,10}, {StatType::CRate,5}, {StatType::Def,19} } },
	Artefact{ ArtType::Boots, ArtSet::Atk, 5, 12, StatType::Spd, { {StatType::CRate,9}, {StatType::Def_p,6}, {StatType::CDmg,5} } },

	// Def
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::HP_p,4}, {StatType::Atk_p,15} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::CRate,8}, {StatType::HP_p,10}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::Acc,19}, {StatType::CRate,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::Acc,17}, {StatType::Res,19} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::CDmg,10}, {StatType::Spd,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 5, 8, StatType::Atk, { {StatType::CRate,11}, {StatType::Atk_p,12} } },
	Artefact{ ArtType::Weapon, ArtSet::Def, 4, 12, StatType::Atk, { {StatType::CRate,11}, {StatType::Acc,15,1}, {StatType::HP_p,4,1} } },

	Artefact{ ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, { {StatType::Def_p,11}, {StatType::Atk_p,10} } },
	Artefact{ ArtType::Helmet, ArtSet::Def, 5, 8, StatType::HP, { {StatType::CRate,5}, {StatType::Acc,26} } },

	Artefact{ ArtType::Shield, ArtSet::Def, 5, 16, StatType::Def, { {StatType::Def_p,10,2}, {StatType::Acc,9,2}, {StatType::Res,10,2}, {StatType::HP_p,5,1} } },
	  Artefact{ ArtType::Shield, ArtSet::Def, 5, 16, StatType::Def, { {StatType::Spd,10,1}, {StatType::Def_p,9,1}, {StatType::CDmg,15}, {StatType::CRate,6} }, ChampionName::Rotos },
	Artefact{ ArtType::Shield, ArtSet::Def, 5, 12, StatType::Def, { {StatType::CRate,6}, {StatType::Res,20,2}, {StatType::HP_p,16,1} } },

	Artefact{ ArtType::Gloves, ArtSet::Def, 5, 16, StatType::Def_p, { {StatType::Spd,13,1}, {StatType::Acc,9,2}, {StatType::CRate,6}, {StatType::Res,8,2} } },
	Artefact{ ArtType::Gloves, ArtSet::Def, 5, 8, StatType::HP_p, { {StatType::CRate,10}, {StatType::CDmg,11} } },

	Artefact{ ArtType::Chest, ArtSet::Def, 5, 12, StatType::HP_p, { {StatType::Def,43,10}, {StatType::CDmg,12}, {StatType::Res,10,2} } },
	Artefact{ ArtType::Chest, ArtSet::Def, 5, 8, StatType::HP_p, { {StatType::Def_p,9}, {StatType::Res,10} } },

	  Artefact{ ArtType::Boots, ArtSet::Def, 5, 16, StatType::Spd, { {StatType::CRate,4}, {StatType::CDmg,6}, {StatType::HP_p,5,2}, {StatType::Atk,13,5} }, ChampionName::Rotos },
	Artefact{ ArtType::Boots, ArtSet::Def, 5, 13, StatType::Spd, { {StatType::Atk,25}, {StatType::Acc,9}, {StatType::Def,17} } },
	Artefact{ ArtType::Boots, ArtSet::Def, 4, 16, StatType::Spd, { {StatType::Def_p,8,1}, {StatType::Atk,6,10}, {StatType::Res,9,2}, {StatType::Def,10,5} } },

	// CRate
	Artefact{ ArtType::Weapon, ArtSet::CRate, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::HP_p,10}, {StatType::Res,9} } },
	Artefact{ ArtType::Weapon, ArtSet::CRate, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::Atk_p,10}, {StatType::Acc,10} } },

	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::Acc,26}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 14, StatType::HP, { {StatType::CDmg,11}, {StatType::CRate,11}, {StatType::Spd,4,1} } },

	Artefact{ ArtType::Shield, ArtSet::CRate, 5, 16, StatType::Def, { {StatType::CRate,6}, {StatType::Res,18}, {StatType::Def_p,20,1}, {StatType::Acc,8,2} } },
	Artefact{ ArtType::Shield, ArtSet::CRate, 5, 12, StatType::Def, { {StatType::Def_p,9}, {StatType::Spd,8}, {StatType::CRate,6} } },

	Artefact{ ArtType::Gloves, ArtSet::CRate, 5, 12, StatType::CRate, { {StatType::CDmg,8}, {StatType::Spd,9}, {StatType::Acc,19} } },
	Artefact{ ArtType::Gloves, ArtSet::CRate, 5, 12, StatType::CRate, { {StatType::CDmg,6}, {StatType::Atk_p,14}, {StatType::Atk,16} } },
	Artefact{ ArtType::Gloves, ArtSet::CRate, 5, 16, StatType::CDmg, { {StatType::CRate,4}, {StatType::Spd,10}, {StatType::Def_p,15}, {StatType::Def,23} } },

	Artefact{ ArtType::Weapon, ArtSet::CRate, 5, 16, StatType::Atk, { {StatType::Acc,20,2}, {StatType::CRate,17}, {StatType::Res,18,1}, {StatType::CDmg,5} }, ChampionName::Zargala/*Voitelnica*/ },
	Artefact{ ArtType::Helmet, ArtSet::CRate, 5, 16, StatType::HP, { {StatType::Atk_p,10,2}, {StatType::CDmg,11}, {StatType::HP_p,5,1}, {StatType::CRate,5} }, ChampionName::Zargala/*Voitelnica*/ },

	// CDmg
	Artefact( ArtType::Weapon, ArtSet::CritDmg, 5, 12, StatType::Atk, { {StatType::Atk_p,12}, {StatType::Acc,11}, {StatType::CRate,9}, {StatType::HP,506} } ),
	Artefact( ArtType::Weapon, ArtSet::CritDmg, 5, 12, StatType::Atk, { {StatType::Atk_p,9}, {StatType::Acc,21}, {StatType::HP_p,5} } ),

	Artefact( ArtType::Helmet, ArtSet::CritDmg, 6, 12, StatType::HP, { {StatType::Acc,11}, {StatType::Spd,20}, {StatType::Res,10} } ),
	Artefact( ArtType::Helmet, ArtSet::CritDmg, 5, 12, StatType::HP, { {StatType::Acc,28}, {StatType::CDmg,5}, {StatType::HP_p,5} } ),

	Artefact( ArtType::Shield, ArtSet::CritDmg, 5, 16, StatType::Def, { {StatType::HP_p,16}, {StatType::CRate,6}, {StatType::CDmg,8}, {StatType::Spd,10} } ),
	Artefact( ArtType::Shield, ArtSet::CritDmg, 5, 12, StatType::Def, { {StatType::Res,11}, {StatType::Spd,8}, {StatType::CDmg,4}, {StatType::Acc,29} } ),

	Artefact( ArtType::Chest, ArtSet::CritDmg, 5, 8, StatType::HP_p, { {StatType::Def_p,6}, {StatType::CDmg,10}, {StatType::Spd,5}, {StatType::CRate,9} } ),
#endif

	// Acc
	Artefact{ ArtType::Weapon, ArtSet::Acc, 6, 12, StatType::Atk, { {StatType::Spd,17}, {StatType::CDmg,5}, {StatType::HP,883} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 6, 8, StatType::Atk, { {StatType::Atk_p,12}, {StatType::Spd,12} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 16, StatType::Atk, { {StatType::Res,10}, {StatType::Spd,9}, {StatType::CRate,23}, {StatType::Atk_p,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 16, StatType::Atk, { {StatType::Atk_p,15}, {StatType::CRate,5}, {StatType::Spd,10}, {StatType::Res,11} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 16, StatType::Atk, { {StatType::CDmg,9}, {StatType::Atk_p,4}, {StatType::CRate,17}, {StatType::Acc,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 16, StatType::Atk, { {StatType::Spd,13}, {StatType::Atk_p,6}, {StatType::HP_p,10}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 12, StatType::Atk, { {StatType::CRate,6}, {StatType::Spd,14}, {StatType::Acc,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 8, StatType::Atk, { {StatType::Spd,14}, {StatType::Atk_p,5} } },
	//Artefact{ ArtType::Weapon, ArtSet::Acc, 5, 12, StatType::Atk, { {StatType::Acc,18}, {StatType::CRate,10} } },

	  Artefact( ArtType::Helmet, ArtSet::Acc, 6, 16, StatType::HP, { {StatType::Def_p,13,2}, {StatType::Atk_p,12,2}, {StatType::Acc,21,1}, {StatType::CRate,5} }, ChampionName::Alura ),
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP, { {StatType::Def_p,9,1}, {StatType::Spd,4,1}, {StatType::Res,28,2}, {StatType::CRate,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP, { {StatType::Atk_p,11}, {StatType::CRate,6}, {StatType::Spd,14}, {StatType::Acc,20} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP, { {StatType::Spd,8}, {StatType::Atk_p,16}, {StatType::CRate,9}, {StatType::CDmg,4} } },
#ifndef DEBUG_FIND_BEST
	  Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP, { {StatType::Spd,9,1}, {StatType::HP_p,9,1}, {StatType::Acc,11,2}, {StatType::Def_p,6,1} }, ChampionName::Razen },
	  Artefact( ArtType::Helmet, ArtSet::Acc, 5, 16, StatType::HP, { {StatType::Acc,9,3}, {StatType::HP_p,12,1}, {StatType::Spd,15,2}, {StatType::CRate,5} }, ChampionName::Arbitr ),
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Def_p,16,1}, {StatType::Atk,18,4}, {StatType::Acc,20,2} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Acc,28}, {StatType::Spd,9}, {StatType::Atk,15,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Spd,13}, {StatType::Res,9}, {StatType::HP_p,11} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Res,19}, {StatType::HP_p,5}, {StatType::CRate,15} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::Spd,12}, {StatType::CRate,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::HP_p,12}, {StatType::Atk_p,11}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Spd,13}, {StatType::HP_p,6}, {StatType::Res,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Acc, 5, 12, StatType::HP, { {StatType::Spd,9}, {StatType::HP_p,9}, {StatType::Atk_p,6} } },

	Artefact{ ArtType::Shield, ArtSet::Acc, 6, 12, StatType::Def, { {StatType::CDmg,12}, {StatType::Res,10}, {StatType::Acc,10}, {StatType::CRate,13} } },
	  Artefact( ArtType::Shield, ArtSet::Acc, 6, 16, StatType::Def, { {StatType::Def_p,14,2}, {StatType::HP_p,11,2}, {StatType::CDmg,12}, {StatType::CRate,5} }, ChampionName::ColdHeart ),
	  Artefact( ArtType::Shield, ArtSet::Acc, 6, 12, StatType::Def, { {StatType::Spd,17,3}, {StatType::CDmg,6}, {StatType::CRate,6} }, ChampionName::Arbitr ),
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 16, StatType::Def, { {StatType::HP_p,6,1}, {StatType::Def_p,5,2}, {StatType::CDmg,15}, {StatType::CRate,16} }/*, ChampionName::*/ },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 16, StatType::Def, { {StatType::Acc,21}, {StatType::CDmg,5}, {StatType::CRate,14}, {StatType::HP_p,9} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 12, StatType::Def, { {StatType::Acc,8}, {StatType::CDmg,11}, {StatType::Res,27}, {StatType::HP_p,6} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 12, StatType::Def, { {StatType::Def_p,5}, {StatType::CRate,16}, {StatType::Spd,4} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 12, StatType::Def, { {StatType::Def_p,5}, {StatType::Acc,29}, {StatType::Spd,4} } },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 12, StatType::Def, { {StatType::Acc,26}, {StatType::Spd,5}, {StatType::HP,413} }, ChampionName::SteelSkull },
	Artefact{ ArtType::Shield, ArtSet::Acc, 5, 12, StatType::Def, { {StatType::Spd,14}, {StatType::Def_p,5}, {StatType::CDmg,5} }, ChampionName::SteelSkull },

	  Artefact( ArtType::Gloves, ArtSet::Acc, 6, 16, StatType::Atk_p, { {StatType::CRate,20}, {StatType::Spd,5,3}, {StatType::HP_p,5,1}, {StatType::Def_p,7,1} }, ChampionName::Zargala ),
	Artefact{ ArtType::Gloves, ArtSet::Acc, 6, 12, StatType::Def_p, { {StatType::CRate,7}, {StatType::CDmg,16}, {StatType::Spd,6} } },
	Artefact{ ArtType::Gloves, ArtSet::Acc, 5, 12, StatType::CRate, { {StatType::Def_p,4}, {StatType::Res,37}, {StatType::Acc,9} } },
	Artefact( ArtType::Gloves, ArtSet::Acc, 5, 12, StatType::CRate, { {StatType::Atk_p,10}, {StatType::HP_p,5}, {StatType::Spd,14} } ),
	  Artefact( ArtType::Gloves, ArtSet::Acc, 5, 16, StatType::CRate, { {StatType::Atk,19,15}, {StatType::Acc,21,1}, {StatType::Def_p,19,1}, {StatType::HP_p,5,1} }, ChampionName::Alura ),
	Artefact{ ArtType::Gloves, ArtSet::Acc, 5, 8, StatType::Def_p, { {StatType::Res,19}, {StatType::CRate,11} } },

	  Artefact{ ArtType::Chest, ArtSet::Acc, 5, 16, StatType::Def_p, { {StatType::HP_p,5,1}, {StatType::HP,711,100}, {StatType::Spd,4,1}, {StatType::CRate,5} }, ChampionName::SteelSkull },
	  Artefact{ ArtType::Chest, ArtSet::Acc, 5, 16, StatType::Atk_p, { {StatType::Def_p,5,1}, {StatType::CDmg,15}, {StatType::Acc,10,2}, {StatType::Res,9} }, ChampionName::Zargala/*Voitelnica*/ },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 12, StatType::HP_p, { {StatType::CRate,5}, {StatType::Def_p,10}, {StatType::Acc,31} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 12, StatType::Atk_p, { {StatType::HP_p,10}, {StatType::Def_p,10}, {StatType::Spd,4} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 12, StatType::Def_p, { {StatType::HP_p,10}, {StatType::Spd,9}, {StatType::Atk_p,5} } },
	Artefact{ ArtType::Chest, ArtSet::Acc, 5, 8, StatType::Def_p, { {StatType::Atk_p,4}, {StatType::CDmg,16} } },

	Artefact( ArtType::Boots, ArtSet::Acc, 5, 16, StatType::Spd, { {StatType::Def,55,15}, {StatType::Atk_p,5,1}, {StatType::HP_p,11,1}, {StatType::Atk,23} } ),
	  Artefact{ ArtType::Boots, ArtSet::Acc, 5, 16, StatType::Spd, { {StatType::Acc,20,3}, {StatType::Def_p,4,2}, {StatType::CDmg,16}, {StatType::CRate,4} }, ChampionName::Razen },
	Artefact{ ArtType::Boots, ArtSet::Acc, 5, 12, StatType::Spd,  { {StatType::CRate,11}, {StatType::Atk,31}, {StatType::Acc,10} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 5, 8, StatType::Def_p, { {StatType::CRate,4}, {StatType::Spd,12} } },
	Artefact{ ArtType::Boots, ArtSet::Acc, 4, 12, StatType::Spd,  { {StatType::CRate,4}, {StatType::HP_p,12}, {StatType::Def_p,4} } },


	// Spd
	  Artefact( ArtType::Weapon, ArtSet::Speed, 6, 12, StatType::Atk, { {StatType::HP_p,7,1}, {StatType::CDmg,17}, {StatType::CRate,11}, {StatType::HP,527,125} }, ChampionName::Guard ),
	  Artefact( ArtType::Weapon, ArtSet::Speed, 6, 16, StatType::Atk, { {StatType::HP_p,6,2}, {StatType::Spd,12,3}, {StatType::HP,816}, {StatType::Acc,31,1} }, ChampionName::Gorgorab ),
	Artefact{ ArtType::Weapon, ArtSet::Speed, 6, 12, StatType::Atk, { {StatType::Atk_p,19}, {StatType::Acc,21}, {StatType::HP,307} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::Res,10}, {StatType::CRate,8}, {StatType::Atk_p,15}, {StatType::Spd,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::Acc,32}, {StatType::HP_p,5,1}, {StatType::CDmg,11}, {StatType::Atk_p,5} } },
	  Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::CRate,11}, {StatType::HP_p,8,1}, {StatType::Spd,5,1}, {StatType::Res,8,2} }, ChampionName::Lekar },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::Spd,9}, {StatType::HP,521}, {StatType::CDmg,9}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Acc,17}, {StatType::Atk_p,5}, {StatType::Res,31} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Res,19}, {StatType::Acc,20}, {StatType::CDmg,8} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::CDmg,11}, {StatType::CRate,10}, {StatType::HP_p,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::CRate,10}, {StatType::Spd,9}, {StatType::HP_p,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 16, StatType::Atk, { {StatType::CRate,11}, {StatType::HP_p,11,1}, {StatType::Atk_p,5,1}, {StatType::Spd,5+1} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::Atk_p,10}, {StatType::Acc,11} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::Acc,17}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 12, StatType::Atk, { {StatType::Spd,8}, {StatType::HP_p,9}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Speed, 5, 8, StatType::Atk, { {StatType::CRate,13}, {StatType::Spd,5} } },

	Artefact( ArtType::Helmet, ArtSet::Speed, 6, 16, StatType::HP, { {StatType::Spd,12,1}, {StatType::HP_p,12,2}, {StatType::Atk_p,12,2}, {StatType::CDmg,6} } ),
	Artefact( ArtType::Helmet, ArtSet::Speed, 6, 12, StatType::HP, { {StatType::Spd,12}, {StatType::Res,10}, {StatType::CRate,16} } ),
	Artefact{ ArtType::Helmet, ArtSet::Speed, 6, 12, StatType::HP, { {StatType::Def_p,18}, {StatType::Atk_p,12}, {StatType::Acc,11} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 6, 16, StatType::HP, { {StatType::CDmg,11}, {StatType::Def_p,11,2}, {StatType::HP_p,14,2}, {StatType::Spd,6,2} } },
	Artefact( ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::Spd,12,3}, {StatType::Res,10,3}, {StatType::Atk_p,11,2}, {StatType::Def_p,5,2} } ),
	  Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::HP_p,10,3}, {StatType::CRate,5}, {StatType::Spd,15,3}, {StatType::CDmg,11} }, ChampionName::Gorgorab },
	  Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::HP_p,11,3}, {StatType::Atk_p,5,1}, {StatType::CDmg,15}, {StatType::Res,11,2} }, ChampionName::Straholud },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 12, StatType::HP, { {StatType::Atk_p,16}, {StatType::CRate,10}, {StatType::Spd,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::Acc,19}, {StatType::Atk_p,11}, {StatType::CRate,4}, {StatType::Def,17} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::Atk_p,16}, {StatType::Acc,8}, {StatType::CDmg,5}, {StatType::HP_p,6} } },
	Artefact{ ArtType::Helmet, ArtSet::Speed, 5, 16, StatType::HP, { {StatType::CDmg,10}, {StatType::Spd,10}, {StatType::Def_p,5}, {StatType::Res,10} } },

	  Artefact( ArtType::Shield, ArtSet::Speed, 6, 16, StatType::Def, { {StatType::Res,20,2}, {StatType::HP_p,5,1}, {StatType::CRate,12}, {StatType::CDmg,17} }, ChampionName::VGalek ),
	Artefact{ ArtType::Shield, ArtSet::Speed, 6, 12, StatType::Def, { {StatType::CDmg,19}, {StatType::CRate,7}, {StatType::Def_p,12} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 6, 12, StatType::Def, { {StatType::CRate,6}, {StatType::Spd,5}, {StatType::Acc,39} } },
	  Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::HP_p,4,2}, {StatType::CDmg,6}, {StatType::Def_p,9,1}, {StatType::CRate,18} }, ChampionName::Tyrel },
	  Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::HP_p,10,1}, {StatType::Acc,10,2}, {StatType::Def_p,16,1}, {StatType::Spd,4} }, ChampionName::Mavzolejnik },
	Artefact( ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::Res,31}, {StatType::CRate,9}, {StatType::Spd,9}, {StatType::Acc,9} } ),
	Artefact( ArtType::Shield, ArtSet::Speed, 5, 12, StatType::Def, { {StatType::HP_p,11}, {StatType::Spd,9}, {StatType::CRate,10} } ),
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::Def_p,15}, {StatType::HP_p,5}, {StatType::CDmg,9}, {StatType::Spd,5} } },
	Artefact( ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::HP_p,16,2}, {StatType::Spd,9,4}, {StatType::Def_p,5,3}, {StatType::HP,184,175} } ),
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::Acc,17,1}, {StatType::HP_p,10,1}, {StatType::Res,10}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::Def_p,10}, {StatType::CDmg,10}, {StatType::CRate,5}, {StatType::Res,10} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::Spd,8}, {StatType::CDmg,9}, {StatType::CRate,5}, {StatType::Def_p,6} } },
	Artefact{ ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::HP_p,12}, {StatType::Acc,9}, {StatType::Spd,4,1}, {StatType::CDmg,6} } },
	Artefact( ArtType::Shield, ArtSet::Speed, 5, 12, StatType::Def, { {StatType::HP_p,9}, {StatType::Spd,9}, {StatType::CDmg,4} } ),
	Artefact( ArtType::Shield, ArtSet::Speed, 5, 12, StatType::Def, { {StatType::CRate,4}, {StatType::Spd,13}, {StatType::Acc,9} } ),
	Artefact( ArtType::Shield, ArtSet::Speed, 5, 16, StatType::Def, { {StatType::HP,197}, {StatType::CDmg,15}, {StatType::Spd,9+3}, {StatType::Acc,10,2} } ),

	  Artefact( ArtType::Gloves, ArtSet::Speed, 6, 16, StatType::HP_p, { {StatType::Spd,22,4}, {StatType::Def,16,12}, {StatType::CRate,6}, {StatType::Def_p,6,3} }, ChampionName::Arbitr ),
	  Artefact{ ArtType::Gloves, ArtSet::Speed, 6, 16, StatType::CRate, { {StatType::Acc,29,3}, {StatType::HP,301,100}, {StatType::Atk,79}, {StatType::Def_p,7,1} }, ChampionName::Yuliana },
	Artefact( ArtType::Gloves, ArtSet::Speed, 6, 12, StatType::CRate, { {StatType::Atk_p,7}, {StatType::Spd,16}, {StatType::HP_p,13} } ),
	//Artefact{ ArtType::Gloves, ArtSet::Speed, 6, 1, StatType::, { {StatType::Def_p,6}, {StatType::CRate,6}, {StatType::Def,18}, {StatType::HP,220} } },
	  Artefact( ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::CRate, { {StatType::Def_p,11,1}, {StatType::Def,12,5}, {StatType::Atk,35,5}, {StatType::Spd,13,1} }, ChampionName::VGalek ),
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::Def_p, { {StatType::CRate,10}, {StatType::Acc,22}, {StatType::CDmg,4}, {StatType::HP_p,5} } },
	//Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 1, StatType::CDmg, { {StatType::Spd,4}, {StatType::Def,20}, {StatType::Res,11}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::Atk_p, { {StatType::Def,37,5}, {StatType::Def_p,12,1}, {StatType::CRate,10}, {StatType::Acc,8,1} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::CRate, { {StatType::Acc,29,2}, {StatType::Def_p,10,1}, {StatType::Atk,21,5}, {StatType::Res,11,2} } },
	  Artefact( ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::Def_p, { {StatType::Def,30}, {StatType::HP_p,5,1}, {StatType::Atk_p,15,1}, {StatType::HP,208} }, ChampionName::Gorgorab ),
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 12, StatType::CRate, { {StatType::Atk,21,5}, {StatType::Def,34,5}, {StatType::Spd,14,1} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 12, StatType::Def_p, { {StatType::HP,403}, {StatType::HP_p,12}, {StatType::Acc,25} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::Atk_p, { {StatType::Spd,13}, {StatType::Def,16,5}, {StatType::Acc,9}, {StatType::CRate,5} } },
	  Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 16, StatType::Def_p, { {StatType::Acc,21,2}, {StatType::CRate,10}, {StatType::HP,290,100}, {StatType::HP_p,5,1} }, ChampionName::Tyrel },
	  Artefact( ArtType::Gloves, ArtSet::Speed, 5, 12, StatType::CDmg, { {StatType::HP_p,11,1}, {StatType::Atk_p,5,4}, {StatType::Spd,14,1}, {StatType::Def,13,20} }, ChampionName::Guard ),
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 12, StatType::CDmg, { {StatType::CRate,9}, {StatType::Acc,20}, {StatType::Spd,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Speed, 5, 8, StatType::CRate, { {StatType::Spd,9}, {StatType::Def_p,5} } },

	//Artefact{ ArtType::Chest, ArtSet::Speed, 6, 1, StatType::Res, { {StatType::CRate,6}, {StatType::Atk_p,7}, {StatType::Def_p,6} } },
	Artefact( ArtType::Chest, ArtSet::Speed, 6, 12, StatType::HP_p, { {StatType::Acc,21}, {StatType::CDmg,14}, {StatType::Atk_p,6} } ),
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 8, StatType::HP_p, { {StatType::HP,155}, {StatType::CDmg,6}, {StatType::Acc,29} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 16, StatType::Atk_p, { {StatType::Def_p,4}, {StatType::HP_p,15}, {StatType::Spd,5}, {StatType::Acc,11} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 16, StatType::Def_p, { {StatType::Atk_p,16,1}, {StatType::Acc,11,3}, {StatType::Spd,5,1}, {StatType::CRate,6} } },
	Artefact{ ArtType::Chest, ArtSet::Speed, 5, 12, StatType::HP_p, { {StatType::HP,418}, {StatType::Spd,10}, {StatType::Def,23} } },
	//Artefact{ ArtType::Chest, ArtSet::Speed, 5, 1, StatType::Atk_p, {  } },
	  Artefact( ArtType::Chest, ArtSet::Speed, 4, 16, StatType::Def_p, { {StatType::HP_p,5,1}, {StatType::Spd,10,1}, {StatType::HP,277}, {StatType::Acc,8,2} }, ChampionName::Gorgorab ),

	  Artefact( ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::Def_p,9,1}, {StatType::Acc,19,2}, {StatType::Res,19,2}, {StatType::Atk,16,5} }, ChampionName::Arbitr ),
	Artefact( ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::CDmg,5}, {StatType::CRate,9}, {StatType::Atk_p,16} , {StatType::Def_p,4} } ),
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 8, StatType::Atk_p, { {StatType::Def_p,10,1}, {StatType::Acc,20,2}, {StatType::Spd,5,1} } },
	Artefact( ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::Acc,18}, {StatType::CDmg,11}, {StatType::CRate,5}, {StatType::Atk_p,5} } ),
	  Artefact( ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::Atk,37,4}, {StatType::CRate,11}, {StatType::Def_p,4,3}, {StatType::Res,10,2} }, ChampionName::Lekar ),
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 12, StatType::Spd, { {StatType::Atk_p,14}, {StatType::Def_p,5}, {StatType::CDmg,6} } },
	Artefact{ ArtType::Boots, ArtSet::Speed, 5, 16, StatType::Spd, { {StatType::Res,17,2}, {StatType::Acc,8,1}, {StatType::HP,234,100}, {StatType::Atk,23} } },
	  Artefact{ ArtType::Boots, ArtSet::Speed, 5, 16, StatType::HP_p, { {StatType::Atk,32,10}, {StatType::Spd,10}, {StatType::CRate,4}, {StatType::CDmg,4} }, ChampionName::Mavzolejnik },
	Artefact{ ArtType::Boots, ArtSet::Speed, 4, 8, StatType::Spd, { {StatType::Def_p,7}, {StatType::CDmg,8} } },
	  Artefact{ ArtType::Boots, ArtSet::Speed, 4, 16, StatType::Spd, { {StatType::Def_p,13,1}, {StatType::Atk_p,4,1}, {StatType::Acc,8,1}, {StatType::HP_p,5,1} }, ChampionName::Yuliana },

	// Vamp
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 6, 16, StatType::Atk, { {StatType::HP_p,6,2}, {StatType::Acc,22,2}, {StatType::CDmg,16}, {StatType::Atk_p,13, } }, ChampionName::ColdHeart },
	Artefact( ArtType::Weapon, ArtSet::Vamp, 6, 16, StatType::Atk, { {StatType::CRate,18}, {StatType::Atk_p,14,5}, {StatType::CDmg,6}, {StatType::Spd,5} } ),
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 6, 12, StatType::Atk, { {StatType::Res,33}, {StatType::HP_p,12}, {StatType::Atk_p,6} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 6, 12, StatType::Atk, { {StatType::Spd,17}, {StatType::CRate,6}, {StatType::Acc,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 6, 12, StatType::Atk, { {StatType::Spd,11}, {StatType::Acc,21}, {StatType::CDmg,6} } },
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::HP,629}, {StatType::CRate,14}, {StatType::Acc,8,3}, {StatType::Spd,5,2} }, ChampionName::Sohaty },
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::HP_p,5,2}, {StatType::Atk_p,5,2}, {StatType::Acc,35,2}, {StatType::HP,196} }, ChampionName::Yuliana },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::CRate,8}, {StatType::Atk_p,10}, {StatType::Spd,5}, {StatType::Res,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::Atk_p,15}, {StatType::Spd,4}, {StatType::CRate,5}, {StatType::Res,9} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::CDmg,14}, {StatType::CRate,6}, {StatType::Acc,9}, {StatType::HP,205} } },
	Artefact( ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::CDmg,6}, {StatType::CRate,15}, {StatType::Atk_p,5,3}, {StatType::Acc,9,2} }, ChampionName::Foly ),
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::Spd,4,1}, {StatType::Acc,31,2}, {StatType::HP,358,100} }, ChampionName::Razen },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::Atk_p,9}, {StatType::CRate,6}, {StatType::Res,10} } },
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::Atk_p,14,2}, {StatType::HP_p,6,2}, {StatType::CRate,6}, {StatType::Res,10,2} }, ChampionName::Alura },
	Artefact( ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::Acc,9,1}, {StatType::Atk_p,16,2}, {StatType::Spd,5,1}, {StatType::HP,379,275} } ),
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CDmg,4}, {StatType::Atk_p,16}, {StatType::HP_p,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CRate,11}, {StatType::Spd,8}, {StatType::Atk_p,4} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::CDmg,5}, {StatType::Spd,14}, {StatType::Res,10} } },
	Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 12, StatType::Atk, { {StatType::Spd,15}, {StatType::CRate,5}, {StatType::Atk_p,4} } },
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::CDmg,6}, {StatType::Spd,12}, {StatType::Acc,10}, {StatType::HP,360} }, ChampionName::SteelSkull },
	  Artefact{ ArtType::Weapon, ArtSet::Vamp, 5, 16, StatType::Atk, { {StatType::CRate,10}, {StatType::Spd,4}, {StatType::HP_p,6}, {StatType::Atk_p,5} }, ChampionName::Tyrel },

	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 6, 16, StatType::HP, { {StatType::Acc,11,2}, {StatType::CRate,17}, {StatType::HP_p,5,2}, {StatType::Atk_p,18,3} }, ChampionName::Foly },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 6, 12, StatType::HP, { {StatType::Acc,22}, {StatType::CDmg,17}, {StatType::HP_p,7} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 6, 12, StatType::HP, { {StatType::Spd,11}, {StatType::CRate,13}, {StatType::CDmg,7} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 6, 12, StatType::HP, { {StatType::Spd,18}, {StatType::CRate,7}, {StatType::Atk_p,6} } },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 6, 16, StatType::HP, { {StatType::HP_p,20,1}, {StatType::Spd,5,1}, {StatType::Def,24,15}, {StatType::Atk_p,7,2} }, ChampionName::VisirOvelis },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Def_p,12,1}, {StatType::Spd,5,1}, {StatType::Atk,34}, {StatType::Atk_p,4,2} }, ChampionName::Yuliana },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Spd,15,1}, {StatType::Def_p,6,2}, {StatType::CDmg,4}, {StatType::Res,9,3} }, ChampionName::Tyrel },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::HP_p,10,1}, {StatType::Acc,19,2}, {StatType::Spd,4,1}, {StatType::Def_p,4,1} }, ChampionName::SteelSkull },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::CRate,9}, {StatType::Spd,13}, {StatType::Acc,10}, {StatType::Res,9} }, ChampionName::Krisk },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Acc,11,5}, {StatType::Spd,6,1}, {StatType::HP_p,20,2}, {StatType::Atk_p,10,3} }, ChampionName::ColdHeart },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Atk_p,5}, {StatType::HP_p,6}, {StatType::Res,18}, {StatType::Spd,18} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::CDmg,10}, {StatType::CRate,12}, {StatType::Def_p,15}, {StatType::Atk_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Def_p,5,1}, {StatType::Acc,26,2}, {StatType::CRate,6}, {StatType::Res,10,1} } },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 16, StatType::HP, { {StatType::Atk_p,5}, {StatType::Acc,31}, {StatType::HP_p,4}, {StatType::Def,17} }, ChampionName::Sohaty },
	  Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::Spd,15,1}, {StatType::Def_p,4,1}, {StatType::Atk,15,5} }, ChampionName::Gromoboy },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::CRate,10}, {StatType::CDmg,11}, {StatType::Spd,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::Spd,9}, {StatType::Atk_p,10}, {StatType::Acc,9} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::Spd,5}, {StatType::CRate,14}, {StatType::Def_p,5} } },
	Artefact{ ArtType::Helmet, ArtSet::Vamp, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::Spd,8}, {StatType::Acc,11} } },

	  Artefact( ArtType::Shield, ArtSet::Vamp, 6, 16, StatType::Def, { {StatType::Spd,17,1}, {StatType::Def_p,6,1}, {StatType::CRate,6}, {StatType::CDmg,7} }, ChampionName::Alura ),
	  Artefact{ ArtType::Shield, ArtSet::Vamp, 6, 16, StatType::Def, { {StatType::HP_p,11,1}, {StatType::Def_p,13,3}, {StatType::Res,9,2}, {StatType::Spd,5,1} }, ChampionName::Yuliana },
	  Artefact{ ArtType::Shield, ArtSet::Vamp, 6, 16, StatType::Def, { {StatType::Acc,11,2}, {StatType::CRate,19}, {StatType::HP,423,100}, {StatType::Def_p,6,1} }, ChampionName::Gromoboy },
	  Artefact{ ArtType::Shield, ArtSet::Vamp, 6, 16, StatType::Def, { {StatType::CDmg,7}, {StatType::Spd,17,2}, {StatType::Def_p,5,3}, {StatType::HP,296,125} }, ChampionName::Krisk },
	  Artefact( ArtType::Shield, ArtSet::Vamp, 6, 16, StatType::Def, { {StatType::Spd,6}, {StatType::CRate,18}, {StatType::HP_p,6,3}, {StatType::HP,513,175} }, ChampionName::Foly ),
	  Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::CRate,5}, {StatType::Acc,31}, {StatType::HP,397} }, ChampionName::Razen },
	  Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 16, StatType::Def, { {StatType::CRate,4}, {StatType::Spd,10,1}, {StatType::HP_p,14,3}, {StatType::HP,314,225} }, ChampionName::VisirOvelis },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::HP_p,6}, {StatType::CDmg,10}, {StatType::Def_p,14} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::Res,28}, {StatType::CRate,11}, {StatType::Spd,4} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::CDmg,15}, {StatType::Spd,5}, {StatType::HP_p,5} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::Spd,8}, {StatType::HP_p,9}, {StatType::CRate,5} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::HP_p,6}, {StatType::Def_p,15}, {StatType::CDmg,5} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::CDmg,16}, {StatType::Spd,4}, {StatType::HP_p,4} } },
	Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 12, StatType::Def, { {StatType::Spd,5}, {StatType::CRate,14}, {StatType::Def_p,5} } },
	  Artefact{ ArtType::Shield, ArtSet::Vamp, 5, 8, StatType::Def, { {StatType::Def_p,5}, {StatType::Acc,30} }, ChampionName::Sohaty },

	Artefact{ ArtType::Gloves, ArtSet::Vamp, 6, 12, StatType::Atk_p, { {StatType::Def_p,13}, {StatType::Res,33}, {StatType::Acc,11} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::Def_p, { {StatType::Acc,18}, {StatType::Res,21}, {StatType::Atk,35}, {StatType::CRate,5} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Spd,9}, {StatType::Acc,16}, {StatType::Def,20} } },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::CDmg, { {StatType::HP_p,14}, {StatType::Acc,10}, {StatType::Atk_p,10} } },
	Artefact( ArtType::Gloves, ArtSet::Vamp, 5, 16, StatType::CRate, { {StatType::Spd,14,2}, {StatType::Atk_p,4,2}, {StatType::Res,9,3}, {StatType::Acc,9,2} } ),
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Atk_p,9}, {StatType::Res,19}/*, {StatType::HP,236}*/ } },
	  Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 16, StatType::CRate, { {StatType::Acc,10,2}, {StatType::Spd,12,1}, {StatType::HP_p,5,2}, {StatType::Def_p,5,2} }, ChampionName::ColdHeart },
	  Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::Def,21,5}, {StatType::HP_p,17,2}, {StatType::HP,153,100}, {StatType::Res,10,2} }, ChampionName::SteelSkull },
	  Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::CRate, { {StatType::CDmg,12}, {StatType::Acc,21}, {StatType::Def,31} }, ChampionName::Razen },
	  Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::Def_p, { {StatType::CDmg,10}, {StatType::Def,35,12}, {StatType::Acc,8,3} }, ChampionName::Gromoboy },
	Artefact{ ArtType::Gloves, ArtSet::Vamp, 5, 12, StatType::CRate, { {StatType::Acc,25}, {StatType::Spd,9}, {StatType::Atk,16,5} } },
	  Artefact{ ArtType::Gloves, ArtSet::Vamp, 4, 12, StatType::CRate, { {StatType::CDmg,9}, {StatType::Acc,15}, {StatType::Def_p,4} }, ChampionName::Sohaty },

	Artefact{ ArtType::Chest, ArtSet::Vamp, 6, 12, StatType::HP_p, { {StatType::Atk_p,12}, {StatType::Acc,20}, {StatType::CDmg,6} } },
	  Artefact( ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::CDmg,6}, {StatType::CRate,15}, {StatType::Atk_p,11,2}, {StatType::Spd,5,1} }, ChampionName::ColdHeart ),
	//Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 1, StatType::Res, { {StatType::Acc,9}, {StatType::CDmg,4}, {StatType::CRate,4} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Res,31}, {StatType::Def_p,6}/*, {StatType::Def,18}*/ } },
	  Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::CDmg,11}, {StatType::HP_p,4}, {StatType::Spd,4}, {StatType::Atk_p,4} }, ChampionName::Razen },
	  Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::Spd,9}, {StatType::Def,16}, {StatType::HP_p,5}, {StatType::Atk_p,4,2} }, ChampionName::Yuliana },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Res,31}, {StatType::Def_p,6}, {StatType::Def,18} } },
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Acc,30}, {StatType::Spd,5}, {StatType::CDmg,6} } },
	  Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::Spd,9,1}, {StatType::HP_p,10,1}, {StatType::Atk,20,15}, {StatType::Res,11,2} }, ChampionName::VisirOvelis },
	  Artefact( ArtType::Chest, ArtSet::Vamp, 5, 12, StatType::HP_p, { {StatType::Spd,14}, {StatType::Acc,11}, {StatType::HP,360} }, ChampionName::Alura ),
	Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 12, StatType::Def_p, { {StatType::Atk_p,10}, {StatType::Res,21}, {StatType::CDmg,5} } },
	  Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::Def_p, { {StatType::CDmg,9}, {StatType::CRate,10}, {StatType::Res,9,2}, {StatType::HP,364,100} }, ChampionName::Tyrel },
	  Artefact{ ArtType::Chest, ArtSet::Vamp, 5, 16, StatType::HP_p, { {StatType::Atk_p,10,1}, {StatType::CRate,6}, {StatType::Def_p,5,1}, {StatType::HP,351,175} }, ChampionName::Krisk },

	Artefact{ ArtType::Boots, ArtSet::Vamp, 6, 8, StatType::Atk_p, { {StatType::CRate,7}, {StatType::Def_p,12}, {StatType::Atk,26}, {StatType::HP_p,11} } },
	  Artefact( ArtType::Boots, ArtSet::Vamp, 6, 16, StatType::Spd, { {StatType::CRate,20}, {StatType::Def_p,6,2}, {StatType::HP_p,6,2}, {StatType::Atk,28,15} }, ChampionName::Alura ),
	  Artefact( ArtType::Boots, ArtSet::Vamp, 6, 16, StatType::Spd, { {StatType::HP_p,18,2}, {StatType::Atk_p,12,2}, {StatType::Atk,18,15}, {StatType::Acc,10,2} }, ChampionName::ColdHeart ),
	Artefact( ArtType::Boots, ArtSet::Vamp, 6, 16, StatType::Spd, { {StatType::CDmg,13}, {StatType::Atk_p,12,3}, {StatType::Res,11,4}, {StatType::Def,21,10} } ),
	Artefact( ArtType::Boots, ArtSet::Vamp, 5, 16, StatType::Spd, { {StatType::Def_p,17}, {StatType::HP_p,5}, {StatType::Acc,18}, {StatType::Res,9} } ),
	  Artefact( ArtType::Boots, ArtSet::Vamp, 5, 16, StatType::Spd, { {StatType::Def_p,8,2}, {StatType::Atk_p,11,3}, {StatType::CDmg,5}, {StatType::Res,10,3} }, ChampionName::Foly ),
	  Artefact( ArtType::Boots, ArtSet::Vamp, 5, 16, StatType::Spd, { {StatType::CRate,10}, {StatType::Acc,19,3}, {StatType::Atk,17,10}, {StatType::Def,15,10} }, ChampionName::VisirOvelis ),
	Artefact( ArtType::Boots, ArtSet::Vamp, 5, 12, StatType::Spd, { {StatType::CDmg,9}, {StatType::Acc,8}, {StatType::Def_p,8}, {StatType::HP,500} } ),
	Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 8, StatType::HP_p, { {StatType::Acc,9}, {StatType::Atk_p,15} } },
	Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 8, StatType::HP_p, { {StatType::HP,274}, {StatType::Spd,13} } },
	Artefact{ ArtType::Boots, ArtSet::Vamp, 4, 8, StatType::Spd, { {StatType::HP,266}, {StatType::CDmg,8} } },
	  Artefact{ ArtType::Boots, ArtSet::Vamp, 4, 16, StatType::Spd, { {StatType::Atk_p,4,1}, {StatType::Acc,25,3}, {StatType::HP_p,5}, {StatType::Def_p,3} }, ChampionName::SteelSkull },
	  Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 16, StatType::Spd, { {StatType::Acc,19,3}, {StatType::HP_p,10,2}, {StatType::CRate,4}, {StatType::Atk_p,4,1} }, ChampionName::Gromoboy },
	  Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 16, StatType::Spd, { {StatType::Res,18,10}, {StatType::Acc,20,2}, {StatType::Def_p,5,2}, {StatType::HP_p,5,1} }, ChampionName::Krisk },
	  Artefact{ ArtType::Boots, ArtSet::Vamp, 5, 16, StatType::Spd, { {StatType::Acc,29,2}, {StatType::Atk,22,5}, {StatType::HP,137,100}, {StatType::Def,19,5} }, ChampionName::Tyrel },

	//
	Artefact( ArtType::Boots, ArtSet::Resist, 5, 1, StatType::Spd, { {StatType::CRate,5}, {StatType::Def_p,5} } ),

	// ������
	Artefact( ArtType::Gloves, ArtSet::Daze, 5, 8, StatType::HP_p, { {StatType::Acc,19}, {StatType::Atk_p,6}, {StatType::Def_p,9} } ),
	Artefact( ArtType::Chest, ArtSet::Daze, 5, 8, StatType::HP_p, { {StatType::CRate,6}, {StatType::Atk_p,6}, {StatType::Def_p,14} } ),
	Artefact( ArtType::Chest, ArtSet::Daze, 5, 8, StatType::HP_p, { {StatType::Atk_p,6}, {StatType::CRate,14} } ),
	Artefact( ArtType::Boots, ArtSet::Daze, 4, 16, StatType::Spd, { {StatType::Acc,21,2}, {StatType::Def_p,5,1}, {StatType::Atk_p,4}, {StatType::HP,310,100} }, ChampionName::Sohaty ),

	//
	Artefact( ArtType::Boots, ArtSet::Cursed, 5, 8, StatType::Spd, { {StatType::Def_p,10}/*, {StatType::HP,380}*/ } ),

	// Frost
	Artefact( ArtType::Gloves, ArtSet::Frost, 5, 8, StatType::CRate, { {StatType::Def_p,8}, {StatType::Spd,5}, {StatType::Acc,18} } ),

	// Immunitet
	Artefact( ArtType::Gloves, ArtSet::Immunitet, 5, 8, StatType::HP_p, { {StatType::CRate,14}, {StatType::CDmg,5} } ),

	// ������
	Artefact( ArtType::Gloves, ArtSet::Gibel, 5, 8, StatType::CRate, { {StatType::HP_p,5}, {StatType::Atk_p,15} } ),

	// Shield
	  Artefact( ArtType::Weapon, ArtSet::Shield, 5, 12, StatType::Atk, { {StatType::Acc,19,1}, {StatType::Spd,10,1}, {StatType::HP_p,9,1} }, ChampionName::Hangar ),
	Artefact{ ArtType::Weapon, ArtSet::Shield, 5, 12, StatType::Atk, { {StatType::Atk_p,9}, {StatType::Res,21}, {StatType::Spd,9} } },

	Artefact{ ArtType::Helmet, ArtSet::Shield, 5, 12, StatType::HP, { {StatType::Spd,9}, {StatType::CDmg,5}, {StatType::Def_p,15} } },
	Artefact{ ArtType::Helmet, ArtSet::Shield, 5, 12, StatType::HP, { {StatType::CRate,8}, {StatType::CDmg,10}, {StatType::Spd,4} } },
	  Artefact( ArtType::Helmet, ArtSet::Shield, 5, 16, StatType::HP, { {StatType::CRate,10}, {StatType::HP_p,12,2}, {StatType::Spd,4,2}, {StatType::Acc,10,3} }, ChampionName::Hangar ),

	Artefact{ ArtType::Shield, ArtSet::Shield, 5, 12, StatType::Def, { {StatType::CRate,16}, {StatType::CDmg,6}, {StatType::Res,20}, {StatType::Acc,9} } },
	  Artefact( ArtType::Shield, ArtSet::Shield, 5, 16, StatType::Def, { {StatType::Def_p,15,2}, {StatType::CDmg,5}, {StatType::HP_p,10,2}, {StatType::Spd,4,1} }, ChampionName::Hangar ),
	Artefact{ ArtType::Shield, ArtSet::Shield, 5, 12, StatType::Def, { {StatType::Spd,10}, {StatType::Def_p,15}, {StatType::Res,9} } },
	Artefact{ ArtType::Shield, ArtSet::Shield, 5, 12, StatType::Def, { {StatType::Def_p,5}, {StatType::Spd,15}, {StatType::CRate,6} } },
	Artefact{ ArtType::Shield, ArtSet::Shield, 5, 12, StatType::Def, { {StatType::Def_p,5}, {StatType::Spd,14}, {StatType::Res,9} } },

	  Artefact( ArtType::Gloves, ArtSet::Shield, 5, 16, StatType::CRate, { {StatType::HP_p,13,1}, {StatType::CDmg,5}, {StatType::Def_p,9,1}, {StatType::Acc,10,2} }, ChampionName::Hangar ),
	Artefact{ ArtType::Gloves, ArtSet::Shield, 5, 12, StatType::CDmg, { {StatType::Atk,17}, {StatType::Def_p,9}, {StatType::Spd,13} } },
	Artefact{ ArtType::Gloves, ArtSet::Shield, 5, 12, StatType::Def_p, { {StatType::Acc,10}, {StatType::CDmg,13}, {StatType::HP_p,4} } },

	Artefact{ ArtType::Chest, ArtSet::Shield, 5, 12, StatType::Def_p, { {StatType::HP_p,14}, {StatType::Acc,10}, {StatType::Def,22} } },

	Artefact( ArtType::Boots, ArtSet::Shield, 5, 12, StatType::Spd, { {StatType::CRate,11}, {StatType::Res,17}, {StatType::Acc,11} } ),

	//
	Artefact( ArtType::Gloves, ArtSet::Revenge, 5, 8, StatType::CRate, { {StatType::Spd,12,1}, {StatType::Acc,11} } ),

	//
	Artefact( ArtType::Gloves, ArtSet::Vozmezdie, 6, 12, StatType::HP_p, { {StatType::Spd,24}, {StatType::CRate,6}, {StatType::Def,28} } ),
	Artefact( ArtType::Chest, ArtSet::Vozmezdie, 5, 8, StatType::HP_p, { {StatType::Def_p,4}, {StatType::CRate,15} } ),
	Artefact( ArtType::Boots, ArtSet::Vozmezdie, 5, 8, StatType::Spd, { {StatType::CRate,9}, {StatType::Def_p,9}, {StatType::Atk_p,4} } ),

	//
	Artefact( ArtType::Boots, ArtSet::Doblest, 5, 8, StatType::Spd, { {StatType::Def_p,6}, {StatType::CRate,4}, {StatType::HP,980} } ),

	//Artefact( ArtType::Boots, ArtSet::Fury, 4, 8, StatType::Spd, { {StatType::Acc,15}, {StatType::Atk_p,4} } ),
	//Artefact( ArtType::Boots, ArtSet::Reflex, 4, 8, StatType::Spd, { {StatType::HP_p,9}, {StatType::CDmg,8} } ),

	// Curing
	Artefact{ ArtType::Chest, ArtSet::Curing, 5, 16, StatType::Def_p, { {StatType::HP_p,12,1}, {StatType::Def,13,5}, {StatType::Acc,9}, {StatType::Spd,4,1} } },

	// Savage
	Artefact( ArtType::Shield, ArtSet::Savage, 5, 12, StatType::Def, { {StatType::CDmg,9}, {StatType::Acc,19}, {StatType::Def_p,6} } ),

	// Cruel
	Artefact( ArtType::Weapon, ArtSet::Cruel, 6, 12, StatType::Atk, { {StatType::Res,12}, {StatType::HP_p,11}, {StatType::Spd,5}, {StatType::Acc,32} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Atk_p,11}, {StatType::Spd,9}, {StatType::HP_p,12}, {StatType::CDmg,10} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Atk_p,5}, {StatType::Res,26}, {StatType::Acc,21}, {StatType::CDmg,10} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Acc,10}, {StatType::HP_p,19}, {StatType::Spd,5}, {StatType::CDmg,10} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Acc,30}, {StatType::HP,697}, {StatType::Spd,9}, {StatType::HP_p,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Atk_p,15}, {StatType::CDmg,5}, {StatType::CRate,9}, {StatType::Acc,18} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Res,8}, {StatType::Atk_p,10}, {StatType::HP_p,15}, {StatType::Spd,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Spd,17}, {StatType::HP,408}, {StatType::CDmg,10}, {StatType::Atk_p,4} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::Acc,17}, {StatType::HP,135}, {StatType::Res,17}, {StatType::Spd,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::HP_p,4,3}, {StatType::CRate,16}, {StatType::Atk_p,5,4}, {StatType::CDmg,10} }, ChampionName::Mashalled ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::Res,11}, {StatType::Atk_p,10}, {StatType::Spd,4}, {StatType::CDmg,17} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::CRate,15}, {StatType::Acc,9}, {StatType::Spd,10}, {StatType::Res,10} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 16, StatType::Atk, { {StatType::Spd,14}, {StatType::Acc,20}, {StatType::HP_p,6}, {StatType::Res,8} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::CDmg,4}, {StatType::CRate,15}, {StatType::Spd,8} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::Atk_p,14}, {StatType::CRate,5}, {StatType::Acc,17} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::Atk_p,10}, {StatType::CDmg,9}, {StatType::CRate,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::Atk_p,13}, {StatType::Acc,9}, {StatType::CRate,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Cruel, 5, 12, StatType::Atk, { {StatType::Acc,18}, {StatType::CRate,9}, {StatType::CDmg,5} } ),

	Artefact( ArtType::Helmet, ArtSet::Cruel, 6, 12, StatType::HP, { {StatType::Acc,11}, {StatType::Spd,10}, {StatType::HP_p,17}, {StatType::CRate,6} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 16, StatType::HP, { {StatType::Res,17}, {StatType::CDmg,15}, {StatType::Atk_p,6}, {StatType::Def_p,11} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 16, StatType::HP, { {StatType::CRate,11}, {StatType::HP_p,4,4}, {StatType::Def_p,22,3}, {StatType::CDmg,6} }, ChampionName::Mashalled ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 12, StatType::HP, { {StatType::CDmg,8}, {StatType::Acc,10}, {StatType::CRate,16} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 12, StatType::HP, { {StatType::HP_p,10}, {StatType::CDmg,5}, {StatType::Spd,4}, {StatType::Res,27} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 12, StatType::HP, { {StatType::Spd,10}, {StatType::Res,18}, {StatType::CDmg,11} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 12, StatType::HP, { {StatType::CRate,11}, {StatType::Atk_p,16}, {StatType::Acc,9} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 12, StatType::HP, { {StatType::CDmg,10}, {StatType::Spd,9}, {StatType::Def_p,4} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 12, StatType::HP, { {StatType::Atk_p,9}, {StatType::HP_p,9}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Helmet, ArtSet::Cruel, 5, 8, StatType::HP, { {StatType::Atk_p,10}, {StatType::HP_p,10}, {StatType::Acc,9} } ),

	Artefact( ArtType::Shield, ArtSet::Cruel, 6, 12, StatType::Def, { {StatType::Spd,12}, {StatType::CRate,7}, {StatType::Acc,20}, {StatType::Def_p,12} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 16, StatType::Def, { {StatType::HP_p,5}, {StatType::CDmg,10}, {StatType::Res,9}, {StatType::Acc,39} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 16, StatType::Def, { {StatType::Acc,10,3}, {StatType::CRate,15}, {StatType::CDmg,14}, {StatType::HP_p,6,3} }, ChampionName::Mashalled ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 16, StatType::Def, { {StatType::Acc,18}, {StatType::HP,184}, {StatType::Spd,10}, {StatType::Def_p,15} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 16, StatType::Def, { {StatType::CDmg,5}, {StatType::Def_p,11}, {StatType::Spd,14}, {StatType::HP_p,6} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::Acc,18}, {StatType::CDmg,16}, {StatType::Spd,4} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::Spd,4}, {StatType::CDmg,8}, {StatType::CRate,13} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::CRate,10}, {StatType::Def_p,11}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::Spd,9}, {StatType::HP_p,8}, {StatType::Res,9} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::CDmg,9}, {StatType::Spd,9}, {StatType::CRate,5} } ),
	Artefact( ArtType::Shield, ArtSet::Cruel, 5, 12, StatType::Def, { {StatType::Spd,9}, {StatType::CDmg,11}, {StatType::HP_p,5} } ),

	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 16, StatType::Def_p, { {StatType::CRate,6}, {StatType::Acc,40}, {StatType::Atk_p,4}, {StatType::Atk,16} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 12, StatType::Def_p, { {StatType::Res,10}, {StatType::Spd,14}, {StatType::Def,18} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 12, StatType::Def_p, { {StatType::CRate,16}, {StatType::CDmg,4}, {StatType::Def,22} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 12, StatType::CRate, { {StatType::Res,9}, {StatType::Def_p,14}, {StatType::Atk_p,6} } ),
	Artefact( ArtType::Gloves, ArtSet::Cruel, 5, 12, StatType::Atk_p, { {StatType::HP_p,10}, {StatType::Def_p,9}, {StatType::Acc,9} } ),

	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 12, StatType::Atk_p, { {StatType::Res,18}, {StatType::Def,18,5}, {StatType::Spd,9}, {StatType::HP_p,9} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 12, StatType::Acc, { {StatType::Def_p,11}, {StatType::Atk_p,10}, {StatType::HP,409}, {StatType::Spd,9} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 12, StatType::HP_p, { {StatType::Acc,17}, {StatType::Def,18}, {StatType::CDmg,15} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 12, StatType::Atk_p, { {StatType::HP_p,10}, {StatType::CRate,4}, {StatType::Res,28} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 12, StatType::Atk_p, { {StatType::Def_p,4}, {StatType::CRate,10}, {StatType::CDmg,14} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 8, StatType::Atk_p, { {StatType::Atk,12,10}, {StatType::CDmg,9}, {StatType::Res,17} } ),
	Artefact( ArtType::Chest, ArtSet::Cruel, 5, 16, StatType::Atk_p, { {StatType::CRate,4}, {StatType::Spd,15,1}, {StatType::HP_p,5,3}, {StatType::Res,11,5} }, ChampionName::Mashalled ),

	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 16, StatType::Spd, { {StatType::CDmg,19}, {StatType::Atk_p,4}, {StatType::CRate,5}, {StatType::HP_p,5} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 12, StatType::Spd, { {StatType::HP,409}, {StatType::Atk_p,10}, {StatType::Atk,15,5} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 12, StatType::Spd, { {StatType::CRate,10}, {StatType::Res,19}, {StatType::Def_p,4} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 12, StatType::Spd, { {StatType::Acc,20}, {StatType::HP_p,10}, {StatType::Res,9} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 5, 12, StatType::Spd, { {StatType::CRate,8}, {StatType::HP,418}, {StatType::Atk_p,5} } ),
	Artefact( ArtType::Boots, ArtSet::Cruel, 4, 8, StatType::Spd, { {StatType::Atk,23,10}, {StatType::CRate,10}, {StatType::Acc,7}, {StatType::Def_p,5} } ),

	// Immortal
	Artefact( ArtType::Weapon, ArtSet::Immortal, 6, 12, StatType::Atk, { {StatType::CRate,6}, {StatType::CDmg,11}, {StatType::Atk_p,6}, {StatType::Res,30} } ),
	  Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::Acc,19,2}, {StatType::CRate,14}, {StatType::Atk_p,5,1}, {StatType::CDmg,9} }, ChampionName::Gromoboy ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::CRate,10}, {StatType::Acc,26,2}, {StatType::HP_p,5,2}, {StatType::HP,689,175} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::CDmg,6}, {StatType::Atk_p,14}, {StatType::CRate,11}, {StatType::Acc,22} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::CDmg,10}, {StatType::Spd,14}, {StatType::HP_p,10}, {StatType::Res,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::Atk_p,5}, {StatType::Spd,9}, {StatType::Acc,29}, {StatType::Res,18} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::Atk_p,4}, {StatType::Spd,9}, {StatType::HP_p,15}, {StatType::HP,299} } ),
	//Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::HP_p,14,1}, {StatType::Spd,8,1}, {StatType::CDmg,4}, {StatType::HP,709,225} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::Acc,10,1}, {StatType::Spd,12,1}, {StatType::CDmg,10}, {StatType::Res,10,2} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::Acc,17}, {StatType::Res,9}, {StatType::CDmg,15} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::HP_p,10}, {StatType::CDmg,9}, {StatType::CRate,6}, {StatType::Res,10} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::Acc,19}, {StatType::Spd,9}, {StatType::CDmg,6}, {StatType::HP_p,5} } ),
	  Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 16, StatType::Atk, { {StatType::Spd,9}, {StatType::HP_p,15}, {StatType::CDmg,6}, {StatType::HP,318} }, ChampionName::Straholud ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::Acc,21}, {StatType::Spd,9}, {StatType::Atk_p,6} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::CRate,9}, {StatType::Acc,8} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::HP_p,15}, {StatType::Spd,5}, {StatType::CRate,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Immortal, 5, 12, StatType::Atk, { {StatType::CRate,11}, {StatType::Atk_p,11}, {StatType::Spd,9} } ),

	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::CRate,5}, {StatType::Spd,5}, {StatType::CDmg,9}, {StatType::Acc,40} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::Def_p,5}, {StatType::Acc,20}, {StatType::HP_p,10}, {StatType::Atk_p,15} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::Spd,9+1}, {StatType::Res,10}, {StatType::Acc,29,2}, {StatType::HP_p,10,1} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::Atk_p,9}, {StatType::Spd,9}, {StatType::Res,30}, {StatType::CRate,5} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::Spd,5}, {StatType::Atk,32}, {StatType::Def_p,9}, {StatType::CRate,10} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::HP_p,12}, {StatType::Atk_p,9}, {StatType::Spd,5}, {StatType::Acc,8} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::Atk_p,5}, {StatType::Def,58}, {StatType::HP_p,4}, {StatType::Spd,12} } ),
	  Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::Def_p,8,1}, {StatType::CRate,9}, {StatType::Acc,20,2}, {StatType::Atk_p,4,1} }, ChampionName::VGalek ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::Acc,35}, {StatType::CDmg,5}, {StatType::Def_p,5,1}, {StatType::Res,8,2} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::Res,18}, {StatType::Acc,10}, {StatType::Def_p,16} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::CRate,9}, {StatType::Def,18}, {StatType::HP_p,17} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::CRate,5}, {StatType::Spd,13}, {StatType::Def_p,10} } ),
	Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 12, StatType::HP, { {StatType::CRate,9}, {StatType::Spd,9}, {StatType::Res,11} } ),
	  Artefact( ArtType::Helmet, ArtSet::Immortal, 5, 16, StatType::HP, { {StatType::HP_p,4,1}, {StatType::Def_p,13,1}, {StatType::Spd,5}, {StatType::Atk,21} }, ChampionName::Mavzolejnik ),

	  Artefact( ArtType::Shield, ArtSet::Immortal, 6, 12, StatType::Def, { {StatType::CRate,11}, {StatType::Spd,11,1}, {StatType::Res,10,2}, {StatType::Def_p,13,2} }, ChampionName::Lekar ),
	  Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::HP_p,19}, {StatType::Spd,4}, {StatType::CDmg,6}, {StatType::CRate,9} }, ChampionName::Straholud ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::Def_p,6}, {StatType::Acc,28}, {StatType::HP_p,4}, {StatType::Spd,15} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::Res,27}, {StatType::CDmg,11}, {StatType::Spd,10}, {StatType::CRate,5} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::CRate,5}, {StatType::CDmg,10}, {StatType::Acc,16}, {StatType::Spd,13} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Def_p,10}, {StatType::Acc,8}, {StatType::CRate,16}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Def_p,10}, {StatType::HP,547}, {StatType::Spd,5}, {StatType::HP_p,9} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Res,21}, {StatType::CRate,11}, {StatType::CDmg,10}, {StatType::Acc,9} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::Spd,9}, {StatType::Acc,10}, {StatType::CDmg,16}, {StatType::Def_p,5} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::CDmg,16}, {StatType::Spd,10}, {StatType::CRate,6}, {StatType::Res,10} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 13, StatType::Def, { {StatType::CRate,10}, {StatType::Spd,9}, {StatType::Def_p,8} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Res,29}, {StatType::Def_p,4}, {StatType::Spd,9} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::CRate,8}, {StatType::HP_p,14,1}, {StatType::Spd,5,2}, {StatType::Acc,9,2} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Acc,21}, {StatType::Res,21}, {StatType::Spd,9} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::HP_p,15}, {StatType::Res,9}, {StatType::Spd,9} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::CDmg,5}, {StatType::Acc,28}, {StatType::Spd,4}, {StatType::HP_p,5} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 13, StatType::Def, { {StatType::Def_p,15,1}, {StatType::Spd,4+1}, {StatType::Res,11} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 16, StatType::Def, { {StatType::CRate,11}, {StatType::Spd,9,1}, {StatType::Acc,9,2}, {StatType::Def_p,6,1} } ),
	Artefact( ArtType::Shield, ArtSet::Immortal, 5, 12, StatType::Def, { {StatType::Def_p,11}, {StatType::CRate,11}, {StatType::Res,11} } ),

	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 16, StatType::Atk_p, { {StatType::CRate,15}, {StatType::HP_p,4}, {StatType::Spd,10}, {StatType::Acc,19} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 16, StatType::Atk_p, { {StatType::CRate,5}, {StatType::Acc,16}, {StatType::HP_p,17}, {StatType::Res,10} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 16, StatType::Def_p, { {StatType::CDmg,5}, {StatType::Atk_p,14}, {StatType::CRate,10}, {StatType::Acc,9} } ),
	  Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 14, StatType::CRate, { {StatType::HP_p,15}, {StatType::Atk_p,6}, {StatType::Def,37} }, ChampionName::Straholud ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 12, StatType::HP_p, { {StatType::Atk,41}, {StatType::CRate,14}, {StatType::Spd,4} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 12, StatType::Atk_p, { {StatType::Def_p,5}, {StatType::Spd,5}, {StatType::CRate,22} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 12, StatType::Atk_p, { {StatType::Acc,20}, {StatType::HP_p,10}, {StatType::Spd,10} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 12, StatType::CRate, { {StatType::Acc,29}, {StatType::HP,378}, {StatType::CDmg,10} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 12, StatType::HP_p, { {StatType::Spd,4}, {StatType::Def_p,9}, {StatType::CDmg,16} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 5, 8, StatType::Def_p, { {StatType::CRate,11}, {StatType::Res,8} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 4, 16, StatType::Def_p, { {StatType::Spd,6,1}, {StatType::CRate,4}, {StatType::Atk,18}, {StatType::CDmg,13} } ),
	Artefact( ArtType::Gloves, ArtSet::Immortal, 4, 12, StatType::Def_p, { {StatType::Spd,3,1}, {StatType::Res,15,2}, {StatType::HP_p,14,1} } ),

	  Artefact( ArtType::Chest, ArtSet::Immortal, 5, 16, StatType::Def_p, { {StatType::Atk,14,10}, {StatType::Res,19,2}, {StatType::CRate,10}, {StatType::HP_p,15,2} }, ChampionName::Guard ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 16, StatType::Def_p, { {StatType::Acc,17}, {StatType::CDmg,4}, {StatType::Atk_p,16}, {StatType::CRate,5} } ),
	  Artefact( ArtType::Chest, ArtSet::Immortal, 5, 16, StatType::HP_p, { {StatType::Def,37,12}, {StatType::Atk_p,8,1}, {StatType::Spd,9,2}, {StatType::Acc,9,4} }, ChampionName::Hangar ),
	  Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::Def_p, { {StatType::HP,203}, {StatType::Spd,13}, {StatType::Def,17} }, ChampionName::Mavzolejnik ),
	  Artefact( ArtType::Chest, ArtSet::Immortal, 5, 16, StatType::Def_p, { {StatType::CRate,9}, {StatType::Spd,9}, {StatType::Acc,9}, {StatType::HP,330} }, ChampionName::Gromoboy ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::HP_p, { {StatType::Def_p,9}, {StatType::Atk_p,14}, {StatType::Def,14,5} } ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::HP_p, { {StatType::CDmg,10}, {StatType::Atk,57}, {StatType::Spd,5} } ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::Def_p, { {StatType::Acc,30}, {StatType::CDmg,10}, {StatType::CRate,5} } ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::Def_p, { {StatType::Res,18}, {StatType::CRate,13}, {StatType::Spd,4} } ),
	  Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::Def_p, { {StatType::Spd,14,1}, {StatType::CRate,6}, {StatType::Def,13,5}, {StatType::Atk,18,5} }, ChampionName::Lekar ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 12, StatType::Def_p, { {StatType::Spd,4}, {StatType::Atk_p,14}, {StatType::HP_p,6} } ),
	Artefact( ArtType::Chest, ArtSet::Immortal, 5, 13, StatType::HP_p, { {StatType::Def_p,9,1}, {StatType::Atk_p,4,1}, {StatType::Atk,17} } ),
	  Artefact( ArtType::Chest, ArtSet::Immortal, 5, 16, StatType::Def_p, { {StatType::CRate,5}, {StatType::HP_p,22,1}, {StatType::Atk_p,5,2}, {StatType::Res,11,2} }, ChampionName::VGalek ),

	Artefact( ArtType::Boots, ArtSet::Immortal, 5, 16, StatType::Spd, { {StatType::Acc,18,3}, {StatType::HP_p,10,2}, {StatType::CRate,11}, {StatType::Atk,37,10} } ),
	  Artefact( ArtType::Boots, ArtSet::Immortal, 5, 16, StatType::Spd, { {StatType::HP_p,10}, {StatType::Acc,21}, {StatType::Atk_p,12}, {StatType::CDmg,8} }, ChampionName::Straholud ),
	  Artefact( ArtType::Boots, ArtSet::Immortal, 5, 16, StatType::Spd, { {StatType::HP_p,5,2}, {StatType::CRate,16}, {StatType::Atk_p,9,3}, {StatType::Acc,11,2} }, ChampionName::Guard ),
	  Artefact( ArtType::Boots, ArtSet::Immortal, 5, 12, StatType::Spd, { {StatType::Acc,29}, {StatType::Atk_p,5,1}, {StatType::HP_p,5,1}, {StatType::HP,252,325} }, ChampionName::Hangar ),
	Artefact( ArtType::Boots, ArtSet::Immortal, 4, 16, StatType::Spd, { {StatType::Atk,23,5}, {StatType::Atk_p,14,1}, {StatType::Def_p,4,1}, {StatType::CDmg,4} } ),


	// DivAtk
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 6, 16, StatType::Atk, { {StatType::HP,860}, {StatType::HP_p,12}, {StatType::Atk_p,10}, {StatType::Acc,12,1} } ),
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 5, 12, StatType::Atk, { {StatType::Atk_p,5}, {StatType::HP_p,10}, {StatType::CDmg,16} } ),
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 5, 12, StatType::Atk, { {StatType::Spd,4}, {StatType::Atk_p,10}, {StatType::CRate,16} } ),
	Artefact( ArtType::Weapon, ArtSet::DivAtk, 5, 8, StatType::Atk, { {StatType::Res,29}, {StatType::Acc,9}, {StatType::Atk_p,5} } ),

	Artefact( ArtType::Helmet, ArtSet::DivAtk, 6, 8, StatType::HP, { {StatType::Res,9}, {StatType::HP_p,12}, {StatType::Atk_p,11} } ),
	  Artefact( ArtType::Helmet, ArtSet::DivAtk, 5, 16, StatType::HP, { {StatType::CDmg,15}, {StatType::CRate,5}, {StatType::Atk_p,10,4}, {StatType::Def_p,5,2} }, ChampionName::Rotos ),
	Artefact( ArtType::Helmet, ArtSet::DivAtk, 5, 12, StatType::HP, { {StatType::HP_p,9}, {StatType::Def_p,9}, {StatType::Spd,4}, {StatType::Def,34} } ),
	Artefact( ArtType::Helmet, ArtSet::DivAtk, 5, 12, StatType::HP, { {StatType::Acc,20}, {StatType::Atk_p,4}, {StatType::Spd,13} } ),

	Artefact( ArtType::Shield, ArtSet::DivAtk, 6, 12, StatType::Def, { {StatType::Spd,5}, {StatType::Res,19}, {StatType::CRate,13}, {StatType::Def_p,11} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 6, 12, StatType::Def, { {StatType::Spd,11}, {StatType::Def_p,12}, {StatType::Acc,12} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::Acc,9}, {StatType::HP_p,11}, {StatType::CDmg,16} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::Def_p,15}, {StatType::Acc,18}, {StatType::HP_p,4} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::CDmg,20}, {StatType::Acc,11}, {StatType::Def_p,6} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::CRate,6}, {StatType::CDmg,4}, {StatType::Def_p,20} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::CRate,6}, {StatType::Acc,35}, {StatType::CDmg,6} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::Acc,10}, {StatType::CRate,14}, {StatType::Spd,5} } ),
	Artefact( ArtType::Shield, ArtSet::DivAtk, 5, 12, StatType::Def, { {StatType::Acc,28}, {StatType::Res,9}, {StatType::CRate,9} } ),

	Artefact( ArtType::Gloves, ArtSet::DivAtk, 6, 12, StatType::CRate, { {StatType::Def_p,13}, {StatType::HP,1020}, {StatType::Res,22} } ),
	  Artefact( ArtType::Gloves, ArtSet::DivAtk, 5, 12, StatType::CRate, { {StatType::Atk,57}, {StatType::CDmg,5}, {StatType::Def_p,11} }, ChampionName::Rotos ),

	Artefact( ArtType::Chest, ArtSet::DivAtk, 6, 12, StatType::HP_p, { {StatType::Spd,6}, {StatType::CRate,18}, {StatType::CDmg,11} } ),
	//Artefact( ArtType::Chest, ArtSet::DivAtk, 6, 8, StatType::HP_p, { {StatType::CRate,13}, {StatType::Atk_p,6}, {StatType::Atk,52} } ),
	Artefact( ArtType::Chest, ArtSet::DivAtk, 5, 12, StatType::HP_p, { {StatType::CRate,15}, {StatType::Acc,9}, {StatType::HP,456} } ),

	Artefact( ArtType::Boots, ArtSet::DivAtk, 4, 8, StatType::Spd, { {StatType::Def,21}, {StatType::Acc,18} } ),

	// DivCritRate
	Artefact( ArtType::Weapon, ArtSet::DivCritRate, 5, 12, StatType::Atk, { {StatType::CDmg,10}, {StatType::Atk_p,17}, {StatType::Acc,9}, {StatType::CRate,5} } ),
	Artefact( ArtType::Weapon, ArtSet::DivCritRate, 5, 12, StatType::Atk, { {StatType::HP_p,11}, {StatType::Atk_p,10}, {StatType::Acc,19} } ),
	Artefact( ArtType::Weapon, ArtSet::DivCritRate, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::CRate,5}, {StatType::Res,28} } ),

	Artefact( ArtType::Helmet, ArtSet::DivCritRate, 5, 12, StatType::HP, { {StatType::CDmg,12}, {StatType::Spd,9}, {StatType::Res,10} } ),

	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 16, StatType::Def, { {StatType::CRate,10}, {StatType::CDmg,10}, {StatType::Acc,18}, {StatType::Spd,8} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::Res,8}, {StatType::Def_p,14}, {StatType::Acc,21}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::Acc,19}, {StatType::Def_p,5}, {StatType::HP_p,16} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::CDmg,10}, {StatType::Res,8}, {StatType::Spd,15} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::HP,176}, {StatType::Spd,14}, {StatType::CRate,12} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::CDmg,15}, {StatType::HP_p,4}, {StatType::Spd,5} } ),
	Artefact( ArtType::Shield, ArtSet::DivCritRate, 5, 12, StatType::Def, { {StatType::Spd,10}, {StatType::HP_p,5}, {StatType::CRate,4} } ),

	Artefact( ArtType::Gloves, ArtSet::DivCritRate, 5, 12, StatType::Atk_p, { {StatType::Def_p,11}, {StatType::Acc,9}, {StatType::CDmg,15} } ),
	Artefact( ArtType::Gloves, ArtSet::DivCritRate, 5, 16, StatType::Def_p, { {StatType::CRate,4}, {StatType::Spd,4,1}, {StatType::Acc,37,2}, {StatType::Def,15,15} }, ChampionName::Mashalled ),
	Artefact( ArtType::Gloves, ArtSet::DivCritRate, 5, 12, StatType::HP_p, { {StatType::CRate,9}, {StatType::Spd,9}, {StatType::Atk_p,6} } ),

	Artefact( ArtType::Chest, ArtSet::DivCritRate, 5, 12, StatType::Def_p, { {StatType::HP_p,5}, {StatType::Def,12,5}, {StatType::Spd,19} } ),

	Artefact( ArtType::Boots, ArtSet::DivCritRate, 6, 16, StatType::Spd, { {StatType::Acc,33,2}, {StatType::Atk,55,12}, {StatType::HP_p,6,3}, {StatType::Atk_p,7,2} }, ChampionName::Mashalled ),
	Artefact( ArtType::Boots, ArtSet::DivCritRate, 5, 12, StatType::Spd, { {StatType::Atk_p,10}, {StatType::HP,789}, {StatType::Def_p,4}, {StatType::Def,36} } ),
	Artefact( ArtType::Boots, ArtSet::DivCritRate, 5, 8, StatType::Atk_p, { {StatType::Acc,17}, {StatType::CDmg,5}, {StatType::HP_p,9} } ),

	// DivLife
	  Artefact( ArtType::Weapon, ArtSet::DivLife, 6, 16, StatType::Atk, { {StatType::HP_p,12,1}, {StatType::Atk_p,24,1}, {StatType::Spd,5}, {StatType::CDmg,6} }, ChampionName::VGalek ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 6, 16, StatType::Atk, { {StatType::HP_p,11}, {StatType::CDmg,6}, {StatType::Atk_p,17}, {StatType::Acc,11} } ),
	  Artefact( ArtType::Weapon, ArtSet::DivLife, 5, 12, StatType::Atk, { {StatType::Spd,4}, {StatType::Acc,21}, {StatType::HP_p,15}, {StatType::HP,239} }, ChampionName::Krisk ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 5, 8, StatType::Atk, { {StatType::Res,10}, {StatType::CDmg,4}, {StatType::Atk_p,14} } ),
	Artefact( ArtType::Weapon, ArtSet::DivLife, 5, 12, StatType::Atk, { {StatType::CDmg,9}, {StatType::Atk_p,10}, {StatType::Acc,18} } ),

	Artefact( ArtType::Helmet, ArtSet::DivLife, 6, 16, StatType::HP, { {StatType::Spd,16}, {StatType::Atk_p,7}, {StatType::CDmg,13}, {StatType::Atk,25} } ),
	Artefact( ArtType::Helmet, ArtSet::DivLife, 6, 12, StatType::HP, { {StatType::CDmg,6}, {StatType::Acc,42}, {StatType::Res,11} } ),
	Artefact( ArtType::Helmet, ArtSet::DivLife, 6, 12, StatType::HP, { {StatType::CRate,14}, {StatType::Spd,5}, {StatType::Res,31} } ),
	  Artefact( ArtType::Helmet, ArtSet::DivLife, 5, 12, StatType::HP, { {StatType::Def_p,5,3}, {StatType::HP_p,11,2}, {StatType::CRate,9}, {StatType::Spd,9,2} }, ChampionName::Lekar ),
	Artefact( ArtType::Helmet, ArtSet::DivLife, 5, 12, StatType::HP, { {StatType::CRate,4}, {StatType::Def_p,14}, {StatType::Acc,18} } ),
	Artefact( ArtType::Helmet, ArtSet::DivLife, 5, 12, StatType::HP, { {StatType::CDmg,11}, {StatType::Spd,5}, {StatType::Acc,25} } ),
	Artefact( ArtType::Helmet, ArtSet::DivLife, 5, 12, StatType::HP, { {StatType::Atk,50}, {StatType::Spd,9}, {StatType::Def_p,5} } ),

	Artefact( ArtType::Shield, ArtSet::DivLife, 6, 12, StatType::Def, { {StatType::HP_p,16,1}, {StatType::Def_p,5,1}, {StatType::HP,800,175}, {StatType::Res,10,3} } ),
	Artefact( ArtType::Shield, ArtSet::DivLife, 5, 12, StatType::Def, { {StatType::CRate,4}, {StatType::Res,19}, {StatType::HP_p,9}, {StatType::Spd,10} } ),
	Artefact( ArtType::Shield, ArtSet::DivLife, 5, 12, StatType::Def, { {StatType::CDmg,4}, {StatType::Spd,13}, {StatType::HP_p,12}, {StatType::HP,256} } ),
	Artefact( ArtType::Shield, ArtSet::DivLife, 5, 12, StatType::Def, { {StatType::HP_p,10}, {StatType::Def_p,14}, {StatType::HP,257} } ),

	  Artefact( ArtType::Gloves, ArtSet::DivLife, 6, 16, StatType::Def_p, { {StatType::HP_p,11,4}, {StatType::Def,27,10}, {StatType::Acc,10,2}, {StatType::CRate,23} }, ChampionName::Krisk ),
	  Artefact( ArtType::Gloves, ArtSet::DivLife, 5, 12, StatType::Def_p, { {StatType::HP,437,150}, {StatType::Acc,9}, {StatType::CRate,21} }, ChampionName::Lekar ),
	Artefact( ArtType::Gloves, ArtSet::DivLife, 5, 12, StatType::Def_p, { {StatType::HP_p,11}, {StatType::Atk,19}, {StatType::Acc,28} } ),
	Artefact( ArtType::Gloves, ArtSet::DivLife, 5, 8, StatType::HP_p, { {StatType::Acc,10}, {StatType::CDmg,14}, {StatType::Atk,13,5} } ),

	Artefact( ArtType::Chest, ArtSet::DivLife, 6, 16, StatType::Def_p, { {StatType::Spd,6,2}, {StatType::Def,29,5}, {StatType::HP_p,14,4}, {StatType::Acc,41,3} }, ChampionName::Sohaty ),
	Artefact( ArtType::Chest, ArtSet::DivLife, 5, 12, StatType::Def_p, { {StatType::Spd,4}, {StatType::HP_p,4}, {StatType::Res,38} } ),

	  Artefact( ArtType::Boots, ArtSet::DivLife, 6, 16, StatType::Spd, { {StatType::Atk,27,10}, {StatType::Atk_p,13,1}, {StatType::Def_p,12,1}, {StatType::Acc,31,2} }, ChampionName::VGalek ),
	Artefact( ArtType::Boots, ArtSet::DivLife, 5, 8, StatType::HP_p, { {StatType::Def_p,10}, {StatType::HP,252}, {StatType::Spd,10} } ),

	// DivSpeed
	Artefact( ArtType::Weapon, ArtSet::DivSpeed, 6, 8, StatType::Atk, { {StatType::HP,226}, {StatType::HP_p,13}, {StatType::Res,10}, {StatType::Acc,20} } ),
	  Artefact( ArtType::Weapon, ArtSet::DivSpeed, 6, 12, StatType::Atk, { {StatType::CRate,6}, {StatType::Spd,16}, {StatType::HP,1062} }, ChampionName::Mavzolejnik ),
	Artefact( ArtType::Weapon, ArtSet::DivSpeed, 6, 12, StatType::Atk, { {StatType::Acc,22}, {StatType::CDmg,16}, {StatType::HP_p,6} } ),
	Artefact( ArtType::Weapon, ArtSet::DivSpeed, 5, 12, StatType::Atk, { {StatType::HP_p,5}, {StatType::CDmg,21}, {StatType::CRate,5} } ),
	  Artefact( ArtType::Weapon, ArtSet::DivSpeed, 5, 16, StatType::Atk, { {StatType::Spd,18,4}, {StatType::HP_p,6,4}, {StatType::Res,9,4}, {StatType::CRate,9} }, ChampionName::Arbitr ),
	Artefact( ArtType::Weapon, ArtSet::DivSpeed, 5, 16, StatType::Atk, { {StatType::Acc,22}, {StatType::CRate,10}, {StatType::HP_p,5}, {StatType::Res,9} } ),

	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 6, 16, StatType::HP, { {StatType::Atk_p,13}, {StatType::HP_p,19}, {StatType::CRate,6}, {StatType::Atk,22} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 6, 12, StatType::HP, { {StatType::CDmg,5}, {StatType::HP_p,16}, {StatType::Res,9} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 5, 12, StatType::HP, { {StatType::Atk,36}, {StatType::Def_p,5}, {StatType::Spd,13} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 5, 12, StatType::HP, { {StatType::Spd,9}, {StatType::Acc,30}, {StatType::Atk_p,6} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 5, 12, StatType::HP, { {StatType::Spd,8}, {StatType::Atk_p,4}, {StatType::CRate,5}, {StatType::Atk,62} } ),
	Artefact( ArtType::Helmet, ArtSet::DivSpeed, 5, 16, StatType::HP, { {StatType::Spd,10}, {StatType::Def,30}, {StatType::CRate,4}, {StatType::Acc,8} } ),

	Artefact( ArtType::Shield, ArtSet::DivSpeed, 6, 16, StatType::Def, { {StatType::Acc,10,2}, {StatType::HP,859}, {StatType::Spd,11,3}, {StatType::CDmg,11} }, ChampionName::Gorgorab ),
	  Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 16, StatType::Def, { {StatType::Acc,20,1}, {StatType::Spd,9,3}, {StatType::Def_p,5,1}, {StatType::CDmg,6} }, ChampionName::Zargala/*Voitelnica*/ ),
	Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 12, StatType::Def, { {StatType::CRate,9}, {StatType::HP,228}, {StatType::Spd,9}, {StatType::Res,19} } ),
	Artefact( ArtType::Shield, ArtSet::DivSpeed, 5, 12, StatType::Def, { {StatType::Def_p,10}, {StatType::CRate,5}, {StatType::Acc,30} } ),

	  Artefact( ArtType::Gloves, ArtSet::DivSpeed, 5, 12, StatType::HP_p, { {StatType::Def_p,11}, {StatType::Def,53}, {StatType::Atk,21} }, ChampionName::Mavzolejnik ),

	  Artefact( ArtType::Chest, ArtSet::DivSpeed, 5, 16, StatType::Def_p, { {StatType::CRate,9}, {StatType::Res,19,5}, {StatType::Spd,10,4}, {StatType::Acc,11,4} }, ChampionName::Arbitr ),
	Artefact( ArtType::Chest, ArtSet::DivSpeed, 5, 12, StatType::Atk_p, { {StatType::CRate,10}, {StatType::Acc,29}, {StatType::Spd,4} } ),

	Artefact( ArtType::Boots, ArtSet::DivSpeed, 6, 16, StatType::Spd, { {StatType::Atk_p,18,1}, {StatType::Res,10,3}, {StatType::CDmg,6}, {StatType::HP,240,275} }, ChampionName::Gorgorab ),
	Artefact( ArtType::Boots, ArtSet::DivSpeed, 5, 16, StatType::Spd, { {StatType::Atk_p,14,2}, {StatType::Atk,15,12}, {StatType::CRate,9} }, ChampionName::Zargala ),

	// Zhivuchest'
	Artefact( ArtType::Weapon, ArtSet::Zhivuchest, 5, 12, StatType::Atk, { {StatType::HP_p,15}, {StatType::CDmg,5}, {StatType::CRate,9} } ),
	Artefact( ArtType::Weapon, ArtSet::Zhivuchest, 5, 12, StatType::Atk, { {StatType::CRate,5}, {StatType::CDmg,15}, {StatType::Acc,11} } ),
	Artefact( ArtType::Weapon, ArtSet::Zhivuchest, 5, 12, StatType::Atk, { {StatType::CDmg,15}, {StatType::CRate,19}, {StatType::Atk_p,5} } ),

	Artefact( ArtType::Helmet, ArtSet::Zhivuchest, 6, 12, StatType::HP, { {StatType::CRate,12}, {StatType::CDmg,11}, {StatType::Def,24}, {StatType::Spd,11} } ),
	Artefact( ArtType::Helmet, ArtSet::Zhivuchest, 6, 12, StatType::HP, { {StatType::Def_p,12}, {StatType::HP_p,14}, {StatType::CRate,6} } ),
	Artefact( ArtType::Helmet, ArtSet::Zhivuchest, 5, 12, StatType::HP, { {StatType::CRate,10}, {StatType::HP_p,4}, {StatType::CDmg,10}, {StatType::Atk_p,11} } ),
	Artefact( ArtType::Helmet, ArtSet::Zhivuchest, 5, 12, StatType::HP, { {StatType::CRate,20}, {StatType::CDmg,5}, {StatType::Def_p,4} } ),

	Artefact( ArtType::Shield, ArtSet::Zhivuchest, 5, 16, StatType::Def, { {StatType::Spd,4}, {StatType::HP_p,14}, {StatType::Acc,31}, {StatType::CDmg,5} } ),
	Artefact( ArtType::Shield, ArtSet::Zhivuchest, 5, 12, StatType::Def, { {StatType::Acc,9}, {StatType::CDmg,15}, {StatType::HP_p,10}, {StatType::CRate,5} } ),
	Artefact( ArtType::Shield, ArtSet::Zhivuchest, 5, 12, StatType::Def, { {StatType::CRate,6}, {StatType::Res,31}, {StatType::Acc,17} } ),
	Artefact( ArtType::Shield, ArtSet::Zhivuchest, 5, 12, StatType::Def, { {StatType::HP_p,9}, {StatType::Res,8}, {StatType::CRate,14} } ),

	Artefact( ArtType::Gloves, ArtSet::Zhivuchest, 6, 12, StatType::HP_p, { {StatType::Atk_p,12}, {StatType::CRate,13}, {StatType::Def,26}, {StatType::Spd,11} } ),
	Artefact( ArtType::Gloves, ArtSet::Zhivuchest, 5, 12, StatType::HP_p, { {StatType::CDmg,5}, {StatType::Def,39}, {StatType::Def_p,6}, {StatType::Spd,12} } ),

	Artefact( ArtType::Boots, ArtSet::Zhivuchest, 5, 12, StatType::Spd, { {StatType::Acc,10}, {StatType::CDmg,15}, {StatType::HP,742} } ),

	// Rastoropnost'
	Artefact( ArtType::Weapon, ArtSet::Rastoropnost, 5, 12, StatType::Atk, { {StatType::Atk_p,5}, {StatType::CDmg,11}, {StatType::CRate,14} } ),
	Artefact( ArtType::Weapon, ArtSet::Rastoropnost, 5, 12, StatType::Atk, { {StatType::CRate,11}, {StatType::Spd,9}, {StatType::HP_p,10} } ),
	Artefact( ArtType::Weapon, ArtSet::Rastoropnost, 5, 16, StatType::Atk, { {StatType::Spd,5}, {StatType::HP_p,14}, {StatType::Res,11}, {StatType::Atk_p,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Rastoropnost, 5, 12, StatType::Atk, { {StatType::CRate,9}, {StatType::Acc,18}, {StatType::HP_p,5} } ),
	Artefact( ArtType::Weapon, ArtSet::Rastoropnost, 5, 12, StatType::Atk, { {StatType::Spd,9}, {StatType::Acc,19}, {StatType::CRate,5} } ),

	  Artefact( ArtType::Helmet, ArtSet::Rastoropnost, 6, 16, StatType::HP, { {StatType::HP_p,6,1}, {StatType::CDmg,12}, {StatType::CRate,20}, {StatType::Acc,10,3} }, ChampionName::Guard ),
	Artefact( ArtType::Helmet, ArtSet::Rastoropnost, 6, 12, StatType::HP, { {StatType::CRate,11}, {StatType::Spd,11}, {StatType::Def_p,6} } ),
	Artefact( ArtType::Helmet, ArtSet::Rastoropnost, 5, 12, StatType::HP, { {StatType::Def_p,9}, {StatType::HP_p,10}, {StatType::Atk_p,11}, {StatType::Atk,18,5} } ),
	Artefact( ArtType::Helmet, ArtSet::Rastoropnost, 5, 12, StatType::HP, { {StatType::Spd,10}, {StatType::HP_p,12}, {StatType::Def_p,9} } ),
	Artefact( ArtType::Helmet, ArtSet::Rastoropnost, 5, 12, StatType::HP, { {StatType::CRate,5}, {StatType::Def_p,17}, {StatType::Atk_p,12} } ),

	  Artefact( ArtType::Shield, ArtSet::Rastoropnost, 6, 16, StatType::Def, { {StatType::Res,22,2}, {StatType::HP_p,13,1}, {StatType::CDmg,11}, {StatType::CRate,12} }, ChampionName::Guard ),
	Artefact( ArtType::Shield, ArtSet::Rastoropnost, 6, 12, StatType::Def, { {StatType::Res,11}, {StatType::Def_p,14}, {StatType::Spd,15}, {StatType::HP_p,6} } ),
	Artefact( ArtType::Shield, ArtSet::Rastoropnost, 5, 12, StatType::Def, { {StatType::CRate,15}, {StatType::Res,9}, {StatType::CDmg,12} } ),
	Artefact( ArtType::Shield, ArtSet::Rastoropnost, 5, 12, StatType::Def, { {StatType::Acc,18}, {StatType::Res,21}, {StatType::CRate,11} } ),
	Artefact( ArtType::Shield, ArtSet::Rastoropnost, 5, 12, StatType::Def, { {StatType::CRate,15}, {StatType::Acc,11}, {StatType::HP_p,6} } ),

	Artefact( ArtType::Gloves, ArtSet::Rastoropnost, 6, 16, StatType::Atk_p, { {StatType::Spd,16}, {StatType::Acc,10}, {StatType::HP_p,5}, {StatType::CRate,6} } ),
	Artefact( ArtType::Gloves, ArtSet::Rastoropnost, 6, 12, StatType::HP_p, { {StatType::CDmg,6}, {StatType::Def_p,17}, {StatType::Spd,6} } ),
	Artefact( ArtType::Gloves, ArtSet::Rastoropnost, 5, 12, StatType::CDmg, { {StatType::Spd,14}, {StatType::Def_p,6}, {StatType::HP_p,4} } ),

	Artefact( ArtType::Boots, ArtSet::Rastoropnost, 5, 12, StatType::Spd, { {StatType::CRate,9}, {StatType::Atk,37}, {StatType::Def_p,6} } ),
	Artefact( ArtType::Boots, ArtSet::Rastoropnost, 4, 12, StatType::Spd, { {StatType::CRate,14}, {StatType::Def_p,5}, {StatType::Res,9} } ),
#endif

	// Баннереты
	Artefact( ArtType::Ring, ArtSet::Bannerets, 5, 8, StatType::HP, { {StatType::Atk,14,5}, {StatType::Atk_p,10}, {StatType::Def_p,10} }, ChampionName::Sohaty ),
	Artefact( ArtType::Necklace, ArtSet::Bannerets, 5, 8, StatType::CDmg, { {StatType::Acc,25}, {StatType::Atk,17,5} }, ChampionName::Sohaty ),

	// Высшие Эльфы
	Artefact( ArtType::Ring, ArtSet::SupremeElves, 5, 12, StatType::HP, { {StatType::Def_p,10,3}, {StatType::HP_p,10,2}, {StatType::Atk_p,6,2} }, ChampionName::Arbitr ),
	Artefact{ ArtType::Ring, ArtSet::SupremeElves, 5, 16, StatType::Def, { {StatType::Def_p,10,1}, {StatType::Atk_p,9,1}, {StatType::HP_p,5,1}, {StatType::Atk,21,5} }, ChampionName::Tyrel },
	Artefact{ ArtType::Ring, ArtSet::SupremeElves, 5, 12, StatType::Def, { {StatType::Def_p,5,1}, {StatType::Atk_p,16,1}, {StatType::HP_p,9,1} }, ChampionName::Guard },
	Artefact{ ArtType::Ring, ArtSet::SupremeElves, 5, 12, StatType::Def, { {StatType::HP_p,5,3}, {StatType::Def_p,14,3}, {StatType::HP,222,225} }, ChampionName::Lekar },
	Artefact{ ArtType::Necklace, ArtSet::SupremeElves, 5, 12, StatType::CDmg, { {StatType::HP,387,100}, {StatType::Res,31,2}, {StatType::Acc,9,2} }, ChampionName::Tyrel },
	Artefact( ArtType::Necklace, ArtSet::SupremeElves, 5, 12, StatType::CDmg, { {StatType::Res,11,}, {StatType::Acc,31,4}, {StatType::Def,18,15} }, ChampionName::Guard ),
	Artefact( ArtType::Necklace, ArtSet::SupremeElves, 5, 12, StatType::Def, { {StatType::Acc,20}, {StatType::HP,596}, {StatType::Atk,15,5}, {StatType::CDmg,4} }, ChampionName::Lekar ),
	//Artefact{ ArtType::Necklace, ArtSet::SupremeElves, 5, 8, StatType::HP, { {StatType::Acc,8}, {StatType::Atk,50}, {StatType::CDmg,5}, {StatType::Def,15} }, ChampionName::Tyrel },
	Artefact( ArtType::Necklace, ArtSet::SupremeElves, 5, 12, StatType::HP, { {StatType::Acc,27,5}, {StatType::Res,8,3}, {StatType::HP,281,225} }, ChampionName::Arbitr ),
	//Artefact{ ArtType::Banner, ArtSet::SupremeElves, 5, 12, StatType::HP, { {StatType::HP_p,5,1}, {StatType::Def_p,5,1}, {StatType::Atk,53} }, ChampionName:: },
	Artefact{ ArtType::Banner, ArtSet::SupremeElves, 5, 16, StatType::Acc, { {StatType::Spd,9,1}, {StatType::Def_p,8,3}, {StatType::Atk_p,5,2}, {StatType::HP_p,4,2} }, ChampionName::Tyrel },
	Artefact( ArtType::Banner, ArtSet::SupremeElves, 5, 12, StatType::Acc, { {StatType::Spd,9,1}, {StatType::Atk_p,11,3}, {StatType::Def_p,4,3} }, ChampionName::Guard ),
	Artefact( ArtType::Banner, ArtSet::SupremeElves, 5, 12, StatType::HP, { {StatType::Def_p,10,2}, {StatType::Spd,8,1}, {StatType::Atk_p,6,1} }, ChampionName::Lekar ),
	Artefact( ArtType::Banner, ArtSet::SupremeElves, 5, 12, StatType::Def, { {StatType::Spd,10,4}, {StatType::Def_p,9,4}, {StatType::HP,289,175} }, ChampionName::Arbitr ),

	// Священный орден
	Artefact( ArtType::Ring, ArtSet::HolyOrden, 5, 12, StatType::HP, { {StatType::Def,46,5}, {StatType::Def_p,5,1}, {StatType::HP_p,5,1} }, ChampionName::Gromoboy ),
	Artefact{ ArtType::Ring, ArtSet::HolyOrden, 5, 16, StatType::HP, { {StatType::Def_p,11,2}, {StatType::HP_p,5,2}, {StatType::Atk_p,5,2}, {StatType::Def,17,10} }, ChampionName::Yuliana },
	Artefact( ArtType::Necklace, ArtSet::HolyOrden, 5, 8, StatType::CDmg, { {StatType::HP,673,150}, {StatType::Acc,19,2} }, ChampionName::Gromoboy ),
	Artefact( ArtType::Necklace, ArtSet::HolyOrden, 5, 12, StatType::CDmg, { {StatType::Def,32,6}, {StatType::Acc,18,5}, {StatType::Res,11,1} }, ChampionName::Yuliana ),
	//Artefact( ArtType::Necklace, ArtSet::HolyOrden, 4, 8, StatType::CDmg, { {StatType::HP,372,150}, {StatType::Acc,16,2} }, ChampionName:: ),
	Artefact( ArtType::Banner, ArtSet::HolyOrden, 4, 12, StatType::Acc, { {StatType::HP_p,12,1}, {StatType::HP,232}, {StatType::Def_p,4,1} }, ChampionName::Gromoboy ),
	Artefact( ArtType::Banner, ArtSet::HolyOrden, 5, 14, StatType::Acc, { {StatType::Spd,8,1}, {StatType::Def_p,10,1}, {StatType::Def,23} }, ChampionName::Yuliana ),

	// Варвары
	//Artefact{ ArtType::Ring, ArtSet::Barbarians, 5, 8, StatType::HP, { {StatType::Atk_p,8,1}, {StatType::Def_p,9,1} }, ChampionName:: },
	Artefact{ ArtType::Ring, ArtSet::Barbarians, 5, 12, StatType::HP, { {StatType::HP_p,10,2}, {StatType::Atk_p,6,2}, {StatType::Def_p,15,2} }, ChampionName::Zargala },
	Artefact{ ArtType::Necklace, ArtSet::Barbarians, 5, 8, StatType::Atk, { {StatType::Res,8,2}, {StatType::Acc,28}, {StatType::HP,172,100} }, ChampionName::Zargala },
	Artefact( ArtType::Banner, ArtSet::Barbarians, 5, 12, StatType::Def, { {StatType::Spd,9,3}, {StatType::Atk_p,10,2}, {StatType::HP,280,150} }, ChampionName::Zargala ),

	// Ящеролюды
	Artefact{ ArtType::Ring, ArtSet::Snakes, 5, 12, StatType::Def, { {StatType::HP_p,5,3}, {StatType::HP,583,150}, {StatType::Def_p,14,4}, {StatType::Atk,12,5} }, ChampionName::Krisk },
	//Artefact( ArtType::Ring, ArtSet::Snakes, 5, 12, StatType::Def, { {StatType::Def_p,15,2}, {StatType::Atk,14,15}, {StatType::HP,292,275} } ),
	Artefact{ ArtType::Necklace, ArtSet::Snakes, 5, 12, StatType::HP, { {StatType::CDmg,11}, {StatType::Atk,32}, {StatType::Acc,21,3} }, ChampionName::Krisk },
	Artefact{ ArtType::Banner, ArtSet::Snakes, 6, 12, StatType::HP, { {StatType::HP_p,6,1}, {StatType::Def_p,17,1}, {StatType::Atk,15,5} }, ChampionName::Krisk },

	// Оборотни
	Artefact( ArtType::Ring, ArtSet::Werewolfs, 6, 8, StatType::Def, { {StatType::Atk_p,5}, {StatType::HP_p,16} }, ChampionName::Fein ),
	Artefact( ArtType::Ring, ArtSet::Werewolfs, 5, 12, StatType::Def, { {StatType::Def_p,12,3}, {StatType::HP_p,10,4}, {StatType::Atk_p,4,2} }, ChampionName::SteelSkull ),
	//Artefact( ArtType::Ring, ArtSet::Werewolfs, 5, 8, StatType::Def, { {StatType::Atk_p,10}, {StatType::Atk,21}, {StatType::HP_p,8,1} } ),
	Artefact( ArtType::Necklace, ArtSet::Werewolfs, 5, 12, StatType::Def, { {StatType::Acc,19,2}, {StatType::CDmg,11}, {StatType::HP,505,225} }, ChampionName::SteelSkull ),
	Artefact( ArtType::Necklace, ArtSet::Werewolfs, 5, 12, StatType::Def, { {StatType::Acc,24}, {StatType::Res,20}, {StatType::CDmg,5} }, ChampionName::Fein ),
	Artefact( ArtType::Banner, ArtSet::Werewolfs, 4, 16, StatType::Acc, { {StatType::Atk_p,3,1}, {StatType::HP_p,12,1}, {StatType::Def_p,3,1} }, ChampionName::SteelSkull ),
	Artefact( ArtType::Banner, ArtSet::Werewolfs, 5, 12, StatType::Acc, { {StatType::Def,30}, {StatType::Atk_p,11}, {StatType::Def_p,11} }, ChampionName::Fein ),
	//Artefact( ArtType::Banner, ArtSet::Werewolfs, 5, 12, StatType::Res, { {StatType::HP_p,15,1}, {StatType::Spd,4+1}, {StatType::Atk_p,4,1}, {StatType::Def_p,10,1} } ),

	// Орки
	Artefact{ ArtType::Ring, ArtSet::Orks, 5, 8, StatType::HP, { {StatType::Atk_p,8,1}, {StatType::HP_p,9,1} }, ChampionName::VGalek },
	Artefact{ ArtType::Ring, ArtSet::Orks, 4, 8, StatType::HP, { {StatType::Atk,14,5}, {StatType::Def,19}, {StatType::HP_p,8}, {StatType::Atk_p,4} }, ChampionName::Hangar },
	Artefact{ ArtType::Necklace, ArtSet::Orks, 5, 8, StatType::Def, { {StatType::Acc,28}, {StatType::CDmg,6}, {StatType::Atk,17}, {StatType::HP,316} }, ChampionName::VGalek },
	Artefact{ ArtType::Necklace, ArtSet::Orks, 5, 8, StatType::HP, { {StatType::CDmg,14}, {StatType::Atk,13,5} }, ChampionName::Hangar },
	Artefact{ ArtType::Banner, ArtSet::Orks, 5, 12, StatType::Acc, { {StatType::HP,687}, {StatType::Spd,5}, {StatType::Atk,35}, {StatType::Def_p,11} }, ChampionName::VGalek },
	Artefact{ ArtType::Banner, ArtSet::Orks, 5, 8, StatType::Acc, { {StatType::HP,646}, {StatType::HP_p,9} }, ChampionName::Hangar },

	// Deads
	Artefact( ArtType::Ring, ArtSet::Deads, 5, 16, StatType::HP, { {StatType::Atk_p,10,3}, {StatType::HP_p,9,3}, {StatType::Def_p,6,3}, {StatType::Def,17,17} }, ChampionName::Rotos ),
	Artefact( ArtType::Ring, ArtSet::Deads, 5, 12, StatType::Atk, { {StatType::Def_p,12,3}, {StatType::Atk_p,6,3}, {StatType::HP_p,14,3} }, ChampionName::Mashalled ),
	Artefact( ArtType::Ring, ArtSet::Deads, 5, 12, StatType::Def, { {StatType::Atk,41,7}, {StatType::Def_p,4,2}, {StatType::HP_p,14,2} }, ChampionName::Gorgorab ),
	Artefact( ArtType::Ring, ArtSet::Deads, 5, 12, StatType::Def, { {StatType::HP_p,4,1}, {StatType::Atk_p,5,1}, {StatType::HP,1055} }, ChampionName::Mavzolejnik ),
	//Artefact( ArtType::Ring, ArtSet::Deads, 4, 12, StatType::Def, { {StatType::HP_p,13,1}, {StatType::Atk_p,4,1}, {StatType::Def_p,4,1} } ),
	Artefact( ArtType::Necklace, ArtSet::Deads, 5, 12, StatType::CDmg, { {StatType::Res,18,2}, {StatType::Acc,28,2}, {StatType::Def,14,5} }, ChampionName::Mashalled ),
	Artefact( ArtType::Necklace, ArtSet::Deads, 5, 12, StatType::CDmg, { {StatType::Res,32,5}, {StatType::Acc,10,5}, {StatType::Atk,13,15} }, ChampionName::Rotos ),
	Artefact( ArtType::Necklace, ArtSet::Deads, 4, 12, StatType::HP, { {StatType::Acc,24,1}, {StatType::Res,15,2}, {StatType::CDmg,4} }, ChampionName::Gorgorab ),
	Artefact( ArtType::Banner, ArtSet::Deads, 5, 16, StatType::Res, { {StatType::Atk_p,8,3}, {StatType::HP_p,10,2}, {StatType::Def_p,6,2}, {StatType::HP,261,100} }, ChampionName::Rotos ),
	Artefact( ArtType::Banner, ArtSet::Deads, 5, 16, StatType::Def, { {StatType::Spd,9,4}, {StatType::HP_p,11,3}, {StatType::Def_p,5,3}, {StatType::HP,445,275} }, ChampionName::Gorgorab ),
	Artefact( ArtType::Banner, ArtSet::Deads, 5, 12, StatType::Acc, { {StatType::HP_p,14}, {StatType::Def_p,5}, {StatType::Spd,4} }, ChampionName::Mashalled ),
	Artefact( ArtType::Banner, ArtSet::Deads, 4, 12, StatType::Def, { {StatType::Spd,11,1}, {StatType::HP,252}, {StatType::HP_p,7,1} }, ChampionName::Mavzolejnik ),

	// Темные Эльфы
	Artefact{ ArtType::Ring, ArtSet::DarkElves, 5, 16, StatType::HP, { {StatType::Atk_p,16,3}, {StatType::HP_p,6,3}, {StatType::Def_p,4,2}, {StatType::Atk,20,15} }, ChampionName::ColdHeart },
	Artefact{ ArtType::Ring, ArtSet::DarkElves, 5, 12, StatType::HP, { {StatType::HP_p,5,3}, {StatType::Def_p,13,2}, {StatType::Atk,17,12} }, ChampionName::VisirOvelis },
	Artefact{ ArtType::Ring, ArtSet::DarkElves, 5, 12, StatType::HP, { {StatType::Def_p,17,3}, {StatType::Atk_p,5,3}, {StatType::Atk_p,4,3} }, ChampionName::Foly },
	//Artefact{ ArtType::Ring, ArtSet::DarkElves, 5, 16, StatType::Def, { {StatType::Def_p,10,1}, {StatType::Atk_p,9,1}, {StatType::HP_p,5,1}, {StatType::Atk,21} } },
	  Artefact{ ArtType::Necklace, ArtSet::DarkElves, 6, 8, StatType::Atk, { {StatType::Acc,33,2}, {StatType::Def,24,10}, {StatType::HP,381,150} }, ChampionName::VisirOvelis },
	Artefact{ ArtType::Necklace, ArtSet::DarkElves, 5, 12, StatType::CDmg, { {StatType::Def,44}, {StatType::Acc,19,3}, {StatType::HP,433,275} }, ChampionName::ColdHeart },
	Artefact( ArtType::Necklace, ArtSet::DarkElves, 6, 12, StatType::CDmg, { {StatType::Acc,22,3}, {StatType::Atk,35,12}, {StatType::Res,10,2} }, ChampionName::Foly ),
	//Artefact{ ArtType::Necklace, ArtSet::DarkElves, 5, 8, StatType::CDmg, { {StatType::Res,19}, {StatType::Atk,22}, {StatType::Def,33} } },
	Artefact{ ArtType::Banner, ArtSet::DarkElves, 5, 16, StatType::Acc, { {StatType::Atk_p,9,3}, {StatType::Def_p,9,2}, {StatType::Spd,5+1}, {StatType::Atk,20,15} }, ChampionName::Foly },
	Artefact{ ArtType::Banner, ArtSet::DarkElves, 5, 16, StatType::Acc, { {StatType::Def,38,10}, {StatType::HP_p,15,2}, {StatType::Spd,5+1}, {StatType::Def_p,5,3} }, ChampionName::VisirOvelis },
	Artefact{ ArtType::Banner, ArtSet::DarkElves, 5, 16, StatType::Acc, { {StatType::HP,183,275}, {StatType::Spd,10,1}, {StatType::HP_p,15,3}, {StatType::Def,19,15} }, ChampionName::ColdHeart },
	//Artefact{ ArtType::Banner, ArtSet::None, 4, 8, StatType::Acc, { {StatType::HP,394}, {StatType::HP_p,9}, {StatType::Def_p,3} } },

	// Демоны
	Artefact{ ArtType::Ring, ArtSet::Demons, 5, 12, StatType::Def, { {StatType::Def_p,6,1}, {StatType::Atk_p,16,3}, {StatType::Atk,19,12} }, ChampionName::Alura },
	Artefact{ ArtType::Necklace, ArtSet::Demons, 5, 12, StatType::CDmg, { {StatType::Atk,34,9}, {StatType::Acc,17,2}, {StatType::Res,9,2} }, ChampionName::Alura },
	Artefact{ ArtType::Banner, ArtSet::Demons, 6, 12, StatType::Acc, { {StatType::HP_p,17,1}, {StatType::Atk_p,6,2}, {StatType::Def_p,6,2} }, ChampionName::Alura },

	// Отступники
	Artefact{ ArtType::Ring, ArtSet::Renegates, 6, 8, StatType::HP, { {StatType::Atk_p,5,1}, {StatType::Def_p,18,1} }, ChampionName::Straholud },
	Artefact{ ArtType::Necklace, ArtSet::Renegates, 5, 12, StatType::HP, { {StatType::CDmg,4}, {StatType::Atk,49}, {StatType::Acc,19,1} }, ChampionName::Straholud },
	Artefact{ ArtType::Banner, ArtSet::Renegates, 6, 12, StatType::Def, { {StatType::Def_p,7,1}, {StatType::HP,916}, {StatType::Spd,15,1} }, ChampionName::Straholud },

	// Rings
	Artefact{ ArtType::Ring, ArtSet::None, 4, 8, StatType::Def, { {StatType::HP_p,7,1}, {StatType::Def_p,2,1}, {StatType::Atk_p,7,1} }, ChampionName::Kael },	//actually 3-12
	Artefact{ ArtType::Ring, ArtSet::None, 5, 8, StatType::HP, { {StatType::Atk_p,16,1}, {StatType::HP_p,5,1} }, ChampionName::Voitelnica },

	// Necklace
	Artefact{ ArtType::Necklace, ArtSet::None, 4, 8, StatType::Atk, { {StatType::Acc,16,1}, {StatType::Def,17,5} }, ChampionName::Kael },
	//Artefact{ ArtType::Necklace, ArtSet::None, 5, 8, StatType::Def, { {StatType::HP,138}, {StatType::Acc,29} }, ChampionName::Voitelnica },
	Artefact{ ArtType::Necklace, ArtSet::None, 4, 12, StatType::Atk, { {StatType::Acc,24}, {StatType::HP,522}, {StatType::CDmg,5} }, ChampionName::Voitelnica },

	// Banner
	Artefact{ ArtType::Banner, ArtSet::None, 5, 8, StatType::Acc, { {StatType::HP_p,12,1}, {StatType::Def,39,5} }, ChampionName::Kael },
};

/////////////////////////////////////////////////////////////////////////////
