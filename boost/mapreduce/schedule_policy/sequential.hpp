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
 
#ifndef BOOST_MAPREDUCE_SEQUENTIAL_SCHEDULE_HPP
#define BOOST_MAPREDUCE_SEQUENTIAL_SCHEDULE_HPP

namespace boost {

namespace mapreduce {

namespace schedule_policy {

template<typename Job>
class sequential
{
  public:
    void operator()(Job &job, results &result)
    {
        // Map Tasks
        time_t start_time = time(NULL);
        void *key = 0;
        while (job.get_next_map_key(key)  &&  job.run_map_task(key, result))
            ;
        result.map_runtime = time(NULL) - start_time;

        // Reduce Tasks
        start_time = time(NULL);
        for (unsigned partition=0; partition<job.number_of_partitions(); ++partition)
        {
            typename Job::filenames_t filenames;
            if (job.get_partition_filenames(partition, filenames))
                job.run_reduce_task(partition, filenames, result);
        }
        result.reduce_runtime = time(NULL) - start_time;

        result.counters.actual_map_tasks    = 1;
        result.counters.actual_reduce_tasks = 1;
        result.counters.num_result_files    = job.number_of_partitions();
    }
};

}   // namespace schedule_policy

}   // namespace mapreduce 

}   // namespace boost

#endif  // BOOST_MAPREDUCE_SEQUENTIAL_SCHEDULE_HPP
