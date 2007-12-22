// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_FUSION_COMPONENT_HPP
#define BOOST_DATAFLOW_SUPPORT_FUSION_COMPONENT_HPP

#include <boost/dataflow/detail/transform_remove_reference.hpp>
#include <boost/dataflow/support/component.hpp>
#include <boost/dataflow/support/fusion_port_vector.hpp>


namespace boost { namespace dataflow {

/// Convenience base class for FusionComponentTraits types.
template<typename Sequence, typename DefaultPorts=mpl::map<>, typename Tag=default_tag >
struct fusion_component_traits
    : public component_traits<
        typename detail::transform_remove_reference<Sequence>::type,
        DefaultPorts,
        Tag>
{
    /// MPL Sequence of result types for get_port
    typedef Sequence fusion_ports;
};

}}

#endif // BOOST_DATAFLOW_SUPPORT_FUSION_COMPONENT_HPP
