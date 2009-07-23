// Boost.MapReduce library
//
//  Copyright (C) 2009 Craig Henderson.
//  cdm.henderson@googlemail.com
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/mapreduce/
//

#ifndef BOOST_MAPREDUCE_HPP
#define BOOST_MAPREDUCE_HPP

#ifdef BOOST_MSVC
#   if !defined(__SGI_STL_PORT)
#   pragma message("warning: using STLPort is recommended to avoid STL container locking in MSVC supplied libraries.")
#   endif
#endif

#include <vector>

namespace boost {

namespace mapreduce {

struct specification
{
    unsigned map_tasks;
    unsigned reduce_tasks;

    specification()
      : map_tasks(0),                   // ideal number of map tasks to use
        reduce_tasks(10)                // ideal number of reduce tasks to use
    {
    }
};

struct results
{
    struct tag_counters
    {
        unsigned actual_map_tasks;      // number of map tasks actually used
        unsigned actual_reduce_tasks;   // number of reduce tasks actually used
        unsigned map_tasks;
        unsigned map_tasks_error;
        unsigned map_tasks_completed;
        unsigned num_result_files;      // number of result files created

        tag_counters()
          : actual_map_tasks(0),
            actual_reduce_tasks(0),
            map_tasks(0),
            map_tasks_error(0),
            map_tasks_completed(0),
            num_result_files(0)
        {
        }
    } counters;

    time_t                job_runtime;
    time_t                map_runtime;
    time_t                reduce_runtime;
    std::vector<time_t>   map_times;
    std::vector<time_t>   reduce_times;
};

}   // namespace mapreduce

}   // namespace boost

#include <boost/throw_exception.hpp>
#include <boost/mapreduce/platform.hpp>
#include <boost/mapreduce/mergesort.hpp>
#include <boost/mapreduce/intermediates.hpp>
#include <boost/mapreduce/schedule_policy.hpp>
#include <boost/mapreduce/datasource.hpp>
#include <boost/mapreduce/null_combiner.hpp>
#include <boost/mapreduce/job.hpp>

#endif  // BOOST_MAPREDUCE_HPP
