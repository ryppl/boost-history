/*==============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/addressof.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct begin_impl;

    template<>
    struct begin_impl<cons_tag>
    {
        template<typename Seq>
        struct apply
        {
            typedef typename result_of::empty<Seq>::type empty;

            typedef typename
                mpl::if_c<
                    empty::value
                  , nil_iterator
                  , cons_iterator<typename detail::add_lref<Seq>::type>
                >::type
            type;

            static type
            call(Seq seq, mpl::false_)
            {
                return type(boost::addressof(seq), 0);
            }

            static type
            call(Seq, mpl::true_)
            {
                return type();
            }

            static type
            call(Seq seq)
            {
                return call(static_cast<Seq>(seq), empty());
            }
        };
    };

    template<>
    struct begin_impl<list_tag>
      : begin_impl<cons_tag>
    {};
}}}

#endif
