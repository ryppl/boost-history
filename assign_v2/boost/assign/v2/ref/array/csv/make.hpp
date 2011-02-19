//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CSV_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CSV_MAKE_ER_2010_HPP
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/ref/array/alloc/instant.hpp>
#include <boost/assign/v2/ref/array/csv/nth_result_of.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/assign/v2/detail/traits/type/or_const.hpp>
#else
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#endif

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

    template<typename T>
    struct empty_csv_array
        : nth_result_of::csv_array<0, T>{};

}// result_of

    template<typename T>
    typename result_of::empty_csv_array<T>::type
    csv_array( keyword_aux::nil )
    {
        typedef typename result_of::empty_csv_array<T>::type result_;
        return result_();
    }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
namespace csv_array_aux{

    template<std::size_t n,typename R>
    void assign(R& r){}

    template<std::size_t n, typename U, typename R, typename... Args>
    void assign(R& r, U& u, Args&...args)
    {
        typedef typename R::wrapper_type wrapper_;
        r.rebind( n, u );
        csv_array_aux::assign<n + 1>(r, args...);
    }

    template<typename U, typename... Args>
    struct result
    {
        BOOST_STATIC_CONSTANT(std::size_t, n = 1 + sizeof...(Args) );
        typedef typename nth_result_of::csv_array<
            n,
            U
        >::type type;

        static type call(U& u, Args&...args)
        {
            type result;
            csv_array_aux::assign<0>(result, u, args...);
            return result;
        }

    };

}// csv_array_aux

    template<typename T,typename...TArgs>
    typename boost::lazy_disable_if<
        v2::type_traits::or_const<T, TArgs...>,
        csv_array_aux::result<T, TArgs...>
    >::type
    csv_array( T& t, TArgs&...args )
    {
        typedef csv_array_aux::result<T, TArgs...> caller_;
        return caller_::call( t, args... );
    }

    template<typename T, typename...TArgs>
    typename csv_array_aux::result<T const, TArgs const...>::type
    csv_array( T const& t, TArgs const&...args )
    {
        typedef csv_array_aux::result<T const, TArgs const...> caller_;
        return caller_::call( t, args... );
    }

#else

#define BOOST_ASSIGN_V2_MACRO1(z, i, data) r.rebind(i, BOOST_PP_CAT(data, i) );
#define BOOST_ASSIGN_V2_MACRO2(z, n, U)\
namespace csv_array_aux{\
    template<typename T>\
    typename nth_result_of::csv_array<n, T>::type\
    csv_array( BOOST_PP_ENUM_PARAMS(n, T& _) )\
    {\
        typedef typename nth_result_of::csv_array<n, T>::type result_;\
        result_ r;\
        BOOST_PP_REPEAT( n, BOOST_ASSIGN_V2_MACRO1, _ )\
        return r;\
    }\
}\
    template<typename T>\
    typename nth_result_of::csv_array<n, T>::type\
    csv_array( BOOST_PP_ENUM_PARAMS(n, T& _) ){ \
        return csv_array_aux::csv_array<T>( BOOST_PP_ENUM_PARAMS(n, _) );\
    }\
    template<typename T>\
    typename nth_result_of::csv_array<n, T const>::type\
    csv_array( BOOST_PP_ENUM_PARAMS(n, T const& _) ){ \
        return csv_array_aux::csv_array<T const>( BOOST_PP_ENUM_PARAMS(n, _) );\
    }\
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

}// ref
}// v2
}// assign
}// boost

#endif
