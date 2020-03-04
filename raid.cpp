#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////

void ApplyBonus( ArtSet set, Champion ch )
{
}

/////////////////////////////////////////////////////////////////////////////

Champion ChampionFactory::Gromoboy()
{
	return Champion({ 15855, 727, 1443,  97,  15, 50,  30, 0 });
}
