// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_ENABLE_IF_DEFINED_HPP
#define SIGNAL_NETWORK_ENABLE_IF_DEFINED_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace signals {
    
namespace detail
{
/*    template<typename T>
    struct defined : public boost::true_type {};
    
    template<typename T, typename Result=void>
    struct enable_if_defined : public boost::enable_if<defined<T>, Result> 
    {};*/
    template<typename T, typename Result=void>
    struct enable_if_defined
    {
        typedef Result type;
    };
}

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_ENABLE_IF_DEFINED_HPP
