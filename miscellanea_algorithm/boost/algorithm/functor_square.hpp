//////////////////////////////////////////////////////////////////////////////
// functor_square.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_FUNCTOR_SQUARE_HPP_ER_2009
#define BOOST_FUNCTOR_SQUARE_HPP_ER_2009
namespace boost{

    template<typename T>
    struct functor_square{
        functor_square(){}
        T operator()(T x)const{return x*x; }
    };
}
#endif
