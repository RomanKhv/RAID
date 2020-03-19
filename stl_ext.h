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

template <typename ENUM>
constexpr int enum_to_int( ENUM e )
{
	return static_cast<int>(e);
}

template <typename ENUM, typename VALUE_TYPE>
void copy_map_to_array( const std::map<ENUM, VALUE_TYPE>& m, VALUE_TYPE arr[] )
{
	for ( const auto& p : m )
		arr[stl::enum_to_int( p.first )] = p.second;
}

}