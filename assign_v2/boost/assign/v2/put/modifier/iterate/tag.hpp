//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_ITERATE_TAG_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_ITERATE_TAG_ER_2010_HPP
#include <cstddef>
//#include <boost/lambda/lambda.hpp>
//#include <boost/typeof/typeof.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace modifier_tag{ 

    // TODO think of lambda expression
    struct iterate_arg
    {
        
        typedef std::size_t result_type;
        
        iterate_arg():i( 0 ){}
        iterate_arg( result_type i_ ):i( i_ ){}
        
        result_type operator()()const{ return this->i++; }

        typedef iterate_arg type;
        
        private:
        mutable result_type i;
    };


    template<typename Arg = iterate_arg::type > struct iterate{}; 

}// modifier_tag
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_MODIFIER_ITERATE_TAG_ER_2010_HPP
