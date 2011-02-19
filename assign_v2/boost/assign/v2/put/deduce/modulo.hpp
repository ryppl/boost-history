//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DEDUCE_MODULO_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_DEDUCE_MODULO_ER_2010_HPP
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/assign/v2/put/deduce/modifier.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_aux{

    struct kwd_deduce{}; 

    template<typename T>
    typename T::result_of_modulo::deduce::type
    operator%(T const& t, kwd_deduce const& kwd)
    {
        return t.modulo_deduce();
    }

    template<typename Option1, typename Option2>
    struct deduce : ::boost::mpl::eval_if<
        boost::is_same<Option1, boost::use_default>,
        boost::mpl::identity<Option2>,
        boost::mpl::identity<Option1>
    >{};

}// put_aux
namespace{
    const put_aux::kwd_deduce _deduce = put_aux::kwd_deduce();
}    
}// v2
}// assign
}// boost

#endif
