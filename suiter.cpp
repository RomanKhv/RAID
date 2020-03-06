#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

void FindBestEquipment( Champion& ch, const MatchOptions& matching, Equipment& best_eq )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	for ( const Artefact& art : _MyArts )
	{
		assert( art.Type != ArtType::None );
		arts_by_type[art.Type].push_back( art );
	}

	Equipment eq;
	for ( const auto& at : arts_by_type )
	{
		for ( const Artefact& art : at.second )
		{
			eq[at.first] = art;
		}
	}
}
