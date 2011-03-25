//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_REPLACE_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_REPLACE_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_adapter_replace_parameter
namespace interpreter_aux{

	//Specialize on D
    template<
        typename D // Derives from interpreter_crtp
    > 
    struct replace_data_generator{
    
        // Return transformation by D::fun_type = F
        template<typename F> 
        struct apply/*<-*/{}/*->*/;
    };

	//Specialize on D
    template<
        typename D // Derives from interpreter_crtp
    > 
    struct replace_modifier_tag{

        // Return transformation by D::modifier_tag = Tag
        template<typename Tag> 
        struct apply{};
    };
    
}// interpreter_aux
//]
}// v2
}// assign
}// boost
    
#endif // BOOST_ASSIGN_V2_INTERPRETER_REPLACE_ER_2010_HPP
