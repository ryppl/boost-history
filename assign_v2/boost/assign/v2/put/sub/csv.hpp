//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_SUB_CSV_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_SUB_CSV_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/detail/traits/container/value.hpp>
#include <boost/assign/v2/put/sub/functor.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace csv_put_aux{

    template<typename V>
    class sub
    	: protected ref::copy_wrapper<V>::type
    {

		protected:

		typedef typename ref::copy_wrapper<V>::type super1_t;

		typedef typename v2::container_traits::value<
            V
        >::type value_type;

		public:

		sub(){}
		explicit sub( V& v ) : super1_t( v ) {}

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        protected:
        template<typename R>
        void impl(R& r){}

        template<typename R,typename... Args>
        void impl(R& r, value_type const& t, Args&&...args)
        {
            r( t );
            this->impl( r, std::forward<Args>( args )... );
        }

        public:
        template<typename... Args>
        typename result_of::put<V>::type
        operator()(Args&&...args)
        {
            typedef typename result_of::put<V>::type result_;
            result_ result = put( this->get() );
            this->impl(
                result,
                args...
            );
            return result;
        }

#else
#define MACRO1(z, i, data) ( BOOST_PP_CAT(_, i) )
#define MACRO2(z, N, data)\
    typename result_of::put<V>::type\
    operator()( BOOST_PP_ENUM_PARAMS(N, value_type const & _) )\
    {\
        return put( this->get() ) BOOST_PP_REPEAT(N, MACRO1, ~ );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    MACRO2,
    ~
)
#undef MACRO1
#undef MACRO2
#endif
		V& container()const{
        	return static_cast<super1_t const&>(*this).get();
        }

    };

}// csv_put_aux
namespace result_of{

	template<typename V>
    struct csv_put
    {
		typedef csv_put_aux::sub<V> type;
	};

}// result_of

	template<typename V>
	typename result_of::csv_put<V>::type
	csv_put( V& v )
    {
		typedef typename result_of::csv_put<V>::type result_;
    	return result_( v );
    }

}// v2
}// assign
}// boost

#endif
