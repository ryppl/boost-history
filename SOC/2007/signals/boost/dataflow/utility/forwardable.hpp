// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_FORWARDABLE_HPP
#define BOOST_DATAFLOW_UTILITY_FORWARDABLE_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_reference.hpp>

namespace boost { namespace dataflow { namespace utility {

template<typename T>
struct forwardable
{
    typedef const T & type;
};

template<typename T>
struct forwardable<T &>
{
    typedef T & type;
};

} } }

#endif // BOOST_DATAFLOW_UTILITY_FORWARDABLE_HPP
