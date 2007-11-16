// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SIGNAL_RUNTIME_SUPPORT_HPP
#define SIGNAL_NETWORK_SIGNAL_RUNTIME_SUPPORT_HPP

#include <boost/dataflow/blueprint/connection_t.hpp>

namespace boost { namespace dataflow {

namespace signals {

template<typename PortTraits>
struct mechanism::runtime_connection<
    PortTraits,
    typename enable_if<is_same<mechanism, typename PortTraits::mechanism> >::type >
{
    typedef blueprint::connection_t<
        mechanism,
        boost::signal<typename PortTraits::signature_type> &,
        boost::function<typename PortTraits::signature_type>
    > type;
};

}

} }

#endif // SIGNAL_NETWORK_SIGNAL_RUNTIME_SUPPORT_HPP