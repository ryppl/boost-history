// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP

#include <boost/dataflow/support/binary_operation.hpp>

#include <boost/utility/result_of.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {

namespace extension
{
    template<typename KeyedPortTag, typename KeyPortTag>
    struct get_keyed_port_impl
    {        
        typedef detail::not_specialized result_type;

        template<typename KeyedPort, typename Key>
        result_type operator()(KeyedPort &)
        {
            // Error: get_keyed_port_impl has not been implemented
            // for KeyedPortTag and KeyPort.
            BOOST_STATIC_ASSERT(sizeof(KeyedPort)==0);
            return result_type();
        }
    };
}

template<typename KeyPortTraits, typename Tag, typename KeyedPort>
inline typename boost::result_of<
    extension::get_keyed_port_impl<
        typename port_traits_of<KeyedPort, Tag>::type,
        KeyPortTraits
    > (KeyedPort &)
>::type
get_keyed_port(KeyedPort &p)
{
    return extension::get_keyed_port_impl<
        typename port_traits_of<KeyedPort, Tag>::type,
        KeyPortTraits
    >()(p);
}

namespace extension
{
    template<typename ProducerTag, typename ConsumerTag, typename Operation>
    struct binary_operation_impl<ProducerTag, ConsumerTag, Operation,
            typename enable_if<
            boost::is_same<typename ProducerTag::concept, concepts::keyed_port>
        >::type >
    {
        typedef void result_type;
        
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            binary_operation<Operation, typename ConsumerTag::tag>
                (get_keyed_port<ConsumerTag, typename ProducerTag::tag>(producer), consumer);
        }
    };
    
    template<typename ProducerTag, typename ConsumerTag, typename Operation>
    struct binary_operation_impl<ProducerTag, ConsumerTag, Operation,
            typename enable_if<
            boost::is_same<typename ConsumerTag::concept, concepts::keyed_port>
        >::type >
    {
        typedef void result_type;

        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            binary_operation<Operation, typename ProducerTag::tag>
                (producer, get_keyed_port<ProducerTag, typename ConsumerTag::tag>(consumer));
        }
    };

}

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
