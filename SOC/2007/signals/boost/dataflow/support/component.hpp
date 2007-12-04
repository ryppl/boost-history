// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

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
template<typename PortSequence, typename DefaultPorts=mpl::map<>, typename ComponentConcept=concepts::component, typename Tag=default_tag>
struct component_traits
{
    /// MPL Sequence of exposed port types.
    typedef PortSequence ports;
    /// MPL Sequence of default ports.
    typedef DefaultPorts default_ports;
    /// Concept the Component models.
    typedef ComponentConcept concept;
    /// Tag.
    typedef Tag tag;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((mpl::is_sequence<PortSequence>));
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((mpl::is_sequence<DefaultPorts>));
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((is_base_of<concepts::component, ComponentConcept>));
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
    typename detail::enable_if_defined<
        detail::all_of<
            typename ComponentTraits::ports,
            typename ComponentTraits::default_ports,
            typename ComponentTraits::concept,
            typename ComponentTraits::tag
        >
    >::type>
 : public mpl::true_
{};

/// Metafunction returning the ComponentTraits of a Component.
template<typename T, typename Tag=default_tag, typename Enable=void>
struct component_traits_of
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT((is_same<Enable, void>));
};

#ifndef DOXYGEN_DOCS_BUILD
// Specialization allowing intrusive specification of the ComponentTraits.
template<typename T, typename Tag>
struct component_traits_of<
    T,
    Tag,
    typename detail::enable_if_defined<typename T::component_traits>::type >
{
    typedef typename T::component_traits type;
    BOOST_MPL_ASSERT(( is_component_traits<type> ));
};
#endif // DOXYGEN_DOCS_BUILD

/// Boolean metafunction determining whether a type is a Component.
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_component
    : public mpl::false_ {};

/// INTERNAL ONLY
template<typename T, typename Tag>
struct is_component<
    T,
    Tag,
    typename detail::enable_if_defined<
        typename component_traits_of<T, Tag>::type
    >::type >
    : public mpl::true_ {};

/// Convenience base class for Component types.
template<typename ComponentTraits>
struct component
{
    /// ComponentTraits of the Component.
    typedef ComponentTraits component_traits;
};

#ifndef DOXYGEN_DOCS_BUILD
namespace extension {

    template<typename ComponentTraits, typename Enable=void>
    struct get_port_impl
    {
        BOOST_MPL_ASSERT((is_same<Enable, void>));

        typedef void result_type;
        
        typedef void not_specialized;

        template<typename Component, int N>
        void operator()(Component &, mpl::int_<N>)
        {
            // Error: get_port_impl has not been
            // implemented for ComponentTraits.
            BOOST_STATIC_ASSERT(sizeof(Component)==0);
        }
    };
    
} // namespace extension

namespace result_of
{
    template<typename Component, int N>
    struct get_port_c
    {
        typedef typename boost::result_of<
            extension::get_port_impl<
            typename component_traits_of<Component>::type
        >(Component &, mpl::int_<N>)>::type type;
    };

    template<typename Component, typename N>
    struct get_port
    {
        typedef typename get_port_c<Component, N::value>::type type;
    };
}

template<int N, typename Component>
inline typename result_of::get_port_c<Component, N>::type
get_port_c(Component &component)
{
    return
        extension::get_port_impl<
            typename component_traits_of<Component>::type
        >()(component, mpl::int_<N>());
}

template<typename N, typename Component>
inline typename result_of::get_port<Component, N>::type
get_port(Component &component)
{
    return
        extension::get_port_impl<
            typename component_traits_of<Component>::type
        >()(component, mpl::int_<N::value>());
}

template<typename Direction, typename Mechanism=default_mechanism>
struct default_port_selector
{
    typedef Direction direction;
    typedef Mechanism mechanism;
};

template<typename T, typename Tag>
struct traits_of<T, Tag, typename enable_if<is_component<T> >::type>
    : public component_traits_of<T>
{};

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
            typedef typename result_of::get_port<Component, port_number>::type type;
        };
        
        template<typename Component>
        typename result<get_default_port_impl(Component &)>::type operator()(Component &c)
        {
            return get_port<port_number>(c);
        }
    };
    
} // namespace extension
#endif // DOXYGEN_DOCS_BUILD

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
