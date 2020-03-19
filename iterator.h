#pragma once
#include "raid.h"

class arts_by_type_iterator
{
public:
	using arts_vec_t = std::vector<Artefact>;
	using arts_map_t = std::map<ArtType, arts_vec_t>;

	arts_by_type_iterator( const arts_map_t& m )
		: _map(m)
	{
		_count = 1;
		for ( const auto& p : _map )
		{
			_count *= p.second.size();
		}
	}

	bool begin()
	{
		//_progress = 0;
		for ( const auto& p : _map )
		{
			_vectors.push_back( &p );
			_curr_iterators.push_back( p.second.begin() );
		}
		_ASSERTE( _map.size() == _vectors.size() );
		_ASSERTE( _map.size() == _curr_iterators.size() );
		return !finished();
	}

	void next()
	{
		if ( _curr_iterators.empty() )
			return;

		const size_t last_index = _curr_iterators.size() - 1;
		size_t iter_index = last_index;
		for ( bool accepted = false; !accepted; )
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
				accepted = true;
		}

		_ASSERTE( _map.size() == _vectors.size() );
		_ASSERTE( _map.size() == _curr_iterators.size() );
	}

	bool finished() const
	{
		return _curr_iterators.empty();
	}

	bool get( Equipment& eq ) const
	{
		eq.Clear();
		for ( size_t i = 0; i < _curr_iterators.size(); ++i )
			eq[ _vectors[i]->first ] = *_curr_iterators[i];
		return !finished();
	}

	bool get( EquipmentRef& eq ) const
	{
		eq.Clear();
		for ( size_t i = 0; i < _curr_iterators.size(); ++i )
			eq.Arts[ _vectors[i]->first ] = &(*_curr_iterators[i]);
		return !finished();
	}

	Equipment get() const
	{
		Equipment eq;
		get( eq );
		return eq;
	}

	size_t count() const { return _count; }

private:
	void reset()
	{
		_vectors.clear();
		_curr_iterators.clear();
	}

private:
	const arts_map_t& _map;
	std::vector<const arts_map_t::value_type*> _vectors;
	std::vector<arts_vec_t::const_iterator> _curr_iterators;
	size_t _count = 0;
	//size_t _progress = 0;
};

