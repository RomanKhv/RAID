#pragma once

namespace stl
{

template <class KeyT, class... ValueT>
bool contains( const std::map<KeyT,ValueT...>& c, const KeyT& key )
{
	return c.count( key ) > 0;
}

template <class CONTAINER>
bool contains( const CONTAINER& c, typename const CONTAINER::value_type& val )
{
	return std::find( c.begin(), c.end(), val ) != c.end();
}

template <class MAP>
bool get_value( const MAP& m, typename const MAP::key_type& key, typename MAP::mapped_type& val )
{
	const auto i = m.find( key );
	if ( i == m.end() )
		return false;
	val = i->second;
	return true;
}

template <class MAP>
typename MAP::mapped_type get_value_or( const MAP& m, typename const MAP::key_type& key, typename const MAP::mapped_type& def_val )
{
	const auto i = m.find( key );
	if ( i == m.end() )
		return def_val;
	return i->second;
}

template <class MAP>
typename const MAP::mapped_type* get_value_ptr( const MAP& m, typename const MAP::key_type& key )
{
	const auto i = m.find( key );
	if ( i == m.end() )
		return nullptr;
	return &i->second;
}

template <typename RET_TYPE, typename ENUM>
constexpr RET_TYPE enum_to_( ENUM e )
{
	return static_cast<RET_TYPE>(e);
}

template <typename ENUM>
constexpr int enum_to_int( ENUM e )
{
	return static_cast<int>(e);
}

template <typename ENUM>
constexpr bool is_valid_enum_as_index( ENUM e )
{
	return static_cast<int>(e) >= 0;
}

template <typename ENUM, typename VALUE_TYPE>
void copy_map_to_array( const std::map<ENUM, VALUE_TYPE>& m, VALUE_TYPE arr[] )
{
	for ( const auto& p : m )
		arr[stl::enum_to_int( p.first )] = p.second;
}

}

/////////////////////////////////////////////////////////////////////////////

//#define INDEX_MAP_READY_FOR_RANGE_FOR

template <typename IMAP>
struct index_map_iterator
{
	using this_type = index_map_iterator<IMAP>;

	index_map_iterator( IMAP& m, typename IMAP::key_type index_key )
		: _map( m )
	{
		_index = index_key;
#ifdef _DEBUG
		_instances_counter++;
#endif
	}

	typename IMAP::key_type key() const
	{
		return _index;
	}

	typename IMAP::mapped_type& value() const
	{
		_ASSERTE( !finished() );
		return _map[_index];
	}

	bool operator==( const this_type& rhs ) const
	{
		return _index == rhs._index;
	}

	bool operator!=( const this_type& rhs ) const
	{
		return _index != rhs._index;
	}

	this_type& operator++()
	{
		if ( _index < _map.end_key() )
			_index = static_cast<typename IMAP::key_type>(static_cast<int>(_index) + 1);
		return *this;
	}

#ifdef INDEX_MAP_READY_FOR_RANGE_FOR
	const this_type& operator*() const
	{
		return *this;
	}
#endif

#ifdef _DEBUG
	static int _instances_counter;
	index_map_iterator( const this_type& rhs ) : index_map_iterator( rhs._map, rhs._index ) { _instances_counter++; }
#endif

private:
	bool finished() const
	{
		return _index == _map.end_key();
	}

private:
	IMAP& _map;
	typename IMAP::key_type _index;
};

/////////////////////////////////////////////////////////////////////////////

#include <array>

template <typename Key_t, size_t SIZE, class Value_t>
struct index_map
{
	using key_type = Key_t;
	using mapped_type = Value_t;
	using this_type = index_map< Key_t, SIZE, Value_t >;

	constexpr size_t size() const { return SIZE; }
	std::array<Value_t, SIZE> _Arr;

	index_map()
	{
		_Arr.fill( mapped_type{} );
	}

	const Value_t& operator[]( const key_type key ) const
	{
		_ASSERTE( stl::is_valid_enum_as_index( key ) );
		return _Arr[stl::enum_to_int( key )];
	}
	Value_t& operator[]( const key_type key )
	{
		_ASSERTE( stl::is_valid_enum_as_index( key ) );
		return _Arr[stl::enum_to_int( key )];
	}

	key_type begin_key() const
	{
		return static_cast<key_type>(0);
	}
	key_type end_key() const
	{
		return static_cast<key_type>(SIZE);
	}

	index_map_iterator<this_type> begin()
	{
		return index_map_iterator<this_type>( *this, begin_key() );
	}
	index_map_iterator<this_type> end()
	{
		return index_map_iterator<this_type>( *this, end_key() );
	}
};

template <typename Key_t, Key_t END_KEY, class Value_t>
using enum_index_map = index_map< Key_t, stl::enum_to_<size_t>( END_KEY ), Value_t >;

/////////////////////////////////////////////////////////////////////////////

template <typename ENUM>
struct enum_iterator
{
	enum_iterator( ENUM b, ENUM e )
		: _value( b ), _end( e )
	{
		_ASSERTE( _value <= _end );
	}

	enum_iterator( ENUM e )
		: _value( static_cast<ENUM>(0) )
		, _end( e )
	{
		_ASSERTE( _value <= _end );
	}

	void operator++()
	{
		if ( !finished() )
			_value = static_cast<ENUM>(static_cast<std::underlying_type_t<ENUM>>(_value) + 1);
	}

	bool finished() const
	{
		return _value >= _end;
	}

	ENUM value()     const { return _value; }
	ENUM operator*() const { return _value; }

private:
	ENUM _value;
	const ENUM _end;
};

/////////////////////////////////////////////////////////////////////////////
