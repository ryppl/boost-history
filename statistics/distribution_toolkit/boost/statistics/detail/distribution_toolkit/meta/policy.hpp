//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::meta::policy.hpp                                   //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_POLICY_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_META_POLICY_HPP_ER_2009

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace meta{

    template<typename D>
    struct policy{
        typedef typename D::policy_type type;
    };
    
}// meta
}// distribution_toolkit
}// detail
}// statistics
}// boost


#endif