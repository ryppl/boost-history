//////////////////////////////////////////////////////////////////////////////
// skip_one_step_iterator.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_SKIP_ONE_STEP_ITERATOR_HPP_ER2009
#define BOOST_ITERATOR_SKIP_ONE_STEP_ITERATOR_HPP_ER2009
#include <iterator>
#include <stdexcept>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
namespace boost{

/// Traverses elements of a sequences, skipping a subsequence
template<typename BaseIter>
class skip_one_step_iterator
  : public iterator_adaptor<
        skip_one_step_iterator<BaseIter>,
        BaseIter>
{
 typedef iterator_adaptor<
        skip_one_step_iterator<BaseIter>,
        BaseIter>                                       super_type;
 public:

    skip_one_step_iterator(){}

    explicit skip_one_step_iterator(
        BaseIter i,
        BaseIter skip
    )
    :base_iter(i),skip_(skip){
        if(base_is_skip()){
            throw std::runtime_error(
                "skip_one_step_iterator: base_is_skip at construction"
            );
        }
    }

    //TODO copy constructor?

 private:

    friend class boost::iterator_core_access;

    typename super_type::difference_type
    distance_to(skip_one_step_iterator const& other) const{
        typedef typename super_type::difference_type difference_type;

        difference_type d = std::distance(this->base_iter,other.base_iter);
        difference_type dist_this
            = std::distance(this->base_iter,skip_);
        difference_type dist_other
            = std::distance(other.base_iter,skip_);
        bool same_sign = ((dist_this<0) && (dist_other<0))
            || ((dist_this>0) && (dist_other>0));
        if(!same_sign){
            if(d<(difference_type)(0)){
                d+=(difference_type)(1);
            }else{
                d-=(difference_type)(1);
            }
        }
        return d;
    }

    bool equal(skip_one_step_iterator const& other) const
    {
        return this->base_iter == other.base_iter;
    }

    void advance(typename super_type::difference_type n)
    {
        std::advance(this->base_iter,1);
        if(base_is_skip()){
            std::advance(this->base_iter,1);
        }
    }

    typename super_type::reference dereference() const {
        return *base_iter; }

    void increment(){
        advance((typename super_type::difference_type)(1)); }
    //void decrement();

    bool base_is_skip()const{
        return ((this->base_iter)==(this->skip_)); }
    BaseIter base_iter;
    BaseIter skip_;
};

template<typename Iter>
skip_one_step_iterator<Iter>
make_skip_one_step_begin_iterator(Iter b,Iter skip){
    Iter new_b = b;
    if(new_b == skip){
        std::advance(new_b,1); }
    return skip_one_step_iterator<Iter>(new_b,skip);
};

template<typename Iter>
skip_one_step_iterator<Iter>
make_skip_one_step_end_iterator(Iter e,Iter skip){
    // constructor below will throw if skip == e
    return skip_one_step_iterator<Iter>(e,skip);
};



}
#endif
