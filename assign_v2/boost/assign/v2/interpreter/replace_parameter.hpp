//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_REPLACE_PARAMETER_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_REPLACE_PARAMETER_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_adapter_replace_parameter
namespace aux{
                
    template<typename /*<<Inherits crtp\<\> >>*/D> 
    struct /*<<Meta-function class to be specialized on D>>*/replace_fun{
        template<typename F> 
        struct /*<<Transforms D by replacing D::fun_type with F>>*/apply{};
    };

    template<typename /*<<Inherits crtp\<\> >>*/ D> 
    struct /*<<Meta-function class to be specialized on D>>*/ replace_modifier_tag{
        template<typename Tag> 
        struct /*<<Transforms D by replacing Tag D::modifier_tag with Tag>>*/ apply{};
    };
    
}// aux
//]
}// v2
}// assign
}// boost
    
#endif // BOOST_ASSIGN_V2_INTERPRETER_REPLACE_PARAMETER_ER_2010_HPP
