// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_SEQUENCE_ASSIGN_HPP
#define BOOST_FUSION_SUPPORT_SEQUENCE_ASSIGN_HPP

#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>

namespace boost{namespace fusion{

    namespace detail
    {
        template<typename Seq>
        struct sequence_assign_type
        {
            sequence_assign_type(Seq seq_):
                seq(seq_)
            {
            }

            Seq get()const
            {
                return seq;
            }

        private:
            Seq seq;
        };
    }

    template<typename>
    struct is_sequence_assign
      : mpl::false_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq>>
       : mpl::true_
    {
    };

#   define IS_SEQUENCE_ASSIGN(COMBINATION)\
    template<typename Seq>\
    struct is_sequence_assign<detail::sequence_assign_type<Seq> COMBINATION>\
      : mpl::true_\
    {\
    };

    BOOST_FUSION_ALL_CV_REF_COMBINATIONS(IS_SEQUENCE_ASSIGN);

#   undef IS_SEQUENCE_ASSIGN

    template<typename Seq>
    typename mpl::if_<
        is_sequence_assign<BOOST_FUSION_R_ELSE_LREF(Seq)>
      , BOOST_FUSION_R_ELSE_LREF(Seq)
      , detail::sequence_assign_type<BOOST_FUSION_R_ELSE_LREF(Seq)>
    >::type
    sequence_assign(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        return seq;
    }
}}

#endif
