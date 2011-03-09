//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CPP0X_CSV_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CPP0X_CSV_ARRAY_ER_2010_HPP
#include <utility>
#include <boost/assign/v2/ref/array/csv/common.hpp>
#include <boost/assign/v2/detail/traits/type/or_const.hpp>
#include <boost/config.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{
namespace result_of{

    template<typename U, typename... Args>
    struct csv_array : nth_result_of::csv_array<
    	1 + sizeof...(Args),  U
    >
    {};

}// result_of

    template<size_type I, typename R>
    void csv_assign( R& result ){}

    template<size_type I, typename R, typename U, typename... Args>
    void csv_assign( R& result, U& u, Args&...args )
    {
        typedef typename R::wrapper_type wrapper_;
        r.rebind( I::value, u );
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

    template<typename T, typename...Args>
    typename boost::lazy_disable_if<
        v2::type_traits::or_const<T, Args...>,
        result_of::csv_array<T, Args...>
    >::type
    csv_array( T& t, Args&...args )
    {
    	return csv_helper<T, Args...>::call( t, args... );
    }

    template<typename T, typename...Args>
    typename result_of::csv_array<T const, Args const...>
    csv_array( T const& t, Args const&...args )
    {
    	return csv_helper<T const, Args const ...>::call( t, args... );
    }

}// array_aux

using array_aux::csv_array;

//[result_of_csv_array
namespace result_of{

    template<typename /*<<U& has to be a reference>>*/U, typename... Args>
    struct csv_array : nth_result_of::csv_array<
    	1 + sizeof...(Args),  U
    >
    {};

}// result_of
//]
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_CPP0X_CSV_ARRAY_ER_2010_HPP
