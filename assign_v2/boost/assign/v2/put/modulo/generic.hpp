//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_GENERIC_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_GENERIC_ER_2010_HPP
#include <boost/typeof/typeof.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo{    
    
    // Is this needed?
    template<typename T>
    struct generic_
    {
    	template<typename P>
        struct apply
        {
        	static T t;
            static P p;
            typedef BOOST_TYPEOF_TPL( t % p ) type;
        };    
    };
    
}// result_of_modulo
}// v2
}// assign
}// boost

#endif

