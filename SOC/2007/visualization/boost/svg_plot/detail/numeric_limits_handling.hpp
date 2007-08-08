// numeric_limits_handling.hpp 
// Copyright (C) Jacob Voytko 2007
//
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP
#define _BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP

#include <limits>
#include <cmath>

namespace boost{
namespace svg{
namespace detail{

inline bool limit_max(double a)
{
    return (a == std::numeric_limits<double>::max()
         || a == std::numeric_limits<double>::infinity());
}

inline bool limit_min(double a)
{
    return (a == std::numeric_limits<double>::min()
        || a == -std::numeric_limits<double>::infinity()
        || a == std::numeric_limits<double>::denorm_min());
}

inline bool limit_NaN(double a)
{
    // Ternary operator used to remove warning of performance of casting
    // int to bool.
#if defined (BOOST_MSVC)
    return _isnan(a) ? true : false;
#else
    return std::fpclassify(a) == FP_NAN;
#endif
}

inline bool is_limit(double a)
{
    return limit_max(a) || limit_min(a) || limit_NaN(a);
}

inline bool pair_is_limit(std::pair<double, double> a)
{
        return limit_max(a.first) || limit_min(a.first) || limit_NaN(a.first)
        || limit_max(a.second) || limit_min(a.second) || limit_NaN(a.second);
}

}
}
}
#endif
