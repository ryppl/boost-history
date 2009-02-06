//////////////////////////////////////////////////////////////////////////////
// skip_range_iterator.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_SKIP_RANGE_ITERATOR_HPP_ER2009
#define BOOST_ITERATOR_SKIP_RANGE_ITERATOR_HPP_ER2009
#include <iterator>
#include <stdexcept>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
namespace boost{

/// Traverses all elements of a sequence except a specified
/// range
template<typename BaseIter>
class skip_range_iterator
  : public iterator_facade<
        skip_range_iterator<BaseIter>
      , typename iterator_value<BaseIter>::type
      , typename iterator_category<BaseIter>::type
    >
{
    typedef iterator_range<BaseIter>                    iterator_range_type;
    typedef iterator_facade<
        skip_range_iterator<BaseIter>
      , typename iterator_value<BaseIter>::type
      , typename iterator_category<BaseIter>::type
    >                                                   super_type;

 public:
    typedef typename super_type::difference_type        difference_type;
    typedef typename super_type::reference              reference;

    skip_range_iterator(){}

    explicit skip_range_iterator(
        BaseIter i,
        iterator_range_type skip_range
    )
    :base_iter(i),skip_range_(skip_range){
        if(is_in_skip_range()){
            throw std::runtime_error(
                "skip_range_iterator: is_in_skip_range at construction"
            );
        }
    }

 private:

    friend class boost::iterator_core_access;

    void increment() { advance((difference_type)(1)); }

    difference_type distance_to(skip_range_iterator const& other) const{
        difference_type d = std::distance(this->base_iter,other.base_iter);
        difference_type dist_this
            = std::distance(this->base_iter,begin(skip_range_));
        difference_type dist_other
            = std::distance(other.base_iter,begin(skip_range_));
        bool same_sign = ((dist_this<0) && (dist_other<0))
            || ((dist_this>0) && (dist_other>0))
            || (size(skip_range_)==0);
        if(!same_sign){
            if(d<(difference_type)(0)){
                d+=(difference_type)(boost::size(skip_range_));
            }else{
                d-=(difference_type)(boost::size(skip_range_));
            }
        }
        return d;
    }

    bool equal(skip_range_iterator const& other) const
    {
        return this->base_iter == other.base_iter;
    }

    reference dereference() const { return *base_iter; }

    void advance(difference_type n)
    {
        difference_type dist_before
            = std::distance(this->base_iter,begin(skip_range_));
        std::advance(this->base_iter,n);

        if(size(skip_range_)>0){
            if(dist_before>0){
                difference_type dist_after
                    = std::distance(this->base_iter,begin(skip_range_));
                if(dist_after<=0){
                    std::advance(this->base_iter,size(skip_range_));
                }
            }
        }

        BOOST_ASSERT(!is_in_skip_range());
    }

    bool is_in_skip_range()const{
        bool res = false;
        if(size(skip_range_)>0){
            difference_type d_to_first
                = std::distance(this->base_iter,begin(skip_range_));
            if(d_to_first<=0){
                difference_type d_to_end
                    = std::distance(this->base_iter,end(skip_range_));
                if(d_to_end>0){
                    res = true;
                }
            }
        }
        return res;
    }
    BaseIter base_iter;
    iterator_range_type skip_range_;
};

}
#endif
