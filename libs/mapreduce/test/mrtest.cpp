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

// configuration options
#define WORD_COUNT_MEMORY_MAP_FILE
#define USE_WORDCOUNT_COMBINER
//#define USE_IN_MEMORY_INTERMEDIATES

#if defined(_DEBUG)
#define RUN_SEQUENTIAL_MAP_REDUCE
#endif

namespace wordcount {

typedef
#ifdef WORD_COUNT_MEMORY_MAP_FILE
    std::pair<char const *, char const *>
#else
    std::ifstream
#endif
map_value_type;

template<typename T>
struct map_task : public boost::mapreduce::map_task<
                             std::string,            // MapKey
                             map_value_type,         // MapValue
                             std::string,            // IntermediateKey
                             unsigned>               // IntermediateValue
{
    template<typename Runtime>
    static void map(Runtime &runtime, std::string const &key, T &value);
};
typedef map_task<map_value_type> map_task_type;

struct reduce_task : public boost::mapreduce::reduce_task<unsigned>
{
    template<typename Runtime, typename It>
    static void reduce(Runtime &runtime, std::string const &key, It it, It const ite)
    {
        runtime.emit(key, std::accumulate(it, ite, 0));
    }
};

template<> template<typename Runtime>
void
map_task<
    std::pair<
        char const *, char const *> >::map(
            Runtime           &runtime,
            std::string const &/*key*/,
            std::pair<char const *, char const *> &value)
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
                runtime.emit_intermediate(std::string(word,ptr-word), 1);
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
        runtime.emit_intermediate(std::string(word,ptr-word), 1);
    }
}


template<> template<typename Runtime>
void
map_task<std::ifstream>::map(
    Runtime            &runtime,
    std::string const  &/*key*/,
    std::ifstream      &value)
{
    while (!value.eof())
    {
        std::string word;
        value >> word;
        std::transform(word.begin(), word.end(), word.begin(),
                       std::bind1st(
                           std::mem_fun(&std::ctype<char>::tolower),
                           &std::use_facet<std::ctype<char> >(std::locale::classic())));

        size_t length = word.length();
        size_t const original_length = length;
        std::string::const_iterator it;
        for (it=word.begin();
             it!=word.end()  &&  !std::isalnum(*it, std::locale::classic());
             ++it)
        {
            --length;
        }

        for (std::string::const_reverse_iterator rit=word.rbegin();
             length>0  &&  !std::isalnum(*rit, std::locale::classic());
             ++rit)
        {
            --length;
        }

        if (length > 0)
        {
            if (length == original_length)
                runtime.emit_intermediate(word, 1);
            else
                runtime.emit_intermediate(std::string(&*it,length), 1);
        }
    }
}


class combiner;

typedef
boost::mapreduce::job<
    wordcount::map_task_type
  , wordcount::reduce_task
#ifdef USE_WORDCOUNT_COMBINER
  , wordcount::combiner
#else
  , boost::mapreduce::null_combiner
#endif
#ifdef USE_IN_MEMORY_INTERMEDIATES
  , boost::mapreduce::datasource::directory_iterator<wordcount::map_task_type>
  , boost::mapreduce::intermediates::in_memory<wordcount::map_task_type>
#endif
> job;



class combiner
{
  public:
    void start(job::map_task_type::intermediate_key_type const &)
    {
        total_ = 0;
    }

    template<typename IntermediateStore>
    void finish(job::map_task_type::intermediate_key_type const &key, IntermediateStore &intermediate_store)
    {
        if (total_ > 0)
            intermediate_store.insert(key, total_);
    }

    void operator()(job::map_task_type::intermediate_value_type const &value)
    {
        total_ += value;
    }

  private:
    unsigned total_;
};

}   // namespace wordcount



int main(int argc, char **argv)
{
#if defined(BOOST_MSVC)  &&  defined(_DEBUG)
//    _CrtSetBreakAlloc(380);
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::cout << "MapReduce Wordcount Application";
    if (argc < 2)
    {
        std::cerr << "Usage: wordcount directory [num_map_tasks]\n";
        return 1;
    }

    boost::mapreduce::specification spec;
    boost::mapreduce::results       result;

    spec.input_directory = argv[1];
    wordcount::job::datasource_type datasource(spec);

    std::cout << "\n" << std::max(1,(int)boost::thread::hardware_concurrency()) << " CPU cores";
    std::cout << "\n" << typeid(wordcount::job).name() << "\n";

    try
    {
#ifdef RUN_SEQUENTIAL_MAP_REDUCE
        std::cout << "\nRunning Sequential MapReduce...";

        spec.map_tasks = 1;

        wordcount::job      job(datasource, spec);
        job.run<boost::mapreduce::schedule_policy::sequential<wordcount::job> >(result);
        std::cout << "\nFinished.";
#else
        std::cout << "\nRunning CPU Parallel MapReduce...";

        if (argc > 2)
            spec.map_tasks = atoi(argv[2]);

        boost::mapreduce::run<wordcount::job>(spec, result);
        std::cout << "\nCPU Parallel MapReduce Finished.";
#endif
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
