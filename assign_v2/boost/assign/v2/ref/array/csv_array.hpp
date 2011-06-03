//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CSV_ARRAY_ER_2011_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CSV_ARRAY_ER_2011_HPP
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/ref/array/alloc/instant.hpp>
#include <boost/assign/v2/ref/array/size_type.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/assign/v2/support/traits/type/or_const.hpp>
#include <boost/config.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
//[syntax_ref_array_csv_array
namespace ref{
namespace array_aux{
namespace nth_result_of{

    template<
        array_size_type N
        , typename U // U& is a reference
    >
    struct csv_array/*<-*/
    {
        typedef instant_alloc<N, U> type;
    }/*->*/;

}// nth_result_of

    template<typename U>
    typename nth_result_of::csv_array<0, U>::type
    csv_array()/*<-*/
    {
        typedef typename nth_result_of::csv_array<0, U>::type result_;
        return result_();
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

/*<-*/
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
/*->*/
namespace result_of{

    template<typename U, typename... Args>
    struct csv_array/*<-*/
        : nth_result_of::csv_array<
            1 + sizeof...(Args),  U
        >
    {}/*->*/;

}// result_of

//<-
    template<size_type I, typename R>
    void csv_assign( R& /*unused*/){}

    template<size_type I, typename R, typename U, typename... Args>
    void csv_assign( R& result, U& u, Args&...args )
    {
        typedef typename R::wrapper_type wrapper_;
        result.rebind( I, u );
        csv_assign<I + 1>( result, args... );
    }

    template<typename U, typename... Args>
    struct csv_helper
    {
        typedef typename result_of::csv_array<U, Args...>::type type;

        static type call( U& u, Args&...args )
        {
            type result;
            csv_assign<0>( result, u, args... );
            return result;
        }

    };
//->

    template<typename T, typename...Args>
    /*<-*/typename boost::lazy_disable_if<
        v2::type_traits::or_const<T, Args...>,
        result_of::csv_array<T, Args...>
    >::type BOOST_ASSIGN_V2_IGNORE(/*->*/unspecified/*<-*/)/*->*/
    csv_array( T& t, Args&...args )/*<-*/
    {
        return csv_helper<T, Args...>::call( t, args... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename T, typename...Args>
    typename result_of::csv_array<T const, Args const...>::type
    csv_array( T const& t, Args const&...args )/*<-*/
    {
        return csv_helper<T const, Args const ...>::call( t, args... );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

/*<-*/
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
/*->*/

}// array_aux
/*<-*/
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
/*->*/

    using array_aux::csv_array;

namespace result_of{

    using array_aux::result_of::csv_array;

}// result_of
/*<-*/
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
/*->*/
namespace nth_result_of{

    using array_aux::nth_result_of::csv_array;

}// nth_result_of

}// ref
//]
}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/ref/array/cpp03/csv_array.hpp>
#endif // !BOOST_ASSIGN_V2_ENABLE_CPP0X

#endif // BOOST_ASSIGN_V2_REF_ARRAY_CSV_ARRAY_ER_2011_HPP
