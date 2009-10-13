//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz
//////////////////////////////////////////////////////////////////////////////

// requires interthread and range available at boost-sandbox and boost-vault

#include <iostream>
#include <algorithm>

#include <boost/progress.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/tp/pool.hpp>
#include <boost/tp/unbounded_channel.hpp>
#include <boost/tp/fifo.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/inplace_merge.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/interthreads/fork.hpp>
#include <boost/interthreads/algorithm/wait.hpp>
#include <boost/interthreads/scheduler.hpp>
#include <boost/array.hpp>

#include <assert.h>

#define BOOST_PARTS 2
#define NN 10000000

class scoped_timer {
    boost::posix_time::ptime start_;
public:    
    scoped_timer() 
		: start_(boost::posix_time::microsec_clock::universal_time())
    {}
    ~scoped_timer() {
		boost::posix_time::ptime stop( boost::posix_time::microsec_clock::universal_time() );
		std::cout << " " << ( stop - start_).total_milliseconds() << " milli seconds" << std::endl;
    }
};

template <typename Range>
class partition
{
public:
    boost::iterator_range<typename boost::range_iterator<Range>::type> range_;
    std::size_t parts_;
    partition(boost::iterator_range<typename boost::range_iterator<Range>::type>& range, std::size_t parts):
        range_(range),
        parts_(parts)
    {}
    boost::iterator_range<typename boost::range_iterator<Range>::type> operator[](unsigned i) {
        unsigned size = boost::size(range_);
        if (i<(parts_-1))
            return boost::make_sliced_range(range_, i*(size/parts_), ((i+1)*(size/parts_)));
        else 
            return boost::make_sliced_range(range_, (parts_-1)*(size/parts_), size);
    }
};

typedef boost::tp::pool<
  boost::tp::unbounded_channel< boost::tp::fifo >
> pool_type;

#ifdef TASK_POOL
        typedef boost::tp::task< pool_type,  void > task_type;
#else
        typedef boost::tp::task< void > task_type;
#endif


template <
    typename DirectSolver,
    typename Composer,
    typename AE,
    typename Range
>
  void inplace_solve( AE & ae, 
        boost::iterator_range<typename boost::range_iterator<Range>::type> range, 
        unsigned cutoff );
        
template <
    typename DirectSolver,
    typename Composer,
    typename AE,
    typename Range
>
  void inplace_solve( AE & ae, 
        boost::iterator_range<typename boost::range_iterator<Range>::type> range, 
        unsigned cutoff )
  {
    unsigned size = boost::size(range);
    //std::cout << "<<par_ " << size;  
    if ( size <= cutoff) DirectSolver()(range);
    else {
        partition<Range> parts(range, BOOST_PARTS);
        std::list<task_type> tasks;
        #if 0 // this code do not compiles with gcc 3.4.4 cygwin
        boost::transform(parts, boost::begin(tasks), 
                          boost::bind(&AE::submit, boost::ref(ae),
                          //boost::bind(&boost::interthreads::fork<AE>, boost::ref(ae),
                                      boost::bind(&inplace_solve<DirectSolver,Composer,AE,Range>, boost::ref(ae),_1,cutoff)));
        #else
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            task_type tmp(ae.submit(
                boost::bind(
                    &inplace_solve<DirectSolver,Composer,AE,Range>,
                    boost::ref(ae),
                    parts[i],
                    cutoff
            )));
            tasks.push_back(tmp);
        }
        #endif
        inplace_solve<DirectSolver,Composer,AE,Range>(ae, parts[BOOST_PARTS-1], cutoff);
        boost::for_each(tasks, &boost::interthreads::wait_act<task_type>);
        
        //std::cout << "par_inplace_merge_fct " << size << ">>"<< std::endl;  
        Composer()(range);
        //std::cout << "par_ " << size << ">>"<< std::endl;  
        
    }
  }

struct sort_fct {
    template<class RandomAccessRange>
    RandomAccessRange& operator()(RandomAccessRange rng) {
        return boost::sort(rng);
    }
};

struct inplace_merge_fct {
    template<class BidirectionalRange>
    BidirectionalRange&
    operator()( BidirectionalRange rng) {
        return boost::inplace_merge(rng, boost::begin(rng)+(boost::size(rng)/2));
    }
};
template <typename AE, typename Range>
void parallel_sort(AE& ae, Range& range, unsigned cutoff=10000) {
    boost::iterator_range<typename boost::range_iterator<Range>::type> rng(range);
    inplace_solve<sort_fct,inplace_merge_fct,pool_type,Range>( ae, rng, cutoff);
}

int sorted[NN];
int values1[NN];
int values2[NN];
int values3[NN];
int values4[NN];
int values5[NN];
int values6[NN];

int main() {
    for (unsigned i=0; i<NN; ++i) sorted[i]=i;
   
    for (unsigned i=0; i<NN; ++i) values1[i]=NN-i-1;
    {
    std::cout << "std::sort: reverse 0.." << NN;
    scoped_timer t;  // start timing
    std::sort(boost::begin(values1), boost::end(values1));
    }
    assert(boost::equal(values1, sorted));
    {
    std::cout << "std::sort: 0.." << NN;
    scoped_timer t;  // start timing
    std::sort(boost::begin(values1), boost::end(values1));
    }
    
    for (unsigned i=0; i<NN; ++i) values2[i]=NN-i-1;
    {
    std::cout << "boost::sort: reverse 0.."<<NN;
    scoped_timer t;  // start timing
    boost::sort(values2);
    }
    assert(boost::equal(values2, sorted));
    {
    std::cout << "boost::sort: 0.."<<NN;
    scoped_timer t;  // start timing
    boost::sort(values2);
    }

    // creates a threadpool with two worker-threads
    pool_type pool( boost::tp::poolsize( 2) );

// 	// creates a threadpool with as many worker-threads
// 	// as CPUs/Cores are online and bind each worker-thread
// 	// to a specific CPU/Core
// 	// requires BOOST_BIND_WORKER_TO_PROCESSORS to be specified
// 	pool_type pool;

    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/2<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/2);
    }   
    assert(boost::equal(values5, sorted));
    {
    std::cout << "parallel_sort "<<NN/2<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/2);
    }   

    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/4<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/4);
    }   
    assert(boost::equal(values5, sorted));
    {
    std::cout << "parallel_sort "<<NN/4<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/4);
    }   

    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/8<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/8);
    }   
    assert(boost::equal(values5, sorted));
    {
    std::cout << "parallel_sort "<<NN/8<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/8);
    }   

    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/16<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/16);
    }   
    assert(boost::equal(values5, sorted));
    {
    std::cout << "parallel_sort "<<NN/16<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/16);
    }   

    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/32<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/32);
    }   
    assert(boost::equal(values5, sorted));
    {
    std::cout << "parallel_sort "<<NN/32<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(pool, values5, NN/32);
    }   

    
    std::cout << "shutdown"<< std::endl;
    pool.shutdown();
    std::cout << "end"<< std::endl;
    return 0;
}
