#pragma once
#include <ctime>

class scope_profile_time
{
public:
	scope_profile_time( const char* name )
		:_name( name )
		,_t0( std::time(nullptr) )
	{
	}
	~scope_profile_time()
	{
		const std::time_t t = std::time(nullptr);
		std::cout << _name << ": " << (t - _t0) << " sec\n";
		//BOOST_TEST_MESSAGE( _name << ": " << (t - _t0) << " sec" );
	}

private:
	const std::string _name;
	const std::time_t _t0 = 0;
};