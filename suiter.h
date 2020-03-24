#pragma once
#include <boost/circular_buffer.hpp>
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

struct MatchOptions
{
	enum class ArtFactor {
		NotInterested = 0,
		Minor = 1,
		Moderate = 2,
		Magor = 3,
		Max = 4,
		MinCap,
	};
	ArtFactor Factors[ChampionStats::Count] = { ArtFactor::NotInterested };		// StatType -> ArtFactor

	std::vector<ArtSet> RequiedSets;
	std::set<ArtSet> SetFilter;

	static const bool ConsiderMaxLevels = true;
	int MinCap[ChampionStats::Count] = { 0 };				//StatType -> value
	int MaxCap[ChampionStats::Count] = { 0 };				//StatType -> value
//TODO:	StatType RequiredArtStats[Equipment::TotalSize] = {0};	//ArtType -> StatType

	MatchOptions() = default;
	MatchOptions( std::map<StatType, ArtFactor>,
				  std::vector<ArtSet> req_filter = {},
				  std::set<ArtSet> set_filter = {},
				  bool consider_max_lvl = true,
				  std::map<StatType, int> min_caps = {},
				  std::map<StatType, int> max_caps = {} );

	ArtFactor Factor( StatType st ) const
	{
		return Factors[stl::enum_to_int( st )];
	}
	bool IsSetAccepted( ArtSet ) const;
	bool IsArtAccepted( const Artefact&, ChampionName ) const;
	bool IsEqHasRequiredSets( const EquipmentRef& ) const;
};

/////////////////////////////////////////////////////////////////////////////

bool EstimateMinCap( int value, int ref_value, int width, float& f );
void FindBestEquipment( const std::vector<Artefact>&, const Champion&, const MatchOptions&, Equipment& );
void FindBestEquipment2( const std::vector<Artefact>&, const Champion&, const MatchOptions&, std::vector<Equipment>& );
Equipment FindRealBestEquipment( ChampionExt&, const MatchOptions& );
void FindRealBestEquipment2( const Champion&, const MatchOptions&, std::vector<Equipment>& );

/////////////////////////////////////////////////////////////////////////////

struct EqEst
{
	float _Est = 0;
	EquipmentRef _Eq;

	EqEst() = default;
	EqEst( float e, const EquipmentRef& eq ) : _Est(e), _Eq(eq) {}

	bool Initialized() const { return _Est > 0; }

	void _join( const EqEst& rhs )
	{
		if ( rhs._Est > _Est )
		{
			*this = rhs;
		}
	}
};

struct EqEstPool
{
	boost::circular_buffer<EqEst> _Arr;
	static const size_t DefaultSize = 5;

	EqEstPool();
	explicit EqEstPool(size_t custom_buffer_size);

	void join( const EqEst& );
	void join( const EqEstPool& );
};

