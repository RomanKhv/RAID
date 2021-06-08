#pragma once
#include <boost/container/static_vector.hpp>
#include "raid.h"

class arts_by_type_iterator2
{
public:
	using arts_vec_t = std::vector<Artefact>;
	using arts_map_t = std::map<ArtType, arts_vec_t>;

	arts_by_type_iterator2( const arts_map_t& m, size_t root_index )
		: _map(m)
		//, _root_index( root_index )
		, _root_art( m.empty() ? nullptr : &(m.cbegin()->second[root_index]) )
	{
	}

	bool begin()
	{
		if ( _map.empty() )
			return false;
		//_progress = 0;
		auto i = _map.begin();
		for ( ++i; i != _map.end(); ++i )
		{
			_vectors.push_back( &(*i) );
			_curr_iterators.push_back( i->second.begin() );
		}
		return !finished();
	}

	void next()
	{
		if ( finished() )
			return;

		const size_t last_index = _curr_iterators.size() - 1;
		size_t iter_index = last_index;
		while ( true )
		{
			auto& i = _curr_iterators[iter_index];
			i++;
			if ( i == _vectors[iter_index]->second.end() )
			{
				if ( iter_index == 0 )
				{
					reset();
					return;	//finished
				}
				else {
					i = _vectors[iter_index]->second.begin();
					iter_index--;
				}
			}
			else
				break;		//accepted
		}

		_ASSERTE( _map.size() == (1 + _vectors.size()) );
		_ASSERTE( _map.size() == (1 + _curr_iterators.size()) );
	}

	bool finished() const
	{
		return _curr_iterators.empty();
	}

	bool get( Equipment& eq ) const
	{
		eq.Clear();
		if ( !finished() )
		{
			//eq.Arts[_map.cbegin()->first] = front_vec()[_root_index];
			eq.Arts[_map.cbegin()->first] = *_root_art;

			for ( size_t i = 0; i < _curr_iterators.size(); ++i )
				eq[_vectors[i]->first] = *_curr_iterators[i];
		}
		return !finished();
	}

	bool get( EquipmentRef& eq ) const
	{
		eq.Clear();
		if ( !finished() )
		{
			//eq.Arts[_map.cbegin()->first] = &(front_vec()[_root_index]);
			eq.Arts[_map.cbegin()->first] = _root_art;

			for ( size_t i = 0; i < _curr_iterators.size(); ++i )
				eq.Arts[_vectors[i]->first] = &(*_curr_iterators[i]);
		}
		return !finished();
	}

	Equipment get() const
	{
		Equipment eq;
		get( eq );
		return eq;
	}

private:
	const arts_vec_t& front_vec() const
	{
		return _map.cbegin()->second;
	}
	void reset()
	{
		_vectors.clear();
		_curr_iterators.clear();
	}

private:
	const arts_map_t& _map;
	//const size_t _root_index;
	const Artefact* const _root_art;
	boost::container::static_vector<const arts_map_t::value_type*,9-1> _vectors;
	boost::container::static_vector<arts_vec_t::const_iterator,9-1> _curr_iterators;
};

