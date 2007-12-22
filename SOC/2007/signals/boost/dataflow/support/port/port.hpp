// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PORT_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_PORT_PORT_HPP

#include <boost/dataflow/utility/enable_if_type.hpp>
#include <boost/dataflow/support/port/traits.hpp>
#include <boost/dataflow/support/tags.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace dataflow {

/// Boolean Metafunction determining whether a type is a Port.
/** is_port<T, Tag> is an Integral Constant of type bool.
    It evaluates to true if T models Port, false otherwise.
*/
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_port
    : public mpl::false_
{
#ifdef DOXYGEN_DOCS_BUILD
    /// Integral Constant value type
    typedef bool value_type;
    /// true if T models Port, false otherwise.
    typedef detail::unspecified value;
    /// Integral Constant
    typedef detail::unspecified type;
#endif
};

/// INTERNAL ONLY
template<typename T, typename Tag>
struct is_port<
    T,
    Tag,
    typename enable_if<
        is_port_traits<typename traits_of<T, Tag>::type>
    >::type >
    : public mpl::true_ {};

/// Convenience base class for Port types. 
template<typename PortTraits>
struct port
{
    /// PortTraits for the Port.
    typedef PortTraits dataflow_traits;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port_traits<PortTraits> ));
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_PORT_PORT_HPP
