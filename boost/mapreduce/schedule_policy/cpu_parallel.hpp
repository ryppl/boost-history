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
 
#ifndef BOOST_MAPREDUCE_CPU_PARALLEL_HPP
#define BOOST_MAPREDUCE_CPU_PARALLEL_HPP

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4244)  //  warning C4244: 'argument' : conversion from 'X' to 'Y', possible loss of data 
#endif
#include <boost/thread.hpp>
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

namespace boost {

namespace mapreduce {

namespace schedule_policy {

namespace detail {

template<typename Job>
inline void run_next_map_task(Job &job, results &result, boost::mutex &m)
{
    try
    {
        bool run = true;
        while (run)
        {
            void *key = 0;

            m.lock();
            run = job.get_next_map_key(key);
            m.unlock();

            if (run)
                job.run_map_task(key, result);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "\nError: " << e.what() << "\n";
    }
}

template<typename Job>
inline void run_next_reduce_task(Job &job, unsigned &partition, results &result, boost::mutex &m)
{
    try
    {
        bool run = true;
        while (run)
        {
            typename Job::filenames_t filenames;

            m.lock();
            run = job.get_partition_filenames(partition, filenames);
            m.unlock();

            if (run)
                job.run_reduce_task(partition, filenames, result);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "\nError: " << e.what() << "\n";
    }
}

}   // namespace detail


template<typename Job>
class cpu_parallel
{
  public:
    void operator()(Job &job, specification const &spec, results &result)
    {
        unsigned const num_cpus = std::max(1,boost::thread::hardware_concurrency());
        job.number_of_partitions(spec.reduce_tasks);

        typedef std::vector<boost::shared_ptr<results> > all_results_t;
        all_results_t all_results;
        boost::mutex  m;

        // run the Map Tasks
        time_t start_time = time(NULL);
        boost::thread_group map_threads;

        unsigned const map_tasks = (spec.map_tasks==0)? num_cpus : std::min(num_cpus, spec.map_tasks);

        for (unsigned loop=0; loop<map_tasks; ++loop)
        {
            boost::shared_ptr<results> this_result(new results);
            all_results.push_back(this_result);

            boost::thread *thread =
                new boost::thread(
                    detail::run_next_map_task<Job>,
                    boost::ref(job),
                    boost::ref(*this_result),
                    boost::ref(m));
            map_threads.add_thread(thread);
        }
        map_threads.join_all();
        result.map_runtime = time(NULL) - start_time;

        // run the Reduce Tasks
        start_time = time(NULL);
        boost::thread_group reduce_threads;

        unsigned const reduce_tasks =
            std::min<unsigned const>(num_cpus, job.number_of_partitions());

        unsigned partition = 0;
        for (unsigned loop=0; loop<reduce_tasks; ++loop)
        {
            boost::shared_ptr<results> this_result(new results);
            all_results.push_back(this_result);

            boost::thread *thread =
                new boost::thread(
                    detail::run_next_reduce_task<Job>,
                    boost::ref(job),
                    boost::ref(partition),
                    boost::ref(*this_result),
                    boost::ref(m));
            reduce_threads.add_thread(thread);
        }
        reduce_threads.join_all();
        result.reduce_runtime = time(NULL) - start_time;

        // we're done with the map/reduce job, collate the statistics before returning
        for (all_results_t::const_iterator it=all_results.begin();
             it!=all_results.end();
             ++it)
        {
            result.counters.map_tasks           += (*it)->counters.map_tasks;
            result.counters.map_tasks_error     += (*it)->counters.map_tasks_error;
            result.counters.map_tasks_completed += (*it)->counters.map_tasks_completed;

            std::copy(
                (*it)->map_times.begin(),
                (*it)->map_times.end(),
                std::back_inserter(result.map_times));
            std::copy(
                (*it)->reduce_times.begin(),
                (*it)->reduce_times.end(),
                std::back_inserter(result.reduce_times));

        }
        result.counters.actual_map_tasks    = map_tasks;
        result.counters.actual_reduce_tasks = reduce_tasks;
        result.counters.num_result_files    = job.number_of_partitions();
    }
};

}   // namespace schedule_policy

}   // namespace mapreduce 

}   // namespace boost

#endif  // BOOST_MAPREDUCE_CPU_PARALLEL_HPP
