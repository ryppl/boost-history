/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_GENERATOR_HPP)
#define BOOST_FUSION_TUPLE_GENERATOR_HPP

#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/spirit/fusion/tuple.hpp>
#include <boost/spirit/fusion/detail/make_tuple.ipp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>

#define BOOST_FUSION_DEREF_ITERATOR(z, n, data)                                 \
    typename checked_deref_iterator<                                            \
        BOOST_PP_CAT(T, n), LastT>::type

#define BOOST_FUSION_NEXT_ITERATOR(z, n, data)                                  \
    typename BOOST_PP_CAT(T, n) =                                               \
        typename checked_next_iterator<                                         \
            BOOST_PP_CAT(T, BOOST_PP_DEC(n)), LastT>::type

namespace boost { namespace fusion
{
    namespace tuple_detail
    {
        template <typename FirstT, typename LastT>
        struct checked_deref_iterator
        {
            typedef typename
                mpl::if_<
                    is_same<FirstT, LastT>
                  , mpl::identity<void_t>
                  , typename FirstT::deref_type
                >::type
            result_type;

            typedef typename
                tuple_detail::as_tuple_element<
                    typename remove_reference<
                        typename result_type::type
                    >::type
                >::type
            type;
        };

        template <typename FirstT, typename LastT>
        struct checked_next_iterator
        {
            typedef typename
                mpl::if_<
                    is_same<FirstT, LastT>
                  , mpl::identity<LastT>
                  , typename FirstT::next_type
                >::type
            result_type;

            typedef typename result_type::type type;
        };

        template <
            typename FirstT
          , typename LastT
          , typename T0 = FirstT
          , BOOST_PP_ENUM_SHIFTED(BOOST_FUSION_MAX_ARITY
                , BOOST_FUSION_NEXT_ITERATOR, _)
        >
        struct generate_tuple
        {
            typedef tuple<BOOST_PP_ENUM(BOOST_FUSION_MAX_ARITY
                , BOOST_FUSION_DEREF_ITERATOR, _)> type;
        };
    }

    template <typename FirstT, typename LastT>
    typename tuple_detail::generate_tuple<FirstT, LastT>::type
    generate(FirstT const& first, LastT const&)
    {
        typedef typename
            tuple_detail::generate_tuple<FirstT, LastT>::type
        result;
        return result(first);
    }

    template <typename ViewT>
    typename tuple_detail::generate_tuple<
        typename ViewT::begin_type
      , typename ViewT::end_type
    >::type
    generate(ViewT const& view)
    {
        return generate(view.begin(), view.end());
    }

}} // namespace boost::fusion

#undef BOOST_FUSION_DEREF_ITERATOR
#undef BOOST_FUSION_NEXT_ITERATOR
#endif


