//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FRAME_CRTP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FRAME_CRTP_ER_2010_HPP
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>

#include <boost/assign/v2/put/frame/base.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>

#endif

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

	template<typename F>
	struct fun_holder{
    
		typedef F fun_type;

    	fun_holder(){}
        fun_holder(fun_type const& f) : fun( f ){}

		fun_type fun;
    
    };
    
	template<typename Tag>
	struct modifier_holder{
    
    	typedef Tag modifier_tag;
		typedef put_aux::modifier<Tag> modifier_type;

    	modifier_holder(){}
        modifier_holder(modifier_type const& m) : modifier( m ){}

		typedef put_aux::modifier<Tag> modifier_;

		modifier_type modifier;
    
    };

	// D has to model concept_sub::Pre3, with respect to container C, functor F 
    // and Tag. It then models concept_sub::Post
    template<typename C, typename F, typename Tag, typename D>
    class crtp :
    	public put_base
    	, public fun_holder<F> 
    	, public modifier_holder<Tag>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//do nothing
#else
    	, public functor_aux::crtp_unary_and_up<
    		crtp<C, F, Tag, D>,
        	::boost::mpl::always< D const& >
    	>
#endif
    {

		public:
		typedef D const& result_type;

		protected:

		typedef fun_holder<F> fun_holder_;
		typedef modifier_holder<Tag> modifier_holder_;
        typedef typename modifier_holder_::modifier_type modifier_;

        public:

		D & derived(){ return static_cast<D&>(*this); }
		D const& derived()const{ return static_cast<D const&>(*this); }

		//public:
		
		crtp(){}
        explicit crtp( F const& f ) : fun_holder_( f ){}
        explicit crtp( F const& f, modifier_ const& m )
        	: fun_holder_( f ), modifier_holder_( m ){}

    	result_type operator()()const
    	{
			return this->modify( this->fun() );
    	}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

		//[crtp_functor
        template<typename...Args>
        result_type operator()( Args&&...args )const
        {
            return this->modify(
                this->fun( std::forward<Args>(args)... )
            );
        }
        //]

#else
        protected:
        typedef functor_aux::crtp_unary_and_up<
            crtp,
            ::boost::mpl::always<result_type>
        > super_t;

        public:
		using super_t::operator();

#define MACRO(z, N, data) \
	template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    result_type \
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N, T, & _) )const \
	{ \
        return this->modify( this->fun(BOOST_PP_ENUM_PARAMS(N,_)) ); \
	} \
/**/
BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    MACRO,
    ~
)
#undef MACRO
#endif
		C& container()const{ return this->derived().container(); }

		struct result_of_modulo{

            struct deduce
            {
				typedef functor_aux::deduce_constructor<C> caller_;
        		typedef typename caller_::type cons_;
                typedef v2::result_of_modulo::fun<D> meta_;
                typedef typename ::boost::mpl::apply1<meta_, cons_>::type type;
            };

        };

		typename result_of_modulo::deduce::type
		modulo_deduce()const
        {
			typedef functor_aux::deduce_constructor<C> caller_;
        	typedef typename caller_::type cons_;
    		typedef typename result_of_modulo::deduce::type result_;
    		return result_(
        		this->derived().container(),
             	caller_::call()
        	);
        }

		protected:

		template<typename T>
        result_type modify(T* t)const
        {
        	typedef put_concept::ModifierImpl<modifier_, C, T*> concept_;
            BOOST_CONCEPT_ASSERT(( concept_ ));
			this->modifier.impl( this->derived().container(), t );
            return this->derived();
        }

		template<typename T>
		void check_modifier( BOOST_ASSIGN_V2_forward_param(T, t) )const
        {
        	typedef put_concept::ModifierImpl<modifier_, C, 
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            	T&&
#else
				T&
#endif                                
            > concept_;
            BOOST_CONCEPT_ASSERT(( concept_ ));
        }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

		//[crtp_modify
		template<typename T>
        result_type modify(T&& t)const
        {
			check_modifier( t );
			this->modifier.impl(
                this->derived().container(),
                std::forward<T>( t )
            );
            return this->derived();
        }
        //]
#else
		template<typename T>
        result_type modify(T& t)const
        {
			check_modifier( t );
			this->modifier.impl( this->derived().container(), t );
            return this->derived();
        }

		template<typename T>
        result_type modify(T const& t)const
        {
			check_modifier( t );
			this->modifier.impl( this->derived().container(), t );
            return this->derived();
        }

#endif

    };

}// put_aux
}// v2
}// assign
}// boost

#endif
