// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_DEFAULT_PORT_PROVIDER_HPP
#define BOOST_DATAFLOW_SUPPORT_DEFAULT_PORT_PROVIDER_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>
#include <boost/dataflow/support/tags.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace dataflow {

/// Metafunction returning the traits of a type.
template<typename T, typename Tag=default_tag, typename Enable=void>
struct traits_of
{
#ifdef DOXYGEN_DOCS_BUILD
    /// The traits of T.
    typedef detail::unspecified type;
#endif
};

/// Boolean Metafunction determining whether a type has traits.
template<typename T, typename Tag=default_tag, typename Enable=void>
struct has_traits : public mpl::false_
{
#ifdef DOXYGEN_DOCS_BUILD
    /// Integral Constant value type
    typedef bool value_type;
    /// true if T has traits, false otherwise.
    typedef detail::unspecified value;
    /// Boolean Integral Constant
    typedef detail::unspecified type;
#endif
};

/// INTERNAL ONLY
template<typename T, typename Tag>
struct has_traits<
    T,
    Tag,
    typename detail::enable_if_defined<typename traits_of<T, Tag>::type>::type >
    : public mpl::true_
{};

} } // namespace boost::dataflow

// ***************************************
// * get_default_port, has_default_port
// ***************************************
#ifdef DOXYGEN_DOCS_BUILD

namespace boost { namespace dataflow {

// THIS IS A DOXYGEN DEFINITION ONLY!
/// Returns the default port for a Direction and Mechanism.
template<typename Direction, typename Mechanism,
    Tag=default_tag, typename T1>
inline detail::unspecified get_default_port(T1 &t1);

// THIS IS A DOXYGEN DEFINITION ONLY!
/// Boolean Metafunction determining whether a type has a default port.
template<
    typename T,
    typename Direction, typename Mechanism=default_mechanism,
    typename Tag=default_tag,
    typename Enable=void>
struct has_default_port
{};

}}

#else

#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME get_default_port
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK has_default_port
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES_WDEFAULTS typename Direction, typename Mechanism=default_mechanism
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES typename Direction, typename Mechanism
#define DATAFLOW_SPECIALIZABLE_OPERATION_TEMPLATES Direction, Mechanism
#define DATAFLOW_SPECIALIZABLE_OPERATION_USE_TAG_IN_IMPL
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 1
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS has_traits
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>

#endif // DOXYGEN_DOCS_BUILD
namespace boost { namespace dataflow {

/// Metafunction returning the PortTraits of a default Port.
template<typename T, typename Direction, typename Mechanism=default_mechanism, typename Tag=default_tag, typename Enable=void>
struct default_port_traits_of
{
#ifndef DOXYGEN_DOCS_BUILD
    typedef typename port_traits_of<
        typename remove_reference<
            typename result_of::get_default_port<T, Direction, Mechanism, Tag>::type
        >::type,
        Tag
    >::type type;
#else
    typedef detail::unspecified type; ///< A PortTraits type.
#endif
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_DEFAULT_PORT_PROVIDER_HPP
