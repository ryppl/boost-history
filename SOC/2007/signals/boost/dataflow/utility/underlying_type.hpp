// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UNDERLYING_TYPE_HPP
#define BOOST_DATAFLOW_UNDERLYING_TYPE_HPP

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace dataflow { namespace utility {

template<typename T>
struct underlying_type
{
    typedef
        typename remove_cv<
            typename remove_reference<T>::type
        >::type type;
};

} } }

#endif // BOOST_DATAFLOW_UNDERLYING_TYPE_HPP