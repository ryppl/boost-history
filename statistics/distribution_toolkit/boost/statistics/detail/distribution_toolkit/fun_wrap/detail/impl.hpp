//////////////////////////////////////////////////////////////////////////////
// distribution_toolkitfun_wrap::detail::fun_wrap.hpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_FUN_WRAP_DETAIL_IMPL_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_FUN_WRAP_DETAIL_IMPL_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/meta/pointer.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace fun_wrap{

    template<typename D,typename meta::pointer<D>::type f>
    struct impl{
        typedef typename meta::pointer<D>::type type;
        static type instance;
    };
    
    template<typename D,typename meta::pointer<D>::type f>
    typename impl<D,f>::type
    impl<D,f>::instance = f;
    
}// fun_wrap
}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif
