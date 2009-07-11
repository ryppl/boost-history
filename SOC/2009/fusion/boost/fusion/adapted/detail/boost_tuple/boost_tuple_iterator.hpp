/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_BOOST_TUPLE_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_BOOST_TUPLE_ITERATOR_HPP

#include <boost/fusion/iterator/iterator_facade.hpp>

#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct forward_traversal_tag;

    template <typename ConsRef>
    struct boost_tuple_iterator
      : iterator_facade<boost_tuple_iterator<ConsRef>, forward_traversal_tag>
    {
        template <typename ItRef>
        struct value_of
        {
            typedef typename
                detail::remove_reference<ConsRef>::type::head_type
            type;
        };

        template <typename ItRef>
        struct deref
        {
            typedef typename
                detail::forward_as<
                    ConsRef
                  , typename detail::remove_reference<
                        ConsRef
                    >::type::head_type
                >::type
            type;

            static type
            call(ItRef it)
            {
                return it.cons->get_head();
            }
        };

        template <typename ItRef>
        struct next
        {
            typedef
                boost_tuple_iterator<
                    typename detail::forward_as<
                        ConsRef
                      , typename detail::remove_reference<
                            ConsRef
                        >::type::tail_type
                    >::type
                >
            type;

            static type
            call(ItRef it)
            {
                return type(it.cons->get_tail(),0);
            }
        };

        template <typename It1Ref, typename It2Ref>
        struct equal_to
          : is_same<
                typename detail::identity<ConsRef>::type
              , typename detail::identity<
                    typename detail::remove_reference<It2Ref>::type::cons_type
                >::type
            >
        {};

        typedef ConsRef cons_type;

        template<typename OtherIt>
        boost_tuple_iterator(OtherIt const& it)
          : cons(it.cons)
        {}

        boost_tuple_iterator(ConsRef cons, int)
          : cons(&cons)
        {}

        template<typename OtherIt>
        boost_tuple_iterator&
        operator=(OtherIt const& it)
        {
            cons=it.cons;
            return *this;
        }

        typename detail::remove_reference<ConsRef>::type* cons;
    };
}}

#endif
