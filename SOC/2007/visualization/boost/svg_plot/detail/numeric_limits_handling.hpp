// numeric_limits_handling.hpp 
// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP
#define _BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP

#include <limits>

namespace boost{
namespace svg{
namespace detail{

bool limit_max(double a)
{
    return (a == std::numeric_limits<double>::max()
         || a == std::numeric_limits<double>::infinity());
}

bool limit_min(double a)
{
    return (a == std::numeric_limits<double>::min()
        || a == -std::numeric_limits<double>::infinity()
        || a == std::numeric_limits<double>::denorm_min());
}

bool limit_NaN(double a)
{
    return (a == std::numeric_limits<double>::quiet_NaN()
        || a == std::numeric_limits<double>::signaling_NaN());
}

}
}
}
#endif
