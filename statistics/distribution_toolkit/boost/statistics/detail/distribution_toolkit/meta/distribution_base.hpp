//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::meta::distribution_base.hpp                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_DISTRIBUTION_BASE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_DISTRIBUTION_BASE_HPP_ER_2009
#include <boost/mpl/identity.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace meta{


    // TODO remove
    template<typename D>
    struct distribution_base : mpl::identity<D>{};

}// meta
}// distribution
}// toolkit

}// detail
}// statistics
}// boost

#endif 