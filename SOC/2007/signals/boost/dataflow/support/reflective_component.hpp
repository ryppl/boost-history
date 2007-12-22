// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_INTROSPECTION_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_INTROSPECTION_HPP

#include <boost/dataflow/support/component.hpp>



namespace boost { namespace dataflow {

template<typename Mechanism, typename PortSequence>
struct reflective_component_traits
{
    typedef Mechanism mechanism;
    typedef PortSequence ports;
};

namespace extension {

    template<typename ComponentTraits, typename Enable=void>
    struct get_port_impl
    {
        typedef void result_type;
        
        struct detail
        {
            typedef void not_specialized;
        };

        template<typename Component, typename T>
        void operator()(Component &, T)
        {
            // Error: get_port_impl has not been
            // implemented for ComponentTraits.
            BOOST_STATIC_ASSERT(sizeof(Component)==0);
        }
    };
    
} // namespace extension

template<typename Mechanism, typename T, typename Component>
struct get_port_result_type
{
    typedef typename result_of<
    extension::get_port_impl<
        typename traits_of<Mechanism, Component>::type
    >(Component &, T)>::type type;
};

template<typename Mechanism, typename T, typename Component>
typename get_port_result_type<Mechanism, T, Component>::type
get_port(Component &component)
{
    return
        extension::get_port_impl<
            typename traits_of<Mechanism, Component>::type
        >()(component, T());
}

} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_INTROSPECTION_HPP