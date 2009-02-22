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

#define STAT
#ifdef STAT
template <typename Range, std::size_t Parts>
class partition
{
public:
    boost::array<boost::sub_range<Range>,Parts> parts_;
    partition(boost::sub_range<Range>& range)
    {
        std::size_t size = boost::size(range);
        parts_[0]=boost::sub_range<Range>(boost::begin(range), boost::begin(range)+(size/Parts));
        for (std::size_t i=1; i< Parts-1; ++i) {
            parts_[i]=boost::sub_range<Range>(boost::begin(range)+i*(size/Parts), boost::begin(range)+(i+1)*(size/Parts));
        }
        parts_[Parts-1]=boost::sub_range<Range>(boost::begin(range)+(Parts-1)*(size/Parts), boost::end(range));
    }
    boost::sub_range<Range>& operator[](unsigned i) {
        return parts_[i];
    }
};
#else
template <typename Range>
class partition
{
    boost::sub_range<Range> range_;
    std::size_t parts_;
public:
    partition(boost::sub_range<Range>& range, std::size_t parts) : range_(range), parts_() {}
    boost::sub_range<Range> operator[](unsigned i) {
        std::size_t size = boost::size(range_);
        if (i==parts_ - 1)  return boost::sub_range<Range>(boost::begin(range_)+i*(size/parts_), boost::end(range_));
        else              return boost::sub_range<Range>(boost::begin(range_)+i*(size/parts_), boost::begin(range_)+((i+1)*(size/parts_)));
    }
};
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
#ifdef STAT
  void seq_( boost::sub_range<Range>& range)
#else
  void seq_( boost::iterator_range<typename boost::range_iterator<Range>::type> range)
#endif
  {
    sort_fct()(range);
  }

  template <typename Range>
#ifdef STAT
  void par_( boost::sub_range<Range>& range)
#else
  void par_( boost::iterator_range<typename boost::range_iterator<Range>::type> range)
#endif
  {
    unsigned size = boost::size(range);
    //std::cout << "<<par_ " << size << std::endl;  
    if ( size <= cutoff_) return seq_<Range>( range);
    else
    {
#if 1
        #define BOOST_PARTS 2
#ifdef STAT
        partition<Range, BOOST_PARTS> parts(range);
#else
//        partition<Range> parts(range, BOOST_PARTS);
#endif
        task_type tasks[BOOST_PARTS];
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            task_type tmp(pool_.submit(
                boost::bind(
                    & x_sort::par_<Range>,
                    boost::ref( * this),
#ifdef STAT
                    boost::ref(parts[i])
#else
                    boost::make_sliced_range(range, i*(size/BOOST_PARTS), ((i+1)*(size/BOOST_PARTS)))
#endif
            )));
            tasks[i] = tmp;

        }
#ifdef STAT
        this->par_(parts[BOOST_PARTS-1]);
#else
        //this->par_(parts[BOOST_PARTS-1]);
        this->par_<Range>(boost::make_sliced_range(range, (BOOST_PARTS-1)*(size/BOOST_PARTS), size));
#endif
        for (unsigned i=0;i < BOOST_PARTS-1; ++i) {
            tasks[i].wait();
        };
        
#else
        boost::sub_range<Range> left(boost::begin(range), boost::begin(range)+(size/2));
        boost::sub_range<Range> right(boost::begin(range)+(size/2), boost::end(range));
        // fork a new sub-action t1 in pool
        task_type task(
            pool_.submit(
                boost::bind(
                    & x_sort::par_<Range>,
                    boost::ref( * this),
                    boost::ref(left))
            )
        );

        this->par_<Range>(right);
        task.wait();
#endif
    //std::cout << "par_inplace_merge_fct " << size << ">>"<< std::endl;  
        inplace_merge_fct()(range, boost::begin(range)+(size/2));
    //std::cout << "par_ " << size << ">>"<< std::endl;  
        
    }
  }
public:
  x_sort( pool_type & pool, unsigned cutoff)
  : pool_( pool), cutoff_( cutoff)
  {}

  template <typename Range>
  void execute( Range& range) {
#ifdef STAT
    boost::sub_range<Range> rng(boost::begin(range), boost::end(range));
#else
    boost::iterator_range<typename boost::range_iterator<Range>::type> rng(range);
#endif    
    par_<Range>( rng);
  }
};

template <typename Range>
void parallel_sort(Range& range, unsigned cutoff=10000) {
    pool_type pool( boost::tp::poolsize( 2) );
    x_sort fct( pool, cutoff);
    fct.execute(range);
    std::cout << "parallel_sort " << ">>"<< std::endl;  
}

#define NN 1000000
int sorted[NN];
int values1[NN];
int values2[NN];
int values3[NN];
int values4[NN];
int values5[NN];
int values6[NN];

int main() {
    //pool_type ae(boost::tp::poolsize(2));
    for (unsigned i=0; i<NN; ++i) sorted[i]=i;

    
   
    for (unsigned i=0; i<NN; ++i) values1[i]=NN-i-1;
    {
    std::cout << "std::sort: reverse 0.." << NN << std::endl;
    boost::progress_timer t;  // start timing
    std::sort(boost::begin(values1), boost::end(values1));
    }
    assert(boost::equal(values1, sorted));
    
    for (unsigned i=0; i<NN; ++i) values2[i]=NN-i-1;
    {
    std::cout << "boost::sort: reverse 0.."<<NN << std::endl;
    boost::progress_timer t;  // start timing
    boost::sort(values2);
    }
    assert(boost::equal(values2, sorted));

    // creates a threadpool with two worker-threads
    pool_type pool( boost::tp::poolsize( 2) );


    for (unsigned i=0; i<NN; ++i) values3[i]=NN-i-1;
    std::cout << "parallel_sort "<<NN/16<<":  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values3, NN/16);
    }
    
    //for (unsigned i=0; i<NN; ++i) std::cout << sorted[i] << " " <<values3[i] << std::endl;
    assert(boost::equal(values3, sorted));

#if 0
    for (unsigned i=0; i<NN; ++i) values3[i]=NN-i-1;
    std::cout << "parallel_sort 8000:  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values3);
    }
    for (unsigned i=0; i<NN; ++i) values[i]=NN-i-1;
    std::cout << "parallel_sort 4000:  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values, 4000);
    }
    
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

    for (unsigned i=0; i<NN; ++i) values4[i]=NN-i-1;
    {
    std::cout << "std::sort: reverse 0.." << NN << std::endl;
    boost::progress_timer t;  // start timing
    std::sort(boost::begin(values4), boost::end(values4));
    }
    assert(boost::equal(values4, sorted));
    
    for (unsigned i=0; i<NN; ++i) values5[i]=NN-i-1;
    std::cout << "parallel_sort "<<NN/16<<":  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values5, NN/16);
    }
    
    //for (unsigned i=0; i<NN; ++i) std::cout << sorted[i] << " " <<values3[i] << std::endl;
    assert(boost::equal(values5, sorted));

    for (unsigned i=0; i<NN; ++i) values6[i]=NN-i-1;
    std::cout << "parallel_sort "<<NN/16<<":  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values6, NN/16);
    }
    
    //for (unsigned i=0; i<NN; ++i) std::cout << sorted[i] << " " <<values3[i] << std::endl;
    assert(boost::equal(values6, sorted));

    for (unsigned i=0; i<NN; ++i) values1[i]=NN-i-1;
    std::cout << "parallel_sort "<<NN/16<<":  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values1, NN/16);
    }
    
    //for (unsigned i=0; i<NN; ++i) std::cout << sorted[i] << " " <<values3[i] << std::endl;
    assert(boost::equal(values1, sorted));
    
#if 0
    for (unsigned i=0; i<NN; ++i) values[i]=NN-i-1;
    {
    std::cout << "boost::sort: reverse 0.."<<NN << std::endl;
    boost::progress_timer t;  // start timing
    boost::sort(values);
    }
    assert(boost::equal(values, sorted));
    
    for (unsigned i=0; i<NN; ++i) values[i]=NN-i-1;
    std::cout << "parallel_sort "<<NN/32<<":  reverse 0.."<<NN << std::endl;
    {
    boost::progress_timer tmr;  // start timing
    parallel_sort(values, NN/32);
    }
    
    //for (unsigned i=0; i<NN; ++i) std::cout << sorted[i] << " " <<values[i] << std::endl;
    assert(boost::equal(values, sorted));
    //std::cout << "sleep"<<std::endl;
#endif    

    //boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    std::cout << "shutdown"<< std::endl;
    pool.shutdown();
    std::cout << "end"<< std::endl;
    return 0;
}
