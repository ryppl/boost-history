/////////////////////////////////////////////////////////////////////////////
// fwd.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FWD_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FWD_HPP_ER_2009
#include <boost/conditionally_specified_distribution/result_of/function_support.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace function{

template <typename D,typename Args>
inline
typename function_support<D>::type
support(const D& d,const Args& args)
{
    return support(d);
}

}
}//conditionally_specified_distribution
}//boost

#endif
