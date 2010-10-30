/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PP_IS_ITERATING
#   include <boost/fusion/container/vector/vector_fwd.hpp>
#   ifdef BOOST_FUSION_TAGGED_VECTOR
#       include <boost/fusion/container/vector/detail/variadic_templates/tagged_impl.hpp>
#   endif
#   include <boost/fusion/sequence/intrinsic/front.hpp>
#   include <boost/fusion/sequence/intrinsic/begin.hpp>
#   include <boost/fusion/iterator/deref.hpp>
#   include <boost/fusion/iterator/next.hpp>
#   include <boost/fusion/iterator/advance_c.hpp>
#   include <boost/fusion/support/sequence_base.hpp>
#   include <boost/fusion/support/internal/sequence_assign.hpp>
#   include <boost/fusion/support/internal/template.hpp>
#   include <boost/fusion/support/internal/is_explicitly_convertible.hpp>

#   include <boost/preprocessor/cat.hpp>
#   include <boost/preprocessor/dec.hpp>
#   include <boost/preprocessor/empty.hpp>
#   include <boost/preprocessor/repetition/enum.hpp>
#   include <boost/preprocessor/repetition/repeat.hpp>
#   include <boost/preprocessor/repetition/repeat_from_to.hpp>
#   include <boost/preprocessor/iteration/iterate.hpp>

#   include <boost/mpl/int.hpp>
#   include <boost/mpl/bool.hpp>
#   ifdef BOOST_FUSION_PREFER_MPL
#       include <boost/mpl/at.hpp>
#   endif
#   include <boost/type_traits/add_const.hpp>
#   include <boost/utility/enable_if.hpp>

#   include <boost/fusion/container/vector/detail/at_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_at_impl.hpp>
#   include <boost/fusion/container/vector/detail/begin_impl.hpp>
#   include <boost/fusion/container/vector/detail/end_impl.hpp>
#   include <boost/fusion/container/vector/detail/deref_impl.hpp>
#   include <boost/fusion/container/vector/detail/value_of_impl.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;

#ifdef BOOST_MSVC
#   pragma warning(push)
    //'class' : multiple copy constructors specified
#   pragma warning(disable:4521)
#endif

#   define BOOST_PP_FILENAME_1 <boost/fusion/container/vector/detail/pp/vector_n.hpp>
#   define BOOST_PP_ITERATION_LIMITS (BOOST_FUSION_FROM, BOOST_FUSION_TO)
#   include BOOST_PP_ITERATE()

#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif
}}

#else
#   define BOOST_FUSION_N BOOST_PP_ITERATION()

#   if !BOOST_FUSION_N
    VARIADIC_TEMPLATE_WITH_DEFAULT(1)
    struct vector0;

    template<>
    struct vector0<void_>
      : sequence_base<vector0<void_> >
#   else
#       ifndef BOOST_FUSION_PREFER_MPL
    template<typename Vector>
    struct
    vector_meta_value_at<Vector,BOOST_PP_DEC(BOOST_FUSION_N)>
    {
        typedef typename
            Vector::BOOST_PP_CAT(t,BOOST_PP_DEC(BOOST_FUSION_N))
        type;
    };
#       endif

    VARIADIC_TEMPLATE(BOOST_FUSION_N)
    struct BOOST_PP_CAT(vector, BOOST_FUSION_N)
      : sequence_base<BOOST_PP_CAT(vector, BOOST_FUSION_N)<
            EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N)>
        >
#   endif
    {
#   ifdef BOOST_FUSION_PREFER_MPL
        typedef
            mpl::BOOST_PP_CAT(vector, BOOST_FUSION_N)<
                EXPAND_TEMPLATE_ARGUMENTS(BOOST_FUSION_N)
            >
        types;
#   endif
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag;
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<BOOST_FUSION_N> size;

#   define BOOST_FUSION_DEFAULT_MEMBER_INIT(Z, N, _) BOOST_PP_CAT(m,N)()

        BOOST_PP_CAT(vector, BOOST_FUSION_N)()
#   if BOOST_FUSION_N
          : BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_DEFAULT_MEMBER_INIT, _)
#   endif
        {}

#   undef BOOST_FUSION_DEFAULT_MEMBER_INIT

#   if BOOST_FUSION_N
#       ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
#           define BOOST_FUSION_MEMBER_INIT(Z, N, MODIFIER)\
                BOOST_PP_CAT(m,N)(vec.BOOST_PP_CAT(m,N))
#       else
#           define BOOST_FUSION_MEMBER_INIT(Z, N, MODIFIER)\
                 BOOST_PP_CAT(m,N)(static_cast<\
                     typename detail::forward_as<\
                         int MODIFIER\
                       , BOOST_PP_CAT(T,N)\
                     >::type>(vec.BOOST_PP_CAT(m,N)))
#       endif

#       define BOOST_FUSION_VECTOR_CTOR(MODIFIER,_)\
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(\
            BOOST_PP_CAT(vector, BOOST_FUSION_N) MODIFIER vec)\
          : BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_MEMBER_INIT, MODIFIER)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VECTOR_CTOR,_)

#       undef BOOST_FUSION_MEMBER_INIT
#   else
#       define BOOST_FUSION_VECTOR_CTOR(MODIFIER,_)\
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(\
            BOOST_PP_CAT(vector, BOOST_FUSION_N) MODIFIER)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VECTOR_CTOR,_)
#   endif
#   undef BOOST_FUSION_VECTOR_CTOR

#   if BOOST_FUSION_N
#       define BOOST_FUSION_MEMBER_INIT(Z, N, _)\
        BOOST_PP_CAT(m,N)(\
            BOOST_FUSION_FORWARD(BOOST_PP_CAT(A,N), BOOST_PP_CAT(_,N)))

#       ifndef BOOST_FUSION_NO_RVALUE_REFERENCES
        VARIADIC_TEMPLATE_A(BOOST_FUSION_N)
#       endif
        explicit
        BOOST_PP_CAT(vector, BOOST_FUSION_N)(
#       ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
            EXPAND_TEMPLATE_ARGUMENTS_CALL_PARAMS(BOOST_FUSION_N)
#       else
            EXPAND_TEMPLATE_ARGUMENTS_PARAMETERS_A_R_ELSE_CLREF(BOOST_FUSION_N)
#           if BOOST_FUSION_N==1
          , typename enable_if<
                detail::is_explicitly_convertible<A0&&, T0>
            >::type* =0
#           endif
#       endif
            )
          : BOOST_PP_ENUM(BOOST_FUSION_N, BOOST_FUSION_MEMBER_INIT, _)
        {}

#       undef BOOST_FUSION_MEMBER_INIT

#       if BOOST_FUSION_N==1
#           define BOOST_FUSION_MEMBER_INIT(Z, N, SEQ)\
            m0(fusion::front(SEQ))
#       else
#           define BOOST_FUSION_MEMBER_INIT(Z, N, SEQ)\
            BOOST_PP_CAT(m,N)(fusion::deref(\
                fusion::advance_c<N>(fusion::begin(SEQ))))
#       endif

        template<class Seq>
        BOOST_PP_CAT(vector,BOOST_FUSION_N)(
            BOOST_FUSION_R_ELSE_CLREF(Seq) seq
#       if BOOST_FUSION_N==1
          , typename disable_if<
                detail::is_explicitly_convertible<
                    BOOST_FUSION_R_ELSE_CLREF(Seq)
                  , T0
                >
            >::type* =0
#       endif
            )
          : BOOST_PP_ENUM(BOOST_FUSION_N,
                  BOOST_FUSION_MEMBER_INIT,
                  BOOST_FUSION_FORWARD(Seq,seq))
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::equal_to<
                    size
                  , result_of::size<BOOST_FUSION_R_ELSE_CLREF(Seq)>
                >))
        }

#       define BOOST_FUSION_VECTOR_SEQ_ASSIGN_CTOR(MODIFIER,_)\
        template<typename SeqRef>\
        BOOST_PP_CAT(vector,BOOST_FUSION_N)(\
            detail::sequence_assign_type<SeqRef> MODIFIER seq_assign)\
          : BOOST_PP_ENUM(BOOST_FUSION_N,\
                  BOOST_FUSION_MEMBER_INIT,\
                  seq_assign.get())\
        {\
            BOOST_FUSION_MPL_ASSERT((\
                mpl::equal_to<size,result_of::size<SeqRef> >));\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(
            BOOST_FUSION_VECTOR_SEQ_ASSIGN_CTOR,_)

#       undef BOOST_FUSION_VECTOR_SEQ_ASSIGN_CTOR
#       undef BOOST_FUSION_MEMBER_INIT
#   else
#       define BOOST_FUSION_VECTOR_ASSIGN_CTOR(MODIFIER,_)\
        template<typename SeqRef>\
        BOOST_PP_CAT(vector,BOOST_FUSION_N)(\
            detail::sequence_assign_type<SeqRef> MODIFIER)\
        {\
            BOOST_FUSION_MPL_ASSERT((result_of::empty<SeqRef>))\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VECTOR_ASSIGN_CTOR,_)

        template<typename Seq>
        BOOST_PP_CAT(vector,BOOST_FUSION_N)(Seq const&)
        {
            BOOST_FUSION_MPL_ASSERT((result_of::empty<Seq const&>))
        }

#       undef BOOST_FUSION_VECTOR_ASSIGN_CTOR
#   endif

        template<typename Seq>
        BOOST_PP_CAT(vector, BOOST_FUSION_N)&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq)
#   if BOOST_FUSION_N
                        seq
#   endif
                        )
        {
            BOOST_FUSION_MPL_ASSERT((
                mpl::equal_to<size,result_of::size<Seq> >))

#   if BOOST_FUSION_N
            typedef typename
                result_of::begin<BOOST_FUSION_R_ELSE_CLREF(Seq)>::type
            It0;
            It0 it0 = fusion::begin(BOOST_FUSION_FORWARD(Seq,seq));

            m0=fusion::deref(it0);

#       define BOOST_FUSION_DEREF_MEMBER_ASSIGN(Z, N, _)\
            typedef typename\
                result_of::next<BOOST_PP_CAT(It, BOOST_PP_DEC(N))>::type\
            BOOST_PP_CAT(It, N);\
            BOOST_PP_CAT(It, N) BOOST_PP_CAT(it, N)=\
                fusion::next(BOOST_PP_CAT(it, BOOST_PP_DEC(N)));\
            \
            BOOST_PP_CAT(m, N) = fusion::deref(BOOST_PP_CAT(it, N));

            BOOST_PP_REPEAT_FROM_TO(
                    1,
                    BOOST_FUSION_N,
                    BOOST_FUSION_DEREF_MEMBER_ASSIGN,
                    _)

#       undef BOOST_FUSION_DEREF_MEMBER_ASSIGN
#   endif

            return *this;
        }

        BOOST_PP_CAT(vector, BOOST_FUSION_N)&
        operator=(BOOST_PP_CAT(vector, BOOST_FUSION_N)const& seq)
        {
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
            return this->operator=<BOOST_PP_CAT(vector, BOOST_FUSION_N)>(seq);
#else
            return this->operator=<BOOST_PP_CAT(vector, BOOST_FUSION_N) const&>(
                seq);
#endif
        }

#   ifdef BOOST_FUSION_PREFER_MPL
#       define BOOST_FUSION_META_VALUE_AT(N)
#   else
#       define BOOST_FUSION_META_VALUE_AT(N)\
            typedef BOOST_PP_CAT(T,N) BOOST_PP_CAT(t,N);
#   endif

#   define BOOST_FUSION_MEMBER_DECL(Z, N, _)\
        BOOST_FUSION_META_VALUE_AT(N)\
        \
        typename detail::add_lref<BOOST_PP_CAT(T,N)>::type\
        at_impl(mpl::int_<N>)\
        {\
            return static_cast<\
                typename detail::add_lref<BOOST_PP_CAT(T,N)>::type\
            >(BOOST_PP_CAT(m,N));\
        }\
        \
        typename detail::add_lref<\
            typename add_const<BOOST_PP_CAT(T,N)>::type\
        >::type\
        at_impl(mpl::int_<N>) const\
        {\
            return static_cast<\
                typename detail::add_lref<\
                    typename add_const<BOOST_PP_CAT(T,N)>::type\
                >::type\
            >(BOOST_PP_CAT(m,N));\
        }\
        \
        BOOST_PP_CAT(T,N) BOOST_PP_CAT(m,N);

        BOOST_PP_REPEAT(BOOST_FUSION_N, BOOST_FUSION_MEMBER_DECL, _)

#   undef BOOST_FUSION_MEMBER_DECL
#   undef BOOST_FUSION_META_VALUE_AT
    };

#   undef BOOST_FUSION_N
#endif

