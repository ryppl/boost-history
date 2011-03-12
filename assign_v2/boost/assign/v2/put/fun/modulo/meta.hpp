//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FUN_MODULO_META_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FUN_MODULO_META_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/put/frame/replace_parameter.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{
namespace result_of{

    template<typename D>
    struct modulo_fun{
            
        typedef put_aux::replace_fun<D> meta_;
            
        template<typename F>
        struct apply : ::boost::mpl::apply1<meta_, F>{};
        
    };
            
}// result_of
}// put_aux
//[result_of_modulo_fun
namespace result_of{

    template<typename /*<<Inherits crtp\<\> >>*/ D>
    struct /*<<Meta-function class>>*/ modulo_fun 
        : put_aux::result_of::modulo_fun<D>
    {};

}// result_of
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_FUN_MODULO_META_ER_2010_HPP