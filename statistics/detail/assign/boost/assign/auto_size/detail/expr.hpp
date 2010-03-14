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
#include <ostream>
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/concept_check.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/next_prior.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range.hpp>
#include <boost/assign/auto_size/array/has_static_size.hpp>
#include <boost/assign/auto_size/array/static_size.hpp>
#include <boost/assign/auto_size/reference_wrapper/copy.hpp>
#include <boost/assign/auto_size/reference_wrapper/rebind.hpp>
#include <boost/assign/auto_size/detail/policy.hpp>
#include <boost/assign/auto_size/detail/types.hpp>
#include <boost/assign/auto_size/detail/expr_size.hpp>
#include <boost/assign/auto_size/detail/expr_elem.hpp>
//#include <boost/assign/auto_size/detail/result_of_chain.hpp>
#include <boost/assign/auto_size/array/lazy.hpp>

// Creates a collection of references by deducing the number of arguments
// at compile time. The functionality is controlled by parameter R which
// specifies a reference wrapper, and P, an arbitrary policy, usually intended
// to expose a container interface.
//
// Note:
// - In most situations, a reference wrapper that has copy rather than rebind 
// semantics for operator= is preferable. 
// - The older counterpart of this class is assign::static_generic_list<>.
//
// Acknowledgement: The idea of this class was developed in collaboration 
// with M.P.G
//
// range<int>() is provided to achieve conformity with the rest of Boost.Assign
// but it is in fact ill suited here. 

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
                        
    // ---- fwd declare ---- //

    // expr<> keeps a reference to a new element and links to the previous 
    // collection.
    // E : previous collection 
    // T : element 
    // N : size of the collection
    // R : reference wrapper around an element
    // P : policy
    // F : use reference to link to the previous collection 

    template<typename E,typename T,int N,
        template<typename> class R,typename P,bool F>
    class expr;

    // ---- describe ---- //
    
    template<typename E,typename T,
        template<typename> class R,typename P,typename F,bool F>
	void describe(std::ostream& os,const expr<E,T,1,R,P,F>& e){
    	os << e.ref;
    }

    template<typename E,typename T,int N,
        	template<typename> class R,typename P,bool F>
	void describe(std::ostream& os,const expr<E,T,N,R,P,F>& e){
    	describe(os,e.previous);
    	os << ',' << e.ref;
    }

    // ---- Traits ---- //
    
    template<typename E,typename T,int N,
        template<typename>class R,typename P,bool F>
    struct expr_size<expr<E,T,N,R,P,F> >
     	: boost::mpl::int_<N>{};

    template<typename E,typename T,int N,
        template<typename>class R,typename P,bool F>
    struct expr_elem<expr<E,T,N,R,P,F> >
    {
    	typedef T type;
    };

    namespace result_of{
    
        // Assumes F is set to default
        template<typename T,int N,template<typename> class R,typename P>
    	struct expr;

    }

    // ---- insert_range ---- //

    namespace result_of{
        template<int K,typename E,typename I>
        struct insert_range
        {
    	    typedef E curr_;
            typedef typename E::alt_result_type result_;
            typedef insert_range<K-1,result_,I> iter_;
            typedef typename iter_::type type;
        
            static type call(const E& e,I i){
                BOOST_CONCEPT_ASSERT(
                    (boost_concepts::ForwardTraversalConcept<I>));
                return iter_::call(e.alt(*i),boost::next(i));
            }
    	
        };

        template<typename E,typename I>
        struct insert_range<0,E,I>
        {
            typedef E type;        
            static type call(const E& e,I i){
                return e;
            }
        };
    }

    template<int K,typename E,typename I>
    typename result_of::insert_range<K,E,I>::type
    insert_range(const E& e,I i){
        typedef result_of::insert_range<K,E,I> result_;
        return result_::call(e,i);
    }

    // ---- Collection builder ---- //

    template<typename E,typename T,int N,
        template<typename> class R,typename P,bool F = true>
    class expr : public policy<P>::template apply<expr<E,T,N,R,P,F>,R>::type{
        typedef boost::mpl::int_<1> int_1_;
        typedef boost::mpl::int_<N> int_n_;
        typedef typename R<T>::type ref_;
        typedef typename policy<P>::template apply<expr,R>::type super_;

        template<typename E1>
        struct next{
            typedef expr<E1,T,N+1,R,P,true> type;
        };

        template<typename E1>
        struct alt_next{
            // necessary for insert_range
            typedef expr<E1,T,N+1,R,P,false> type;
        };

        typedef typename boost::mpl::if_c<F,const E&,E>::type storage_;

        template<int K,typename I>
        struct result_of_range 
            : auto_size::result_of::insert_range<K,expr,I>{};

        template<int K,typename Range>
        struct result_of_range2 : result_of_range<
            K, typename boost::range_iterator<Range>::type>{};

        template<typename Range>
        struct result_of_range3 : result_of_range2<
            auto_size::static_size<
                typename boost::remove_const<Range>::type>::value, Range>{};

        public:       

        typedef typename boost::mpl::equal_to<int_n_,int_1_>::type is_first_;
        typedef typename boost::mpl::if_<is_first_,E,storage_>::type previous_;
        typedef typename next<expr>::type result_type;
        typedef typename alt_next<expr>::type alt_result_type;

        explicit expr(const E& p,T& t):previous(p),ref(t){} 
        // Needed by csv.hpp :
        template<typename E1,typename P1,bool F1>
        explicit expr(const expr<E1,T,N,R,P1,F1>& that)
            :super_(that)
            ,previous(that.previous)
            ,ref(that.ref){}

        result_type operator()(T& t)const{ return result_type(*this,t); }
        alt_result_type alt(T& t)const{ return alt_result_type(*this,t); }
        
        template<int K,typename I>
        typename result_of_range<K,I>::type
        range(I b,I e)const{
            BOOST_ASSERT(std::distance(b,e)>=K);
            typedef result_of_range<K,I> result_; 
            return result_::call(*this,b);
        }

        template<int K,typename Range>
        typename result_of_range2<K,Range>::type
        range(Range& r)const{    	
            return this->range<K>(
                boost::begin(r),
                boost::end(r)
            );
        }

        template<int K,typename Range>
        typename result_of_range2<K,const Range>::type
        range(const Range& r)const{    	
            return this->range<K>(
            	boost::begin(r),
                boost::end(r)
            );
        }

        template<typename Range>
        typename boost::lazy_enable_if_c<
            has_static_size<Range>::value,
            result_of_range3<Range>
        >::type
        range(Range& r){ 
            return this->range<auto_size::static_size<Range>::value>(r); 
        }

        template<typename Range>
        typename boost::lazy_enable_if_c<
            has_static_size<Range>::value,
            result_of_range3<const Range>
        >::type
        range(const Range& r){
            return this->range<auto_size::static_size<Range>::value>(r); 
        }

        mutable previous_ previous;
        mutable ref_ ref;

        private:
        expr();

    };

    // ---- write_to_array ---- //
	
    // Some library extension may one day need Nshift, but not at present
    template<int Nshift,typename A,typename E,typename T,int N,
    	template<typename> class R,typename P,bool F>
    void write_to_array(A& a,const expr<E,T,N,R,P,F>& e,false_ /*exit*/){
        a[Nshift+N-1] = e.ref;
        write_to_array(a,e.previous);
    }
            
    template<int Nshift,typename A,typename E,typename T,int N,
    	template<typename> class R,typename P,bool F>
    void write_to_array(A& a,const expr<E,T,N,R,P,F>& e,true_ /*exit*/){
        a[Nshift+N-1] = e.ref;
    }

    template<int Nshift,typename A,typename E,typename T,int N,
    	template<typename> class R,typename P,bool F>
    void write_to_array(A& a,const expr<E,T,N,R,P,F>& e){
        typedef expr<E,T,N,R,P,F> expr_;
        typedef typename expr_::is_first_ exit_;
        write_to_array<Nshift>(a,e,exit_());
    }

    template<typename A,typename E,typename T,int N,
    	template<typename> class R,typename P,bool F>
    void write_to_array(A& a,const expr<E,T,N,R,P,F>& e){
		return write_to_array<0>(a,e);
	}

    // ---- ref wrappers ---- //

    template<typename T>
    struct ref_copy{
        typedef boost::assign::detail::assign_reference_copy<T> type;
    };

    template<typename T>
    struct ref_rebind{
        typedef boost::assign::detail::assign_reference_rebind<T> type;
    };

    // ---- first expr ---- //

    template<typename T,template<typename> class R,typename P>
    struct first_expr{
        typedef detail::auto_size::expr<detail::auto_size::top_,T,1,R,P> type;   
        static type call(T& a){ return type(top_(),a); }
    };

    template<typename T,template<typename> class R>
    struct first_expr_no_policy : first_expr<T,R,no_policy>{};

    template<template<typename> class R,typename T>
    typename first_expr_no_policy<T,R>::type 
    make_first_expr_no_policy(T& a){
    	return first_expr_no_policy<T,R>::call(a);
    }

    template<template<typename> class R,typename T>
    typename first_expr_no_policy<const T,R>::type 
    make_first_expr_no_policy(const T& a){
    	return first_expr_no_policy<const T,R>::call(a);
    }

    template<typename T,typename P = default_policy>
    struct first_copy : first_expr<T,ref_copy,P>{};

    template<typename T,typename P = default_policy>
    struct first_rebind : first_expr<T,ref_rebind,P>{};

    // ---- result_of ---- //
	
    namespace result_of{
    	template<typename T,int N,
        	template<typename> class R,typename P>
    	struct expr{
            typedef typename result_of::expr<T,N-1,R,P>::type previous;
            typedef auto_size::expr<previous,T,N,R,P> type;
    	};

    	template<typename T,template<typename> class R,typename P>
    	struct expr<T,1,R,P> : first_expr<T,R,P>{};

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

