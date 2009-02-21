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

#include <iostream>
#include <algorithm>

#include <boost/progress.hpp>
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

template <typename Range, std::size_t Parts>
class partition;

#if 0
template <typename Range>
class partition<Range,2>
{
        boost::sub_range<Range> p0;
        boost::sub_range<Range> p1;
public:
    partition(Range& range):
        : p0(boost::begin(range), boost::begin(range)+(size/2))
        , p1(boost::begin(range)+(size/2)+1, boost::end(range))
    {}
};

#endif

template <typename Range, std::size_t Parts>
class partition
{
public:
    boost::array<boost::sub_range<Range>,Parts> parts;
    partition(boost::sub_range<Range>& range)
    {
        std::size_t size = boost::size(range);
        parts[0]=boost::sub_range<Range>(boost::begin(range), boost::begin(range)+(size/Parts));
        for (std::size_t i=1; i< Parts-1; ++i) {
            parts[i]=boost::sub_range<Range>(boost::begin(range)+i*(size/Parts)+1, boost::begin(range)+(i+1)*(size/Parts)+1);
        }
        parts[Parts-1]=boost::sub_range<Range>(boost::begin(range)+(Parts-1)*(size/Parts)+1, boost::end(range));
    }
};



#define X_SORT
#define COUT

#ifdef COUT
boost::mutex cout_sync;
#endif
typedef boost::tp::pool<
  boost::tp::unbounded_channel< boost::tp::fifo >
> pool_type;

#ifdef TASK_POOL
        typedef boost::tp::task< pool_type,  void > task_type;
#else
        typedef boost::tp::task< void > task_type;
#endif


struct sort_fct {
    template<class RandomAccessRange>
    RandomAccessRange& operator()(RandomAccessRange& rng) {
        return boost::sort(rng);
    }
};

struct inplace_merge_fct {
    template<class BidirectionalRange>
    BidirectionalRange&
    operator()( BidirectionalRange& rng,
                   typename boost::range_iterator<BidirectionalRange>::type middle ) {
        return boost::inplace_merge(rng, middle);
    }
};

class x_sort
{
private:
  pool_type &  pool_;
  unsigned    cutoff_;

  template <typename Range>
  void seq_( boost::sub_range<Range>& range)
  {
    sort_fct()(range);
  }

  template <typename Range>
  void par_( boost::sub_range<Range>& range)
  {
    unsigned size = boost::size(range);
    if ( size <= cutoff_) return seq_( range);
    else
    {
        #if 1
        #define BOOST_PARTS 2
        partition<Range, BOOST_PARTS> parts(range);
        //boost::array<task_type, BOOST_PARTS> tasks;
        task_type tasks[BOOST_PARTS];
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            task_type tmp(pool_.submit(
                boost::bind(
                    & x_sort::par_<Range>,
                    boost::ref( * this),
                    boost::ref(parts.parts[i]))
            ));
            tasks[i] = tmp;

        }
        this->par_(parts.parts[BOOST_PARTS-1]);
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            tasks[i].wait();
        };
        
        #else
        boost::sub_range<Range> left(boost::begin(range), boost::begin(range)+(size/2));
        boost::sub_range<Range> right(boost::begin(range)+(size/2)+1, boost::end(range));
        // fork a new sub-action t1 in pool
        task_type task(
            pool_.submit(
                boost::bind(
                    & x_sort::par_<Range>,
                    boost::ref( * this),
                    boost::ref(left))
            )
        );

        this->par_(right);
        task.wait();
        #endif
        inplace_merge_fct()(range, boost::begin(range)+(size/2));
    }
  }
public:
  x_sort( pool_type & pool, unsigned cutoff)
  : pool_( pool), cutoff_( cutoff)
  {}

  template <typename Range>
  void execute( boost::sub_range<Range>& n)
  {
        //std::cout << "execute("<<boost::size(n)<<") >>" << std::endl;
    par_( n);
        //std::cout << "execute("<<boost::size(n)<<") >>" << std::endl;
  }
};

template <typename Range>
void parallel_sort(Range& range, unsigned cutoff=10000) {
    pool_type pool( boost::tp::poolsize( 2) );
    boost::sub_range<Range> rng(boost::begin(range), boost::end(range));
    x_sort fct( pool, cutoff);
    fct.execute(rng);
}

#define NN 500000


int main() {
    //pool_type ae(boost::tp::poolsize(2));
    int values[NN];

    for (unsigned i=NN-1; i>0; --i) values[i]=NN-i;
    std::cout << "std::sort: reverse 0.." << NN << std::endl;
    {
    boost::progress_timer t;  // start timing
    std::sort(values, values+NN);
    }
    
    for (unsigned i=NN-1; i>0; --i) values[i]=NN-i;
    std::cout << "boost::sort: reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer t;  // start timing
    boost::sort(values);
    }

    // creates a threadpool with two worker-threads
    pool_type pool( boost::tp::poolsize( 2) );

#if 0
    for (unsigned i=NN-1; i>0; --i) values[i]=NN-i;
    std::cout << "parallel_sort 4000:  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values, 4000);
    }
    
#endif

    for (unsigned i=NN-1; i>0; --i) values[i]=NN-i;
    std::cout << "parallel_sort 8000:  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values);
    }

    for (unsigned i=NN-1; i>0; --i) values[i]=NN-i;
    std::cout << "parallel_sort 16000:  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values, 16000);
    }
    
#if 0
    std::cout << "std::sort: 0.." << NN << std::endl;
    {
    boost::progress_timer t;  // start timing
    std::sort(values, values+NN);
    }
    std::cout << "boost::sort: 0.."<<NN << std::endl;
    {
    boost::progress_timer t;  // start timing
    boost::sort(values);
    }
    std::cout << "parallel_sort:  0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values);
    }
#endif
    
    pool.shutdown();
    return 0;
}
