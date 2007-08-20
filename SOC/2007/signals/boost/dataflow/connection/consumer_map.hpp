// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_CONNECTION_CONSUMER_MAP_HPP
#define BOOST_DATAFLOW_CONNECTION_CONSUMER_MAP_HPP

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

namespace boost { namespace dataflow {

struct fusion_map_consumer;

template<class T>
struct consumer_map : public T
{
    consumer_map(const T& t) : T(t) {}
    typedef fusion_map_consumer consumer_category;
    typedef
        typename boost::remove_const<
            typename boost::remove_reference<
                typename boost::fusion::result_of::front<T>::type
            >::type
        >::type::second_type proxy_producer_for;
    typedef mutable_proxy_producer proxy_producer_category;

    typename get_proxied_producer_type<proxy_producer_for>::type &get_proxied_producer() const
    {
        return boost::dataflow::get_proxied_producer(boost::fusion::front(*this).second);
    }
};

namespace extension
{
    // component >>= map
    template<typename ProducerTag, typename ConsumerTag>
    struct connect_impl<ProducerTag, ConsumerTag,
        typename boost::enable_if<//boost::mpl::and_<
            //boost::mpl::not_<boost::is_base_of<fusion_map_consumer, ProducerTag> >,
            boost::is_base_of<fusion_map_consumer, ConsumerTag>
        /*>*/ >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, const Consumer &consumer)
            {
                connect(
                    producer,
                    boost::fusion::at_key<
                        typename produced_type_of<Producer>::type
                    >(consumer));
            }
            static void call(const Producer &producer, Consumer &consumer)
            {
                connect(
                    producer,
                    boost::fusion::at_key<
                        typename produced_type_of<Producer>::type
                    >(consumer));
            }
            static void call(Producer &producer, const Consumer &consumer)
            {
                connect(
                    producer,
                    boost::fusion::at_key<
                        typename produced_type_of<Producer>::type
                    >(consumer));
            }
            static void call(Producer &producer, Consumer &consumer)
            {
                connect(
                    producer,
                    boost::fusion::at_key<
                        typename produced_type_of<Producer>::type
                    >(consumer));
            } 
        };
    };
    
    // map >>= component
    template<typename ProducerTag, typename ConsumerTag>
        struct connect_impl<ProducerTag, ConsumerTag,
            typename boost::enable_if<//boost::mpl::and_<
                boost::is_base_of<fusion_map_consumer, ProducerTag>//,
                //boost::mpl::not_<boost::is_base_of<fusion_group_consumer, ConsumerTag> >
        /*>*/ >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, const Consumer &consumer)
            {
                connect(boost::fusion::front(producer).second, consumer);
            }
            static void call(const Producer &producer, Consumer &consumer)
            {
                connect(boost::fusion::front(producer).second, consumer);
            }
            static void call(Producer &producer, const Consumer &consumer)
            {
                connect(boost::fusion::front(producer).second, consumer);
            }
            static void call(Producer &producer, Consumer &consumer)
            {
                connect(boost::fusion::front(producer).second, consumer);
            }
        };
    };
}
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_CONNECTION_CONSUMER_MAP_HPP


