// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_SEQUENCE_ASSIGN_HPP
#define BOOST_FUSION_SUPPORT_SEQUENCE_ASSIGN_HPP

#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/bool.hpp>

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

#ifndef BOOST_NO_RVALUE_REFERENCES
    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq> const&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq> const volatile&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq> volatile&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq>&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq> const&&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq> const volatile&&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq> volatile&&>
      : mpl::true_
    {
    };

    template<typename Seq>
    struct is_sequence_assign<detail::sequence_assign_type<Seq>&&>
      : mpl::true_
    {
    };

    template<typename Seq>
    detail::sequence_assign_type<BOOST_FUSION_R_ELSE_LREF(Seq)>
    sequence_assign(BOOST_FUSION_R_ELSE_LREF(Seq) seq)
    {
        return seq;
    }
#endif
}}

#endif
