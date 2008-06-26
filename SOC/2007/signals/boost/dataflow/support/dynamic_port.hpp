// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_DYNAMIC_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_DYNAMIC_PORT_HPP

#include <boost/dataflow/support/port/traits.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/static_assert.hpp>

#ifndef DOXYGEN_DOCS_BUILD
// ***************************************
// * get_dynamic_port_size, has_dynamic_port_size
// ***************************************
#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME get_dynamic_port_size
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK has_dynamic_port_size
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 1
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_entity
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>

#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME get_subport
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK has_subport
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 1
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_entity
#define DATAFLOW_SPECIALIZABLE_OPERATION_EXTRA_TYPENAME_ARGS , size_t n
#define DATAFLOW_SPECIALIZABLE_OPERATION_EXTRA_TYPENAMES , size_t
#define DATAFLOW_SPECIALIZABLE_OPERATION_EXTRA_ARGS , n
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>

#endif // DOXYGEN_DOCS_BUILD

namespace boost { namespace dataflow {

template<typename SubPort, typename Tag=default_tag>
struct dynamic_port_traits
    : public port_traits<typename traits_of<SubPort,Tag>::type::category, Tag>
{
    typedef SubPort subport_type;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port<SubPort, Tag> ));
};

/// Boolean Metafunction determining whether a type is a DynamicPortTraits.
template<typename DynamicPortTraits, typename Enable=detail::enable_guard>
struct is_dynamic_port_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename DynamicPortTraits>
struct is_dynamic_port_traits<DynamicPortTraits,
    typename utility::enable_if_type<
        utility::all_of<
            typename DynamicPortTraits::subport_type,
            typename DynamicPortTraits::category,
            typename DynamicPortTraits::tag
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port<typename DynamicPortTraits::subport_type, typename DynamicPortTraits::tag> ));
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_tag<typename DynamicPortTraits::tag>));
};

/// Boolean Metafunction determining whether a type is a DynamicPort.
/** is_port<T, Tag> is an Integral Constant of type bool.
    It evaluates to true if T models Port, false otherwise.
*/
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_dynamic_port
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
struct is_dynamic_port<
    T,
    Tag,
    typename enable_if<mpl::and_<
        is_dynamic_port_traits<typename traits_of<T, Tag>::type>,
        has_dynamic_port_size<T, Tag>,
        has_subport<T, Tag>
    > >::type >
    : public mpl::true_ {};

/// Convenience base class for Port types. 
template<typename DynamicPortTraits>
struct dynamic_port
{
    /// PortTraits for the Port.
    typedef DynamicPortTraits dataflow_traits;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_dynamic_port_traits<DynamicPortTraits> ));
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_DYNAMIC_PORT_HPP


