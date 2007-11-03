// test_ts_resource.cpp : Defines the entry point for the console application.
//

/* 
    Tests tss_resource_with_cache

    I have a vector, that is modified by different threads.

    Every once in a while I take a snapshot of this vector. Different threads reading this, they will be equal to the last snapshot, or
    the snapshot took before. I repeat this several times, to see that what I write to the vector, really propagates.
    
    Changing the vector:
    - first we start with one element : "0"
    - at each iteration:
      - see the last element in the vector = last_val
      - refill the vector with all elements starting with "last_val + 1"
      - the vector will have the same size as before
    - at each 500 iterations
      - I increase the size of the vector by 1
*/


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
#include <vector>
#include <sstream>
#include <iostream>

using namespace boost;
typedef std::vector<int> array;
typedef logging::locker::tss_resource_with_cache<array> resource;

template<class type> struct ts_value {
    void set(const type & src) {
        mutex::scoped_lock lk(cs);
        m_val = src;
    }

    type get() const {
        mutex::scoped_lock lk(cs);
        return m_val;
    }

private:
    mutable mutex cs;
    type m_val;
};

// for our resource, at what period is it updated on all threads?
// note: this value should be at least 5 in order for the test to work:
//       we sleep a bit, then we do lots of modifications, then we sleep a lot - so that the snapshots can be accurately taken
int g_cache_period_secs = 10;

// the vector we're constantly changing
resource g_resource( array(), g_cache_period_secs);

// the current value we've set in the resource
ts_value<array> g_cur_val;
// make sure only one thread updates the vector
mutex g_cur_val_cs; 
// the index of the current change
ts_value<int> g_change_idx;
// at how many iterations do I increase vector size
const int INCREASE_VECTOR_SIZE_PERIOD = 500;

// the 2 snapshots of the vector
ts_value<array> g_snapshot;
ts_value<array> g_prev_snapshot;


void update_cur_val() {
    // only one thread at a time ;)
    mutex::scoped_lock lk(g_cur_val_cs);

    array cur = g_cur_val.get();
    int change_idx = g_change_idx.get();
    int last_val = 0;
    if ( !cur.empty() )
        last_val = cur.back();

    if ( change_idx % INCREASE_VECTOR_SIZE_PERIOD == 0 ) {
        cur.resize( cur.size() + 1);
        std::cout << "****** new vector size " << cur.size() + 1;
    }

    for ( int i = 0 ; i < (int)cur.size(); ++i)
        cur[i] = ++last_val;
    g_cur_val.set(cur);
    g_change_idx.set( g_change_idx.get() + 1) ;
}


void update_resource() {
    update_cur_val();
    array cur_val = g_cur_val.get();
    resource::write res(g_resource);
    res.use() = cur_val;
}


void get_snapshot() {
    array snap = g_cur_val.get() ;

    array prev_snapshot = g_snapshot.get();
    g_snapshot.set(snap);
    g_prev_snapshot.set( prev_snapshot);
}

void test_resource() {
    array cur_val ;
    {
    resource::read res(g_resource);
    cur_val = res.use() ;
    }

    array snap = g_snapshot.get();
    array prev_snap = g_snapshot.get();

    BOOST_ASSERT( cur_val == snap || cur_val == prev_snap);
}

void do_sleep(int ms) {
    xtime next;
    xtime_get( &next, TIME_UTC);
    next.nsec += (ms % 1000) * 1000000;

    int nano_per_sec = 1000000000;
    next.sec += next.nsec / nano_per_sec;
    next.sec += ms / 1000;
    next.nsec %= nano_per_sec;
    thread::sleep( next);
}





// how many times do we update the resource, in a given pass?
int g_update_per_thread_count = 200;

ts_value<int> g_thread_idx;
// start time - we need all update threads to syncronize - to know until when to sleep
xtime g_start;

void update_thread() {
    xtime next = g_start;
    int thread_idx = g_thread_idx.get();
    g_thread_idx.set(thread_idx + 1);
 
    while ( true) {
        next.sec += g_cache_period_secs;
        do_sleep(1000);

        std::cout << "thread " << thread_idx << " working" << std::endl;
        for ( int i = 0; i < g_update_per_thread_count ; ++i) {
            update_resource();
            do_sleep(10);
        }        
        std::cout << "thread " << thread_idx << " sleeping" << std::endl;

        thread::sleep( next);
    }
}

void test_thread() {
    while ( true) {
        do_sleep(100);
        test_resource();
    }
}






// how many threads that update the resource?
int g_update_thread_count = 1; //10;

// how many threads that test the resource?
int g_test_thread_count = 1; // 10;

int g_run_period_secs = 1000; //100;

int main()
{
    xtime_get( &g_start, TIME_UTC);

    for ( int i = 0; i < g_update_thread_count; ++i)
        thread t(&update_thread);

    for ( int i = 0; i < g_test_thread_count; ++i)
        thread t(&test_thread);

    do_sleep(g_run_period_secs * 1000);
	return 0;
}

 

