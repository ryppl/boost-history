/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VECTOR_ITERATOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VECTOR_ITERATOR_HPP

//TODO cschmidt: move in detail

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    struct vector_iterator_tag;
    struct random_access_traversal_tag;

    template <typename VecRef, int N>
    struct vector_iterator
      : iterator_base<vector_iterator<VecRef, N> >
    {
        typedef mpl::int_<N> index;
        typedef VecRef vector;
        typedef vector_iterator_tag fusion_tag;
        typedef random_access_traversal_tag category;

        template<typename OtherIt>
        vector_iterator(OtherIt const& it)
          : vec(it.vec)
        {}

        vector_iterator(VecRef vec, int)
          : vec(&vec)
        {}

        template<typename OtherIt>
        vector_iterator&
        operator=(OtherIt const& it)
        {
            vec=it.vec;
            return *this;
        }

        typename detail::remove_reference<VecRef>::type* vec;
    };
}}

#endif
