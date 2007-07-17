// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_DETAIL_ENABLE_IF_DEFINED_HPP
#define DATAFLOW_DETAIL_ENABLE_IF_DEFINED_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace dataflow {
    
namespace detail
{
    template<typename T, typename Result=void>
    struct enable_if_defined
    {
        typedef Result type;
    };
}

} } // namespace boost::dataflow

#endif // DATAFLOW_DETAIL_ENABLE_IF_DEFINED_HPP
