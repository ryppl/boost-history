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

#define BOOST_DISABLE_ASSERTS 
#if !defined(_DEBUG) &&  !defined(BOOST_DISABLE_ASSERTS)
#   pragma message("Warning: BOOST_DISABLE_ASSERTS not defined")
#endif

#include <boost/config.hpp>
#if defined(BOOST_MSVC)
#   pragma warning(disable: 4244 4512 4267 4996)
#endif

#include <boost/mapreduce.hpp>
#include <numeric>              // accumulate
#include <boost/algorithm/string.hpp>

#if defined(BOOST_MSVC)  && defined(_DEBUG)
#include <crtdbg.h>
#endif

namespace wordcount {

struct map_task : public boost::mapreduce::map_task<
                             std::string,                               // MapKey (filename)
                             std::pair<char const *, char const *> >    // MapValue (memory mapped file contents)
{
    template<typename Runtime>
    static void map(Runtime &runtime, key_type const &/*key*/, value_type &value)
    {
        bool in_word = false;
        char const *ptr = value.first;
        char const *end = value.second;
        char const *word = ptr;
        for (; ptr != end; ++ptr)
        {
            char const ch = std::toupper(*ptr);
            if (in_word)
            {
                if ((ch < 'A' || ch > 'Z') && ch != '\'')
                {
                    runtime.emit_intermediate(std::make_pair(word,ptr-word), 1);
                    in_word = false;
                }
            }
            else if (ch >= 'A'  &&  ch <= 'Z')
            {
                word = ptr;
                in_word = true;
            }
        }
        if (in_word)
            runtime.emit_intermediate(std::make_pair(word,ptr-word), 1);
    }
};

typedef std::pair<char const *, std::ptrdiff_t> reduce_key_t;

struct reduce_task : public boost::mapreduce::reduce_task<
                                reduce_key_t,
                                unsigned>
{
    template<typename Runtime, typename It>
    static void reduce(Runtime &runtime, key_type const &key, It it, It const ite)
    {
        runtime.emit(key, std::accumulate(it, ite, 0));
    }
};

class combiner
{
  public:
    template<typename IntermediateStore>
    static void run(IntermediateStore &intermediate_store)
    {
        combiner instance;
        intermediate_store.combine(instance);
    }

    void start(reduce_task::key_type const &)
    {
        total_ = 0;
    }

    template<typename IntermediateStore>
    void finish(reduce_task::key_type const &key, IntermediateStore &intermediate_store)
    {
        if (total_ > 0)
            intermediate_store.insert(key, total_);
    }

    void operator()(reduce_task::value_type const &value)
    {
        total_ += value;
    }
        
  private:
    combiner() { }

  private:
    unsigned total_;
};

typedef
boost::mapreduce::job<
    wordcount::map_task
  , wordcount::reduce_task
  , wordcount::combiner
> job;

}   // namespace wordcount


namespace std {

template<>
bool less<wordcount::reduce_key_t>::operator()(wordcount::reduce_key_t const &first, wordcount::reduce_key_t const &second) const
{
    std::ptrdiff_t const len = std::min(first.second, second.second);
    int const cmp = strnicmp(first.first, second.first, len);
    if (cmp < 0)
        return true;
    else if (cmp > 0)
        return false;

    return (first.second < second.second);
}

template<>
bool operator==(wordcount::reduce_key_t const &first, wordcount::reduce_key_t const &second)
{
    if (first.second != second.second)
        return false;
    else if (first.second == 0  &&  first.first == 0  &&  second.first == 0)
        return true;

    return (strnicmp(first.first, second.first, first.second) == 0);
}

}   // namespace std

namespace boost {
namespace mapreduce {
template<>
unsigned hash_partitioner::operator()(wordcount::reduce_key_t const &key, unsigned partitions) const
{
    return boost::hash_range(key.first, key.first+key.second) % partitions;
}
}   // namespace mapreduce {
}   // namespace boost {



int main(int argc, char **argv)
{
    std::cout << "MapReduce Wordcount Application";
    if (argc < 2)
    {
        std::cerr << "Usage: wordcount directory [num_map_tasks]\n";
        return 1;
    }

    boost::mapreduce::specification spec;
    spec.input_directory = argv[1];

    std::cout << "\n" << std::max(1,(int)boost::thread::hardware_concurrency()) << " CPU cores";
    std::cout << "\n" << typeid(wordcount::job).name() << "\n";

    boost::mapreduce::results result;
    wordcount::job::datasource_type datasource(spec);
    try
    {
        if (argc > 2)
            spec.map_tasks = atoi(argv[2]);

        if (argc > 3)
            spec.reduce_tasks = atoi(argv[3]);
        else
            spec.reduce_tasks = std::max(1U,boost::thread::hardware_concurrency());

        std::cout << "\nRunning Parallel WordCount MapReduce...";
        wordcount::job job(datasource, spec);
        job.run<boost::mapreduce::schedule_policy::cpu_parallel<wordcount::job> >(result);
        std::cout << "\nMapReduce Finished.";

        std::cout << std::endl << "\nMapReduce statistics:";
        std::cout << "\n  MapReduce job runtime                     : " << result.job_runtime << " seconds, of which...";
        std::cout << "\n    Map phase runtime                       : " << result.map_runtime << " seconds";
        std::cout << "\n    Reduce phase runtime                    : " << result.reduce_runtime << " seconds";
        std::cout << "\n\n  Map:";
        std::cout << "\n    Total Map keys                          : " << result.counters.map_keys_executed;
        std::cout << "\n    Map keys processed                      : " << result.counters.map_keys_completed;
        std::cout << "\n    Map key processing errors               : " << result.counters.map_key_errors;
        std::cout << "\n    Number of Map Tasks run (in parallel)   : " << result.counters.actual_map_tasks;
        std::cout << "\n    Fastest Map key processed in            : " << *std::min_element(result.map_times.begin(), result.map_times.end()) << " seconds";
        std::cout << "\n    Slowest Map key processed in            : " << *std::max_element(result.map_times.begin(), result.map_times.end()) << " seconds";
        std::cout << "\n    Average time to process Map keys        : " << std::accumulate(result.map_times.begin(), result.map_times.end(), boost::posix_time::time_duration()) / result.map_times.size() << " seconds";

        std::cout << "\n\n  Reduce:";
        std::cout << "\n    Total Reduce keys                       : " << result.counters.reduce_keys_executed;
        std::cout << "\n    Reduce keys processed                   : " << result.counters.reduce_keys_completed;
        std::cout << "\n    Reduce key processing errors            : " << result.counters.reduce_key_errors;
        std::cout << "\n    Number of Reduce Tasks run (in parallel): " << result.counters.actual_reduce_tasks;
        std::cout << "\n    Number of Result Files                  : " << result.counters.num_result_files;
        if (result.reduce_times.size() > 0)
        {
            std::cout << "\n    Fastest Reduce key processed in         : " << *std::min_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
            std::cout << "\n    Slowest Reduce key processed in         : " << *std::max_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
            std::cout << "\n    Average time to process Reduce keys     : " << std::accumulate(result.reduce_times.begin(), result.reduce_times.end(), boost::posix_time::time_duration()) / result.map_times.size() << " seconds";
        }

        wordcount::job::const_result_iterator it  = job.begin_results();
        wordcount::job::const_result_iterator ite = job.end_results();
        if (it != ite)
        {
            typedef std::list<wordcount::job::keyvalue_t> frequencies_t;
            frequencies_t frequencies;
            frequencies.push_back(*it);
            frequencies_t::reverse_iterator it_smallest = frequencies.rbegin();
            for (++it; it!=ite; ++it)
            {
                if (frequencies.size() < 10)    // show top 10
                {
                    frequencies.push_back(*it);
                    if (it->second < it_smallest->second)
                        it_smallest = frequencies.rbegin();
                }
                else if (it->second > it_smallest->second)
                {
                    *it_smallest = *it;
                    it_smallest = std::min_element(frequencies.rbegin(), frequencies.rend(), boost::mapreduce::detail::less_2nd<wordcount::job::keyvalue_t>);
                }
            }

            frequencies.sort(boost::mapreduce::detail::greater_2nd<wordcount::job::keyvalue_t>);
            std::cout << "\n\nMapReduce results:";
            for (frequencies_t::const_iterator freq=frequencies.begin(); freq!=frequencies.end(); ++freq)
                printf("\n%.*s\t%d", freq->first.second, freq->first.first, freq->second);
        }
    }
    catch (std::exception &e)
    {
        std::cout << std::endl << "Error: " << e.what();
    }

    return 0;
}
