//////////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::wrapper::description.hpp                    //
//                                                                                  //
//  (C) Copyright 2009 Erwann Rogard                                                //
//  Use, modification and distribution are subject to the                           //
//  Boost Software License, Version 1.0. (See accompanying file                     //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)                //
//////////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_DESCRIPTION_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_DESCRIPTION_HPP_ER_2009
#include <string>
#include <boost/format.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/wrapper.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename D>
    std::string
    description(const wrapper<D>& w)
    {
        static const char* msg = "wrapper(%1%)";
        format f(msg); f % description(w.const_reference());
        return f.str();
    }
    
}// distribution_toolkit
}// detail
}// statistics
}// boost


#endif