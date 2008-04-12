// Copyright (C) 2001-2003
// William E. Kempf
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/detail/config.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>

#include <boost/test/unit_test.hpp>

namespace {

// Shared variables for generation barrier test
const int N_THREADS=10;
boost::barrier gen_barrier(N_THREADS);
boost::mutex mutex;
long global_parameter;

void barrier_thread()
{
    for (int i = 0; i < 5; ++i)
    {
        if (gen_barrier.wait())
        {
            boost::mutex::scoped_lock lock(mutex);
            global_parameter++;
        }
    }
}

} // namespace

void test_barrier()
{
    boost::thread_group g;
    global_parameter = 0;

    for (int i = 0; i < N_THREADS; ++i)
        g.create_thread(&barrier_thread);

    g.join_all();

    BOOST_CHECK(global_parameter == 5);
}

boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test_framework::test_suite* test =
        BOOST_TEST_SUITE("Boost.Threads: barrier test suite");

    test->add(BOOST_TEST_CASE(&test_barrier));

    return test;
}
