/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_MPL_ITERATOR_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_MPL_ITERATOR_HPP

#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

//TODO doc no assoc iterator!

namespace boost { namespace fusion
{
    struct mpl_iterator_tag;
    struct mpl_sequence_tag;

    namespace extension
    {
        template <typename>
        struct value_of_impl;

        template <typename>
        struct equal_to_impl;

        template <typename>
        struct deref_impl;

        template <typename>
        struct next_impl;

        template <typename>
        struct prior_impl;

        template <typename>
        struct advance_impl;

        template <typename>
        struct distance_impl;
    }

    template <typename It>
    struct mpl_iterator
      : iterator_facade<
            mpl_iterator<It>
          , typename traits::category_of<It>::type
        >
    {
        //cschmidt: dummy typedef to enable fast SFINAE in get_mpl_it.hpp!
        typedef void void_;
        typedef It it_type;

        template <typename>
        struct value_of
          : extension::value_of_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
>>>>>>> .r55867
        {};

        template <typename, typename It2>
        struct equal_to
          : extension::equal_to_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It,It2>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It,It2>
>>>>>>> .r55867
        {};

        template <typename>
        struct deref
          : extension::deref_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
>>>>>>> .r55867
        {};

        template <typename>
        struct next
          : extension::next_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
>>>>>>> .r55867
        {};

        template <typename>
        struct prior
          : extension::prior_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It>
>>>>>>> .r55867
        {};

        template <typename, typename N>
        struct advance
          : extension::advance_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It,N>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It,N>
>>>>>>> .r55867
        {};

        template <typename, typename It2>
        struct distance
          : extension::distance_impl<
<<<<<<< .mine
                typename mpl::apply1<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It,It2>
=======
                typename mpl::apply<mpl::always<mpl_iterator_tag>,It>::type
            >::template apply<It,It2>
>>>>>>> .r55867
        {};
    };
}}

#endif
