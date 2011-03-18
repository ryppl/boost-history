//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/ref/aux_/convert_traits/checking.hpp>
#include <libs/assign/v2/test/ref/aux_/convert_traits.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_aux_{
namespace xxx_convert_traits{

    // --User may skip this file-- //
    // It checks internal details. //
    // --------------------------- //        

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        namespace ns = as2::ref::convert_traits::checking;
        {
            ns::same_value_type::do_check<int>();
        }
    }

}// xxx_convert_traits
}// xxx_aux_
}// xxx_ref
}// test_assign
