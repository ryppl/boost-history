// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_PORT_VECTOR_HPP
#define BOOST_DATAFLOW_SUPPORT_PORT_VECTOR_HPP

#include <boost/dataflow/support/traits.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace dataflow {

template<typename PortSequence, typename Tag=default_tag>
struct port_vector_traits
    : public traits<Tag>
{
    typedef PortSequence ports;
    BOOST_MPL_ASSERT(( mpl::is_sequence<PortSequence> ));
};

#ifndef DOXYGEN_DOCS_BUILD
namespace extension {

    template<typename ComponentTraits, typename Enable=void>
    struct get_port_impl
    {
        BOOST_MPL_ASSERT((is_same<Enable, void>));

        typedef void result_type;
        
        typedef void not_specialized;

        template<typename Component, int N>
        void operator()(Component &, mpl::int_<N>)
        {
            // Error: get_port_impl has not been
            // implemented for ComponentTraits.
            BOOST_STATIC_ASSERT(sizeof(Component)==0);
        }
    };
    
} // namespace extension

namespace result_of
{
    template<typename Component, int N, typename Tag=default_tag>
    struct get_port_c
    {
        typedef typename boost::result_of<
            extension::get_port_impl<
            typename traits_of<Component, Tag>::type
        >(Component &, mpl::int_<N>)>::type type;
    };

    template<typename Component, typename N, typename Tag=default_tag>
    struct get_port
    {
        typedef typename get_port_c<Component, N::value, Tag>::type type;
    };
}

template<int N, typename Tag, typename Component>
inline typename result_of::get_port_c<Component, N, Tag>::type
get_port_c(Component &component)
{
    return
        extension::get_port_impl<
            typename traits_of<Component, Tag>::type
        >()(component, mpl::int_<N>());
}

template<int N, typename Component>
inline typename lazy_enable_if<
    is_entity<Component>,
    result_of::get_port_c<Component, N>
>::type
get_port_c(Component &component)
{
    return
        extension::get_port_impl<
            typename traits_of<Component>::type
        >()(component, mpl::int_<N>());
}

template<typename N, typename Tag, typename Component>
inline typename result_of::get_port<Component, N, Tag>::type
get_port(Component &component)
{
    return
        extension::get_port_impl<
            typename traits_of<Component, Tag>::type
        >()(component, mpl::int_<N::value>());
}

template<typename N, typename Component>
inline typename lazy_enable_if<
    is_entity<Component>,
    result_of::get_port<Component, N>
>::type
get_port(Component &component)
{
    return
        extension::get_port_impl<
            typename traits_of<Component>::type
        >()(component, mpl::int_<N::value>());
}

#endif // DOXYGEN_DOCS_BUILD

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
