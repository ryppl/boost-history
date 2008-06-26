// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP

#include <boost/dataflow/utility/enable_if_type.hpp>
#include <boost/dataflow/support/port/default_port_provider.hpp>
#include <boost/dataflow/support/port_vector.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/result_of.hpp>


namespace boost { namespace dataflow {

/// Convenience base class for ComponentTraits types.
template<typename PortSequence, typename DefaultPorts=mpl::map<>, typename Tag=default_tag>
struct component_traits
    : public traits<Tag>
{
    /// MPL Sequence of exposed port types.
    typedef PortSequence ports;
    /// MPL Sequence of default ports.
    typedef DefaultPorts default_ports;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((mpl::is_sequence<PortSequence>));
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((mpl::is_sequence<DefaultPorts>));
};

/// Boolean metafunction determining whether a type is a ComponentTraits.
template<typename ComponentTraits, typename Enable=void>
struct is_component_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((is_same<Enable, void>));
};

/// INTERNAL ONLY
template<typename ComponentTraits>
struct is_component_traits<ComponentTraits,
    typename utility::enable_if_type<
        utility::all_of<
            typename ComponentTraits::ports,
            typename ComponentTraits::default_ports,
            typename ComponentTraits::tag
        >
    >::type>
 : public mpl::true_
{};

/// Boolean metafunction determining whether a type is a Component.
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_component
    : public mpl::false_ {};

/// INTERNAL ONLY
template<typename T, typename Tag>
struct is_component<
    T,
    Tag,
    typename enable_if<
        is_component_traits<typename traits_of<T, Tag>::type>
    >::type >
    : public mpl::true_ {};

/// Convenience base class for Component types.
template<typename ComponentTraits>
struct component
{
    /// ComponentTraits of the Component.
    typedef ComponentTraits dataflow_traits;
};

#ifndef DOXYGEN_DOCS_BUILD
template<typename Direction, typename Mechanism=default_mechanism>
struct default_port_selector
{
    typedef Direction direction;
    typedef Mechanism mechanism;
};

namespace extension {

    template<typename ComponentTraits, typename Direction, typename Mechanism>
    struct get_default_port_impl<
        ComponentTraits,
        Direction,
        Mechanism,
        typename enable_if<
            mpl::and_<
                is_component_traits<ComponentTraits>,
                mpl::has_key<
                    typename ComponentTraits::default_ports,
                    default_port_selector<Direction, Mechanism>
                >
            >
        >::type >
    {
        typedef typename mpl::at<
                typename ComponentTraits::default_ports,
                default_port_selector<Direction, Mechanism>
            >::type port_number;
            
        template<typename FArgs>
        struct result;
        
        template<typename F, typename Component>
        struct result<F(Component &)>
        {
            typedef typename result_of::get_port<Component, port_number, typename ComponentTraits::tag>::type type;
        };
        
        template<typename Component>
        typename result<get_default_port_impl(Component &)>::type operator()(Component &c)
        {
            return get_port<port_number, typename ComponentTraits::tag>(c);
        }
    };
    
} // namespace extension
#endif // DOXYGEN_DOCS_BUILD

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
