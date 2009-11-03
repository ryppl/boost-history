///////////////////////////////////////////////////////////////////////////////
// statistics::detail::accumulator::threshold.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_ACCUMULATOR_STATISTICS_KEYWORD_THRESHOLD_HPP_2009
#define BOOST_STATISTICS_DETAIL_ACCUMULATOR_STATISTICS_KEYWORD_THRESHOLD_HPP_2009
#include <boost/parameter/name.hpp>
#include <boost/parameter/keyword.hpp>

namespace boost { 
namespace statistics{
namespace detail{
namespace accumulator{
namespace tag{
    struct threshold{};
}// tag
namespace keyword{
    namespace 
    {
        boost::parameter::keyword<tag::threshold>& threshold
            = boost::parameter::keyword<tag::threshold>::get();
    }
}// keyword
}// accumulator
}// detail
}// statistics
}// boost

#endif