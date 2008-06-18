// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPLEMENT_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPLEMENT_PORT_HPP

#include <boost/dataflow/support/port.hpp>

namespace boost { namespace dataflow {

/// Convenience class for ComplementedPortTraits types.
template<typename PortCategory, typename ComplementPort, typename Tag=default_tag>
struct complemented_port_traits
    : public port_traits<PortCategory, Tag>
{
    typedef ComplementPort complement_port_type; ///< complement Port type.
};

/// Boolean metafunction determining whether a type is a ComplementedPortTraits.
template<typename PortTraits, typename Enable=void>
struct is_complemented_port_traits : public mpl::false_
{};

/// INTERNAL ONLY
template<typename PortTraits>
struct is_complemented_port_traits<PortTraits,
    typename utility::enable_if_type<
        utility::all_of<
            typename PortTraits::category,
            typename PortTraits::complement_port_type,
            typename PortTraits::tag
        >
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port_category<typename PortTraits::category> ));
};

/// Boolean metafunction determining whether a type is a ComplementedPort.
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_complemented_port : public mpl::false_
{};

/// INTERNAL ONLY
template<typename T, typename Tag>
struct is_complemented_port<T, Tag,
        typename enable_if<
            is_complemented_port_traits<
                typename traits_of<T, Tag>::type
            >
        >::type >
    : public mpl::true_ {};


} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPLEMENT_PORT_HPP
