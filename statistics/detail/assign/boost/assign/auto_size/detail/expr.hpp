//////////////////////////////////////////////////////////////////////////////
// assign::detail::expr.hpp                                                 //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_EXPR_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_EXPR_ER_2010_HPP
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/type_traits.hpp>
#include <boost/assign/list_of.hpp> // needed for assign_reference
#include <boost/assign/auto_size/detail/assign_reference_copy.hpp>
#include <boost/assign/auto_size/detail/policy.hpp>
#include <boost/assign/auto_size/detail/types.hpp>
#include <boost/assign/auto_size/detail/expr_size.hpp>
#include <boost/assign/auto_size/detail/expr_elem.hpp>
#include <boost/assign/auto_size/array/lazy.hpp>

// Creates a collection of references by deducing the number of arguments
// at compile time. The functionality is controlled by parameter Ref which
// specifies a reference wrapper, and P, an abitrary policy, usually intended
// to expose a container interface.
//
// Note:
// In most situations, a reference wrapper that has copy rather than rebind 
// semantics for operator= is preferable. 
//    
// Acknowledgement: The idea of this class was developed in collaboration 
// with M.P.G

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
                        
    // ----fwd declare ---- //

    template<
    	typename E,typename T,int N,template<typename> class Ref,typename P>
    class expr;

    // ---- Traits --- //
    
    template<typename E,typename T,int N,template<typename>class Ref,typename P>
    struct expr_size<expr<E,T,N,Ref,P> >
     	: boost::mpl::int_<N>{};

    template<typename E,typename T,int N,template<typename>class Ref,typename P>
    struct expr_elem<expr<E,T,N,Ref,P> >
    {
    	typedef T type;
    };

    // ---- Collection builder ---- //

    template<
    	typename E,typename T,int N,template<typename> class Ref,typename P>
    class expr : public policy<P>::template apply<expr<E,T,N,Ref,P>,Ref>::type{
        typedef boost::mpl::int_<1> int_1_;
        typedef boost::mpl::int_<N> int_n_;
        typedef typename Ref<T>::type ref_;
        typedef typename policy<P>::template apply<expr,Ref>::type super_;

		template<typename E1>
        struct next{
        	typedef expr<E1,T,N+1,Ref,P> type;
        };

        public:       

        typedef typename boost::mpl::equal_to<int_n_,int_1_>::type is_1st_;
        typedef typename boost::mpl::if_<is_1st_,E,const E&>::type previous_;
        typedef typename next<expr>::type result_type;

        expr(const E& p,T& t):previous(p),ref(t){} 
        result_type operator()(T& t)const{ return result_type(*this,t); }

		template<typename E1>
		expr(const E1& that)
        	:super_(that),
            previous(that.previous),
            ref(that.ref){}


        mutable previous_ previous;
        mutable ref_ ref;
                
    };

    // ---- write_to_array ---- //

    template<int Nshift,typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e,false_ /*exit*/){
        a[Nshift+N-1] = e.ref;
        write_to_array(a,e.previous);
    }
            
    template<int Nshift,typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e,true_ /*exit*/){
        a[Nshift+N-1] = e.ref;
    }

    template<int Nshift,typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e){
        typedef expr<E,T,N,Ref,P> expr_;
        typedef typename expr_::is_1st_ exit_;
        write_to_array<Nshift>(a,e,exit_());
    }

    template<typename A,typename E,typename T,int N,
    	template<typename> class Ref,typename P>
    void write_to_array(A& a,const expr<E,T,N,Ref,P>& e){
		return write_to_array<0>(a,e);
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
	
    namespace result_of{
    
    	template<typename T,int N,template<typename> class Ref,typename P>
    	struct expr{
        	typedef typename result_of::expr<T,N-1,Ref,P>::type previous;
    		typedef auto_size::expr<previous,T,N,Ref,P> type;
    	};

    	template<typename T,template<typename> class Ref,typename P>
    	struct expr<T,1,Ref,P> : first_expr<T,Ref,P>{};

    	template<typename T,int N,typename P = default_policy>
    	struct copy : result_of::expr<T,N,ref_copy,P>{};	

    	template<typename T,int N,typename P = default_policy>
    	struct rebind : result_of::expr<T,N,ref_rebind,P>{};	

	}
            
}// auto_size  
}// detail      
}// assign
}// boost

#endif

