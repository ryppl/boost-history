//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_DETAIL_TUPLE_CPP0X_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_DETAIL_TUPLE_CPP0X_ER_2010_HPP
#include <utility>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/assign/v2/ref/tuple/cpp0x.hpp>
// The tuple below avoids the returns reference to a temporary error.

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace tuple_aux{

    template<typename ...Args>
    struct container
    {
        container(){}
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
    struct container<T, Args...> : container<Args...>
    {
        typedef container<Args...> super_t;

        container(T&& t, Args&&...args)
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
        typedef container<T, Args...> container_;
        typedef ref::tuple_aux::traits<T> traits_;
        typedef typename traits_::ref_type type;

        template<typename U>
        static type call(U const& u)
        {
            container_ const& t = u;
            return t.value;
        }
    };

}// tuple_aux
namespace result_of{

    template<typename... Args>
    struct tuple
    {
        typedef tuple_aux::container<Args...> type;
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
    get(tuple_aux::container<Args...> const& t)
    {
        typedef result_of::get_tuple<index, Args...> caller_;
        return caller_::call( t );
    }

}// ref
}// v2
}// assign
}// boost

#endif
