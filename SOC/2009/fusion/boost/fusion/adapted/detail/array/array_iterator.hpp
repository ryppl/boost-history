/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_ARRAY_ARRAY_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_ARRAY_ARRAY_ITERATOR_HPP

#include <boost/fusion/support/assert.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>

#include <cstddef>
#include <boost/config.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    template <typename ArrayRef, int Pos>
    struct array_iterator
      : iterator_facade<
            array_iterator<ArrayRef, Pos>
          , random_access_traversal_tag
        >
    {
        //BOOST_FUSION_INDEX_CHECK(Pos, Array::static_size+1);

        template <typename ItRef>
        struct value_of
        {
            typedef typename
                detail::remove_reference<
                    typename detail::remove_reference<ItRef>::type::array_type
                >::type::value_type
            type;
        };

        template <typename ItRef>
        struct deref
        {
            typedef typename detail::remove_reference<ItRef>::type it;

            typedef typename
                detail::result_of_forward_as<
                    typename it::array_type
                  , typename detail::remove_reference<
                        typename it::array_type
                    >::type::value_type
                >::type
            type;

            static type
            call(ItRef it)
            {
                return (*it.array)[it::index::value];
            }
        };

        template <typename ItRef, typename N>
        struct advance
        {
            typedef typename detail::remove_reference<ItRef>::type it;

            typedef
                array_iterator<
                    typename it::array_type
                  , it::index::value + N::value
                >
            type;

            static type
            call(ItRef it)
            {
                return type(*it.array,0);
            }
        };

        template <typename ItRef>
        struct next
          : advance<ItRef, mpl::int_<1> >
        {};

        template <typename ItRef>
        struct prior
          : advance<ItRef, mpl::int_<-1> >
        {};

        template <typename It1Ref, typename It2Ref>
        struct distance
          : mpl::minus<
                typename detail::remove_reference<It2Ref>::type::index
              , typename detail::remove_reference<It1Ref>::type::index
            >
        {};

        typedef mpl::int_<Pos> index;
        typedef ArrayRef array_type;

        template<typename OtherZipViewIt>
        array_iterator(OtherZipViewIt const& it)
          : array(it.array)
        {}

        array_iterator(ArrayRef a, int)
          : array(&a)
        {}

        template<typename OtherZipViewIt>
        array_iterator&
        operator=(OtherZipViewIt const& it)
        {
            array=it.array;
            return *this;
        }

        typename detail::remove_reference<ArrayRef>::type* array;
    };
}}

#endif
