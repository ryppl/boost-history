//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_WRAPPER_CRTP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_WRAPPER_CRTP_ER_2010_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/mpl/always.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/range/reference.hpp>
#include <boost/assign/v2/detail/config/arity_bound.hpp>
#include <boost/assign/v2/detail/functor/functor.hpp>
#include <boost/assign/v2/detail/keyword/keyword.hpp>
#include <boost/assign/v2/put/modifier/modifier.hpp>

// --------------- //
//     Usage       //
// --------------- //
// Calling
//  object = put( v );
// wraps a class object around v, whose overloaded operator() and csv() mf act 
// as a modifiers of v. To recover v, call
//  object.unwrap();
//
// Let cond1 = ( k < BOOST_ASSIGN_V2_LVALUE_CONST_ARITY_BOUND )
// Let cond2 = all arguments are lvalues.
//
// -- Overloaded syntax --
//
//  For 0 <= k(i) < BOOST_ASSIGN_V2_ARITY_BOUND
// Expression														
// 	object( x11, ..., x1k(1) ) ... ( xn1, ..., xnk(n) )
// Side effect
//  for each i=1,...,n puts f( xi1, ..., xik(i) ) into v for each i=1,...,n.
// Returns a reference to the wrapper.
//
// Each argument's lvalue is preserved if either cond1 OR (!cond1 AND cond2)
// In all other cases, arguments are passed as const references.
//
// ----- csv syntax ------
//
// For k(i) = 1, and n < BOOST_ASSIGN_V2_CSV_ARITY_BOUND,
//  object.csv( x11, ..., xn1 )
// Same return and effect as above by redefining cond1 = false. 


namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

	// T = container_type_traits::value<V>::type
	template<typename V>
    struct crtp_traits 
    {
        typedef typename v2::container_type_traits::value<V>::type value_type;
        typedef typename result_of::constructor<value_type>::type functor_type;
        typedef typename put_aux::deduce_modifier<V>::type put_tag;
    };

	// Requirements:
    // 	d.unwrap() 				returns a reference to V&
    // 	D d(U v, F const& f);   Constructs an object, d, of type D. 
    // Usually f is passed to the crtp. U = V& or V const& depending on need.
    template<typename V,typename F, typename Tag,typename D,typename Traits> 
    class crtp : public functor_aux::crtp_unary_and_up<
    	crtp<V,F,Tag,D,Traits>,
        boost::mpl::always< D const& >
    >
    {

		public:
		typedef D const& result_type;

		protected:

        typedef functor_aux::crtp_unary_and_up<
            crtp,
            boost::mpl::always<result_type>
        > super_t;

		typedef put_aux::modifier<Tag> modifier_;

		D & derived(){ return static_cast<D&>(*this); }
		D const& derived()const{ return static_cast<D const&>(*this); }

		public:

		crtp(){}
		explicit crtp(F const& f1 ):f( f1 ) {}

		using super_t::operator();
    	result_type operator()()const
    	{
			return this->arg_deduct( this->f() );
            return this->derived();
    	}

#define BOOST_ASSIGN_V2_impl(z, N, data) \
	template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    result_type \
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N, T, & _) )const \
	{ \
        return this->arg_deduct( this->f(BOOST_PP_ENUM_PARAMS(N,_)) ); \
	} \
/**/
BOOST_PP_REPEAT_FROM_TO(
	1, 
    BOOST_ASSIGN_V2_ARITY_BOUND, 
    BOOST_ASSIGN_V2_impl, 
    ~
)
#undef BOOST_ASSIGN_V2_impl
#define BOOST_ASSIGN_V2_invoke(z,i,data) ( BOOST_PP_CAT( _,i ) ) 
#define BOOST_ASSIGN_V2_csv(z, N, data) \
	template<BOOST_PP_ENUM_PARAMS(N,typename T)> \
    result_type \
    csv( BOOST_PP_ENUM_BINARY_PARAMS(N, T, &_) )const \
    { \
        (*this) BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_invoke, ~ ); \
        return this->derived(); \
    } \
	template<BOOST_PP_ENUM_PARAMS(N,typename T)> \
    result_type \
    csv( BOOST_PP_ENUM_BINARY_PARAMS(N, T, const &_) )const \
    { \
        (*this) BOOST_PP_REPEAT(N, BOOST_ASSIGN_V2_invoke, ~ ); \
        return this->derived(); \
    } \
/**/
BOOST_PP_REPEAT_FROM_TO(
	1, 
    BOOST_ASSIGN_V2_CSV_ARITY_BOUND, 
    BOOST_ASSIGN_V2_csv,
    ~
)
#undef BOOST_ASSIGN_V2_invoke
#undef BOOST_ASSIGN_V2_csv

        F const& fun()const{ return this->f; }

		result_type copy_modifier(const modifier_& that)
        {
        	this->modifier = that;
            return this->derived();
        }

		struct result_of_modulo{

        	template<typename NewTag>
            struct put_tag : Traits::template new_tag<NewTag>{};
            
            template<typename NewF> struct fun
            	: Traits::template new_fun<NewF>{};
        
        	struct incr_lookup : Traits::template new_fun_tag<
            	functor_aux::identity,
                v2::put_tag::incr_lookup
            >{};

			struct repeat
            {
    			typedef v2::put_tag::repeat<Tag> new_tag_;
        		typedef put_aux::modifier<new_tag_> modifier_;
       		 	typedef typename Traits::template new_tag<new_tag_>::type type;
            };
            
            struct deduce
            {
				typedef functor_aux::deduce_constructor<V> caller_;
        		typedef typename caller_::type cons_;
       		 	typedef typename Traits::template new_fun<cons_>::type type;
            };
            
        };

		template<typename NewTag>
    	typename result_of_modulo::template put_tag<NewTag>::type
		modulo_put_tag()const
    	{
    		typedef typename result_of_modulo::template put_tag<
            	NewTag
            >::type result_;
    		return result_( this->derived().unwrap(), this->fun() );
        }

		template<typename F1>
    	typename result_of_modulo::template fun<F1>::type
        modulo_fun( const F1& f)const
        {
    		typedef typename result_of_modulo::template fun<
            	F1
            >::type result_;
    		return result_( this->derived().unwrap(), f );
        }

		typename result_of_modulo::incr_lookup::type
        modulo_incr_lookup()const{
    		return (*this) 
            	% ( _fun = _identity ) 
                % ( _put_tag = v2::put_tag::incr_lookup() );
		}

		template<typename N>
		typename result_of_modulo::repeat::type
        modulo_repeat(N const& n)const
        {
        	typedef typename result_of_modulo::repeat traits_;
            typedef typename traits_::modifier_ modifier_;
    		return (
            	(*this) %  ( _put_tag = v2::put_tag::repeat<Tag>() )
            ).copy_modifier( modifier_( n ) );
		}

		typename result_of_modulo::deduce::type
		modulo_deduce()const
        {
			typedef functor_aux::deduce_constructor<V> caller_;
        	typedef typename caller_::type cons_;
    		typedef typename result_of_modulo::deduce::type result_;
    		return result_( 
        		this->derived().unwrap(), 
             	caller_::call()
        	);
        }

		protected:

		template<typename T>
        result_type arg_deduct(T& t)const
        {
			this->modifier.impl( this->derived().unwrap(), t );        	
            return this->derived();
        }

		template<typename T>
        result_type arg_deduct(T const& t)const
        {
			this->modifier.impl( this->derived().unwrap(), t );        	
            return this->derived();
        }
        modifier_ modifier;
        F f;
    };

}// put_aux
}// v2
}// assign
}// boost

#endif
