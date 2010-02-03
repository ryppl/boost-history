//////////////////////////////////////////////////////////////////////////////
// assign::cref_list_of2.cpp                               					//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_CREF_LIST_OF2_ER_2010_HPP
#define BOOST_ASSIGN_CREF_LIST_OF2_ER_2010_HPP
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/type_traits.hpp>
#include <boost/ref.hpp>
#include <boost/array.hpp>
#include <boost/range.hpp>

namespace boost{
namespace assign{

// Usage : vec = cref_list2_of(a)(b)(c)

// The idea of this class was developed in collaboration with P.M.

template<
	typename T,
	int N, 
    typename B
>
class cref_impl : B{
        
    typedef boost::reference_wrapper<const T> ref_;

	typedef cref_impl<T,N,B> this_;

	typedef typename boost::mpl::plus<
    	boost::mpl::int_<N>,
        boost::mpl::int_<1>
    >::type incr_n_;

    template<typename T1>
    struct next{ typedef cref_impl<T1,incr_n_::value,this_> type; };

	typedef boost::mpl::bool_<false> false_;
	typedef boost::mpl::bool_<true> true_;

	template<typename T1>
    struct array{ typedef boost::array<T1,N+1> type; };
    
	typedef typename boost::is_same<
    	boost::mpl::int_<N>,
    	boost::mpl::int_<0>
    >::type exit_impl_;

	public: 

	typedef boost::mpl::bool_<exit_impl_::value> exit_; 

    
	cref_impl(const T& t):ref(t){} 

	cref_impl(const B& b,const T& t):B(b),ref(t){}
        
	template<typename S>
	struct result{};
    
    template<typename F,typename T1>
    struct result<F(const T1&)> : next<T1>{};

	template<typename T1>
	typename next<T1>::type
	operator()(const T1& t)const{
		typedef typename next<T1>::type next_;
        return next_(*this,t);
    }

	template<typename T1>
	operator typename array<T1>::type(){
		typedef typename array<T1>::type ar_;
        ar_ ar;
        this->write_to_array(ar,exit_());
        return ar;
    }
    
	// Requirement: C(begin,end) constructor
	template<typename C>
	operator C(){
    	// TODO consider either bypassing the array altogether and call push_front
        // to preserve the referenceness of the approach
    
    	typedef typename boost::range_value<C>::type val_;
		typedef typename array<val_>::type ar_; 
        ar_ ar; 
        this->write_to_array(ar,exit_());
        return C(boost::begin(ar),boost::end(ar));
    }
    
	template<typename A>
	void write_to_array(A& ar,true_)const{
        ar[N] = this->ref;
    }

	template<typename A>
	void write_to_array(A& ar,false_)const{
        ar[N] = this->ref;
		return forward(*this,ar);
    }
    
	template<typename A>
    static void forward(const B& b,A& ar){
    	typedef typename B::exit_ exit_;
        return b.write_to_array(ar,exit_());
    }
    
	ref_ ref;
};


template<typename T>
cref_impl<T,0,boost::mpl::empty_base>
cref_list_of2(const T& t){
	typedef cref_impl<T,0,boost::mpl::empty_base> res_;
	return res_(t);
}

}// assign
}// boost
             
#endif
