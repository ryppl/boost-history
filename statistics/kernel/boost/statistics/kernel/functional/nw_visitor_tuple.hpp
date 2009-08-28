///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::functional::nw_visitor.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_STATISTICS_KERNEL_FUNCTIONAL_TUPLE_NW_VISITOR_HPP_ER_2009
#define BOOST_STATISTICS_KERNEL_FUNCTIONAL_TUPLE_NW_VISITOR_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/call_traits.hpp>
#include <boost/binary_op/functional/untupler.hpp>
#include <boost/binary_op/data/tuple_range.hpp>
#include <boost/statistics/kernel/functional/detail/mean_accumulator.hpp>
#include <boost/statistics/kernel/functional/nw_visitor.hpp>

namespace boost{
namespace statistics{
namespace kernel{

// This functor (f) is like nw_visitor, but rather than f(x,y),
// the appropriate call is f(t), where t=(x,y) is a tuple. 
template<typename K,typename X, typename A = typename 
        statistics::kernel::detail::mean_accumulator<typename K::result_type>::type >
class nw_visitor_tuple 
    : public binary_op::untupler<nw_visitor<K,X,A>,0,1>{
    typedef nw_visitor<K,X,A> binary_;
    typedef binary_op::untupler<binary_,0,1> super_t;

    public:
    typedef binary_ nw_visitor_type;
    typedef typename binary_::rp_visitor_type rp_visitor_type;
    typedef typename super_t::result_type result_type;

    // Construction
    nw_visitor_tuple(
        K k,
        typename call_traits<X>::param_type
    );
    nw_visitor_tuple(
        K k,
        typename call_traits<X>::param_type,
        const A&
    );
    nw_visitor_tuple(const nw_visitor_tuple&);
    nw_visitor_tuple& operator=(const nw_visitor_tuple&);

    // Access
    rp_visitor_type rp_visitor()const;
    nw_visitor_type nw_visitor()const;
    result_type rp_estimate()const;
    result_type nw_estimate()const;
    result_type estimate()const; //same as nw_estimate

    private:
    nw_visitor_tuple();
};

// Construction
template<typename K,typename X,typename A>
nw_visitor_tuple<K,X,A>::nw_visitor_tuple(
    K k,
    typename call_traits<X>::param_type x
):super_t(binary_(k,x)){}

    template<typename K,typename X,typename A>
nw_visitor_tuple<K,X,A>::nw_visitor_tuple(
    K k,
    typename call_traits<X>::param_type x,
    const A& a
):super_t(binary_(k,x,a)){}


template<typename K,typename X,typename A>
nw_visitor_tuple<K,X,A>::nw_visitor_tuple(const nw_visitor_tuple& that)
:super_t(static_cast<const super_t&>(that)){}

template<typename K,typename X,typename A>
typename nw_visitor_tuple<K,X,A>::nw_visitor_tuple& 
nw_visitor_tuple<K,X,A>::nw_visitor_tuple::operator=(
    const nw_visitor_tuple& that
){
    if(&that!=this){
        super_t::operator=(that);
    }
    return *this;
}

// Access

template<typename K,typename X,typename A>
typename nw_visitor_tuple<K,X,A>::nw_visitor_type
nw_visitor_tuple<K,X,A>::nw_visitor()const{
    return this->base();
}

template<typename K,typename X,typename A>
typename nw_visitor_tuple<K,X,A>::rp_visitor_type 
nw_visitor_tuple<K,X,A>::rp_visitor()const{
    return (this->nw_visitor()).rp_visitor();
}

template<typename K,typename X,typename A>
typename nw_visitor_tuple<K,X,A>::result_type 
nw_visitor_tuple<K,X,A>::rp_estimate()const{
    return (this->rp_visitor()).estimate();
}

template<typename K,typename X,typename A>
typename nw_visitor_tuple<K,X,A>::result_type 
nw_visitor_tuple<K,X,A>::nw_estimate()const{
    return (this->nw_visitor()).estimate();
}
    
template<typename K,typename X,typename A>
typename nw_visitor_tuple<K,X,A>::result_type 
nw_visitor_tuple<K,X,A>::estimate()const{
    return (this->nw_estimate());
}
    
}// kernel
}// statistics
}// boost

#endif