#include "pch.h"
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

float EstimateEquipment( const Champion& ch, const MatchOptions& matching )
{
	return 0;
}

void FindBestEquipment( Champion& ch, const MatchOptions& matching, Equipment& best_eq )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	for ( const Artefact& art : _MyArts )
	{
		assert( art.Type != ArtType::None );
		arts_by_type[art.Type].push_back( art );
	}

	Equipment eq;
	float best_eq_estimation = 0;
	for ( const auto& at : arts_by_type )
	{
		for ( const Artefact& art : at.second )
		{
			eq[at.first] = art;
			ApplyEquipment( eq, ch );

			const float est = EstimateEquipment( ch, matching );
			if ( est > best_eq_estimation )
			{
				best_eq = eq;
				best_eq_estimation = est;
			}
		}
	}
}
