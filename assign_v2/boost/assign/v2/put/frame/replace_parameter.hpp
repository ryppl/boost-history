//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FRAME_REPLACE_PARAMETER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FRAME_REPLACE_PARAMETER_ER_2010_HPP
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{
                
    template<typename /*<<Inherits crtp\<\> >>*/D> 
    struct /*<<Meta-function class to be specialized on D>>*/replace_fun{
        template<typename F> 
        struct /*<<Transforms D by replacing F for D::fun_type>>*/apply{};
    };

    template<typename /*<<Inherits crtp\<\> >>*/ D> 
    struct /*<<Meta-function class to be specialized on D>>*/ replace_modifier_tag{
        template<typename Tag> 
        struct /*<<Transforms D by replacing Tag for D::modifier_tag>>*/ apply{};
    };
    
}// put_aux
}// v2
}// assign
}// boost
    
#endif // BOOST_ASSIGN_V2_PUT_FRAME_REPLACE_PARAMETER_ER_2010_HPP
