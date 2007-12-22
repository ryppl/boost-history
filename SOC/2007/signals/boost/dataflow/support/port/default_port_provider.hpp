// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PORT_DEFAULT_PORT_PROVIDER_HPP
#define BOOST_DATAFLOW_SUPPORT_PORT_DEFAULT_PORT_PROVIDER_HPP

#include <boost/dataflow/support/default_port_provider.hpp>
#include <boost/dataflow/support/port/port.hpp>
#include <boost/dataflow/support/tags.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace dataflow {

namespace extension {

    template<typename PortTraits, typename Direction, typename Mechanism>
    struct get_default_port_impl<
        PortTraits,
        Direction,
        Mechanism,
        typename enable_if<is_port_traits<PortTraits> >::type >
    {
        template<typename FArgs>
        struct result;
        
        template<typename F, typename Port>
        struct result<F(Port &)>
        {
            typedef Port & type;
        };
        
        template<typename Port>
        Port &operator()(Port &p)
        {
            return p;
        }
    };
    
} // namespace extension

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PORT_DEFAULT_PORT_PROVIDER_HPP
