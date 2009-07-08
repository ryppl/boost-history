/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_STRUCT_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_STRUCT_ITERATOR_HPP

#include <boost/fusion/support/assert.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/adapted/struct/extension.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/config/no_tr1/utility.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    template <typename Struct, int N_>
    struct struct_iterator
        : iterator_facade<struct_iterator<Struct, N_>, random_access_traversal_tag>
    {
        BOOST_FUSION_INDEX_CHECK(N,extension::struct_size<Struct>::value);

        typedef mpl::int_<N_> index;
        typedef Struct struct_type;

        struct_iterator(Struct& struct_)
            : struct_(struct_) {}
        Struct& struct_;

        template <typename Iterator>
        struct value_of
            : extension::struct_member<Struct, N_>
        {
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename
                add_reference<
                    typename extension::struct_member<Struct, N_>::type
                >::type
            type;

            static type
            call(Iterator const& iter)
            {
                return extension::struct_member<Struct, N_>::
                    call(iter.struct_);
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::struct_type struct_type;
            typedef struct_iterator<struct_type, index::value + N::value> type;

            static type
            call(Iterator const& iter)
            {
                return type(iter.struct_);
            }
        };

        template <typename Iterator>
        struct next : advance<Iterator, mpl::int_<1> > {};

        template <typename Iterator>
        struct prior : advance<Iterator, mpl::int_<-1> > {};

        template <typename I1, typename I2>
        struct distance : mpl::minus<typename I2::index, typename I1::index>
        {
            typedef typename
                mpl::minus<
                    typename I2::index, typename I1::index
                >::type
            type;

            static type
            call(I1 const&, I2 const&)
            {
                return type();
            }
        };
    };
}}

#endif
