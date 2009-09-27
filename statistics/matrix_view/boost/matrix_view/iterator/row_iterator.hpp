///////////////////////////////////////////////////////////////////////////////
// view::iterator::row_iterator.hpp                                   //
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_VIEW_ITERATOR_ROW_ITERATOR_HPP_ER_2009
#define BOOST_VIEW_ITERATOR_ROW_ITERATOR_HPP_ER_2009
#include <iterator>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range.hpp>

namespace boost{
namespace view{

// Should be rename rows_iterator
// Maps a pair of iterators and a stride to an iterator that iterates over
// ranges. In matrix terminology,  ++row_iterator moves down one row.
template<
    typename It,
    typename Categ = typename iterator_category<It>::type,
    typename Diff = typename iterator_difference<It>::type
>
class row_iterator
  : public iterator_adaptor<
        row_iterator<It>
      , iterator_range<It>
      , Categ
      , iterator_range<It>
      , Diff
    >
{
    typedef iterator_facade<
        row_iterator<It>
      , iterator_range<It>
      , Categ
      , iterator_range<It>
      , Diff
    > super_;

 public:
    row_iterator() : super_(){}

    explicit row_iterator(It i,std::size_t stride)
    :super_(),stride_(stride),base_iter(i){}
    
    row_iterator(const row_iterator& that)
    :super_(that),stride_(that.stride_),base_iter(that.base_iter){}
    
    // TODO probably not needed
    row_iterator& operator=(const row_iterator& that){
        if(&that!=this){
            super_::operator=(that);
            stride_ = that.stride_;
            base_iter = that.base_iter;
        }
        return *this;
    }

    It base(){ return base_iter; }
    std::size_t stride()const{ return stride_; }
 private:
    typedef iterator_range<It> Value;

    friend class iterator_core_access;

    void increment() { 
        std::advance(this->base_iter,stride_); 
    }

    Diff distance_to(row_iterator const& other) const{
        Diff d = std::distance(this->base_iter,other.base_iter);
        d /= static_cast<Diff>(this->stride());
        return d;
    };

    bool equal(row_iterator const& other) const
    {
        return this->base_iter == other.base_iter;
    }

    Value dereference() const {
        //Value& inappropriate as result is local
        It i = this->base_iter;
        std::advance(i,stride_);
        return make_iterator_range(this->base_iter,i);
    }

    void advance(Diff n)
    {
        std::advance(this->base_iter, n*stride());
    }

    std::size_t stride_;
    It base_iter;
};

template<typename It>
row_iterator<It>
make_end_row_iterator(It b,It e,std::size_t stride){
    //BOOST_ASSERT(std::distance(b,e)>0);
    BOOST_ASSERT( ( std::distance(b,e)>0 ) || ( std::distance(b,e)==0 ) );
    It ee = e;
    //std::size_t d = (std::size_t)(std::distance(b,ee)-1)/stride;

    std::size_t d = (std::size_t)(std::distance(b,ee))/stride;
    d *= stride;
    //ee = b; std::advance(ee,d+1);
    ee = b; std::advance(ee,d);

   return row_iterator<It>(ee,stride);
}

template<typename It>
row_iterator<It>
make_row_iterator(It b,std::size_t stride){
   return row_iterator<It>(b,stride);
}

}// view
}// boost

#endif
