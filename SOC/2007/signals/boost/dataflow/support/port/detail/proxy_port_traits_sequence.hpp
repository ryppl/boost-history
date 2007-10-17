// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

    #ifndef DATAFLOW_SUPPORT_PROXY_PORT_TRAITS_SEQUENCE_HPP
    #define DATAFLOW_SUPPORT_PROXY_PORT_TRAITS_SEQUENCE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>

    #include <boost/mpl/int.hpp>
    #include <boost/mpl/less.hpp>
    #include <boost/mpl/size.hpp>

namespace boost { namespace dataflow {

namespace detail {

    template<typename Mechanism, typename T, int N>
    struct lazy_is_same_proxy_port_traits_mechanism
    {
        typedef typename is_same<Mechanism, typename mpl::at_c<typename T::proxy_port_traits, N>::type::mechanism>::type type;
    };
    
    template<typename PortCategory, typename T, int N>
    struct lazy_is_same_proxy_port_traits_port_category
    {
        typedef typename is_same<PortCategory, typename mpl::at_c<typename T::proxy_port_traits, N>::type::port_category>::type type;
    };
}

} }

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 1, <boost/dataflow/support/port/detail/proxy_port_traits_sequence.hpp>))
    #include BOOST_PP_ITERATE()

    #endif

#else

namespace boost { namespace dataflow {


template<typename Mechanism, typename PortCategory, typename T>
struct proxy_port_traits_of<Mechanism, PortCategory, T,
    typename enable_if<
        mpl::and_<
            mpl::is_sequence<typename T::proxy_port_traits>,
            typename mpl::less<
                mpl::int_<BOOST_PP_ITERATION()>,
                typename mpl::size<typename T::proxy_port_traits>::type
            >::type,
            detail::lazy_is_same_proxy_port_traits_mechanism<Mechanism, T, BOOST_PP_ITERATION()>,
            detail::lazy_is_same_proxy_port_traits_port_category<PortCategory, T, BOOST_PP_ITERATION()>
        >
    >::type
>
{
    typedef typename mpl::at_c<typename T::proxy_port_traits, BOOST_PP_ITERATION()>::type type;
    BOOST_MPL_ASSERT(( is_proxy_port_traits<type> ));
};

}}

#endif
