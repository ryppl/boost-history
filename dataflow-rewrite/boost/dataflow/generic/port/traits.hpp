/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__PORT__TRAITS_HPP
#define BOOST__DATAFLOW__GENERIC__PORT__TRAITS_HPP

#include <boost/dataflow/generic/port/category.hpp>
#include <boost/dataflow/generic/framework_entity/traits.hpp>


namespace boost { namespace dataflow {

/// Convenience class for PortTraits types.
template<typename PortCategory, typename Framework=default_framework>
struct port_traits : public traits<Framework>
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
            typename PortTraits::framework
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_port_category<typename PortTraits::category> ));
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_framework<typename PortTraits::framework>));
};


} } // namespace boost::dataflow


#endif // BOOST__DATAFLOW__GENERIC__PORT__TRAITS_HPP
