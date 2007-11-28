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

namespace extension
{
    template<typename Operation, typename ProducerTag, typename ConsumerTag, typename Enable=void>
    struct binary_operation_impl
    {
        struct detail
        {
            typedef void not_specialized;
        };
        
        template<typename Producer, typename Consumer>
        void operator()(Producer &, Consumer &)
        {
            // Error: binary_operation_impl has not been implemented
            // for ProducerTag and ConsumerTag.
            BOOST_STATIC_ASSERT(sizeof(Producer)==0);
        }
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
            ()(get_port<Mechanism, ports::producer>(producer),
               get_port<Mechanism, ports::consumer>(consumer));
}

template<typename Operation, typename Mechanism, typename ProducerPort, typename ConsumerPort, typename Enable=void>
struct are_binary_operable
    : public mpl::false_
{
    BOOST_MPL_ASSERT((is_same<Enable, void>));
};

template<typename Operation, typename Mechanism, typename Producer, typename Consumer, typename Enable=void>
struct specialized_binary_operation
    : public mpl::true_
{
    BOOST_MPL_ASSERT((is_same<Enable, void>));
};

template<typename Operation, typename Mechanism, typename Producer, typename Consumer>
struct specialized_binary_operation<
    Operation,
    Mechanism,
    Producer,
    Consumer,
    typename detail::enable_if_defined<
        typename extension::binary_operation_impl<
            Operation,
            typename port_traits_of<Mechanism, ports::producer, typename boost::remove_cv<Producer>::type>::type,
            typename port_traits_of<Mechanism, ports::consumer, typename boost::remove_cv<Consumer>::type>::type
        >::detail::not_specialized
    >::type
>
    : public mpl::false_ {};

template<typename Operation, typename Mechanism, typename Producer, typename Consumer>
struct are_binary_operable<
    Operation,
    Mechanism,
    Producer,
    Consumer,
    typename enable_if<
        mpl::and_<
            is_port<Mechanism, ports::producer, typename boost::remove_cv<Producer>::type>,
            is_port<Mechanism, ports::consumer, typename boost::remove_cv<Consumer>::type>,
            specialized_binary_operation<Operation,Mechanism,Producer,Consumer>
        >
    >::type
>
    : public mpl::true_ {};


} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_BINARY_OPERATION_HPP
