#pragma once

namespace stl
{

template <class CONTAINER>
bool contains( const CONTAINER& c, typename const CONTAINER::value_type& val )
{
	return std::find( c.begin(), c.end(), val ) != c.end();
}

}