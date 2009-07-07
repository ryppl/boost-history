/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_CONS_ITERATOR_HPP
#define BOOST_FUSION_CONTAINER_LIST_CONS_ITERATOR_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/container/list/list_fwd.hpp>

#include <boost/fusion/container/list/detail/cons/deref_impl.hpp>
#include <boost/fusion/container/list/detail/cons/next_impl.hpp>
#include <boost/fusion/container/list/detail/cons/value_of_impl.hpp>
#include <boost/fusion/container/list/detail/cons/equal_to_impl.hpp>

namespace boost { namespace fusion
{
    struct nil;
    struct cons_iterator_tag;
    struct forward_traversal_tag;

    template <typename Cons = nil>
    struct cons_iterator
      : iterator_base<cons_iterator<Cons> >
    {
        typedef cons_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;
        typedef Cons cons_type;

        template<typename OtherIt>
        cons_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherIt) it)
          : cons(it.cons)
        {}

        cons_iterator(cons_type cons, int)
          : cons(&cons)
        {}

        template<typename OtherIt>
        cons_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherIt) other_it)
        {
            cons=other_it.cons;
            return *this;
        }

        typename detail::remove_reference<cons_type>::type* cons;
    };

    struct nil_iterator
      : iterator_base<nil_iterator>
    {
        typedef forward_traversal_tag category;
        typedef cons_iterator_tag fusion_tag;
        typedef nil cons_type;

        nil_iterator()
        {}

        nil_iterator(nil const&,int)
        {}
    };

#define NIL_ITERATOR(COMBINATION)\
      template <>\
      struct cons_iterator<nil COMBINATION>\
        : nil_iterator\
      {\
          cons_iterator()\
          {}\
          \
          explicit cons_iterator(nil COMBINATION, int)\
          {}\
      };

    BOOST_FUSION_ALL_CV_REF_COMBINATIONS(NIL_ITERATOR)

#undef NIL_ITERATOR
}}

#endif
