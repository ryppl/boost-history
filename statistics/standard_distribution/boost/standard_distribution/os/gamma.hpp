//////////////////////////////////////////////////////////////////////////////
// distributions::os::gamma.hpp                                             //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_OS_GAMMA_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_OS_GAMMA_HPP_ER_2009
#include <ostream>
#include <boost/format.hpp>
#include <boost/math/distributions/gamma.hpp>

namespace boost{
namespace math{

    template<typename T>
    std::ostream& operator<<(std::ostream& out,
        const gamma_distribution<T>& dist)
    {
        static const char* msg = "gamma(%1%,%2%)";
        format f(msg); f % dist.shape() % dist.scale();
        return (out << f.str());
    }
    
}// boost
}// math


#endif