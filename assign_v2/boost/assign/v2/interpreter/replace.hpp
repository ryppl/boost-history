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
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_adapter_replace_parameter
namespace interpreter_aux{

    template<typename Derived> 
    struct replace_data_generator{
    
        template<typename F> 
        struct apply/*<-*/{}/*->*/;
    };

    template<typename Derived> 
    struct replace_modifier_tag{

        template<typename Tag> struct apply/*<-*/{}/*->*/;
    };

    template<typename Derived> 
    struct replace_data_tag{

        template<typename Tag> 
        struct apply/*<-*/{}/*->*/;
    };
    
}// interpreter_aux
//]
}// v2
}// assign
}// boost
    
#endif // BOOST_ASSIGN_V2_INTERPRETER_REPLACE_ER_2010_HPP
