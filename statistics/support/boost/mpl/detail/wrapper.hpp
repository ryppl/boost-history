///////////////////////////////////////////////////////////////////////////////
// wrapper.hpp                                                               //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_MPL_WRAPPER_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_MPL_WRAPPER_HPP_ER_2010
#include <boost/math/policies/error_handling.hpp>  
#include <boost/math/policies/policy.hpp>  

namespace boost{
namespace statistics{
namespace detail{

    // TODO Is there already generic default constructible wrapper in MPL/Fusion?
    // boost::mpl::for_each<Seq,wrapper<mpl::_> > 
    template<typename Tag>
    struct mpl_wrapper{ mpl_wrapper(){} };

    
}// detail
}// statistics
}// boost

#endif