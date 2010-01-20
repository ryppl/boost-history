///////////////////////////////////////////////////////////////////////////////
// kolmogorov_smirnov::keyword::reference_distribution.hpp         		 	 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_KEYWORD_REFERENCE_DISTRIBUTION_HPP_2010
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_KEYWORD_REFERENCE_DISTRIBUTION_HPP_2010
#include <boost/parameter/name.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/statistics/detail/non_parametric/empirical_distribution/tag/key.hpp> // TODO rename

namespace boost { 
namespace statistics{
namespace detail{
namespace kolmogorov_smirnov{
namespace keyword{
    namespace 
    {
        boost::parameter::keyword<
        	kolmogorov_smirnov::tag::reference_distribution
        >& reference_distribution
            = boost::parameter::keyword<
            	kolmogorov_smirnov::tag::reference_distribution
            >::get();
    }
}// keyword
}// kolmogorov_smirnov
}// detail
}// statistics
}// boost

#endif