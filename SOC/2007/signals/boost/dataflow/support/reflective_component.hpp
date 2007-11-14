// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_INTROSPECTION_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_INTROSPECTION_HPP

#include <boost/dataflow/support/component.hpp>

#include <boost/static_assert.hpp>
#include <boost/utility/result_of.hpp>


namespace boost { namespace dataflow {

namespace extension {

    template<typename ComponentTraits, typename Enable=void>
    struct get_component_port_impl
    {
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

template<typename Mechanism, typename T, typename Component>
typename result_of<
    extension::get_component_port_impl<
        typename component_traits_of<Mechanism, Component>::type
    >(Component &, T)>::type
get_component_port(Component &component)
{
    return
        extension::get_component_port_impl<
            typename component_traits_of<Mechanism, Component>::type
        >()(component, T());
}

} } // namespace boost::dataflow


#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_INTROSPECTION_HPP