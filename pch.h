#pragma once

#include "boost/test/test_tools.hpp"

//#include <tbb/task.h>
//#ifdef _DEBUG
//   	#define _CRTDBG_MAP_ALLOC
//	#include <stdlib.h>
//	#include <crtdbg.h>
//	#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#else
//   #define DEBUG_CLIENTBLOCK
//#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////

#ifndef TBB_PREVIEW_WAITING_FOR_WORKERS
#define TBB_PREVIEW_WAITING_FOR_WORKERS 1
#endif
#include <tbb/task_scheduler_init.h>
#include <boost/test/unit_test.hpp>
tbb::task_scheduler_init globalScheduler( tbb::task_scheduler_init::automatic );

#define NGP_UT_MAKE_TBB_SCHEDULER_FIXTURE(sheduler_name) \
	struct global_tbb_scheduler_fixture \
	{ \
		~global_tbb_scheduler_fixture() \
		{ \
			sheduler_name.blocking_terminate(); \
		} \
	}; \
	BOOST_GLOBAL_FIXTURE(global_tbb_scheduler_fixture)
NGP_UT_MAKE_TBB_SCHEDULER_FIXTURE( globalScheduler );

/////////////////////////////////////////////////////////////////////////////
