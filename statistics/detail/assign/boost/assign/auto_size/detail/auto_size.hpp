//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size.hpp                                            //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_AUTO_SIZE_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_AUTO_SIZE_ER_2010_HPP
#include <iostream> // TODO remove
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/type_traits.hpp>
#include <boost/assign/list_of.hpp> // needed for assign_reference
#include <boost/assign/auto_size/detail/assign_refence_copy.hpp>
#include <boost/assign/auto_size/array/policy.hpp>

// Creates a collection of references by deducing the number of arguments
// at compile time. The functionality is controlled by parameter Ref which
// specifies a reference_wrapper and P, an abitrary policy, usually intended
// to expose a container interface.
//
// Note:
// In most situations, a reference wrapper that has copy rather than rebind 
// semantics for operator= is preferable. 
//    
// Acknowledgement: The idea of this class was developed in collaboration 
// with M.P.G
//
// Revision history:
// Feb 27, 2010 : Support for comma separated arguments (See csv.hpp)
// Feb 25, 2010 : Complemented the boost::array interface
// Feb 21, 2010 : Made member variables mutable and added constness to member 
// functions where appropriate.
// Feb 9, 2010 : 
// 	- Added a template parameter for the reference_wrapper
// 	- The temporary array in the conversion operator is now assigned by calling 
// 	begin() and end() rather than write_to_array() to ensure consistency of 
//  side effect. 
// Feb 5, 2010 : First version. rebind semantics.

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
            
    typedef boost::mpl::void_ top_;
            
    // ---- Policy meta classes --- //

    template<typename L,typename T,int N,template<typename> class Ref,
    	typename P>
    struct expr;

    struct default_policy;

	struct default_policy{
        template<typename E,typename T,int N,template<typename> class Ref>
        struct apply{
			typedef expr<E,T,N,Ref,default_policy> expr_;        	
    		typedef array_policy<T,N,Ref,expr_> type;
    	};
    };

    struct no_policy{
        template<typename E,typename T,int N,template<typename> class Ref>
        struct apply{
    		typedef boost::mpl::empty_base type;
    	};
    };

    // ---- Collection builder ---- //
            
    template<typename E,typename T,int N,template<typename> class Ref,
    	typename P>
    struct next{
        typedef expr<E,T,N,Ref,P> expr_;
        typedef expr<expr_,T,N+1,Ref,P> type;
    };

    template<
    	typename E,typename T,int N,template<typename> class Ref,typename P
    >
    class expr : public P::template apply<E,T,N,Ref>::type{
        typedef boost::mpl::int_<1> int_1_;
        typedef boost::mpl::int_<N> int_n_;
        typedef typename Ref<T>::type ref_;
		typedef typename P::template apply<E,T,N,Ref>::type super_;

        public:       

        typedef typename boost::mpl::equal_to<int_n_,int_1_>::type is_1st_;
        typedef typename boost::mpl::if_<is_1st_,E,const E&>::type previous_;
        typedef typename next<E,T,N,Ref,P>::type next_;
        typedef next_ result_type;

        expr(const E& p,T& t):previous(p),ref(t){} 
        next_ operator()(T& t)const{ return next_(*this,t); }

        mutable previous_ previous;
        mutable ref_ ref;
    };

    // ---- write_to_array ---- //

    typedef boost::mpl::bool_<false> false_;
    typedef boost::mpl::bool_<true> true_;
            
    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e){
        typedef expr<E,T,N,Ref,P> expr_;
        typedef typename expr_::is_1st_ exit_;
        write_to_array(a,e,exit_());
    }
            
    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e,false_ /*exit*/){
        a[N-1] = e.ref;
        write_to_array(a,e.previous);
    }
            
    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e,true_ /*exit*/){
        a[N-1] = e.ref;
    }

	// ---- ref wrappers ---- //

    template<typename T>
    struct ref_copy{
        typedef boost::assign::detail::assign_reference_copy<T> type;
    };

    template<typename T>
    struct ref_rebind{
        typedef boost::assign_detail::assign_reference<T> type;
    };

    // ---- first expr ---- //

    template<typename T,template<typename> class Ref,typename P>
    struct first_expr{
        typedef detail::auto_size::expr<detail::auto_size::top_,T,1,Ref,P> type;   
        static type call(T& a){ return type(top_(),a); }
    };

    template<typename T,template<typename> class Ref>
    struct first_expr_no_policy : first_expr<T,Ref,no_policy>{};

	template<template<typename> class Ref,typename T>
    typename first_expr_no_policy<T,Ref>::type 
    make_first_expr_no_policy(T& a){
    	return first_expr_no_policy<T,Ref>::call(a);
    }

	template<template<typename> class Ref,typename T>
    typename first_expr_no_policy<const T,Ref>::type 
    make_first_expr_no_policy(const T& a){
    	return first_expr_no_policy<const T,Ref>::call(a);
    }

    template<typename T,typename P = default_policy>
    struct first_copy : first_expr<T,ref_copy,P>{};

    template<typename T,typename P = default_policy>
    struct first_rebind : first_expr<T,ref_rebind,P>{};

    // ---- result_of ---- //
	
    template<typename T,int N,template<typename> class Ref,typename P>
    struct result_of{
		typedef typename result_of<T,N-1,Ref,P>::type previous;
    	typedef expr<previous,T,N,Ref,P> type;
    };

    template<typename T,template<typename> class Ref,typename P>
    struct result_of<T,1,Ref,P> : first_expr<T,Ref,P>{};

    template<typename T,int N,typename P = default_policy>
    struct result_of_copy : result_of<T,N,ref_copy,P>{};	

    template<typename T,int N,typename P = default_policy>
    struct result_of_rebind : result_of<T,N,ref_rebind,P>{};	
            
}// auto_size  
}// detail      
}// assign
}// boost

#endif

