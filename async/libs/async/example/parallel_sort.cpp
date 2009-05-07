//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz
//////////////////////////////////////////////////////////////////////////////

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
#include <boost/array.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/adaptor/sliced.hpp>

#include <assert.h>

#define BOOST_PARTS 2
#define NN 400000

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

typedef boost::tp::task< void > task_type;

template <
    typename DirectSolver,
    typename Composer,
    typename AE,
    typename Range
>
  void inplace_solve( AE & ae, boost::sub_range<Range> range, unsigned cutoff );
template <
    typename DirectSolver,
    typename Composer,
    typename AE,
    typename Range
>
  void inplace_solve( AE & ae, 
    boost::sub_range<Range> range, 
    unsigned cutoff)  {
    unsigned size = boost::size(range);
    if ( size <= cutoff) DirectSolver()(range);
    else {
        partition<Range> parts(range, BOOST_PARTS);
        task_type tasks[BOOST_PARTS];
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            task_type tmp(ae.submit(
                boost::bind(
                    &inplace_solve<DirectSolver,Composer,AE,Range>,
                    boost::ref(ae),
                    parts[i],
                    cutoff
            )));
            tasks[i] = tmp;

        }
        inplace_solve<DirectSolver,Composer,AE,Range>(ae, parts[BOOST_PARTS-1], cutoff);
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            tasks[i].result().wait();
        };
        
        Composer()(range);
        
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
template <typename Range>
void parallel_sort(Range& range, unsigned cutoff=10000) {
    pool_type pool( boost::tp::poolsize( 2) );
//    pool_type& pool(boost::this_task::get_thread_pool<pool_type>() );
    boost::sub_range<Range> rng(boost::begin(range), boost::end(range));
    inplace_solve<sort_fct,inplace_merge_fct,pool_type,Range>( pool, rng, cutoff);
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
    //pool_type pool( boost::tp::poolsize( 2) );

    
    for (unsigned i=0; i<NN; ++i) values6[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/2<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/2);
    } 
    assert(boost::equal(values6, sorted));
    {
    std::cout << "parallel_sort "<<NN/2<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/2);
    } 
    
    for (unsigned i=0; i<NN; ++i) values6[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/4<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/4);
    } 
    assert(boost::equal(values6, sorted));
    {
    std::cout << "parallel_sort "<<NN/4<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/4);
    } 
    
    for (unsigned i=0; i<NN; ++i) values6[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/8<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/8);
    } 
    assert(boost::equal(values6, sorted));
    {
    std::cout << "parallel_sort "<<NN/8<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/8);
    } 
    
    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/16<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values5, NN/16);
    }    
    assert(boost::equal(values5, sorted));
    {
    std::cout << "parallel_sort "<<NN/16<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values5, NN/16);
    }    

    for (unsigned i=0; i<NN; ++i) values6[i]=NN-i-1;
    {
    std::cout << "parallel_sort "<<NN/32<<":  reverse 0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/32);
    }    
    assert(boost::equal(values6, sorted));
    {
    std::cout << "parallel_sort "<<NN/32<<":  0.."<<NN;
    scoped_timer tmr;  // start timing
    parallel_sort(values6, NN/32);
    }    

    
    //std::cout << "shutdown"<< std::endl;
    //pool.shutdown();
    std::cout << "end"<< std::endl;
    return 0;
}
