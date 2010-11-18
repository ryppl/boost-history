//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_ANON_CSV_MAKE_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/static_array/alloc/fast_alloc.hpp>
#include <boost/assign/v2/ref/anon/csv/nth_result_of.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/config.hpp>
#else
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

	template<typename Tag1,typename T>
    struct empty_csv_anon
    	: nth_result_of::csv_anon<0, Tag1, T>{};

}// result_of

	template<typename Tag1, typename T>
    typename result_of::empty_csv_anon<Tag1, T>::type
	csv_anon( keyword_aux::nil )
    {
    	typedef typename result_of::empty_csv_anon<Tag1, T>::type result_;
        return result_();
    }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace csv_anon_aux{

    template<std::size_t n,typename U,typename R>
    void assign(R& r){}

    template<std::size_t n, typename U, typename R, typename... Args>
    void assign(R& r, U& u, Args&...args)
    {
        typedef typename R::wrapper_type wrapper_;
        r.get_wrapper( n ) = wrapper_( u );
        csv_anon_aux::assign<n + 1, U>(r, args...);
    }

    template<typename Tag1, typename U, typename... Args>
    struct result
    {
        BOOST_STATIC_CONSTANT(std::size_t, n = 1 + sizeof...(Args) );
        typedef typename nth_result_of::csv_anon<
            n,
            Tag1,
            U
        >::type type;

        static type call(U& u, Args&...args)
        {
            type result;
            csv_anon_aux::assign<0, U>(result, u, args...);
            return result;
        }

    };

}// csv_anon_aux

#define MACRO(U, Brgs)\
    template<typename Tag1,typename T,typename...Args>\
    typename csv_anon_aux::result<Tag1, U, Brgs...>::type\
    csv_anon( U& u, Brgs&...args )\
    {\
        typedef csv_anon_aux::result<Tag1, U, Brgs...> caller_;\
        return caller_::call( u, args... );\
    }\
/**/
MACRO(T, Args)
MACRO(T const, Args const)
#undef MACRO

#else

    template<std::size_t i, typename U, typename R>
    void assign(R& r, U& u)
    {
        typedef typename R::wrapper_type wrapper_;
        r.get_wrapper( i ) = wrapper_( u );
    }

#define MACRO1(z, i, U) assign<i, U>( r, BOOST_PP_CAT(_,i) );
#define MACRO2(z, n, U)\
    template<typename Tag1, typename T>\
    typename nth_result_of::csv_anon<\
        n,\
        Tag1,\
        U\
    >::type\
    csv_anon( BOOST_PP_ENUM_PARAMS(n, U& _) ) \
    {\
    	typedef typename nth_result_of::csv_anon<\
    		n,\
        	Tag1,\
        	U\
    	>::type result_;\
        result_ r;\
        BOOST_PP_REPEAT( n, MACRO1, U )\
        return r;\
    }\
/**/

#define MACRO(z, n, data)\
MACRO2(z, n, T)\
MACRO2(z, n, T const)\
/**/

BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    MACRO,
    ~
)
#undef MACRO1
#undef MACRO2
#undef MACRO
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

}// ref
}// v2
}// assign
}// boost

#endif
