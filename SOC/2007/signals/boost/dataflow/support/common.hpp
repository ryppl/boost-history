// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMMON_HPP
#define BOOST_DATAFLOW_SUPPORT_COMMON_HPP

namespace boost { namespace dataflow {

/// Tag describing that dataflow properties should apply to all mechanisms.
struct all_mechanisms;

namespace connections
{
    struct none;
    struct one;
    struct many;
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMMON_HPP