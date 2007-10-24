// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_OPERATION_HPP

#include <boost/dataflow/support/component.hpp>

#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

namespace detail {
    struct not_implemented;
}

namespace operations
{
    struct invoke;
}

namespace extension
{
    template<typename Operation, typename ComponentTraits, typename Enable=void>
    struct component_operation_impl
    {
        template<typename Component>
        struct apply
        {
            struct detail
            {
                typedef void not_specialized;
            };

            static void call(Component &)
            {
                // Error: component_operation_impl Operation has not been
                // implemented for ComponentTraits.
                BOOST_STATIC_ASSERT(sizeof(Component)==0);
            }
        };
    };
}

template<typename Operation, typename Mechanism, typename T, typename Enable=void>
struct implements_component_operation
    : public mpl::true_ {};

template<typename Operation, typename Mechanism, typename Component, typename T>
struct implements_component_operation<
    Operation,
    Mechanism,
    T,
    typename detail::enable_if_defined<
        typename extension::component_operation_impl<
            Operation,
            typename component_traits_of<Mechanism, Component>::type
        >::template apply<Component>::detail::not_specialized
    >::type
>
    : public mpl::false_ {};

template<typename Operation, typename Mechanism, typename Component>
void component_operation(Component &producer)
{
    extension::component_operation_impl<
        Operation,
        typename component_traits_of<Mechanism, Component>::type
    >::template apply<Component>::call(producer);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_OPERATION_HPP
