//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#  pragma warning (disable : 4535) // calling _set_se_translator() requires /EHa
#endif

//~ define BOOST_TRANSACT_PERF_THREADS equal to the number of threads if test with threads

#ifndef BOOST_TRANSACT_PERF_THREADS
#define BOOST_TRANSACT_PERF_THREADS 2
#endif
#ifndef BOOST_TRANSACT_PERF_NUMX
#define BOOST_TRANSACT_PERF_NUMX 100*1000
#endif
#ifndef BOOST_TRANSACT_PERF_ITER
#define BOOST_TRANSACT_PERF_ITER 1
#endif
#ifndef BOOST_TRANSACT_PERF_STEP
#define BOOST_TRANSACT_PERF_STEP 10
#endif

#if BOOST_TRANSACT_PERF_THREADS>0
#define SINGLEX "threads"
#else
#define SINGLEX "no_threads"
#endif

#if BOOST_TRANSACT_PERF_THREADS>1
#define MTHREADX "multi"
#define BOOST_TRANSACT_PERF_MTHREAD true
#else
#define MTHREADX "single"
#define BOOST_TRANSACT_PERF_MTHREAD false
#endif

#include <boost/transact/simple_transaction_manager.hpp>
#include <boost/mpl/empty_sequence.hpp>

using namespace boost;
using namespace transact;

struct my_rm{
    typedef int transaction;
    typedef mpl::empty_sequence services;
    struct tag{};
    transaction begin_root_transaction(){ return 0; }
    transaction begin_nested_transaction(transaction){ return 0; }
    void commit_transaction(transaction){}
    bool finish_transaction(transaction){ return false; }
    void rollback_transaction(transaction){}
};

#define BOOST_TRANSACT_CONFIGURATION simple_transaction_manager<my_rm,BOOST_TRANSACT_PERF_MTHREAD>

#include <boost/transact/language.hpp>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <stdlib.h>

#include <boost/chrono/stopwatches.hpp>
#include <boost/test/minimal.hpp>

using namespace std;
using namespace boost;


int counter;
void act_1(int max) {
    counter=0;
    //~ chrono::stopwatch_reporter<chrono::stopwatch<> > _;

        for (int i=0; i< max; ++i) {
            //counter+=i;
        }
    //std::cout << "0/0 TX " << max << " " << counter <<std::endl;
}

void act_2(int max) {

    counter=0;
    chrono::stopwatch_reporter<chrono::stopwatch<> > _;

        for (int i=0; i< max; ++i) {
            begin_transaction {
                //counter+=i;
            } end_transaction;
        }
    std::cout << "N/0 TX "<< max << " " << counter <<std::endl;
}

void act_3(int max) {

    counter=0;
    chrono::stopwatch_reporter<chrono::stopwatch<> > _;

    begin_transaction {
        for (int i=0; i< max; ++i) {
            begin_transaction {
                //counter+=i;
            } end_transaction;
        }
    } end_transaction;
    std::cout << "1/N TX "<< max << " " << counter <<std::endl;
}

void act_4(int max) {

    counter=0;
    chrono::stopwatch_reporter<chrono::stopwatch<> > _;

    begin_transaction {
        for (int i=0; i< max; ++i) {
            //counter+=i;
        }
    } end_transaction;
    std::cout << "1/0 TX "<< max << " " << counter <<std::endl;
}


void test_1() {

    int max=BOOST_TRANSACT_PERF_NUMX;
    int iter=BOOST_TRANSACT_PERF_ITER;
    int step=BOOST_TRANSACT_PERF_STEP;

    std::cout << "MT("<<BOOST_TRANSACT_PERF_THREADS<<")" << std::endl;
{
    //std::cout << "*****" << std::endl;
    int j=max;
    for (int i=0; i< iter; ++i) {
        //std::cout << "Main" << std::endl;
        act_1(j);

#if BOOST_TRANSACT_PERF_THREADS>0
        //std::cout << "Single" << std::endl;
        thread  th(bind(act_1, j));
        th.join();

#endif

#if BOOST_TRANSACT_PERF_THREADS>1
        //std::cout << "Multi" << std::endl;
        thread  th1(bind(act_1, j));
        thread  th2(bind(act_1, j));
        th1.join();
        th2.join();
#endif
        j*=step;
    };
}
{
    int j=max;
    //~ std::cout << "*****" << std::endl;
    for (int i=0; i< iter; ++i) {
        //~ std::cout << "Main" << std::endl;
        act_1(j);

#if BOOST_TRANSACT_PERF_THREADS>0
        //~ std::cout << "Single" << std::endl;
        thread  th(bind(act_1, j));
        th.join();

#endif
#if BOOST_TRANSACT_PERF_THREADS>1
        //~ std::cout << "Multi" << std::endl;
        thread  th1(bind(act_1, j));
        thread  th2(bind(act_1, j));
        th1.join();
        th2.join();
#endif
        j*=step;
    };
}
{
    int j=max;
    std::cout << "*****" << std::endl;
    for (int i=0; i< iter; ++i) {
        std::cout << "Main" << std::endl;
        act_2(j);

#if BOOST_TRANSACT_PERF_THREADS>0
        std::cout << "Single" << std::endl;
        thread  th(bind(act_2, j));
        th.join();
#endif

#if BOOST_TRANSACT_PERF_THREADS>1
        std::cout << "Multi" << std::endl;
        thread  th1(bind(act_2, j));
        thread  th2(bind(act_2, j));
        th1.join();
        th2.join();
#endif
        j*=step;
    };
}
{
    int j=max;
    std::cout << "*****" << std::endl;
    for (int i=0; i< iter; ++i) {
        std::cout << "Main" << std::endl;
        act_4(j);

#if BOOST_TRANSACT_PERF_THREADS>0
        std::cout << "Single" << std::endl;
        thread  th(bind(act_4, j));
        th.join();
#endif

#if BOOST_TRANSACT_PERF_THREADS>1
        std::cout << "Multi" << std::endl;
        thread  th1(bind(act_4, j));
        thread  th2(bind(act_4, j));
        th1.join();
        th2.join();
#endif
        j*=step;
    };
}
{
    int j=max;
    std::cout << "*****" << std::endl;
    for (int i=0; i< iter; ++i) {
        std::cout << "Main" << std::endl;
        act_3(j);

#if BOOST_TRANSACT_PERF_THREADS>0
        std::cout << "Single" << std::endl;
        thread  th(bind(act_3, j));
        th.join();

#if BOOST_TRANSACT_PERF_THREADS>1
        std::cout << "Multi" << std::endl;
        thread  th1(bind(act_3, j));
        thread  th2(bind(act_3, j));
        th1.join();
        th2.join();
#endif
#endif
        j*=step;
    };
}

{
    int j=max;
    std::cout << "*****" << std::endl;
    for (int i=0; i< iter; ++i) {
        std::cout << "Main" << std::endl;
        act_3(j);

#if BOOST_TRANSACT_PERF_THREADS>0
        std::cout << "Single" << std::endl;
        thread  th(bind(act_3, j));
        th.join();
#endif

#if BOOST_TRANSACT_PERF_THREADS>0
        std::cout << "Multi" << std::endl;
        thread  th1(bind(act_3, j));
        thread  th2(bind(act_3, j));
        th1.join();
        th2.join();
#endif
        j*=step;
    };
}

}

int test_main(int, char *[]){
    test_1();

    return 0;

}

