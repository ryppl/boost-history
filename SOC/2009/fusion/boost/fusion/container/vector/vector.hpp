/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR_HPP

#include <boost/fusion/container/vector/vector_fwd.hpp>
#ifdef BOOST_NO_VARIADIC_TEMPLATES
#include <boost/fusion/container/vector/vector_iterator.hpp>
#include <boost/fusion/container/vector/detail/vector_n_chooser.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace fusion
{
    struct void_;
    struct fusion_sequence_tag;

    template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename T)>
    struct vector
        : sequence_base<vector<BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)> >
    {
    private:

        typedef typename detail::vector_n_chooser<
            BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)>::type
        vector_n;

        template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        friend struct vector;

    public:

        typedef typename vector_n::types types;
        typedef typename vector_n::fusion_tag fusion_tag;
        typedef typename vector_n::tag tag;
        typedef typename vector_n::size size;
        typedef typename vector_n::category category;
        typedef typename vector_n::is_view is_view;

        vector()
            : vec() {}

        template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        vector(vector<BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, U)> const& rhs)
            : vec(rhs.vec) {}

        vector(vector const& rhs)
            : vec(rhs.vec) {}

        template <typename Sequence>
        vector(Sequence const& rhs)
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1400)
            : vec(ctor_helper(rhs, is_base_of<vector, Sequence>())) {}
#else
            : vec(rhs) {}
#endif

        //  Expand a couple of forwarding constructors for arguments
        //  of type (T0), (T0, T1), (T0, T1, T2) etc. Example:
        //
        //  vector(
        //      typename detail::call_param<T0>::type _0
        //    , typename detail::call_param<T1>::type _1)
        //    : vec(_0, _1) {}
        #include <boost/fusion/container/vector/detail/vector_forward_ctor.hpp>

        template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        vector&
        operator=(vector<BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, U)> const& rhs)
        {
            vec = rhs.vec;
            return *this;
        }

        template <typename T>
        vector&
        operator=(T const& rhs)
        {
            vec = rhs;
            return *this;
        }

        template <int N>
        typename detail::add_lref<
            typename mpl::at_c<types, N>::type
        >::type
        at_impl(mpl::int_<N> index)
        {
            return vec.at_impl(index);
        }

        template <int N>
        typename detail::add_lref<
            typename add_const<
                typename mpl::at_c<types, N>::type
            >::type
        >::type
        at_impl(mpl::int_<N> index) const
        {
            return vec.at_impl(index);
        }

        template <typename I>
        typename detail::add_lref<
            typename mpl::at<types, I>::type
        >::type
        at_impl(I index)
        {
            return vec.at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        typename detail::add_lref<
            typename add_const<
                typename mpl::at<types, I>::type
            >::type
        >::type
        at_impl(I index) const
        {
            return vec.at_impl(mpl::int_<I::value>());
        }

    private:

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1400)
        static vector_n const&
        ctor_helper(vector const& rhs, mpl::true_)
        {
            return rhs.vec;
        }

        template <typename T>
        static T const&
        ctor_helper(T const& rhs, mpl::false_)
        {
            return rhs;
        }
#endif

        vector_n vec;
    };
}}
#else
#   include <boost/fusion/support/ref.hpp>
#   include <boost/fusion/container/vector/vector_iterator.hpp>

#   include <boost/fusion/container/vector/detail/at_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_at_impl.hpp>
#   include <boost/fusion/container/vector/detail/begin_impl.hpp>
#   include <boost/fusion/container/vector/detail/end_impl.hpp>

namespace boost { namespace fusion {

struct vector_tag;
struct fusion_sequence_tag;
struct random_access_traversal_tag;

}}

#   include <boost/fusion/container/vector/detail/variadic_templates/vector_impl.hpp>
#endif

#endif
