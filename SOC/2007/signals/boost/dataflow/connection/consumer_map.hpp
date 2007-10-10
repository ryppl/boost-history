// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_CONNECTION_CONSUMER_MAP_HPP
#define BOOST_DATAFLOW_CONNECTION_CONSUMER_MAP_HPP

#include <boost/dataflow/support.hpp>

#include <boost/mpl/not.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
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

struct fusion_map_consumer
    : public consumer_category<concepts::keyed_consumer> {};

template<class T>
struct consumer_map : public T
{
    consumer_map(const T& t) : T(t) {}
    template<typename Mechanism>
    struct dataflow
    {
        typedef fusion_map_consumer consumer_category;
/*        typedef typename boost::fusion::result_of::value_at_c<T, 0>::type::second_type
            proxy_producer_for;*/
        typedef
            typename boost::remove_const<
                typename boost::remove_reference<
                    typename boost::fusion::result_of::front<T>::type
                >::type
            >::type::second_type proxy_producer_for;
        typedef mutable_proxy_producer<proxy_producer_for> proxy_producer_category;
        
        static typename get_proxied_producer_result_type<Mechanism, proxy_producer_for>::type get_proxied_producer(const T &t)
        {
            return boost::dataflow::get_proxied_producer<Mechanism>(boost::fusion::front(t).second);
        }
    };
};

namespace extension
{
    template<typename Mechanism, typename ProducerTag>
    struct get_keyed_consumer_impl<Mechanism, ProducerTag, fusion_map_consumer>
    {
        template<typename Producer, typename KeyedConsumer>
        struct apply
        {
            typedef typename boost::fusion::result_of::at_key<
                        KeyedConsumer,
                        ProducerTag
                    >::type type;

            static type
            call(Producer &, KeyedConsumer &consumer)
            {
                return boost::fusion::at_key<
                        ProducerTag
                    >(consumer);
            }
        };
    };
}
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_CONNECTION_CONSUMER_MAP_HPP


