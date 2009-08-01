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
 
#if !defined(_DEBUG) &&  !defined(BOOST_DISABLE_ASSERTS)
#   pragma message("Warning: BOOST_DISABLE_ASSERTS not defined")
#endif

#include <boost/config.hpp>
#if defined(BOOST_MSVC)
#   pragma warning(disable: 4244 4512 4267)
#endif

#include <boost/mapreduce.hpp>
#include <numeric>              // accumulate

#if defined(BOOST_MSVC)  && defined(_DEBUG)
#include <crtdbg.h>
#endif

namespace wordcount {

struct map_task : public boost::mapreduce::map_task<
                             std::string,                           // MapKey
                             std::pair<char const *, char const *>, // MapValue
                             std::string,                           // IntermediateKey
                             unsigned>                              // IntermediateValue
{
    template<typename Runtime>
    static void map(Runtime &runtime, std::string const &/*key*/, value_type &value)
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
                    std::string w(word,ptr-word);
                    std::transform(w.begin(), w.end(), w.begin(),
                                   std::bind1st(
                                       std::mem_fun(&std::ctype<char>::tolower),
                                       &std::use_facet<std::ctype<char> >(std::locale::classic())));
                    runtime.emit_intermediate(w, 1);
                    in_word = false;
                }
            }
            else
            {
                if (ch >= 'A'  &&  ch <= 'Z')
                {
                    word = ptr;
                    in_word = true;
                }
            }
        }
        if (in_word)
        {
            BOOST_ASSERT(ptr-word > 0);
            std::string w(word,ptr-word);
            std::transform(w.begin(), w.end(), w.begin(),
                           std::bind1st(
                               std::mem_fun(&std::ctype<char>::tolower),
                               &std::use_facet<std::ctype<char> >(std::locale::classic())));
            runtime.emit_intermediate(w, 1);
        }
    }
};

struct reduce_task : public boost::mapreduce::reduce_task<unsigned>
{
    template<typename Runtime, typename It>
    static void reduce(Runtime &runtime, std::string const &key, It it, It const ite)
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

    void start(map_task::intermediate_key_type const &)
    {
        total_ = 0;
    }

    template<typename IntermediateStore>
    void finish(map_task::intermediate_key_type const &key, IntermediateStore &intermediate_store)
    {
        if (total_ > 0)
            intermediate_store.insert(key, total_);
    }

    void operator()(map_task::intermediate_value_type const &value)
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
    try
    {
        if (argc > 2)
            spec.map_tasks = atoi(argv[2]);

        std::cout << "\nRunning CPU Parallel MapReduce...";
        boost::mapreduce::run<wordcount::job>(spec, result);
        std::cout << "\nCPU Parallel MapReduce Finished.";
    }
    catch (std::exception &e)
    {
        std::cout << std::endl << "Error: " << e.what();
    }

    std::cout << std::endl << "\n" << "MapReduce statistics:";
    std::cout << "\n  " << "MapReduce job runtime                     : " << result.job_runtime << " seconds, of which...";
    std::cout << "\n  " << "  Map phase runtime                       : " << result.map_runtime << " seconds";
    std::cout << "\n  " << "  Reduce phase runtime                    : " << result.reduce_runtime << " seconds";
    std::cout << "\n\n  " << "Map:";
    std::cout << "\n    " << "Total Map keys                          : " << result.counters.map_keys_executed;
    std::cout << "\n    " << "Map keys processed                      : " << result.counters.map_keys_completed;
    std::cout << "\n    " << "Map key processing errors               : " << result.counters.map_key_errors;
    std::cout << "\n    " << "Number of Map Tasks run (in parallel)   : " << result.counters.actual_map_tasks;
    std::cout << "\n    " << "Fastest Map key processed in            : " << *std::min_element(result.map_times.begin(), result.map_times.end()) << " seconds";
    std::cout << "\n    " << "Slowest Map key processed in            : " << *std::max_element(result.map_times.begin(), result.map_times.end()) << " seconds";
    std::cout << "\n    " << "Average time to process Map keys        : " << std::accumulate(result.map_times.begin(), result.map_times.end(), boost::int64_t()) / result.map_times.size() << " seconds";

    std::cout << "\n\n  " << "Reduce:";
    std::cout << "\n    " << "Total Reduce keys                       : " << result.counters.reduce_keys_executed;
    std::cout << "\n    " << "Reduce keys processed                   : " << result.counters.reduce_keys_completed;
    std::cout << "\n    " << "Reduce key processing errors            : " << result.counters.reduce_key_errors;
    std::cout << "\n    " << "Number of Reduce Tasks run (in parallel): " << result.counters.actual_reduce_tasks;
    std::cout << "\n    " << "Number of Result Files                  : " << result.counters.num_result_files;
    if (result.reduce_times.size() > 0)
    {
        std::cout << "\n    " << "Fastest Reduce key processed in         : " << *std::min_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
        std::cout << "\n    " << "Slowest Reduce key processed in         : " << *std::max_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
        std::cout << "\n    " << "Average time to process Reduce keys     : " << std::accumulate(result.reduce_times.begin(), result.reduce_times.end(), boost::int64_t()) / result.map_times.size() << " seconds";
    }

    return 0;
}
