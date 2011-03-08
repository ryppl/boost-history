// TODO remove file
/*
//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_FUN_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_FUN_MODIFIER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <boost/assign/v2/put/modulo/modifier.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo{

    template<typename T> struct fun_modifier
    {
        template<typename F, typename Tag>
        struct apply
        {
            typedef result_of_modulo::fun<T> meta_fun_;
            typedef typename ::boost::mpl::apply1< meta_fun_, F>::type new_t_;
            typedef result_of_modulo::modifier<new_t_> modifier_;
            typedef typename ::boost::mpl::apply1< modifier_, Tag>::type type;
        };
    };

}// result_of_modulo
}// v2
}// assign
}// boost

#endif
*/