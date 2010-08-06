//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_DISTINCT_VALUES_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_DISTINCT_VALUES_ER_2010_HPP
#include <boost/numeric/conversion/conversion_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/assign/v2/ref/detail/convert_traits/add_const.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace convert_traits{
namespace distinct_values{

    template<typename V1,typename V2>
    struct value{
        
        struct arithmetic
        {
            typedef typename boost::numeric::conversion_traits<
                V1,
                V2
            >::supertype type;
            
        };
            
        struct non_arithmetic : boost::mpl::eval_if<
            boost::is_convertible<V1,V2>,
            boost::mpl::identity<V2>,
            boost::mpl::identity<V1>
        >{};

        typedef boost::is_arithmetic<V1> is_ari1_;
        typedef boost::is_arithmetic<V2> is_ari2_;
        typedef boost::mpl::and_<is_ari1_,is_ari2_> is_ari_;
        typedef typename boost::mpl::eval_if<
            is_ari_,
            arithmetic,
            non_arithmetic
        >::type type;
            
    }; // value
        
    // There can be no reference if V1 != V2.
    template<typename V1,typename V2, bool or_const, bool and_ref>
    struct reference :  convert_traits::add_const<
        typename distinct_values::value<V1,V2>::type, 
        or_const
    >{};

}// distinct_values
}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif
