// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP

#include <boost/dataflow/support/binary_operation.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

namespace concepts
{
    struct keyed_port;
}

namespace extension
{
    template<typename KeyedPortTag, typename KeyPortTag>
    struct get_keyed_port_impl
    {        
        template<typename KeyedPort, typename Key>
        struct apply
        {
            typedef void type;
            static void call(KeyedPort &, Key &)
            {
                // Error: get_keyed_port_impl has not been implemented
                // for KeyedPortTag and KeyPort.
                BOOST_STATIC_ASSERT(sizeof(KeyedPort)==0);
            }
        };
    };
}
    
template<typename Mechanism, typename PortCategory, typename T1, typename T2>
typename extension::get_keyed_port_impl<
    typename port_traits_of<
        Mechanism, PortCategory, typename boost::remove_cv<T1>::type
    >::type,
    typename port_traits_of<
        Mechanism, typename PortCategory::complement, typename boost::remove_cv<T2>::type
    >::type
>::template apply<T1, T2>::type
get_keyed_port(T1 &t1, T2 &t2)
{
    return extension::get_keyed_port_impl<
        typename port_traits_of<
            Mechanism, PortCategory, typename boost::remove_cv<T1>::type
        >::type,
        typename port_traits_of<
            Mechanism, typename PortCategory::complement, typename boost::remove_cv<T2>::type
        >::type
    >::template apply<T1, T2>::call(t1, t2);
}

namespace extension
{
    template<typename Operation, typename ProducerTag, typename ConsumerTag>
    struct binary_operation_impl<Operation, ProducerTag, ConsumerTag,
            typename enable_if<
            boost::is_same<typename ProducerTag::concept, concepts::keyed_port>
        >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &producer, Consumer &consumer)
            {
                binary_operation<Operation, typename ProducerTag::mechanism>
                    (get_keyed_port<typename ProducerTag::mechanism, typename ProducerTag::category>(producer, consumer), consumer);
            }
        };
    };
    
    template<typename Operation, typename ProducerTag, typename ConsumerTag>
    struct binary_operation_impl<Operation, ProducerTag, ConsumerTag,
            typename enable_if<
            boost::is_same<typename ConsumerTag::concept, concepts::keyed_port>
        >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &producer, Consumer &consumer)
            {
                binary_operation<Operation, typename ProducerTag::mechanism>
                    (producer, get_keyed_port<typename ProducerTag::mechanism, typename ConsumerTag::category>(consumer, producer));
            }
        };
    };

}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
