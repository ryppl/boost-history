/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_ITERATOR_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_ITERATOR_HPP

#include <boost/fusion/container/list/list_fwd.hpp>
#include <boost/fusion/support/iterator_base.hpp>

namespace boost { namespace fusion
{
    struct cons_iterator_tag;
    struct forward_traversal_tag;

    template<typename ConsRef>
    struct cons_iterator
      : iterator_base<cons_iterator<ConsRef> >
    {
        typedef cons_iterator_tag fusion_tag;
        typedef forward_traversal_tag category;
        typedef ConsRef cons_type;

        template<typename OtherConsRef>
        cons_iterator(cons_iterator<OtherConsRef> const& it)
          : cons(it.cons)
        {}

        cons_iterator(cons_type cons, int)
          : cons(&cons)
        {}

        template<typename OtherConsRef>
        cons_iterator&
        operator=(cons_iterator<OtherConsRef> const& other_it)
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

#define BOOST_FUSION_NIL_ITERATOR(MODIFIER,_)\
      template <>\
      struct cons_iterator<nil MODIFIER>\
        : nil_iterator\
      {\
          cons_iterator()\
          {}\
          \
          explicit\
          cons_iterator(nil MODIFIER, int)\
          {}\
      };\
      \
      template <>\
      struct cons_iterator<list<> MODIFIER>\
        : nil_iterator\
      {\
          cons_iterator()\
          {}\
          \
          explicit\
          cons_iterator(nil MODIFIER, int)\
          {}\
      };

    BOOST_FUSION_ALL_CV_REF_COMBINATIONS(BOOST_FUSION_NIL_ITERATOR,_)

#undef BOOST_FUSION_NIL_ITERATOR
}}

#endif
