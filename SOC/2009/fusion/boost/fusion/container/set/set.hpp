/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_SET_HPP
#define BOOST_FUSION_CONTAINER_SET_SET_HPP

#include <boost/fusion/container/set/set_fwd.hpp>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/set/detail/pp/lookup_key.hpp>
#endif
#include <boost/fusion/container/set/detail/begin_impl.hpp>
#include <boost/fusion/container/set/detail/end_impl.hpp>
#include <boost/fusion/container/set/detail/at_key_impl.hpp>
#include <boost/fusion/container/set/detail/value_at_key_impl.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/detail/pp/forward_ctor.hpp>
#endif

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    struct void_;
#endif

    VARIADIC_TEMPLATE(FUSION_MAX_SET_SIZE)
    struct set
      : sequence_base<set<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_SET_SIZE)> >
    {
        struct category : forward_traversal_tag, associative_sequence_tag {};

        typedef set_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;

        typedef vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_SET_SIZE)> storage_type;
        typedef typename storage_type::size size;

        template <typename Key>
        struct meta_at_impl
        {
            typedef typename
                mpl::if_<
                    typename mpl::contains<
                        typename storage_type::types
                      , Key
                    >::type
                  , Key
                  , void_
                >::type
            type;
        };

        template <typename Key>
        struct meta_find_impl
        {
            typedef
                vector_iterator<
                    storage_type&
                  , mpl::distance<
                        typename mpl::begin<typename storage_type::types>::type
                      , typename mpl::find<
                            typename storage_type::types
                          , Key
                        >::type
                    >::value
                >
            type;
        };

        template <typename Key>
        struct meta_find_impl_const
        {
            typedef
                vector_iterator<
                    storage_type const&
                  , mpl::distance<
                        typename mpl::begin<typename storage_type::types>::type
                      , typename mpl::find<
                            typename storage_type::types
                          , Key
                        >::type
                    >::value
                >
            type;
        };

        set()
        {}

#define SET_CTOR(COMBINATION,_)\
        set(set COMBINATION set_)\
          : data(BOOST_FUSION_FORWARD(set COMBINATION,set_).data)\
        {}

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(SET_CTOR,_)

#undef SET_CTOR

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_FUSION_SEQ_NAME set
#   define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_SET_SIZE
#   include <boost/fusion/container/detail/pp/forward_ctor.hpp>
#   undef BOOST_FUSION_MAX_SEQ_SIZE
#   undef BOOST_FUSION_SEQ_NAME
#else
        template <typename... OtherArguments>
        set(BOOST_FUSION_R_ELSE_CLREF(OtherArguments)... other_arguments)
            : data(BOOST_FUSION_FORWARD(OtherArguments,other_arguments)...)
        {}
#endif

        template <typename Seq>
        set&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            data = BOOST_FUSION_FORWARD(Seq, seq);
            return *this;
        }

        template <typename Key>
        typename meta_find_impl_const<Key>::type
        find_impl(mpl::identity<Key>) const
        {
            return typename meta_find_impl_const<Key>::type(data,0);
        }

        template <typename Key>
        typename meta_find_impl<Key>::type
        find_impl(mpl::identity<Key>)
        {
            return typename meta_find_impl<Key>::type(data,0);
        }

        template <class Key>
        typename detail::add_lref<Key>::type
        at_impl(mpl::identity<Key>)
        {
            return data.at_impl(typename meta_find_impl<Key>::type::index());
        }

        template <class Key>
        typename detail::add_lref<typename add_const<Key>::type>::type
        at_impl(mpl::identity<Key>) const
        {
            return data.at_impl(
                    typename meta_find_impl_const<Key>::type::index());
        }

        storage_type& get_data() { return data; }
        storage_type const& get_data() const { return data; }

    private:

        storage_type data;
    };
}}

#endif
