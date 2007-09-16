// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_CONNECTION_PRODUCER_MAP_HPP
#define BOOST_DATAFLOW_CONNECTION_PRODUCER_MAP_HPP

#include <boost/dataflow/support.hpp>

#include <boost/mpl/not.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/generation/vector_tie.hpp>
#include <boost/fusion/sequence/view/zip_view.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/call_traits.hpp>

namespace boost { namespace dataflow {

struct fusion_map_producer;

template<class T>
struct producer_map : public T
{
    producer_map(const T& t) : T(t) {}
    typedef fusion_map_producer producer_category;
    typedef
        typename boost::remove_const<
            typename boost::remove_reference<
                typename boost::fusion::result_of::front<T>::type
            >::type
        >::type::second_type proxy_consumer_for;
    typedef mutable_proxy_consumer proxy_consumer_category;

    typename boost::call_traits<typename get_proxied_consumer_type<proxy_consumer_for>::type>::reference get_proxied_consumer() const
    {
        return boost::dataflow::get_proxied_consumer(boost::fusion::front(*this).second);
    }
};

namespace extension
{
    // component >>= coxsumer_map
    template<typename ProducerTag, typename ConsumerTag>
    struct connect_impl<ProducerTag, ConsumerTag,
        typename boost::enable_if<
            boost::is_base_of<fusion_map_producer, ProducerTag>
        >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, const Consumer &consumer)
            {
                connect(
                    boost::fusion::at_key<
                        typename consumed_type_of<Consumer>::type
                    >(producer),
                    consumer);
            }
            static void call(const Producer &producer, Consumer &consumer)
            {
                connect(
                    boost::fusion::at_key<
                        typename consumed_type_of<Consumer>::type
                    >(producer),
                    consumer);
            }
            static void call(Producer &producer, const Consumer &consumer)
            {
                connect(
                    boost::fusion::at_key<
                        typename consumed_type_of<Consumer>::type
                    >(producer),
                    consumer);
            }
            static void call(Producer &producer, Consumer &consumer)
            {
                connect(
                    boost::fusion::at_key<
                        typename consumed_type_of<Consumer>::type
                    >(producer),
                    consumer);
            } 
        };
    };
}
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_CONNECTION_PRODUCER_MAP_HPP


