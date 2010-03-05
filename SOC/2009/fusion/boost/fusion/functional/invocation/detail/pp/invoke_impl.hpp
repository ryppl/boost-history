/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#ifndef BOOST_FUSION_NO_MEMBER_PTR
#   include <boost/fusion/sequence/intrinsic/front.hpp>
#   include <boost/fusion/functional/invocation/detail/member_object_pointer_helper.hpp>
#   include <boost/fusion/functional/invocation/detail/that_ptr.hpp>
#endif
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#ifndef BOOST_FUSION_RETURN_VOID
#   include <boost/preprocessor/repetition/enum_binary_params.hpp>
#   include <boost/preprocessor/facilities/intercept.hpp>
#endif
#ifndef BOOST_FUSION_NO_MEMBER_PTR
#   include <boost/preprocessor/repetition/enum_shifted.hpp>
#endif
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace fusion { namespace detail
{
#ifndef BOOST_FUSION_NO_MEMBER_PTR
    struct member_function_ptr;
    struct member_object_ptr;
#endif
    struct function_object;

    namespace random_access_impl
    {
#ifndef BOOST_FUSION_RETURN_VOID
        template<int SeqSize, typename F, typename SeqRef>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME);
#endif

        template<
            typename f_type
          , int SeqSize
          , typename F
          , typename SeqRef
          , typename Result
        >
        struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl);
    }

    namespace forward_impl
    {
#ifndef BOOST_FUSION_RETURN_VOID
        template<int SeqSize, typename F, typename SeqRef>
        struct BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME);
#endif

        template<
            typename f_type
          , int SeqSize
          , typename F
          , typename SeqRef
          , typename Result
        >
        struct BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl);
    }

#define BOOST_PP_FILENAME_1 <boost/fusion/functional/invocation/detail/pp/invoke_impl_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_INVOKE_IMPL_MAX_ARITY)
#include BOOST_PP_ITERATE()

    template<typename F, typename SeqRef>
    struct BOOST_FUSION_INVOKE_NAME
    {
#ifdef BOOST_FUSION_RETURN_VOID
        typedef void type;
#else
        typedef
            mpl::eval_if<
                traits::is_random_access<SeqRef>
              , random_access_impl::
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
                    result_of::size<SeqRef>::value
                  , F
                  , SeqRef
                >
              , forward_impl::
                    BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
                    result_of::size<SeqRef>::value
                  , F
                  , SeqRef
                >
            >
        impl_type;
        typedef typename detail::remove_reference<F>::type f;
        typedef preevaluate<F> preevaluater;

        typedef typename
            mpl::eval_if<
                typename preevaluater::is_preevaluable
              , preevaluater
#   ifndef BOOST_FUSION_NO_MEMBER_PTR
              , mpl::eval_if<
                    is_member_object_pointer<f>
                  , get_object_pointer_result_type_seq<f, SeqRef>
#   endif
                  , impl_type
#   ifndef BOOST_FUSION_NO_MEMBER_PTR
                >
#   endif
            >::type
        type;
#endif

        static type
        call(F f_, SeqRef seq)
        {
#ifdef BOOST_FUSION_RETURN_VOID
            typedef typename detail::remove_reference<F>::type f;
#endif

            typedef
#ifdef BOOST_FUSION_NO_MEMBER_PTR
                function_object
#else
                typename mpl::eval_if<
                    is_member_function_pointer<f>
                  , mpl::identity<member_function_ptr>
                  , mpl::if_<
                        is_member_object_pointer<f>
                      , member_object_ptr
                      , function_object
                    >
                >::type
#endif
            f_type;

            return mpl::if_<
                traits::is_random_access<SeqRef>
              , random_access_impl::
                    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
                    f_type
                  , result_of::size<SeqRef>::value
                  , F
                  , SeqRef
                  , type
                >
              , forward_impl::
                    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_impl)<
                    f_type
                  , result_of::size<SeqRef>::value
                  , F
                  , SeqRef
                  , type
                >
            >::type::call(f_, seq);
        }
    };
}}}

