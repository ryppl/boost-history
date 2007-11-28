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

namespace extension
{
    template<typename KeyedPortTag, typename KeyPortTag>
    struct get_keyed_port_impl
    {        
        typedef void result_type;

        template<typename KeyedPort, typename Key>
        void operator()(KeyedPort &)
        {
            // Error: get_keyed_port_impl has not been implemented
            // for KeyedPortTag and KeyPort.
            BOOST_STATIC_ASSERT(sizeof(KeyedPort)==0);
        }
    };
}

/*template<typename Mechanism, typename PortCategory, typename KeyPort, typename T>
inline typename result_of<
    extension::get_keyed_port_impl<
        typename port_traits_of<
            Mechanism, PortCategory, typename boost::remove_cv<T>::type
        >::type,
        typename port_traits_of<
            Mechanism, typename PortCategory::complement, typename boost::remove_cv<KeyPort>::type
        >::type
    > (T &)
>::type
get_keyed_port(T1 &t1, T2 &t2)
{
    return extension::get_keyed_port_impl<
        typename port_traits_of<
            Mechanism, PortCategory, typename boost::remove_cv<T1>::type
        >::type,
        typename port_traits_of<
            Mechanism, typename PortCategory::complement, typename boost::remove_cv<T2>::type
        >::type
    >()(t1, t2);
}*/

template<typename KeyPortTraits, typename KeyPort>
inline typename result_of<
    extension::get_keyed_port_impl<
        typename port_traits_of<
            typename KeyPortTraits::mechanism,
            typename KeyPortTraits::category::complement,
            typename boost::remove_cv<KeyPort>::type
        >::type,
        KeyPortTraits
    > (KeyPort &)
>::type
get_keyed_port(KeyPort &p)
{
    return extension::get_keyed_port_impl<
        typename port_traits_of<
            typename KeyPortTraits::mechanism,
            typename KeyPortTraits::category::complement,
            typename boost::remove_cv<KeyPort>::type
        >::type,
        KeyPortTraits
    >()(p);
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
        void operator()(Producer &producer, Consumer &consumer)
        {
            binary_operation<Operation, typename ProducerTag::mechanism>
                (get_keyed_port<ConsumerTag>(producer), consumer);
        }
    };
    
    template<typename Operation, typename ProducerTag, typename ConsumerTag>
    struct binary_operation_impl<Operation, ProducerTag, ConsumerTag,
            typename enable_if<
            boost::is_same<typename ConsumerTag::concept, concepts::keyed_port>
        >::type >
    {
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            binary_operation<Operation, typename ProducerTag::mechanism>
                (producer, get_keyed_port<ProducerTag>(consumer));
        }
    };

}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
