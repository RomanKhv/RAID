#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/optional.hpp>
#include "raid.h"

#define USE_TIERS
#define USE_TARGET_VALUES_AS_REF

#ifdef _DEBUG
#define SUITUP_SPEED_INTERVAL
#endif

/////////////////////////////////////////////////////////////////////////////

enum class ArtTier {
	T1,		// 6lvl
	T2,		// ...
	//filters
	filter_T1Only,
};

struct MatchOptions
{
	enum class StatInfluence {
		NotInterested = 0,
		Minor = 1,
		Modrt = 2,
		Major = 3,
		Max = 4,
		StrictInterval,
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
	enum_index_map<ArtType,ArtType::NBasic,boost::optional<StatType>> StatOnArt;
	const ArtTier ArtTierCap = ArtTier::T1;
	enum_index_map<ChampionName,ChampionName::count,bool> Undressable;

	static const bool ConsiderMaxLevels = true;
	static const bool ConsiderMaxLevelsForNonBasicArts = false;	//no data, so far too expensive
	//static const bool OptimizeAddStatFlatBonus = true;

	MatchOptions() = default;
	MatchOptions( std::map<StatType, StatFactor>,
				  std::vector<ArtSet> req_filter = {},
				  //std::set<ArtSet> exclusion_filter = {},
				  std::set<ChampionName> providers = {},
				  ArtTier art_tier_cap = ArtTier::T1 );
	void AllowSets( std::set<ArtSet> );
	void ForbiddenSets( std::set<ArtSet> );
	void RequireSpeedBoots(bool);

	bool IsInputOK() const;
	const StatFactor& Factor( StatType st ) const
	{
		return Factors[stl::enum_to_int( st )];
	}

	bool IsSetAccepted( ArtSet ) const;
	bool IsArtAccepted( const Artefact&, ChampionName ) const;
	bool IsEqHasRequiredSets( const EquipmentRef& ) const;
	bool AreSetsRestrictedToRequired() const;

private:
	bool IsArtAcceptedByTier( const Artefact& ) const;
};

/////////////////////////////////////////////////////////////////////////////

inline bool IsFlatArtStat( StatType t )
{
	BOOST_STATIC_ASSERT( stl::enum_to_int(StatType::HP)  == 0 );
	BOOST_STATIC_ASSERT( stl::enum_to_int(StatType::Atk) == 1 );
	BOOST_STATIC_ASSERT( stl::enum_to_int(StatType::Def) == 2 );
	return stl::enum_to_int(t) <= stl::enum_to_int(StatType::Def);
}

ArtTier GetArtTier( const Artefact& );

bool EstimateMinCap( int value, int ref_value, int width, float& est );
float EstimateEquipment( const ChampionStats&, const MatchOptions& );
void FindBestEquipment( const std::vector<Artefact>&, const Champion&, const MatchOptions&, Equipment& );
void FindBestEquipment2( const std::vector<Artefact>&, const Champion&, const MatchOptions&, std::vector<Equipment>& );
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

/////////////////////////////////////////////////////////////////////////////

struct SuitUp
{
	static const bool ConsiderGlyphsInReport = false;
};