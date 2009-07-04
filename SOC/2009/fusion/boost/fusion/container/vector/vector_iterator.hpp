/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR_ITERATOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR_ITERATOR_HPP

//TODO cschmidt: move in detail

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/container/vector/detail/deref_impl.hpp>
#include <boost/fusion/container/vector/detail/value_of_impl.hpp>
#include <boost/fusion/container/vector/detail/next_impl.hpp>
#include <boost/fusion/container/vector/detail/prior_impl.hpp>
#include <boost/fusion/container/vector/detail/equal_to_impl.hpp>
#include <boost/fusion/container/vector/detail/distance_impl.hpp>
#include <boost/fusion/container/vector/detail/advance_impl.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    struct vector_iterator_tag;
    struct random_access_traversal_tag;

    template <typename Vector, int N>
    struct vector_iterator_identity;

    //TODO VecRef!
    template <typename Vector, int N>
    struct vector_iterator
      : iterator_base<vector_iterator<Vector, N> >
    {
        typedef mpl::int_<N> index;
        typedef Vector vector;
        typedef vector_iterator_tag fusion_tag;
        typedef random_access_traversal_tag category;
        typedef
            vector_iterator_identity<
                typename detail::identity<Vector>::type
              , N>
        identity;

        template<typename OtherVecIt>
        vector_iterator(BOOST_FUSION_R_ELSE_CLREF(OtherVecIt) it)
          : vec(it.vec)
        {}

        vector_iterator(Vector vec, int)
          : vec(&vec)
        {}

        template<typename OtherVecIt>
        vector_iterator&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherVecIt) it)
        {
            vec=it.vec;
            return *this;
        }

        typename detail::remove_reference<Vector>::type* vec;
    };
}}

#endif
