// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_VECTOR_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_VECTOR_PORT_HPP

#include <boost/dataflow/support/port/traits.hpp>
#include <boost/dataflow/support/port_vector.hpp>
#include <boost/dataflow/utility/is_type.hpp>
#include <boost/mpl/is_sequence.hpp>

namespace boost { namespace dataflow {

/// Convenience class for VectorPortTraits types.
template<typename PortCategory, typename PortSequence, typename Tag=default_tag>
struct vector_port_traits
    : public port_traits<PortCategory, Tag>
{
    /// MPL Seqence of Port types.
    typedef PortSequence ports;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( mpl::is_sequence<PortSequence> ));
};

/// Boolean Metafunction determining whether a type is a KeyedPortTraits.
template<typename Traits, typename Enable=detail::enable_guard>
struct is_vector_port_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename Traits>
struct is_vector_port_traits<
    Traits,
    typename enable_if<
        mpl::and_<
            is_port_traits<Traits>,
            utility::is_type<typename Traits::ports>
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( mpl::is_sequence<typename Traits::ports> ));
};

/// Boolean Metafunction determining whether a type is a VectorPort.
/** is_vector_port<T, Tag> is an Integral Constant of type bool.
    It evaluates to true if T models VectorPort, false otherwise.
*/
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_vector_port
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
struct is_vector_port<
    T,
    Tag,
    typename enable_if<
        is_vector_port_traits<typename traits_of<T, Tag>::type>
    >::type >
    : public mpl::true_ {};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_VECTOR_PORT_HPP
