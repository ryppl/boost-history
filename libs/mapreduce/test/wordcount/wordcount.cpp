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
 
/*
Use a variant of Boost built with STLport IOStreams

STLport-5.2.1>set include=C:\root\Development\Library\STLport\STLport-5.2.1\stlport;%include%
STLport-5.2.1>configure msvc8 -p winxp -x --without-thread --with-dynamic-rtl
STLport-5.2.1\build\lib>nmake clean install

Edit boost_1_39_0\tools\build\v2\user-config.jam to
# -------------------
# MSVC configuration.
# -------------------
# Configure specific msvc version (searched for in standard locations and PATH).
using msvc : 8.0 ;
# ----------------------
# STLPort configuration.
# ----------------------
using stlport : : .../STLport/STLport-5.2.1/stlport : .../STLport/STLport-5.2.1/lib/vc8 ;

boost_1_39_0> set include=C:\root\Development\Library\STLport\STLport-5.2.1\stlport;%include%
boost_1_39_0> set INCLUDE=%INCLUDE%;C:\root\Development\Library\zlib\include
boost_1_39_0> set ZLIB_INCLUDE=C:\root\Development\Library\zlib\include
boost_1_39_0> set LIBPATH=%LIBPATH%;C:\root\Development\Library\zlib\lib
boost_1_39_0> set ZLIB_LIBPATH=C:\root\Development\Library\zlib\lib
boost_1_39_0> set ZLIB_BINARY=zdll
boost_1_39_0> ..\bjam --toolset=msvc stdlib=stlport "stdlib:stlport-iostream=on" --without-python --with-filesystem --with-thread --with-date_time
*/

#ifdef _WIN32
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#endif

#include <boost/config.hpp>
#if defined(BOOST_MSVC)
#pragma warning(disable:4996 4512)   // for wordcount std::transform
#endif

#include <boost/mapreduce.hpp>
#include <iostream>
#include <numeric>          // accumulate

#if defined(BOOST_MSVC)  && defined(_DEBUG)
#include <crtdbg.h>
#endif

namespace wordcount {

class map_task;
class reduce_task;
class combiner;

typedef
boost::mapreduce::job
  < wordcount::map_task
  , wordcount::reduce_task
  , wordcount::combiner
#if 0  &&  defined(_DEBUG)
  , boost::mapreduce::datasource::directory_iterator<wordcount::map_task>
  , boost::mapreduce::intermediates::in_memory<wordcount::map_task>
#endif
  >
job;

class map_task : boost::noncopyable
{
  public:
    typedef std::string   key_type;
    typedef std::ifstream value_type;
    typedef std::string   intermediate_key_type;
    typedef unsigned      intermediate_value_type;

    map_task(job::map_task_runner &runner)
      : runner_(runner)
    {
    }

    // not a reference to const to enable streams to be passed
    void operator()(key_type const &/*key*/, value_type &value) 
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
                    runner_.emit_intermediate(word, 1);
                else
                    runner_.emit_intermediate(std::string(&*it,length), 1);
            }
        }
    }

  private:
    job::map_task_runner &runner_;
};

class reduce_task : boost::noncopyable
{
  public:
    typedef unsigned value_type;

    reduce_task(job::reduce_task_runner &runner)
      : runner_(runner)
    {
    }

    template<typename It>
    void operator()(typename map_task::intermediate_key_type const &key, It it, It const ite)
    {
        runner_.emit(key, std::accumulate(it, ite, reduce_task::value_type()));
    }

  private:
    job::reduce_task_runner &runner_;
};

class combiner
{
  public:
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

    wordcount::job::datasource_type datasource;
    datasource.set_directory(argv[1]);

    std::cout << "\n" << std::max(1,(int)boost::thread::hardware_concurrency()) << " CPU cores";
    std::cout << "\n" << typeid(wordcount::job).name() << "\n";
    
#if 0  ||  defined(_DEBUG)
    std::cout << "\nRunning Sequential MapReduce...";

    boost::mapreduce::specification spec;
    spec.map_tasks    = 1;

    boost::mapreduce::results                                     result;
    boost::mapreduce::schedule_policy::sequential<wordcount::job> scheduler;
    wordcount::job                                         mr1(datasource);
    mr1.run(scheduler, spec, result);

    std::cout << "\nFinished.";
#else
    std::cout << "\nRunning CPU Parallel MapReduce...";

    boost::mapreduce::specification spec;
    boost::mapreduce::results       result;
    wordcount::job                  mr2(datasource);

    if (argc > 2)
        spec.map_tasks = atoi(argv[2]);

    mr2.run<boost::mapreduce::schedule_policy::cpu_parallel<wordcount::job> >(spec, result);

    std::cout << "\nCPU Parallel MapReduce Finished.";
#endif
    std::cout << std::endl << "\n" << "MapReduce statistics:";
    std::cout << "\n  " << "MapReduce job runtime                     : " << result.job_runtime << " seconds, of which...";
    std::cout << "\n  " << "  Map phase runtime                       : " << result.map_runtime << " seconds";
    std::cout << "\n  " << "  Reduce phase runtime                    : " << result.reduce_runtime << " seconds";
    std::cout << "\n\n  " << "Map:";
    std::cout << "\n    " << "Total Map keys                          : " << result.counters.map_tasks;
    std::cout << "\n    " << "Map keys processed                      : " << result.counters.map_tasks_completed;
    std::cout << "\n    " << "Map key processing errors               : " << result.counters.map_tasks_error;
    std::cout << "\n    " << "Number of Map Tasks run (in parallel)   : " << result.counters.actual_map_tasks;
    std::cout << "\n    " << "Fastest Map key processed in            : " << *std::min_element(result.map_times.begin(), result.map_times.end()) << " seconds";
    std::cout << "\n    " << "Slowest Map key processed in            : " << *std::max_element(result.map_times.begin(), result.map_times.end()) << " seconds";
    std::cout << "\n    " << "Average time to process Map keys        : " << std::accumulate(result.map_times.begin(), result.map_times.end(), boost::int64_t()) / result.map_times.size() << " seconds";

    std::cout << "\n\n  " << "Reduce:";
    std::cout << "\n    " << "Number of Reduce Tasks run (in parallel): " << result.counters.actual_reduce_tasks;
    std::cout << "\n    " << "Number of Result Files                  : " << result.counters.num_result_files;
    std::cout << "\n    " << "Fastest Reduce key processed in         : " << *std::min_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
    std::cout << "\n    " << "Slowest Reduce key processed in         : " << *std::max_element(result.reduce_times.begin(), result.reduce_times.end()) << " seconds";
    std::cout << "\n    " << "Average time to process Reduce keys     : " << std::accumulate(result.reduce_times.begin(), result.reduce_times.end(), boost::int64_t()) / result.map_times.size() << " seconds";

    return 0;
}
