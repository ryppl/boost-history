/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_DETAIL_ASSOC_FIND_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_DETAIL_ASSOC_FIND_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename SeqRef, typename Key>
    struct assoc_find
    {
        typedef typename detail::remove_reference<SeqRef>::type seq;

        typedef typename
            mpl::eval_if<
                is_const<seq>
              , typename seq::template meta_find_impl_const<Key>
              , typename seq::template meta_find_impl<Key>
            >::type
        type;

        static type
        call(SeqRef seq)
        {
            return seq.find_impl(mpl::identity<Key>());
        }
    };
}}}

#endif
