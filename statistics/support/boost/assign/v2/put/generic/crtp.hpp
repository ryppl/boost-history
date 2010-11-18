//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_CRTP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_CRTP_ER_2010_HPP
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/range/reference.hpp>

#include <boost/assign/v2/detail/type_traits/container/is_ptr_container.hpp>
#include <boost/assign/v2/detail/functor/functor.hpp>
#include <boost/assign/v2/detail/keyword/keyword.hpp>

#include <boost/assign/v2/put/modifier/modifier.hpp>
#include <boost/assign/v2/put/generic/result_of_modulo.hpp>
#include <boost/assign/v2/put/generic/expose_modifier.hpp>
#include <boost/assign/v2/put/generic/expose_fun.hpp>
#include <boost/assign/v2/put/generic/base.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>

#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>

#endif

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

	// T = container_type_traits::value<V>::type
	template<typename V>
    struct crtp_traits
    {
        typedef typename v2::container_type_traits::value<
            V
        >::type value_type;
        typedef typename boost::mpl::eval_if<
        	container_type_traits::is_ptr_container<V>,
            functor_aux::deduce_new_<V>,
            functor_aux::deduce_constructor<V>
        >::type functor_type;
        typedef typename put_aux::deduce_modifier<V>::type modifier_tag;

    };

	// Requirements:
    // 	d.unwrap() 				returns a reference to V&
    // 	D d(U v, F const& f);   Constructs an object, d, of type D.
    // Usually, f is passed to the crtp. U = V& or V const& depending on need.
    // Traits are deprecated but a use may be find in the future.
    template<typename V,typename F, typename Tag, typename D, typename Traits>
    class crtp :
    	public put_base,
    	public  put_aux::expose_fun<F> // protect + friend?
    	, public  put_aux::expose_modifier<Tag> // protect + friend?
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//do nothing
#else
    	, public functor_aux::crtp_unary_and_up<
    		crtp<V, F, Tag, D, Traits>,
        	boost::mpl::always< D const& >
    	>
#endif
    {

		public:
		typedef D const& result_type;

		protected:

		typedef expose_modifier<Tag> expose_modifier_;
        typedef typename expose_modifier_::modifier_type modifier_;
		typedef expose_fun<F> expose_fun_;

        public:

		D & derived(){ return static_cast<D&>(*this); }
		D const& derived()const{ return static_cast<D const&>(*this); }

		//public:

		crtp(){}
        explicit crtp( F const& f ) : expose_fun_( f ){}
        explicit crtp( F const& f, modifier_ const& m )
        	: expose_fun_( f ), expose_modifier_( m ){}

    	result_type operator()()const
    	{
			return this->arg_deduct( this->fun() );
    	}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        template<typename...Args>
        result_type
        operator()( Args&&...args )const
        {
            return this->arg_deduct(
                this->fun( std::forward<Args>(args)... )
            );
        }

#else
        protected:
        typedef functor_aux::crtp_unary_and_up<
            crtp,
            boost::mpl::always<result_type>
        > super_t;

        public:
		using super_t::operator();

#define MACRO(z, N, data) \
	template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    result_type \
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N, T, & _) )const \
	{ \
        return this->arg_deduct( this->fun(BOOST_PP_ENUM_PARAMS(N,_)) ); \
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
		V& unwrap()const{ return this->derived().unwrap(); }

		struct result_of_modulo{

            struct deduce
            {
				typedef functor_aux::deduce_constructor<V> caller_;
        		typedef typename caller_::type cons_;
                typedef typename boost::mpl::apply1<
                	v2::result_of_modulo::new_fun<D>,
                    cons_
                >::type type;
            };

        };

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
        result_type arg_deduct(T* t)const
        {
			this->modifier.impl( this->derived().unwrap(), t );
            return this->derived();
        }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
		template<typename T>
        result_type arg_deduct(T&& t)const
        {
			this->modifier.impl(
                this->derived().unwrap(),
                std::forward<T>( t )
            );
            return this->derived();
        }
#else
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

#endif

    };

}// put_aux
}// v2
}// assign
}// boost

#endif
