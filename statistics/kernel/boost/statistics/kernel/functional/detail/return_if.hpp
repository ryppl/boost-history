///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::functional::detail::return_if.hpp                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_STATISTICS_KERNEL_FUNCTIONAL_DETAIL_RETURN_IF_H_ER_2009
#define BOOST_STATISTICS_KERNEL_FUNCTIONAL_DETAIL_RETURN_IF_H_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_scalar.hpp>

namespace boost{
namespace statistics{
namespace kernel{
namespace detail{

// TODO no longer needed

template<typename X1,typename R>
struct return_if_scalar : boost::enable_if<
    boost::is_scalar<X1>,
    R     
>{};

template<typename X1,typename R>
struct return_if_multi : boost::disable_if<
    boost::is_scalar<X1>,
    R     
>{};

}
}
}
}

#endif