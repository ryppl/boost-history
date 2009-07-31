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

#include <string>
#include <vector>
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

namespace boost {

namespace mapreduce {

struct specification : boost::noncopyable
{
    unsigned         map_tasks;             // ideal number of map tasks to use
    unsigned         reduce_tasks;          // ideal number of reduce tasks to use
    boost::uintmax_t max_file_segment_size; // ideal maximum number of bytes in each input file segment
    std::string      output_filespec;       // filespec of the output files - can contain a directory path if required
    std::string      input_directory;       // directory path to scan for input files

    specification()
      : map_tasks(0),                   
        reduce_tasks(10),
        max_file_segment_size(1048576L),    // default 1Gb               
        output_filespec("mapreduce_")   
    {
    }
};

struct results
{
    struct tag_counters
    {
        unsigned actual_map_tasks;      // number of map tasks actually used
        unsigned actual_reduce_tasks;   // number of reduce tasks actually used

        // counters for map key processing
        unsigned map_keys_executed;
        unsigned map_key_errors;
        unsigned map_keys_completed;

        // counters for reduce key processing
        unsigned reduce_keys_executed;
        unsigned reduce_key_errors;
        unsigned reduce_keys_completed;

        unsigned num_result_files;      // number of result files created

        tag_counters()
          : actual_map_tasks(0),
            actual_reduce_tasks(0),
            map_keys_executed(0),
            map_key_errors(0),
            map_keys_completed(0),
            reduce_keys_executed(0),
            reduce_key_errors(0),
            reduce_keys_completed(0),
            num_result_files(0)
        {
        }
    } counters;

    results()
      : job_runtime(0),
        map_runtime(0),
        reduce_runtime(0)
    {
    }

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

namespace boost {

namespace mapreduce {

template<typename Job>
void run(boost::mapreduce::specification &spec, boost::mapreduce::results &result)
{
    typename Job::datasource_type datasource(spec);
    Job job(datasource, spec);
    job.run<boost::mapreduce::schedule_policy::cpu_parallel<Job> >(result);
}

}   // namespace mapreduce

}   // namespace boost

#endif  // BOOST_MAPREDUCE_HPP
