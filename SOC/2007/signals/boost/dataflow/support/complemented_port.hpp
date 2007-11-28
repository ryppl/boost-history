// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPLEMENT_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPLEMENT_PORT_HPP

#include <boost/dataflow/support/port.hpp>

namespace boost { namespace dataflow {

/// Convenience class for PortTraits types.
template<typename Mechanism, typename PortCategory, typename ComplementPort>
struct complemented_port_traits
{
    typedef Mechanism mechanism;
    typedef PortCategory category; 
    typedef concepts::complemented_port concept;
    typedef ComplementPort complement_port_type;
};

/// Boolean metafunction determining whether a type is a PortTraits.
template<typename PortTraits, typename Enable=void>
struct is_complemented_port_traits : public mpl::false_
{};

template<typename PortTraits>
struct is_complemented_port_traits<PortTraits,
    typename detail::enable_if_defined<
        detail::all_of<
            typename PortTraits::mechanism,
            typename PortTraits::category,
            typename PortTraits::concept,
            typename PortTraits::complement_port_type
        >
    >::type>
 : public mpl::true_
{
    BOOST_MPL_ASSERT(( is_port_category<typename PortTraits::category> ));
};

/// Boolean metafunction determining whether a type is a PortTraits.
template<typename Mechanism, typename PortCategory, typename T, typename Enable=void>
struct is_complemented_port : public mpl::false_
{};

template<typename Mechanism, typename PortCategory, typename T>
struct is_complemented_port<Mechanism, PortCategory, T,
        typename enable_if<
            is_complemented_port_traits<
                typename port_traits_of<
                    Mechanism,
                    PortCategory,
                    typename utility::underlying_type<T>::type
                >::type
            >
        >::type >
    : public mpl::true_ {};


} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPLEMENT_PORT_HPP