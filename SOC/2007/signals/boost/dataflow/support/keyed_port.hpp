// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
#define BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP

#include <boost/dataflow/utility/is_type.hpp>
#include <boost/dataflow/support/binary_operation.hpp>
#include <boost/dataflow/support/vector_port.hpp>
#include <boost/mpl/transform.hpp>

#ifndef DOXYGEN_DOCS_BUILD
// ***************************************
// * get_keyed_port, has_keyed_port
// ***************************************
#define DATAFLOW_SPECIALIZABLE_OPERATION_NAME get_keyed_port
#define DATAFLOW_SPECIALIZABLE_OPERATION_CHECK has_keyed_port
#define DATAFLOW_SPECIALIZABLE_OPERATION_TYPENAME_TEMPLATES typename KeyPortTag
#define DATAFLOW_SPECIALIZABLE_OPERATION_TEMPLATES KeyPortTag
#define DATAFLOW_SPECIALIZABLE_OPERATION_ARITY 1
#define DATAFLOW_SPECIALIZABLE_OPERATION_TRAITS_OF traits_of
#define DATAFLOW_SPECIALIZABLE_OPERATION_HAS_TRAITS is_port
#include <boost/dataflow/support/detail/make_specializable_operation.hpp>
#endif // DOXYGEN_DOCS_BUILD

namespace boost { namespace dataflow {

/// Convenience class for KeyedPortTraits types.
/** \param PortCategory The category of the port.
    \param KeySequence MPL Seqence of key types (PortTraits types).
    \param PortSequence MPL Sequence of Ports.
    \param Tag Tag.
*/
template<typename PortCategory, typename KeySequence, typename PortSequence, typename Tag=default_tag>
struct keyed_port_traits
    : public vector_port_traits<
        PortCategory,
        PortSequence,
        Tag>
{
    /// MPL Seqence of key types (PortTraits types).
    typedef KeySequence port_keys;
};

/// Boolean Metafunction determining whether a type is a KeyedPortTraits.
template<typename Traits, typename Enable=detail::enable_guard>
struct is_keyed_port_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename Traits>
struct is_keyed_port_traits
    <Traits,
    typename enable_if<
        mpl::and_<
            is_vector_port_traits<Traits>,
            utility::is_type<typename Traits::port_keys>
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( mpl::is_sequence<typename Traits::port_keys> ));
};

/// Boolean Metafunction determining whether a type is a KeyedPort.
/** is_vector_port<T, Tag> is an Integral Constant of type bool.
    It evaluates to true if T models VectorPort, false otherwise.
*/
template<typename T, typename Tag=default_tag, typename Enable=void>
struct is_keyed_port
    : public mpl::false_
{
#ifdef DOXYGEN_DOCS_BUILD
    /// Integral Constant value type
    typedef bool value_type;
    /// true if T models Port, false otherwise.
    typedef detail::unspecified value;
    /// Integral Constant
    typedef detail::unspecified type;
#endif
};

/// INTERNAL ONLY
template<typename T, typename Tag>
struct is_keyed_port<
    T,
    Tag,
    typename enable_if<
        is_keyed_port_traits<typename traits_of<T, Tag>::type>
    >::type >
    : public mpl::true_ {};



#ifndef DOXYGEN_DOCS_BUILD
namespace extension
{
    template<typename ProducerTag, typename ConsumerTag, typename Operation>
    struct binary_operation_impl<ProducerTag, ConsumerTag, Operation,
            typename enable_if<
                is_keyed_port_traits<ProducerTag>
//            has_keyed_port<port<ProducerTag>, ConsumerTag, typename ProducerTag::tag>
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
                is_keyed_port_traits<ConsumerTag>
//            has_keyed_port<port<ConsumerTag>, ProducerTag, typename ConsumerTag::tag>
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
    
    
    template<typename KeyedPortTraits>
    struct get_port_impl<
        KeyedPortTraits,
        typename enable_if<is_keyed_port_traits<KeyedPortTraits> >::type>
    {
        template<typename FArgs> struct result;
        
        template<typename F, typename Port, int N>
        struct result<F(Port &, mpl::int_<N>)>
        {
            typedef typename result_of::get_keyed_port<
                Port,
                typename mpl::at<
                    typename KeyedPortTraits::port_keys,
                     mpl::int_<N> >::type,
                typename KeyedPortTraits::tag
            >::type type;
        };
        
        template<typename Port, int N>
        typename result<get_port_impl(Port &, mpl::int_<N>)>::type
        operator()(Port &port, mpl::int_<N>)
        {
            return get_keyed_port<
                typename mpl::at<
                    typename KeyedPortTraits::port_keys,
                     mpl::int_<N>
                >::type,
                typename KeyedPortTraits::tag
            >(port);
        }
    };

}
#endif // DOXYGEN_DOCS_BUILD

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_KEYED_PORT_HPP
