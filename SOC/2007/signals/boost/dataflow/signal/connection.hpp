// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CONNECTION_HPP
#define SIGNAL_NETWORK_CONNECTION_HPP

#include <boost/dataflow/signal/support.hpp>
#include <boost/dataflow/connection/operators.hpp>
#include <boost/dataflow/signal/connection/slot_selector.hpp>
#include <boost/dataflow/signal/connection/slot_selector_map.hpp>

namespace boost { namespace signals {
    
    using boost::dataflow::operators::operator>>=;
    using boost::dataflow::operators::operator|;

} }

#endif