//////////////////////////////////////////////////////////////////////////////
// insert_element_every_n_step.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_INSERT_ELEMENT_EVERY_N_STEP_HPP_ER_2009
#define BOOST_ITERATOR_INSERT_ELEMENT_EVERY_N_STEP_HPP_ER_2009
#include <iterator>
#include <stdexcept>
#include <string>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits.hpp>

namespace boost{


/// Maps a sequence {v[i]:i=0,1,...} to
/// {x,v[0],v[1],...,v[n-1],x,v[n],...,v[2n-1],x,...}
template<typename BaseIter>
class insert_element_every_n_step_iterator
  : public iterator_adaptor<
        insert_element_every_n_step_iterator<BaseIter>,
        BaseIter>
{
 typedef iterator_adaptor<
        insert_element_every_n_step_iterator<BaseIter>,
        BaseIter>                                       super_type;
 public:

    insert_element_every_n_step_iterator(){}

    explicit insert_element_every_n_step_iterator(
        BaseIter i,
        typename iterator_value<super_type>::type x,
        typename super_type::difference_type n
    )
    :base_iter(i),x_(x),n_(n),delta_(-1){}

    //TODO copy constructor?

    typename super_type::difference_type n()const{ return n_; }

    typename iterator_value<super_type>::type element()const{ return x_; }

 private:

    friend class boost::iterator_core_access;

    typename super_type::difference_type
    distance_to(insert_element_every_n_step_iterator const& other) const{
        typedef typename super_type::difference_type d_type;
        d_type d = std::distance(this->base_iter,other.base_iter) / n();
        d = d * (n()+1) + (other.delta_-this->delta_);
        return d;
    }

    bool equal(insert_element_every_n_step_iterator const& other) const
    {
        return (
            (this->base_iter == other.base_iter)
                && (this->delta_ == other.delta_)
                    && (this->x_ == other.x_)
        );
    }

    void advance(typename super_type::difference_type m)
    {
        //TODO simpler computations?
        if(m>0){
            std::size_t k = m/(n()+1);
            std::advance(this->base_iter, k * n());
            delta_ +=  m - ( k * (n()+1) );
            if(delta_ == n()){
                std::advance(this->base_iter, n());
                delta_ = -1;
            }
        }else{
            if(m<0){
                std::string str = "std::advance(";
                str += "insert_element_every_n_step_iterator,k)";
                str += ", k<0 not implemented";
                throw std::runtime_error(str);
            }
        }
    }

    typename super_type::reference dereference() const {
        if(delta_>-1){
            BaseIter i = base_iter;
            std::advance(i,delta_);
            return *i;
        }else{
            return x_;
        };
    };

    void increment(){

        //advance((typename super_type::difference_type)(1));
        ++delta_;
        if(delta_==n()){
            std::advance(this->base_iter,n());
            delta_ = -1;
        }
    }
    //void decrement();

    BaseIter base_iter;
    mutable typename iterator_value<super_type>::type x_;
    typename super_type::difference_type n_;
    typename super_type::difference_type delta_;
};

template<typename BaseIter>
insert_element_every_n_step_iterator<BaseIter>
make_begin_insert_element_every_n_step_iterator(
    BaseIter b,
    typename iterator_value<
        insert_element_every_n_step_iterator<BaseIter> >::type x,
    typename iterator_difference<
        insert_element_every_n_step_iterator<BaseIter> >::type n
){
    return insert_element_every_n_step_iterator<BaseIter>(b,x,n);
};

/// Warning:
///    boo ooo ooo e becomes
///   xbooxoooxooox,
/// and not
///   xbooxoooxoooxe
/// Other situations follow the more logical pattern:
///    boo ooo ooe   becomes
///   xbooxoooxooe  etc.
template<typename BaseIter>
insert_element_every_n_step_iterator<BaseIter>
make_end_insert_element_every_n_step_iterator(
    BaseIter base_b,
    BaseIter base_e,
    typename iterator_value<
        insert_element_every_n_step_iterator<BaseIter> >::type x,
    typename iterator_difference<
        insert_element_every_n_step_iterator<BaseIter> >::type n
){
    typedef insert_element_every_n_step_iterator<BaseIter> iter_type;
    typedef typename iterator_difference<iter_type>::type diff_type;
    diff_type d = std::distance(base_b,base_e);
    std::size_t k = d/n;

    BaseIter base_i = base_b;

    std::advance(base_i, k * n);
    iter_type i(base_i, x, n);
    if(d - k * n>0){
        //TODO maybe throw a warning
        ++i;
        std::advance(i, d - k * n);
    }

    return i;
};

template<typename R>
struct result_of_make_range_insert_element_every_n_step{
    typedef typename range_iterator<R>::type base_type;
    typedef insert_element_every_n_step_iterator<base_type> iter_type;
    typedef iterator_range<iter_type> type;
};

template<typename R>
typename result_of_make_range_insert_element_every_n_step<R>::type
make_range_insert_element_every_n_step(
    R& range,
    typename iterator_value<
        insert_element_every_n_step_iterator<
            typename range_iterator<R>::type
        >
    >::type x,
    typename iterator_difference<
        insert_element_every_n_step_iterator<
            typename range_iterator<R>::type
        >
    >::type n
){
    typedef typename result_of_make_range_insert_element_every_n_step<R>::type
                                                                result_type;

    return result_type(
        make_begin_insert_element_every_n_step_iterator(
            begin(range),
            x,
            n
        ),
        make_end_insert_element_every_n_step_iterator(
            begin(range),end(range),
            x,
            n
        )
    );
};

}
#endif
