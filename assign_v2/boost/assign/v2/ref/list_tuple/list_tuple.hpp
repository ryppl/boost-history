//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_TUPLE_LIST_TUPLE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_TUPLE_LIST_TUPLE_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_tuple_aux{

    typedef int list_size_type;
    typedef int tuple_size_type;

    struct root{};

    template<typename T>
    struct link
    {

        link(T const& t) : unlink( t ){}

        T const& unlink;
    };

}// list_tuple_aux
}// ref
}// v2
}// assign
}// boost

#include <boost/assign/v2/ref/list_tuple/extraction.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/ref/list_tuple/cpp0x/container.hpp>
#else
#include <boost/assign/v2/ref/list_tuple/cpp03/container.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace result_of{

    typedef list_tuple_aux::container<0, list_tuple_aux::root> list_tuple;

}// result_of
namespace
{

    result_of::list_tuple const
        list_tuple = result_of::list_tuple();

}
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_LIST_TUPLE_LIST_TUPLE_ER_2010_HPP
