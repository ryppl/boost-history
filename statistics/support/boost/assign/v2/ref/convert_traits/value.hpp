//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_VALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONVERT_TRAITS_VALUE_ER_2010_HPP
#include <boost/type_traits/is_convertible.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace convert_traits{

    template<
    	typename V1, typename V2, 
        bool is_c1 = boost::is_convertible<V1, V2>::value,
        bool is_c2 = boost::is_convertible<V2, V1>::value
    >
    struct value{ typedef V1 type; };

    template<typename V1, typename V2>
    struct value<V1, V2, false, true>{ typedef V2 type; };

    template<typename V1, typename V2> struct value<V1, V2, false, false>{};
    
    // TODO upgrade
	//	#include <boost/type_traits/is_arithmetic.hpp>
    //  #include <boost/numeric/conversion/conversion_traits.hpp>
    //	struct value : boost::mpl::eval_if<
    //		mpl::and_<is_arithmetic<V1>, is_arithmetic<V2> >,
    //   	boost::numeric::conversion_traits<V1, V2>::supertype.
    //		... as above ...
    //  >{};
        

}// convert_traits
}// ref
}// v2
}// assign
}// boost

#endif
