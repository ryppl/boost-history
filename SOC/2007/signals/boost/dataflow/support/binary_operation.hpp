// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
#define BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP

#include <boost/dataflow/support/proxy_port.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace boost { namespace dataflow {

namespace operations {
    struct connect;
    struct connect_only;
    struct disconnect;
    struct extract;
}

namespace extension
{
    template<typename Operation, typename ProducerTag, typename ConsumerTag, typename Enable=void>
    struct binary_operation_impl
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &, Consumer &)
            {
                // Error: binary_operation_impl has not been implemented
                // for ProducerTag and ConsumerTag.
                BOOST_STATIC_ASSERT(sizeof(Producer)==0);
            }
        };
    };
}

template<typename Operation, typename Mechanism, typename Producer, typename Consumer>
inline void binary_operation(Producer &producer, Consumer &consumer)
{
    typedef typename boost::remove_cv<Producer>::type ProducerNoCV;
    typedef typename boost::remove_cv<Consumer>::type ConsumerNoCV;

    extension::binary_operation_impl<
        Operation,
        typename port_traits_of<Mechanism, ports::producer, ProducerNoCV>::type,
        typename port_traits_of<Mechanism, ports::consumer, ConsumerNoCV>::type>
            ::template apply<
                typename boost::remove_reference<
                    typename get_port_result_type<Mechanism, ports::producer, Producer>::type
                >::type,
                typename boost::remove_reference<
                    typename get_port_result_type<Mechanism, ports::consumer, Consumer>::type
                >::type
            >::call(get_port<Mechanism, ports::producer>(producer),
                    get_port<Mechanism, ports::consumer>(consumer));
}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
