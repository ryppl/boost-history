
// so that we can catch the end of deleting all objects
#define BOOST_LOG_TEST_TSS

#define BOOST_LOG_TSS_USE_INTERNAL
// this includes tss_value class
#include <boost/logging/logging.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "count.h"

using namespace boost;



// we specifically let these 2 objects leak - making them static, would destroy them too soon (before the other objects being destroyed)
extern object_count * g_object_count ;
extern object_count * g_running_thread_count ;

namespace boost { namespace logging { namespace detail {
void on_end_delete_objects() {
    BOOST_ASSERT( g_object_count->count() == 0);
}

}}}

