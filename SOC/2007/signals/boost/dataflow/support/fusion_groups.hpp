// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_FUSION_GROUPS_HPP
#define BOOST_DATAFLOW_SUPPORT_FUSION_GROUPS_HPP

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/sequence/generation/vector_tie.hpp>
#include <boost/fusion/sequence/view/joint_view.hpp>
#include <boost/fusion/sequence/view/zip_view.hpp>
#include <boost/fusion/support/is_sequence.hpp>

namespace boost { namespace dataflow {

struct fusion_group_producer;

template<class T>
struct producer_group : public T
{
    fusion_producer_group(const T& t) : T(t) {}
    typedef fusion_group_producer producer_category;
};

template<typename T1, typename T2> inline
producer_group<typename boost::fusion::result_of::vector_tie<T1,T2>::type>
group(T1 &t1, T2 &t2)
{
    return fusion_group<
        typename boost::fusion::result_of::vector_tie<T1,T2>::type>
        (boost::fusion::vector_tie(t1, t2));
} // need to specialize make_group for T1 or T2 being groups already.

struct fusion_group_consumer;

template <typename Sequence>
struct consumer_category_of<Sequence,
            typename boost::enable_if<
                boost::fusion::traits::is_sequence<Sequence> >::type >
{
    typedef fusion_group_consumer type;
};


namespace extension
{
    namespace detail
    {
        struct zip_connect
        {
            template<typename T>
            void operator()(const T& t) const
            {
                dataflow::connect(boost::fusion::at_c<0>(t),
                                  boost::fusion::at_c<1>(t));
            }
        };
    }
    
    template<typename ProducerTag, typename ConsumerTag>
    struct connect_impl<ProducerTag, ConsumerTag,
            typename boost::enable_if<boost::mpl::and_<
                boost::is_base_of<fusion_group_producer, ProducerTag>,
                boost::is_base_of<fusion_group_consumer, ConsumerTag>
            > >::type >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(const Producer &producer, const Consumer &consumer)
            {
                typedef boost::fusion::vector<const Producer&, const Consumer&> zip_type;
                boost::fusion::zip_view<zip_type>
                    zip(zip_type(producer, consumer));
                boost::fusion::for_each(zip, detail::zip_connect());
            }
        };
    };
    
}
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_FUSION_GROUPS_HPP


