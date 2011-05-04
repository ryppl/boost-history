//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_ITERATE_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_ITERATE_ER_2011_HPP
#include <cstddef>

namespace boost{
namespace assign{
namespace v2{
//syntax_detail_functor_iterate
namespace functor_aux{

    struct iterate
    {
        
        typedef std::size_t result_type;
        
        iterate():i( 0 ){}
        iterate( result_type i_ ):i( i_ ){}
        
        result_type operator()()const{ return this->i++; }

        typedef iterate type;

        private:
        mutable result_type i;

    };

}// functor_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_FUNCTOR_ITERATE_ER_2011_HPP
