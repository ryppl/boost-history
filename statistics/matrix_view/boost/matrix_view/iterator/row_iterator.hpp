///////////////////////////////////////////////////////////////////////////////
// matrix_view::iterator::row_iterator.hpp                                   //
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MATRIX_VIEW_ITERATOR_ROW_ITERATOR_HPP_ER_2009
#define BOOST_MATRIX_VIEW_ITERATOR_ROW_ITERATOR_HPP_ER_2009
#include <iterator>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range.hpp>

namespace boost{
namespace matrix_view{

// Maps a pair of iterators and a stride to an iterator that iterates over
// ranges. In matrix terminology,  ++row_iterator moves down one row.
template<
    typename BaseIter,
    typename Categ = typename iterator_category<BaseIter>::type,
    typename Diff = typename iterator_difference<BaseIter>::type>
class row_iterator
  : public iterator_facade<
        row_iterator<BaseIter>
      , iterator_range<BaseIter>
      , Categ
      , iterator_range<BaseIter>
      , Diff
    >
{
 public:
    row_iterator(){}

    explicit row_iterator(BaseIter i,std::size_t stride)
    :stride_(stride),base_iter(i){}
    row_iterator(const row_iterator& that)
    :stride_(that.stride_),base_iter(that.base_iter){}
    row_iterator& operator=(const row_iterator& that){
        if(&that!=this){
            stride_ = that.stride_;
            base_iter = that.base_iter;
        }
        return *this;
    }

    BaseIter base(){return base_iter;}
    std::size_t stride()const{return stride_;}
 private:
    typedef iterator_range<BaseIter> Value;

    friend class iterator_core_access;

    void increment() { std::advance(this->base_iter,stride_); }

    Diff distance_to(row_iterator const& other) const{
        Diff d = std::distance(this->base_iter,other.base_iter);
        d/=(Diff)(stride());
        return d;
    };

    bool equal(row_iterator const& other) const
    {
        return this->base_iter == other.base_iter;
    }

    Value dereference() const {
        //Value& inappropriate as result is local
        BaseIter i = this->base_iter;
        std::advance(i,stride_);
        return make_iterator_range(this->base_iter,i);
    }

    void advance(Diff n)
    {
        std::advance(this->base_iter, n*stride());
    }

    std::size_t stride_;
    BaseIter base_iter;
};

template<typename BaseIter>
row_iterator<BaseIter>
make_end_row_iterator(BaseIter b,BaseIter e,std::size_t stride){
    //BOOST_ASSERT(std::distance(b,e)>0);
    BOOST_ASSERT( ( std::distance(b,e)>0 ) || ( std::distance(b,e)==0 ) );
    BaseIter ee = e;
    //std::size_t d = (std::size_t)(std::distance(b,ee)-1)/stride;

    std::size_t d = (std::size_t)(std::distance(b,ee))/stride;
    d *= stride;
    //ee = b; std::advance(ee,d+1);
    ee = b; std::advance(ee,d);

   return row_iterator<BaseIter>(ee,stride);
}

template<typename BaseIter>
row_iterator<BaseIter>
make_row_iterator(BaseIter b,std::size_t stride){
   return row_iterator<BaseIter>(b,stride);
}

}// matrix_view
}// boost

#endif
