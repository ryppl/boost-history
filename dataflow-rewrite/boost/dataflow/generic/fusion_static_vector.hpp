// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_FUSION_PORT_VECTOR_HPP
#define BOOST_DATAFLOW_SUPPORT_FUSION_PORT_VECTOR_HPP

#include <boost/dataflow/detail/transform_remove_reference.hpp>
#include <boost/dataflow/support/port_vector.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/adapted.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>


namespace boost { namespace dataflow {

template<typename T, typename Enable=void>
struct is_fusion_port_vector_traits
    : public mpl::false_ {};

template<typename T>
struct is_fusion_port_vector_traits<
    T,
    typename enable_if<
        mpl::and_<
            is_traits<T>,
            fusion::traits::is_sequence<typename T::fusion_ports>
        > >::type >
    : public mpl::true_
{};

#ifndef DOXYGEN_DOCS_BUILD
namespace extension {

    template<typename Traits>
    struct get_port_impl<
        Traits,
        typename enable_if<is_fusion_port_vector_traits<Traits> >::type >
    {
        template<typename FArgs>
        struct result;
        
        template<typename F, typename Entity, typename N>
        struct result<F(Entity &, N)>
        {
            typedef typename fusion::result_of::value_at<
                typename Traits::fusion_ports,
                N>::type type;
        };
        
        template<typename Entity, typename N>
        typename result<get_port_impl(Entity &, N)>::type
        operator()(Entity &c, N)
        {
            return fusion::at<N>(Traits::get_ports(c));
        }
    };

} // namespace extension
#endif // DOXYGEN_DOCS_BUILD

}}

#endif // BOOST_DATAFLOW_SUPPORT_FUSION_PORT_VECTOR_HPP
