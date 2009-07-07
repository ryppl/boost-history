/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_MAP_HPP
#define BOOST_FUSION_CONTAINER_MAP_MAP_HPP

#include <boost/fusion/container/map/map_fwd.hpp>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/support/pair.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/ref.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/map/detail/lookup_key.hpp>
#endif
#include <boost/fusion/container/map/detail/begin_impl.hpp>
#include <boost/fusion/container/map/detail/end_impl.hpp>
#include <boost/fusion/container/map/detail/at_key_impl.hpp>
#include <boost/fusion/container/map/detail/value_at_key_impl.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost { namespace fusion
{
    struct map_tag;
    struct fusion_sequence_tag;
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    struct void_;
#endif

    VARIADIC_TEMPLATE(FUSION_MAX_MAP_SIZE)
    struct map : sequence_base<map<EXPAND_ARGUMENTS(FUSION_MAX_MAP_SIZE)> >
    {
        struct category : forward_traversal_tag, associative_sequence_tag {};

        typedef map_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;

        typedef vector<EXPAND_ARGUMENTS(FUSION_MAX_MAP_SIZE)> storage_type;
        typedef typename storage_type::size size;

private:
        template<class Key>struct is_key
        {
            template<class Pair>struct apply
              : is_same<Key,typename Pair::first_type>
            {
            };
        };
public:
        template <typename Key>
        struct meta_at_impl
        {
            template<class Iterator>struct get_type
            {
                typedef typename
                    mpl::deref<Iterator>::type::second_type
                type;
            };

            typedef typename
                mpl::find_if<
                    typename storage_type::types
                  , is_key<Key>
                >::type
            iterator;

            typedef typename
                mpl::eval_if<
                    is_same<iterator
                      , typename mpl::end<typename storage_type::types>::type
                    >
                  , mpl::identity<void_>
                  , get_type<iterator>
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
                      , typename mpl::find_if<
                            typename storage_type::types
                          , is_key<Key>
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
                      , typename mpl::find_if<
                            typename storage_type::types
                          , is_key<Key>
                        >::type
                    >::value
                >
            type;
        };

        map()
        {}

#define MAP_CTOR(COMBINATION)\
        map(map COMBINATION map_)\
          : data(BOOST_FUSION_FORWARD(map COMBINATION,map_).data)\
        {\
        }

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(MAP_CTOR)

#undef MAP_CTOR

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        template <typename Arg>
        map(BOOST_FUSION_R_ELSE_CLREF(Arg) arg)
            : data(BOOST_FUSION_FORWARD(Arg,arg))
        {}

#   include <boost/fusion/container/map/detail/pp/map_forward_ctor.hpp>
#else
        template <typename... OtherArguments>
        map(BOOST_FUSION_R_ELSE_CLREF(OtherArguments)... other_arguments)
            : data(BOOST_FUSION_FORWARD(OtherArguments,other_arguments)...)
        {}
#endif

        template <typename T>
        map&
        operator=(BOOST_FUSION_R_ELSE_CLREF(T) rhs)
        {
            data = BOOST_FUSION_FORWARD(T, rhs);
            return *this;
        }

#ifndef BOOST_NO_VARIADIC_TEMPLATES
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
        typename detail::add_lref<typename meta_at_impl<Key>::type>::type
        at_impl(mpl::identity<Key>)
        {
            return data.at_impl(
                    typename meta_find_impl<Key>::type::index()).second;
        }

        template <class Key>
        typename detail::add_lref<
            typename add_const<typename meta_at_impl<Key>::type>::type
            >::type
        at_impl(mpl::identity<Key>) const
        {
            return data.at_impl(
                    typename meta_find_impl_const<Key>::type::index()).second;
        }
#endif

        storage_type& get_data() { return data; }
        storage_type const& get_data() const { return data; }

    private:

        storage_type data;
    };
}}

#endif
