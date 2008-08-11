// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

    #ifndef DATAFLOW_SUPPORT_PORT_TRAITS_SEQUENCE_HPP
    #define DATAFLOW_SUPPORT_PORT_TRAITS_SEQUENCE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>

    #include <boost/mpl/at.hpp>
    #include <boost/mpl/int.hpp>
    #include <boost/mpl/less.hpp>
    #include <boost/mpl/size.hpp>


namespace boost { namespace dataflow {

namespace detail {

    template<typename Tag, typename T, int N>
    struct lazy_is_same_port_traits_tag
    {
        typedef typename is_same<Tag, typename mpl::at_c<typename T::port_traits, N>::type::tag>::type type;
    };
    
}

} }

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 2, <boost/dataflow/support/port/detail/port_traits_sequence.hpp>))
    #include BOOST_PP_ITERATE()

    #endif

#else

namespace boost { namespace dataflow {


template<typename T, typename Tag>
struct port_traits_of<
    T,
    Tag,
    typename enable_if<
        mpl::and_<
            mpl::is_sequence<typename T::port_traits>,
            typename mpl::less<
                mpl::int_<BOOST_PP_ITERATION()>,
                typename mpl::size<typename T::port_traits>::type
            >::type,
            detail::lazy_is_same_port_traits_tag<Tag, T, BOOST_PP_ITERATION()>
        >
    >::type
>
{
    typedef typename mpl::at_c<typename T::port_traits, BOOST_PP_ITERATION()>::type type;
    BOOST_MPL_ASSERT(( is_port_traits<type> ));
};

}}

#endif
