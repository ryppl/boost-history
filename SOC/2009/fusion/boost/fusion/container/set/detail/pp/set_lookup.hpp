/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_DETAIL_PP_SET_LOOKUP_HPP
#define BOOST_FUSION_CONTAINER_SET_DETAIL_PP_SET_LOOKUP_HPP
#define FUSION_SET_LOOKUP_09162005_1116

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

#if defined(BOOST_MSVC) && (BOOST_MSVC == 1310)
#pragma warning (push)
#pragma warning(disable: 4348)  // redefinition of default parameter
#endif

    template <typename Key, typename dummy = int>
    struct meta_at_impl
    {
        typedef void_ type;
    };

    template <typename Key, typename dummy = int>
    struct meta_find_impl
    {
        typedef vector_iterator<storage_type, storage_type::size::value> type;
    };

    template <typename Key, typename dummy = int>
    struct meta_find_impl_const
    {
        typedef vector_iterator<storage_type const, storage_type::size::value> type;
    };

    template <typename Key>
    vector_iterator<storage_type const, storage_type::size::value>
    find_impl(mpl::identity<Key>) const
    {
        return vector_iterator<storage_type const, storage_type::size::value>(data);
    }

    template <typename Key>
    vector_iterator<storage_type, storage_type::size::value>
    find_impl(mpl::identity<Key>)
    {
        return vector_iterator<storage_type, storage_type::size::value>(data);
    }

#define BOOST_PP_FILENAME_1 \
    <boost/fusion/container/set/detail/set_lookup.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_DEC(FUSION_MAX_SET_SIZE))
#include BOOST_PP_ITERATE()

#if defined(BOOST_MSVC) && (BOOST_MSVC == 1310)
#pragma warning (pop)
#endif

#endif
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define N BOOST_PP_ITERATION()

    template <typename dummy>
    struct meta_at_impl<
        typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type, dummy>
    {
        typedef BOOST_PP_CAT(T, N) type;
    };

    typename add_reference<BOOST_PP_CAT(T, N)>::type
    at_impl(mpl::identity<typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type>)
    {
        return data.at_impl(mpl::int_<N>());
    }

    typename add_reference<typename add_const<BOOST_PP_CAT(T, N)>::type>::type
    at_impl(mpl::identity<typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type>) const
    {
        return data.at_impl(mpl::int_<N>());
    }

    template <typename dummy>
    struct meta_find_impl<
        typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type, dummy>
    {
        typedef vector_iterator<storage_type, N> type;
    };

    template <typename dummy>
    struct meta_find_impl_const<
        typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type, dummy>
    {
        typedef vector_iterator<storage_type const, N> type;
    };

    vector_iterator<storage_type, N>
    find_impl(mpl::identity<typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type>)
    {
        return vector_iterator<storage_type, N>(data);
    }

    vector_iterator<storage_type const, N>
    find_impl(mpl::identity<typename detail::set_key_type<BOOST_PP_CAT(T, N), N>::type>) const
    {
        return vector_iterator<storage_type const, N>(data);
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
