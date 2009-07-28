// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PP_IS_ITERATING
#   include <boost/fusion/container/vector/vector_fwd.hpp>
#   include <boost/fusion/sequence/intrinsic/begin.hpp>
#   include <boost/fusion/iterator/deref.hpp>
#   include <boost/fusion/iterator/next.hpp>
#   include <boost/fusion/iterator/advance_c.hpp>
#   include <boost/fusion/support/sequence_base.hpp>
#   include <boost/fusion/support/ref.hpp>
#   include <boost/fusion/support/sequence_assign.hpp>

#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/dec.hpp>
#   include <boost/preprocessor/empty.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/enum_params.hpp>
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#   include <boost/preprocessor/repetition/repeat_from_to.hpp>
#   include <boost/preprocessor/iteration/iterate.hpp>

#   include <boost/mpl/int.hpp>
#   include <boost/mpl/bool.hpp>
#   include <boost/mpl/at.hpp>

#   include <boost/type_traits/add_const.hpp>
#   include <boost/utility/enable_if.hpp>
#   ifdef BOOST_NO_RVALUE_REFERENCES
#       include <boost/call_traits.hpp>
#   endif

#   include <boost/fusion/container/vector/detail/at_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_at_impl.hpp>
#   include <boost/fusion/container/vector/detail/vector_iterator.hpp>
#   include <boost/fusion/container/vector/detail/begin_impl.hpp>
#   include <boost/fusion/container/vector/detail/end_impl.hpp>
#   include <boost/fusion/container/vector/detail/deref_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_of_impl.hpp>
#   include <boost/fusion/container/vector/detail/next_impl.hpp>
#   include <boost/fusion/container/vector/detail/prior_impl.hpp>
#   include <boost/fusion/container/vector/detail/equal_to_impl.hpp>
#   include <boost/fusion/container/vector/detail/distance_impl.hpp>
#   include <boost/fusion/container/vector/detail/advance_impl.hpp>

#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/pp/vector_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (BOOST_FUSION_FROM, BOOST_FUSION_TO)
#   include BOOST_PP_ITERATE()

#else
#   define N BOOST_PP_ITERATION()

namespace boost { namespace fusion
{
#   if !N
    struct void_;

    template <class T0=void_>
    struct vector0;

    template<>
    struct vector0<void_>
      : sequence_base<vector0<void_> >
#   else
    template <BOOST_PP_ENUM_PARAMS(N, typename T)>
    struct BOOST_PP_CAT(vector, N)
      : sequence_base<BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N,T)> >
#   endif
    {
        typedef mpl::BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)> types;
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<N> size;

#   define BOOST_FUSION_DEFAULT_MEMBER_INIT(Z, N, _) BOOST_PP_CAT(m,N)()

        BOOST_PP_CAT(vector, N)()
#   if N
          : BOOST_PP_ENUM(N, BOOST_FUSION_DEFAULT_MEMBER_INIT, _)
#   endif
        {}

#   undef BOOST_FUSION_DEFAULT_MEMBER_INIT

#   if N
#       define BOOST_FUSION_SELF_TYPE\
            BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N,T)>

#       define BOOST_FUSION_MEMBER_INIT(Z, N, COMBINATION)\
            BOOST_PP_CAT(m,N)(\
                BOOST_FUSION_FORWARD(BOOST_FUSION_SELF_TYPE COMBINATION,vec).\
                    BOOST_PP_CAT(m,N))

#       define VECTOR_CTOR(COMBINATION,_)\
        BOOST_PP_CAT(vector, N)(\
            BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N,T)> COMBINATION vec)\
          : BOOST_PP_ENUM(N, BOOST_FUSION_MEMBER_INIT, COMBINATION)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_CTOR,_)

#       undef BOOST_FUSION_MEMBER_INIT
#       undef BOOST_FUSION_SELF_TYPE
#   else
#       define VECTOR_CTOR(COMBINATION,_)\
        BOOST_PP_CAT(vector, N)(BOOST_PP_CAT(vector, N)<void_> COMBINATION vec)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_CTOR,_)

#   endif
#   undef VECTOR_CTOR

#   if N
#       define BOOST_FUSION_MEMBER_INIT(Z, N, _)\
        BOOST_PP_CAT(m,N)(\
            BOOST_FUSION_FORWARD(BOOST_PP_CAT(A,N), BOOST_PP_CAT(_,N)))

#ifndef BOOST_NO_RVALUE_REFERENCES
        template <BOOST_PP_ENUM_PARAMS(N, typename A)>
#endif
#       if (N == 1)
        explicit
#       endif
        BOOST_PP_CAT(vector, N)(
#ifdef BOOST_NO_RVALUE_REFERENCES
            BOOST_PP_ENUM_BINARY_PARAMS(
                    N,
                    typename call_traits<T,
                    >::param_type _)
#else
            BOOST_PP_ENUM_BINARY_PARAMS(
                    N,
                    A,
                    BOOST_FUSION_R_ELSE_CLREF(BOOST_PP_EMPTY()) _)
#endif
            )
          : BOOST_PP_ENUM(N, BOOST_FUSION_MEMBER_INIT, _)
        {}

#       undef BOOST_FUSION_MEMBER_INIT
#   endif

#   if N
#       define BOOST_FUSION_MEMBER_INIT(Z, N, _)\
        BOOST_PP_CAT(m,N)(fusion::deref(\
            fusion::advance_c<N>(fusion::begin(seq_assign.get()))))
#       define VECTOR_ASSIGN_CTOR(COMBINATION,_)\
        template<typename SeqRef>\
        BOOST_PP_CAT(vector,N)(\
            support::sequence_assign_type<SeqRef> COMBINATION seq_assign)\
          : BOOST_PP_ENUM(N, BOOST_FUSION_MEMBER_INIT, _)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_ASSIGN_CTOR,_);

#       undef VECTOR_ASSIGN_CTOR
#       undef BOOST_FUSION_MEMBER_INIT
#   else
#       define VECTOR_ASSIGN_CTOR(COMBINATION,_)\
        template<typename SeqRef>\
        BOOST_PP_CAT(vector,N)(\
            support::sequence_assign_type<SeqRef> COMBINATION seq_assign)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(VECTOR_ASSIGN_CTOR,_);

#       undef VECTOR_ASSIGN_CTOR
#   endif

        template <typename Seq>
        BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)>&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
#   if N
            typedef typename
                result_of::begin<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
            It0;
            It0 it0 = fusion::begin(seq);

            m0=fusion::deref(it0);

#       define BOOST_FUSION_DEREF_MEMBER_ASSIGN(Z, N, _)\
            typedef typename\
                result_of::next<BOOST_PP_CAT(It, BOOST_PP_DEC(N))>::type\
            BOOST_PP_CAT(It, N);\
            BOOST_PP_CAT(It, N) BOOST_PP_CAT(it, N)=\
                fusion::next(BOOST_PP_CAT(it, BOOST_PP_DEC(N)));\
            \
            BOOST_PP_CAT(m, N) = fusion::deref(BOOST_PP_CAT(it, N));

            BOOST_PP_REPEAT_FROM_TO(1, N, BOOST_FUSION_DEREF_MEMBER_ASSIGN, _)

#       undef BOOST_FUSION_DEREF_MEMBER_ASSIGN
#   endif

            return *this;
        }

        BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)>&
        operator=(BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)>const& seq)
        {
#ifdef BOOST_NO_RVALUE_REFERENCES
            return this->operator=<
                    BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)>
                >(seq);
#else
            return this->operator=<
                    BOOST_PP_CAT(vector, N)<BOOST_PP_ENUM_PARAMS(N, T)> const&
                >(seq);
#endif
        }

        template<typename I>
        typename detail::add_lref<typename mpl::at<types, I>::type>::type
        at_impl(I)
        {
            return at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        typename detail::add_lref<
            typename add_const<typename mpl::at<types, I>::type>::type
        >::type
        at_impl(I) const
        {
            return at_impl(mpl::int_<I::value>());
        }

#   define BOOST_FUSION_MEMBER_DECL(Z, N, _)\
        typename detail::add_lref<BOOST_PP_CAT(T,N)>::type\
        at_impl(mpl::int_<N>)\
        {\
            return BOOST_PP_CAT(m,N);\
        }\
        \
        typename detail::add_lref<\
            typename add_const<BOOST_PP_CAT(T,N)>::type\
        >::type\
        at_impl(mpl::int_<N>) const\
        {\
            return BOOST_PP_CAT(m,N);\
        }\
        \
        BOOST_PP_CAT(T,N) BOOST_PP_CAT(m,N);

        BOOST_PP_REPEAT(N, BOOST_FUSION_MEMBER_DECL, _)

#   undef BOOST_FUSION_MEMBER_DECL
    };
}}
#   undef N
#endif

