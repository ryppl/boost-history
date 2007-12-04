// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_FUSION_COMPONENT_HPP
#define BOOST_DATAFLOW_SUPPORT_FUSION_COMPONENT_HPP

#include <boost/dataflow/support/component.hpp>

#include <boost/mpl/map.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/adapted.hpp>

namespace boost { namespace dataflow {

#ifndef DOXYGEN_DOCS_BUILD
namespace detail
{
    template<typename T>
    struct transform_remove_reference
    {
        typedef typename boost::mpl::transform<T,
            boost::remove_reference<boost::mpl::_> >::type type;
    };
} // namespace detail
#endif // DOXYGEN_DOCS_BUILD

/// Convenience base class for FusionComponentTraits types.
template<typename Sequence, typename DefaultPorts=mpl::map<> >
struct fusion_component_traits
    : public component_traits<
        typename detail::transform_remove_reference<Sequence>::type,
        DefaultPorts,
        concepts::fusion_component>
{
    /// MPL Sequence of result types for get_port
    typedef Sequence port_result_types;
};

#ifndef DOXYGEN_DOCS_BUILD
namespace extension {

    template<typename ComponentTraits>
    struct get_port_impl<
        ComponentTraits,
        typename enable_if<
            is_same<
                typename ComponentTraits::concept,
                concepts::fusion_component>
        >::type >
    {
        template<typename FArgs>
        struct result;
        
        template<typename F, typename Component, typename N>
        struct result<F(Component &, N)>
        {
            typedef typename fusion::result_of::value_at<
                typename ComponentTraits::port_result_types,
                N>::type type;
        };
        
        template<typename Component, typename N>
        typename result<get_port_impl(Component &, N)>::type
        operator()(Component &c, N)
        {
            return fusion::at<N>(c.get_ports(c));
        }
    };

} // namespace extension
#endif // DOXYGEN_DOCS_BUILD

}}

#endif // BOOST_DATAFLOW_SUPPORT_FUSION_COMPONENT_HPP
