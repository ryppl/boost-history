//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
//[result_of_modulo_modifier
namespace result_of_modulo{

    template<typename /*<<Inherits from put_aux::crtp\<\>>>*/D> struct /*<<Specialize on D>>*/modifier{
        template<typename Tag> struct apply{};
    };

}// result_of_modulo
//]
}// v2
}// assign
}// boost

#endif
