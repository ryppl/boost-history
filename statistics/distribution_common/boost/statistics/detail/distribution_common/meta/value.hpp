//////////////////////////////////////////////////////////////////////////////
// distribution::common::meta::value.hpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_COMMON_META_VALUE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_COMMON_META_VALUE_HPP_ER_2009

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace common{
namespace meta{

    template<typename D>
    struct value{
        typedef typename D::value_type type;
    };
    
}// meta
}// common
}// distribution

}// detail
}// statistics
}// boost

#endif

