// Boost sweepline library benchmark_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE voronoi_benchmark_test
#include <boost/test/test_case_template.hpp>

#ifdef WIN32
#pragma warning( disable : 4996 )
#endif

BOOST_AUTO_TEST_CASE_TEMPLATE(benchmark_test1, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_output<coordinate_type> test_output;

    FILE *bench_file = fopen("benchmark.txt", "a");
    fprintf(bench_file, "Voronoi Segment Sweepline Benchmark Test (time in seconds):\n");

#ifdef NDEBUG
    int max_points = 1000000;
#else
    int max_points = 1000;
#endif

    for (int num_points = 10; num_points <= max_points; num_points *= 10) {
        std::vector< point_2d<T> > points;
        points.reserve(num_points);

        clock_t start_time = clock();
        int num_times = max_points / num_points;
        for (int cur = 0; cur < num_times; cur++) {
            for (int cur_point = 0; cur_point < num_points; cur_point++) {
                points.push_back(make_point_2d<coordinate_type>(
                    static_cast<coordinate_type>(rand() % 5000 - 10000),
                    static_cast<coordinate_type>(rand() % 5000 - 10000)));
            }
            build_voronoi(points, test_output);
            points.clear();
        }
        clock_t end_time = clock();
        double running_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC / num_times;

        fprintf(bench_file,
                "Number of points = %8d; Overall time = %2d; Time per one input = %9.6f.\n",
                num_points, static_cast<int>(end_time - start_time), running_time);
    }
    fclose(bench_file);
}
