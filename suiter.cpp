#include "pch.h"
#include "suiter.h"
#include "iterator.h"
#include "iterator2.h"
#include "stl_ext.h"
#include "profiler.h"
#include "to_string.h"

#if !defined DEBUG_FIND_BEST && !defined _DEBUG
	#define USE_TBB
#endif
#ifdef USE_TBB
struct IUnknown; // Workaround for "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#endif

#define WEAK_EXCESS_LIMIT

/////////////////////////////////////////////////////////////////////////////

float linerp( const int x, const int x1, const float y1, const int x2, const float y2 )
{
	const float a = float(y2 - y1) / (x2 - x1);
	const float b = float(x2*y1 - x1*y2) / (x2 - x1);
	return a * x + b;
}

/////////////////////////////////////////////////////////////////////////////

const ChampionStats::values_t Ref_Stat_Values = {
	/*StatType::HP*/ 35000,
	/*StatType::Atk*/ 3000,
	/*StatType::Def*/ 3000,
	/*StatType::Spd*/ 170,
	/*StatType::CRate*/ 100,
	/*StatType::CDmg*/ 150,
	/*StatType::Res*/ 100,
	/*StatType::Acc*/ 150,
};

const ChampionStats::values_t Def_Max_Stat_Caps = {	// 0 - no cap/no penalty
	/*StatType::HP*/   0,
	/*StatType::Atk*/  0,
	/*StatType::Def*/  0,
	/*StatType::Spd*/  0,
	/*StatType::CRate*/ 100,
	/*StatType::CDmg*/ 0,
	/*StatType::Res*/  0,
	/*StatType::Acc*/  0,
};

#ifdef WEAK_EXCESS_LIMIT

const ChampionStats::values_t Min_Cap_Tol = {	// 0 - no cap/no penalty
	/*StatType::HP*/   2000,
	/*StatType::Atk*/  300,
	/*StatType::Def*/  300,
	/*StatType::Spd*/  20,
	/*StatType::CRate*/ 10,
	/*StatType::CDmg*/ 20,
	/*StatType::Res*/  20,
	/*StatType::Acc*/  10,
};

#else

const ChampionStats::values_t Excess_Tolerance = {	//weight: 1 -> (tol width) -> 0
	/*StatType::HP*/   Ref_Stat_Values[ stl::enum_to_int(StatType::HP) ] * 40 / 100,
	/*StatType::Atk*/  1500,
	/*StatType::Def*/  1500,
	/*StatType::Spd*/  40,
	/*StatType::CRate*/ 50,
	/*StatType::CDmg*/ 40,
	/*StatType::Res*/  Ref_Stat_Values[ stl::enum_to_int(StatType::Res) ] * 40 / 100,
	/*StatType::Acc*/  20,
};

#endif

const float fK_ignore = 0;
const float fK_minor = 0.25f;
const float fK_moderate = 0.5f;
const float fK_major = 0.75f;
const float fK_max = 1;

inline float FactorK( MatchOptions::StatInfluence f )
{
	static const float fks[5] = {
		fK_ignore, fK_minor, fK_moderate, fK_major, fK_max
	};
	return fks[ stl::enum_to_int(f) ];
}

bool FloatEstimationFactor( MatchOptions::StatInfluence f, float& fk )
{
	if ( f >= MatchOptions::StatInfluence::Minor && f <= MatchOptions::StatInfluence::Max )
	{
		fk = FactorK( f );
		return true;
	}
	else
		return false;
}

bool EstimateMinCap( const int value, const int ref_value, const int width, float& e )
{
	if ( value <= (ref_value - width/2) )
	{
		e = fK_ignore;
		return false;
	}
	if ( value < ref_value )
		e = linerp( value, ref_value - width / 2, fK_ignore, ref_value, fK_max );
	else
#ifndef WEAK_EXCESS_LIMIT
		e = fK_max;
#else
	if ( value < (ref_value + width) )
		e = fK_max;
	else
	if ( value < (ref_value + 3*width/2) )
		e = linerp( value, ref_value+width, fK_max, ref_value+3*width/2, fK_moderate );
	else
		e = fK_moderate;
#endif
	return true;
}

int GetActualMaxCap( StatType st, const MatchOptions::StatFactor& f )
{
	const int global_max_stat_cap = Def_Max_Stat_Caps[stl::enum_to_int( st )];
	const int champ_max_stat_cap = f.MaxCap;
	if ( champ_max_stat_cap || global_max_stat_cap )
	{
		if ( champ_max_stat_cap && global_max_stat_cap )
		{
			_ASSERTE( champ_max_stat_cap < global_max_stat_cap );
			return std::min( champ_max_stat_cap, global_max_stat_cap );
		}
		else {
			if ( champ_max_stat_cap )
			{
				_ASSERTE( !global_max_stat_cap );
				return champ_max_stat_cap;
			}
			else {
				return global_max_stat_cap;
			}
		}
	}
	else
		return 0;
}

/*		Max Cap
Weight
1---------------|\
                |   \
                |      \
                |         \
0---------------|-----------|------------- stat value
              ref    tol       cut off
*/

float EstimateEquipment( const ChampionStats& ch_stats, const MatchOptions& matching )
{
	float total_est = 0;

#ifdef WEAK_EXCESS_LIMIT
	static const ChampionStats::StatList optimized_stat_list = {
		StatType::Spd, StatType::Acc, StatType::CRate,								// mincap stats first
		StatType::Def, StatType::HP, StatType::CDmg, StatType::Atk, StatType::Res
	};
	for ( StatType st : optimized_stat_list )
	{
		const MatchOptions::StatFactor f = matching.Factor( st );
		if ( f.IgnoreStat() )
			continue;

		int stat_value = ch_stats[st];
		const int max_stat_cap = GetActualMaxCap( st, f );
		if ( max_stat_cap && stat_value > max_stat_cap )
		{
			stat_value = max_stat_cap;
		}

		float e = 0;
		bool e_calculated = false;

		if ( f.HasMinCap() )
		{
			const int abs_min = f.MinCap - Min_Cap_Tol[stl::enum_to_int( st )] ;
			if ( stat_value <= abs_min )
				return 0.f;	//too small => not accepted

#ifndef USE_TARGET_VALUES_AS_REF
			const int ref_stat_value = Ref_Stat_Values[stl::enum_to_int( st )];
#endif
			if ( stat_value < f.MinCap )
			{
#ifdef USE_TARGET_VALUES_AS_REF
				e = linerp( stat_value, abs_min, 0.f, f.MinCap, 1 );
#else
				const float e_min = (float)f.MinCap / ref_stat_value;
				e = linerp( stat_value, abs_min, 0.f, f.MinCap, e_min );
#endif
				total_est += e;
				continue;
			}
			else {
				const float fk = FactorK( f.Mode );
#ifdef USE_TARGET_VALUES_AS_REF
				e = (f.MinCap + fk * (stat_value - f.MinCap)) / f.MinCap;
#else
				e = (f.MinCap + fk * (stat_value - f.MinCap)) / ref_stat_value;
#endif
				e_calculated = true;
			}
		}

		if ( !e_calculated )
		{
#ifdef USE_TARGET_VALUES_AS_REF
			const int ref_stat_value = f.HasMinCap() ? f.MinCap : Ref_Stat_Values[stl::enum_to_int( st )];
#else
			const int ref_stat_value = Ref_Stat_Values[stl::enum_to_int( st )];
#endif
			const float fk = FactorK( f.Mode );
			e = fk * (float)stat_value / ref_stat_value;
			e_calculated = true;
		}

		_ASSERTE( e_calculated );
		total_est += e;
	}
#else
	// MinCap is most strong filter => so test it firstly
	for ( StatType st : ChampionStats::TypeList )
	{
		const int min_stat_cap = matching.Factor(st).MinCap;
		if ( min_stat_cap <= 0 )
			continue;	//not requested

		float e = 0;
		const int tol = Excess_Tolerance[stl::enum_to_int( st )] / 4;
		if ( !EstimateMinCap( ch_stats[st], min_stat_cap, tol, e ) )
			return 0.f;	//too small => not accepted
		est += e;
	}

	for ( StatType st : ChampionStats::TypeList )
	{
		const MatchOptions::StatFactor& f = matching.Factor( st );
		if ( f.HasMinCap() )
			continue;	//already estimated
		float fk = 0;
		if ( FloatEstimationFactor( f.Mode, fk ) )
		{
			const int stat_value = ch_stats[st];
			const int ref_stat_value = Ref_Stat_Values[ stl::enum_to_int(st) ];
			const int max_stat_cap   = GetActualMaxCap( st, matching );

			float e = 0;
			if ( max_stat_cap && stat_value > max_stat_cap )
			{
				const int tol = Excess_Tolerance[stl::enum_to_int( st )];
				const int excessive_threshold = max_stat_cap + tol;
				if ( stat_value > excessive_threshold )
					continue;	//stat got too high, give 0 estimation (or "return 0;" ?)
				else {
					e = linerp( stat_value, max_stat_cap, 1, excessive_threshold, 0 );
				}
			}
			else
				e = (float)stat_value / ref_stat_value;

			est += fk * e;
		}
		else {
			_ASSERTE( !f.HasMinCap() && !f.HasMaxCap() );
		}
	}
#endif
	return total_est;
}

ArtTier GetArtTier( const Artefact& art )
{
	return ( art.Stars == 6 ) ? ArtTier::T1 : ArtTier::T2;
// 	//if ( art.Stars <= 4 )
// 	//	return ArtTier::T3;
// 
// 	//if ( art.Type==ArtType::Boots && art.MainStatType()==StatType::Spd )
// 	//	return ArtTier::T1;
// 
// 	for ( const Stat& s : art.AddStats )
// 	{
// 		static const int SpeedSubStatThreshold = 17;
// 		if ( s.Type==StatType::Spd && s.Value>=SpeedSubStatThreshold )
// 			return ArtTier::T1;
// 		//if ( IsFlatArtStat( s.Type ) )
// 		//{
// 		//	if ( art.Stars == 6 )
// 		//		return ArtTier::T2;
// 		//	else
// 		//		return ArtTier::T3;
// 		//}
// 	}
// 
// 	return ArtTier::T2;
}

/////////////////////////////////////////////////////////////////////////////

EqEstPool::EqEstPool()
	:_Arr( DefaultSize )
{
}

EqEstPool::EqEstPool( size_t custom_buffer_size )
	:_Arr( custom_buffer_size )
{
}

void EqEstPool::join( const EqEst& rhs )
{
	_ASSERTE( _Arr.capacity() > 0 );
	_ASSERTE( rhs.Initialized() );

	bool inserted = false;
	for ( auto i = _Arr.begin(); i != _Arr.end(); ++i )
	{
		if ( rhs._Est > i->_Est )
		{
			if ( _Arr.full() )
				_Arr.pop_back();
			_Arr.insert( i, rhs );
			inserted = true;
			break;
		}
	}
	if ( !inserted && !_Arr.full() )
		_Arr.push_back( rhs );
}

void EqEstPool::join( const EqEstPool& rhs )
{
	_ASSERTE( _Arr.capacity() > 0 );

	for ( const EqEst& est : rhs._Arr )
		join( est );
}

/////////////////////////////////////////////////////////////////////////////

bool ProcessCombination( const EquipmentRef& eq, const Champion& target_champ, const MatchOptions& matching, EqEst& best_combination )
{
	if ( !matching.RequiedSets.empty() &&
		!matching.IsEqHasRequiredSets( eq ) )
		return false;

	ChampionStats arts_bonus_stats;
	ApplyEquipment( eq, target_champ.BasicStats, arts_bonus_stats, true, false, matching.ConsiderMaxLevels );

	const float est = EstimateEquipment( target_champ.TotalStats( arts_bonus_stats ), matching );

	if ( est > best_combination._Est )
	{
		best_combination._Est = est;
		best_combination._Eq = eq;

#ifdef DEBUG_FIND_BEST
		//std::cout << "\nBetter: (" << est << ")\n" << to_string( eq ) << "\n";
		//std::cout << "\nBetter: (" << est << ")\n" << to_string( arts_bonus_stats ) << "\n";
#endif
		return true;
	}
	return false;
}

void report_combinations( const std::map<ArtType, std::vector<Artefact>>& arts_by_type )
{
	const size_t n_comb = arts_by_type_iterator::n_combinations( arts_by_type );
	std::cout << n_comb << " combinations [";
	for ( const auto& at : arts_by_type )
		std::cout << at.second.size() << "x";
	std::cout << "]";
	
	//std::cout << "(will take ";
	//const double n = _MyArts.size();
	//const double n2 = n * n;
	//const double n3 = n * n2;
	//const double n4 = n * n3;
	//const double n5 = n * n4;
	//const int t = int( 2E-09*n5 - 1E-06*n4 + 0.0005*n3 - 0.0782*n2 + 6.3328*n - 196.8);
	//std::cout << t << " sec)";

	std::cout << "\n";

	//_ASSERTE( n_comb < 310 );
	if ( const auto* arr = stl::get_value_ptr( arts_by_type, ArtType::Ring ) )
		_ASSERTE( arr->size() <= 1 );		//TODO: filter by champion nation
	if ( const auto* arr = stl::get_value_ptr( arts_by_type, ArtType::Necklace ) )
		_ASSERTE( arr->size() <= 1 );		//TODO: filter by champion nation
	if ( const auto* arr = stl::get_value_ptr( arts_by_type, ArtType::Banner ) )
		_ASSERTE( arr->size() <= 1 );		//TODO: filter by champion nation
}

void FindBestEquipment( const std::map<ArtType, std::vector<Artefact>>& arts_by_type, const Champion& target_champ, const MatchOptions& matching, EqEst& best )
{
	report_combinations( arts_by_type );

#ifdef USE_TBB
	best =
	tbb::parallel_reduce( tbb::blocked_range<size_t>( 0, arts_by_type.cbegin()->second.size() ), EqEst(), 
		[&arts_by_type, &target_champ, &matching]( const tbb::blocked_range<size_t>& r, EqEst better )
		{
			for ( size_t root_index = r.begin(); root_index < r.end(); ++root_index )
			{
				arts_by_type_iterator2 eq_i( arts_by_type, root_index );
				EquipmentRef eq;
				for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
				{
					eq_i.get( eq );
					_ASSERTE( eq.CheckValidMapping() );

					ProcessCombination( eq, target_champ, matching, better );
				}
			}
			return better;
		},
		[]( EqEst lhs, const EqEst& rhs )
		{
			lhs._join( rhs );
			return lhs;
		}
	);
#else
#ifdef NDEBUG
#error TBB is disabled
#endif
	arts_by_type_iterator eq_i( arts_by_type );
	EquipmentRef eq;
	for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
	{
		eq_i.get( eq );
		_ASSERTE( eq.CheckValidMapping() );

		ProcessCombination( eq, target_champ, matching, best );
	}
#endif
}

void FindBestEquipment2( const std::map<ArtType, std::vector<Artefact>>& arts_by_type, const Champion& target_champ, const MatchOptions& matching, EqEstPool& best )
{
	report_combinations( arts_by_type );

#ifdef USE_TBB
	best =
	tbb::parallel_reduce( tbb::blocked_range<size_t>( 0, arts_by_type.cbegin()->second.size() ), EqEstPool(),
		[&arts_by_type, &target_champ, &matching]( const tbb::blocked_range<size_t>& r, EqEstPool better )
		{
			for ( size_t root_index = r.begin(); root_index < r.end(); ++root_index )
			{
				arts_by_type_iterator2 eq_i( arts_by_type, root_index );
				EquipmentRef eq;
				for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
				{
					eq_i.get( eq );
					_ASSERTE( eq.CheckValidMapping() );

					EqEst est;
					if ( ProcessCombination( eq, target_champ, matching, est ) )
						better.join( est );
				}
			}
			return better;
		},
		[]( EqEstPool lhs, const EqEstPool& rhs )
		{
			lhs.join( rhs );
			return lhs;
		}
	);
#else
#ifdef NDEBUG
#error TBB is disabled
#endif
	arts_by_type_iterator eq_i( arts_by_type );
	EquipmentRef eq;
	for ( eq_i.begin(); !eq_i.finished(); eq_i.next() )
	{
		eq_i.get( eq );
		_ASSERTE( eq.CheckValidMapping() );

		EqEst est;
		if ( ProcessCombination( eq, target_champ, matching, est ) )
			best.join( est );
	}
#endif
}

void SeparateInventory( const std::vector<Artefact>& inventory, const MatchOptions& matching, ChampionName ch_name,
						std::map<ArtType, std::vector<Artefact>>& arts_by_type )
{
	for ( const Artefact& art : inventory )
	{
		_ASSERTE( art.Initialized() );
		if ( matching.IsArtAccepted( art, ch_name ) )
			arts_by_type[art.Type].push_back( art );
	}
}

void FindBestEquipment( const std::vector<Artefact>& inventory, const Champion& ch, const MatchOptions& matching, Equipment& best_eq )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	SeparateInventory( inventory, matching, ch.Name, arts_by_type );

	EqEst best;
	FindBestEquipment( arts_by_type, ch, matching, best );

	best_eq = best._Eq.as_basic();
}

void FindBestEquipment2( const std::vector<Artefact>& inventory, const Champion& ch, const MatchOptions& matching, std::vector<Equipment>& eqs )
{
	std::map<ArtType, std::vector<Artefact>> arts_by_type;
	SeparateInventory( inventory, matching, ch.Name, arts_by_type );

	_ASSERTE( arts_by_type[ArtType::Weapon].size() < 70 );
	if ( auto v = stl::get_value_ptr( arts_by_type, ArtType::Ring ) )
		_ASSERTE( v->size() <= 1 );
	if ( auto v = stl::get_value_ptr( arts_by_type, ArtType::Necklace ) )
		_ASSERTE( v->size() <= 1 );
	if ( auto v = stl::get_value_ptr( arts_by_type, ArtType::Banner ) )
		_ASSERTE( v->size() <= 1 );

	EqEstPool best;
	FindBestEquipment2( arts_by_type, ch, matching, best );

	for ( const EqEst& rhs : best._Arr )
	{
		std::printf( "Est: %f\n", rhs._Est );
		eqs.emplace_back( rhs._Eq.as_basic() );
	}
}

void FindRealBestEquipment2( const Champion& ch, const MatchOptions& matching, std::vector<Equipment>& eqs )
{
	scope_profile_time prof_time( "FindRealBestEquipment" );

	FindBestEquipment2( _MyArts, ch, matching, eqs );
}
