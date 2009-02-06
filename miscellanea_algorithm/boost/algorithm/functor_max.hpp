//////////////////////////////////////////////////////////////////////////////
// functor_max.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_FUNCTOR_MAX_HPP_ER_2009
#define BOOST_FUNCTOR_MAX_HPP_ER_2009
namespace boost{
        // TODO alternatively, specialize on T and use abs, labs, fabs?
        template<typename T>
        struct functor_max{
            functor_max(){}
            T operator()(T x,T y){ return (x<y)? y : x; }
        };
}
#endif
