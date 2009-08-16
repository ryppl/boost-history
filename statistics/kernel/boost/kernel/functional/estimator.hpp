///////////////////////////////////////////////////////////////////////////////
// kernel::functional::rp_visitor.hpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_KERNEL_FUNCTIONAL_DENSITY_VISITOR_H_ER_2009
#define BOOST_KERNEL_FUNCTIONAL_DENSITY_VISITOR_H_ER_2009
#include <algorithm>
#include <boost/call_traits.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_reference.hpp>

#include <boost/mpl/nested_type.hpp>
#include <boost/kernel/functional/detail/mean_accumulator.hpp>

namespace boost{
namespace kernel{

// S:   sequence of training points 
// V:   visitor
// K:   kernel
// A:   accumulator
//
// V                        S
// rp_visitor               {x[i]:i=1,...,n}
// nw_visitor_tuple         {(x,y)[i]:i=1,...,n}
template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A = typename 
        kernel::detail::mean_accumulator<typename K::result_type>::type 
>
class estimator{
    typedef boost::is_reference<S> is_ref_;
    
    public:
    typedef S data_type;
    typedef K kernel_type;
    typedef A accumulator_type;

    template<typename X>
    struct result{
        // Passing X by ref is highly recommended if it is a range,
        // and it won't hurt if X is a scalar
        typedef V<K,const X&,A> arg_;
        typedef typename mpl::nested_type<arg_>::type type;
    };
    
    // Constructor
    estimator();
    estimator(typename call_traits<S>::param_type data,K k);
    estimator(typename call_traits<S>::param_type data,K k,const A&);
    estimator(const estimator&);
    estimator& operator=(const estimator&);
    
    // Evaluate
    template<typename X>
    typename result<X>::type  operator()(const X& x)const;    

    // Access
    typename call_traits<S>::const_reference data()const;
    const K& kernel()const;
    const A& accumulator()const;

private:
    typename call_traits<S>::value_type data_;
    K k_;
    A a_; // Serves and stays as an initialized accumulator
};

// Constructor

template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<S,V,K,A>::estimator():data_(),k_(),a_(){
    BOOST_MPL_ASSERT((mpl::not_<is_ref_>));
}

template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<S,V,K,A>::estimator(typename call_traits<S>::param_type data,K k)
:data_(data),k_(k),a_(){}

template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<S,V,K,A>::estimator(
    typename call_traits<S>::param_type data,
    K k,
    const A& a
)
:data_(data),k_(k),a_(a){}
    
template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<S,V,K,A>::estimator(const estimator& that)
:data_(that.data_),k_(that.k_),a_(that.a_){}
    
template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
estimator<S,V,K,A>& 
estimator<S,V,K,A>::operator=(const estimator& that){
    if(&that=this){
        BOOST_MPL_ASSERT((mpl::not_<is_ref_>));
        data_ = that.data_;
        k_ = that.k_;
        a_ = that.a_;
    }
    return *this;
}

// Evaluate
template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
    template<typename X>
typename estimator<S,V,K,A>::template result<X>::type 
estimator<S,V,K,A>::operator()(const X& x)const{
    typedef typename estimator<S,V,K,A>::template result<X>::type result_;
    return std::for_each(
        begin(data()),
        end(data()),
        result_(kernel(),x,a_)
    );
}

// Access
template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
typename call_traits<S>::const_reference 
estimator<S,V,K,A>::data()const{ return data_; }

template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
const K& estimator<S,V,K,A>::kernel()const{ return k_; }
    
template<
    typename S, 
    template<typename,typename,typename> class V,
    typename K,
    typename A
>
const A&
estimator<S,V,K,A>::accumulator()const{
    return a_;
}
    
}// kernel
}// boost

#endif