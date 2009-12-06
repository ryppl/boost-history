//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::unscope::os.hpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_UNSCOPE_OS_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_UNSCOPE_OS_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_pseudo_scalar_distribution.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

    template<typename D>
    typename 
    boost::lazy_enable_if<
    	meta::is_pseudo_scalar_distribution<D>,
        boost::mpl::identity<std::ostream&> 
    >::type
    operator<<(std::ostream& os,const D& dist){
        return (os << description(dist));
    }


}// toolkit
}// distribution
}// detail
}// statistics
}// boost

// using boost::statistics::detail::distribution::toolkit::operator<<

#endif





