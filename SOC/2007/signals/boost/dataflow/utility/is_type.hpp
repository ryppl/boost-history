// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_IS_TYPE_HPP
#define BOOST_DATAFLOW_UTILITY_IS_TYPE_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace dataflow {

namespace utility {

template<typename T>
struct is_type
    : public mpl::true_ {};

} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
