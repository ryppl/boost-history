// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/result_of.hpp>


namespace boost { namespace dataflow {

template<typename PortSequence>
struct component_traits
{
    typedef PortSequence ports;
    BOOST_MPL_ASSERT((mpl::is_sequence<PortSequence>));
};

/// Boolean metafunction determining whether a type is a ComponentTraits.
template<typename ComponentTraits, typename Enable=void>
struct is_component_traits : public mpl::false_
{
    BOOST_MPL_ASSERT((is_same<Enable, void>));
};

template<typename ComponentTraits>
struct is_component_traits<ComponentTraits,
    typename detail::enable_if_defined<
        typename ComponentTraits::ports
    >::type>
 : public mpl::true_
{};

/// Metafunction returning the ComponentTraits of a type.
template<typename T, typename Enable=void>
struct component_traits_of
{
    BOOST_MPL_ASSERT((is_same<Enable, void>));
};

/// Specialization allowing intrusive specification of the ComponentTraits.
template<typename T>
struct component_traits_of<T>
{
    typedef typename T::component_traits type;
    BOOST_MPL_ASSERT(( is_component_traits<type> ));
};

namespace extension {

    template<typename ComponentTraits, typename Enable=void>
    struct get_component_port_impl
    {
        BOOST_MPL_ASSERT((is_same<Enable, void>));

        typedef void result_type;
        
        struct detail
        {
            typedef void not_specialized;
        };

        template<typename Component, typename T>
        void operator()(Component &, T)
        {
            // Error: get_component_port_impl has not been
            // implemented for ComponentTraits.
            BOOST_STATIC_ASSERT(sizeof(Component)==0);
        }
    };
    
} // namespace extension

template<typename T, typename Component>
struct get_component_port_result_type
{
    typedef typename result_of<
    extension::get_component_port_impl<
        typename component_traits_of<Component>::type
    >(Component &, T)>::type type;
};

template<typename T, typename Component>
typename get_component_port_result_type<T, Component>::type
get_component_port(Component &component)
{
    return
        extension::get_component_port_impl<
            typename component_traits_of<Component>::type
        >()(component, T());
}


} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
