//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_TUPLE_CPP0X_DATA_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_TUPLE_CPP0X_DATA_ER_2010_HPP
#include <utility>
#include <boost/assign/v2/ref/tuple/cpp0x.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_reference.hpp>

// This tuple variant keeps a copy of rvalue reference. Expression templates
// involving references to tuples otheriwse cause runtime errors in Release mode under OS X.

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace tuple_aux{

    template<typename ...Args>
    struct data
    {
        data(){}
    };

    template<typename T, bool is_ref = boost::is_reference<T>::value>
    struct traits
    {
        typedef typename boost::add_reference<T>::type ref_type;
        typedef ref_type value_type;
    };

    template<typename T>
    struct traits<T, false>
    {
        typedef typename boost::add_const<T>::type value_type;
        typedef typename boost::add_reference<value_type>::type ref_type;
    };

    template<typename T, typename ...Args>
    struct data<T, Args...> : data<Args...>
    {
        typedef data<Args...> super_t;

        data(T&& t, Args&&...args)
            : super_t(
                std::forward<Args>( args )...
            ),
            value(
                std::forward<T>( t )
            ) {}

        typedef tuple_aux::traits<T> traits_;
        typedef typename traits_::value_type value_type;

        value_type value;

    };

    template<int index, int acc, typename...Args>
    struct get_helper{};

    template<int index, int acc, typename T, typename... Args>
    struct get_helper<index, acc, T, Args...>
     : get_helper<index, acc + 1, Args...>{};

    template<int index, typename T, typename... Args>
    struct get_helper<index, index, T, Args...>
    {
        typedef data<T, Args...> data_;
        typedef ref::tuple_aux::traits<T> traits_;
        typedef typename traits_::ref_type type;

        template<typename U>
        static type call(U const& u)
        {
            data_ const& t = u;
            return t.value;
        }
    };

}// tuple_aux
namespace result_of{

    template<typename... Args>
    struct tuple
    {
        typedef tuple_aux::data<Args...> type;
    };

    template<int index, typename... Args>
    struct get_tuple : tuple_aux::get_helper<index, 0 , Args...>{};

}// result_of

    template<typename ... Args>
    typename result_of::tuple<Args...>::type
    tuple(Args&&...args)
    {
        typedef typename result_of::tuple<Args...>::type result_;
        return result_( std::forward<Args>( args )... );
    }

    template<int index, typename... Args>
    typename result_of::get_tuple<index, Args...>::type
    get(tuple_aux::data<Args...> const& t)
    {
        typedef result_of::get_tuple<index, Args...> caller_;
        return caller_::call( t );
    }

}// ref
}// v2
}// assign
}// boost

#endif
