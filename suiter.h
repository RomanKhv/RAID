#pragma once
#include <boost/circular_buffer.hpp>
#include "raid.h"

/////////////////////////////////////////////////////////////////////////////

struct MatchOptions
{
	enum class StatInfluence {
		NotInterested = 0,
		Minor = 1,
		Modrt = 2,
		Major = 3,
		Max = 4,
	};
	struct StatFactor
	{
		StatInfluence Mode = StatInfluence::NotInterested;
		int MinCap = 0;
		int MaxCap = 0;
		StatFactor() = default;
		StatFactor( StatInfluence m, int max_cap = 0 ) : Mode(m), MinCap(0), MaxCap(max_cap) {}
		StatFactor( int min_cap, StatInfluence excessK = StatInfluence::NotInterested, int max_cap = 0 ) : Mode(excessK), MinCap(min_cap), MaxCap(max_cap) {}
		bool IgnoreStat() const { return Mode == StatInfluence::NotInterested && !HasMinCap(); }
		bool HasMinCap() const { return MinCap > 0; }
		bool HasMaxCap() const { return MaxCap > 0; }
	};
	StatFactor Factors[ChampionStats::Count];		// StatType -> StatFactorMode

	std::map<ArtSet, int> RequiedSets;
	enum_index_map<ArtSet,ArtSet::count,bool> ExcludedSets;

	static const bool ConsiderMaxLevels = true;
	static const bool ConsiderMaxLevelsForNonBasicArts = false;	//no data, so far too expensive
	//static const bool OptimizeAddStatFlatBonus = true;

	MatchOptions() = default;
	MatchOptions( std::map<StatType, StatFactor>,
				  std::vector<ArtSet> req_filter = {},
				  std::set<ArtSet> exclusion_filter = {} );

	bool IsInputOK() const;
	const StatFactor& Factor( StatType st ) const
	{
		return Factors[stl::enum_to_int( st )];
	}

	bool IsSetAccepted( ArtSet ) const;
	bool IsArtAccepted( const Artefact&, ChampionName ) const;
	bool IsEqHasRequiredSets( const EquipmentRef& ) const;
};

/////////////////////////////////////////////////////////////////////////////

bool EstimateMinCap( int value, int ref_value, int width, float& est );
float EstimateEquipment( const ChampionStats&, const MatchOptions& );
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

