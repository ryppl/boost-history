//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_UNWRAP_REFERENCE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_UNWRAP_REFERENCE_ER_2010_HPP
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/range/reference.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace unwrap_aux{

    // U					Result
    // ref::wrapper<Tag,T>	T&
    // ref::wrapper<Tag,T>&	T&
    // T&					T&
    template<typename U,typename T = typename boost::remove_reference<U>::type>
    struct reference : boost::mpl::eval_if<
        boost::is_reference_wrapper<T>,
        boost::add_reference<
           typename boost::unwrap_reference<T>::type
        >,
        boost::mpl::identity<U>
    >{
        static void test()
        {
            BOOST_MPL_ASSERT_NOT(( boost::is_reference<T> ));
        };
    };

}// unwrap_aux
}// ref
}// v2
}// assign
}// boost

#endif
