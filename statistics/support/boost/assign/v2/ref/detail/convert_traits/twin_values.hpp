//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_TWIN_VALUES_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_TWIN_VALUES_ER_2010_HPP
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/assign/v2/ref/detail/convert_traits/add_const.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace convert_traits{
namespace twin_values{

    template<typename V, bool or_const, bool and_ref>
    struct reference
    {
    	typedef typename convert_traits::add_const<V,or_const>::type c_;
        typedef typename boost::mpl::eval_if_c<
            and_ref,
            boost::add_reference<c_>,
            boost::mpl::identity<c_>
        >::type type;
    };

}// twin_values 
}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif

