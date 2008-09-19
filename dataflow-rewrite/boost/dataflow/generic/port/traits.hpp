// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PORT_TRAITS_HPP
#define BOOST_DATAFLOW_SUPPORT_PORT_TRAITS_HPP

#include <boost/dataflow/support/port/category.hpp>
#include <boost/dataflow/support/traits.hpp>


namespace boost { namespace dataflow {

/// Convenience class for PortTraits types.
template<typename PortCategory, typename Tag=default_tag>
struct port_traits : public traits<Tag>
{
    /// The PortCategory of the port.
    typedef PortCategory category;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port_category<PortCategory> ));
};


/// Boolean Metafunction determining whether a type is a PortTraits.
template<typename PortTraits, typename Enable=detail::enable_guard>
struct is_port_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename PortTraits>
struct is_port_traits<PortTraits,
    typename utility::enable_if_type<
        utility::all_of<
            typename PortTraits::category,
            typename PortTraits::tag
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port_category<typename PortTraits::category> ));
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_tag<typename PortTraits::tag>));
};


} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_PORT_TRAITS_HPP
