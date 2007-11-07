// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP

#include <boost/dataflow/support/port.hpp>

#include <boost/static_assert.hpp>


namespace boost { namespace dataflow {

namespace detail {
    struct not_implemented;
}

namespace operations
{
    struct disconnect_all;
}

namespace extension
{
    template<typename Operation, typename PortTraits, typename Enable=void>
    struct unary_operation_impl
    {
        struct detail
        {
            typedef void not_specialized;
        };
        template<typename Producer>
        void operator()(Producer &)
        {
            // Error: unary_operation_impl Operation has not been
            // implemented for PortTraits.
            BOOST_STATIC_ASSERT(sizeof(Producer)==0);
        }
    };
}

template<typename Operation, typename Mechanism, typename PortCategory, typename T, typename Enable=void>
struct implements_unary_operation
    : public mpl::true_ {};

template<typename Operation, typename Mechanism, typename PortCategory, typename T>
struct implements_unary_operation<
    Operation,
    Mechanism,
    PortCategory,
    T,
    typename detail::enable_if_defined<
        typename extension::unary_operation_impl<
            Operation,
            typename port_traits_of<Mechanism, PortCategory, T>::type
        >::detail::not_specialized
    >::type
>
    : public mpl::false_ {};

template<typename Operation, typename Mechanism, typename PortCategory, typename Port>
void unary_operation(Port &producer)
{
    extension::unary_operation_impl<
        Operation,
        typename port_traits_of<Mechanism, PortCategory, Port>::type
    >()(producer);
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_UNARY_OPERATION_HPP
