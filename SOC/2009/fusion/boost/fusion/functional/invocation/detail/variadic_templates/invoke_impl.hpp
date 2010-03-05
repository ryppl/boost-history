/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#ifndef BOOST_FUSION_NO_MEMBER_PTR
#   include <boost/fusion/functional/invocation/detail/member_object_pointer_helper.hpp>
#   include <boost/fusion/functional/invocation/detail/that_ptr.hpp>
#endif
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>

namespace boost { namespace fusion { namespace detail
{
#ifndef BOOST_FUSION_RETURN_VOID
    template<typename F, typename... Args>
    struct BOOST_PP_CAT(
        BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
        _impl)
      : boost::result_of<typename get_func_base<F>::type(Args...)>
    {};
#endif

#ifdef BOOST_FUSION_NO_MEMBER_PTR
    template<typename Result, typename F, typename...Args>
    static Result
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(F&& f,Args&&...args)
    {
#   ifndef BOOST_FUSION_RETURN_VOID
        return
#   endif
            f(BOOST_FUSION_FORWARD(Args,args)...);
    }
#else
    template<typename Result, typename F, typename...Args>
    static typename disable_if<
        is_member_pointer<typename remove_reference<F>::type>
      , Result
    >::type
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(F&& f,Args&&...args)
    {
#   ifndef BOOST_FUSION_RETURN_VOID
        return
#   endif
            f(std::forward<Args>(args)...);
    }

    template<typename Result, typename F,  typename Instance,typename...Args>
    static typename enable_if<
        is_member_function_pointer<typename remove_reference<F>::type>
      , Result
    >::type
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(
                    F&& f,
                    Instance&& instance,
                    Args&&... args)
    {
#   ifndef BOOST_FUSION_RETURN_VOID
        return
#   endif
            (that_ptr<typename preevaluate<F>::gen::class_type>::get(
                instance)->*f)(std::forward<Args>(args)...);
    }

#   ifndef BOOST_FUSION_RETURN_VOID
    template<typename Result, typename F,  typename Instance,typename...Args>
    static typename enable_if<
        is_member_object_pointer<typename remove_reference<F>::type>
      , Result
    >::type
    BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)(
                    F&& f,
                    Instance&& instance,
                    Args&&... args)
    {
        BOOST_FUSION_STATIC_ASSERT(!sizeof...(Args));

        return (that_ptr<
                    typename get_object_pointer_result_type<
                        Instance&&
                      , typename remove_reference<F>::type
                    >::cv_class_type&
                >::get(instance)->*f);
    }
#   endif
#endif

    namespace bidirectional_impl
    {
#ifndef BOOST_FUSION_RETURN_VOID
        template<
            int NumElementsLeft
          , typename F
          , typename It4
          , typename...Args
        >
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)
        {
            typedef typename result_of::prior<It4>::type It3;
            typedef typename result_of::prior<It3>::type It2;
            typedef typename result_of::prior<It2>::type It1;
            typedef typename result_of::prior<It1>::type It0;

            typedef typename
                BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
                    NumElementsLeft-4
                  , F
                  , typename result_of::prior<It0>::type
                  , typename result_of::deref<It0>::type
                  , typename result_of::deref<It1>::type
                  , typename result_of::deref<It2>::type
                  , typename result_of::deref<It3>::type
                  , Args...
                >::type
            type;
        };

        template<typename F, typename It3, typename...Args>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
            3
          , F
          , It3
          , Args...
        >
        {
            typedef typename result_of::prior<It3>::type It2;
            typedef typename result_of::prior<It2>::type It1;

            typedef typename
                detail::BOOST_PP_CAT(
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
                    _impl)<
                    F
                  , typename result_of::deref<
                        typename result_of::prior<It1>::type
                    >::type
                  , typename result_of::deref<It1>::type
                  , typename result_of::deref<It2>::type
                  , Args...
                >::type
            type;
        };

        template<typename F, typename It2, typename...Args>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
            2
          , F
          , It2
          , Args...
        >
        {
            typedef typename result_of::prior<It2>::type It1;

            typedef typename
                detail::BOOST_PP_CAT(
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
                    _impl)<
                    F
                  , typename result_of::deref<
                        typename result_of::prior<It1>::type
                    >::type
                  , typename result_of::deref<It1>::type
                  , Args...
                >::type
            type;
        };

        template<typename F, typename It1, typename...Args>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
            1
          , F
          , It1
          , Args...
        >
        {
            typedef typename
                detail::BOOST_PP_CAT(
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
                    _impl)<
                    F
                  , typename result_of::deref<
                        typename result_of::prior<It1>::type
                    >::type
                  , Args...
                >::type
            type;
        };

        template<typename F, typename It0, typename...Args>
        struct BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
            0
          , F
          , It0
          , Args...
        >
        {
            typedef typename
                detail::BOOST_PP_CAT(
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
                    _impl)<
                    F
                  , Args...
                >::type
            type;
        };
#endif

        template<typename F, typename Result>
        struct BOOST_PP_CAT(unrolled_,BOOST_FUSION_INVOKE_NAME)
        {
            template<int NumElementsLeft,typename It4,typename...Args>
            static Result
            call(mpl::int_<NumElementsLeft>,
                            F f,
                            It4 const& it4,
                            Args&&... args)
            {
                typedef typename result_of::prior<It4 const&>::type It3;
                It3 it3 = fusion::prior(it4);
                typedef typename result_of::prior<It3&>::type It2;
                It2 it2 = fusion::prior(it3);
                typedef typename result_of::prior<It2&>::type It1;
                It1 it1 = fusion::prior(it2);
                typedef typename result_of::prior<It1&>::type It0;
                It0 it0 = fusion::prior(it1);

                return call(
                    mpl::int_<NumElementsLeft-4>(),
                    f,
                    it0,
                    fusion::deref(it0),
                    fusion::deref(it1),
                    fusion::deref(it2),
                    fusion::deref(it3),
                    std::forward<Args>(args)...);
            }

            template<typename It3,typename...Args>
            static Result
            call(mpl::int_<3>,
                            F f,
                            It3 const& it3,
                            Args&&... args)
            {
                typedef typename result_of::prior<It3 const&>::type It2;
                It2 it2 = fusion::prior(it3);
                typedef typename result_of::prior<It2&>::type It1;
                It1 it1 = fusion::prior(it2);

                return BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<
                    Result
                >(static_cast<F>(f),
                        fusion::deref(fusion::prior(it1)),
                        fusion::deref(it1),
                        fusion::deref(it2),
                        std::forward<Args>(args)...);
            }

            template<typename It2,typename...Args>
            static Result
            call(mpl::int_<2>,
                            F f,
                            It2 const& it2,
                            Args&&... args)
            {
                typedef typename result_of::prior<It2 const&>::type It1;
                It1 it1 = fusion::prior(it2);

                return BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<
                    Result
                >(static_cast<F>(f),
                        fusion::deref(fusion::prior(it1)),
                        fusion::deref(it1),
                        std::forward<Args>(args)...);
            }

            template<typename It1,typename...Args>
            static Result
            call(mpl::int_<1>,
                            F f,
                            It1 const& it1,
                            Args&&... args)
            {
                return BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<
                    Result
                >(static_cast<F>(f),
                    fusion::deref(fusion::prior(it1)),
                    std::forward<Args>(args)...);
            }

            template<typename It,typename...Args>
            static Result
            call(mpl::int_<0>,
                            F f,
                            It const& it,
                            Args&&... args)
            {
                return BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<
                    Result
                >(static_cast<F>(f),std::forward<Args>(args)...);
            }
        };
    }

    namespace forward_impl
    {
#ifndef BOOST_FUSION_RETURN_VOID
        template<int NumArgsLeft, typename F, typename Begin, typename... Args>
        struct BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)
          : BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
                NumArgsLeft-1
              , F
              , Begin
              , typename result_of::deref<
                    typename result_of::advance_c<Begin, NumArgsLeft-1>::type
                >::type
              , Args...
            >
        {};

        template<typename F, typename Begin, typename... Args>
        struct BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
            0
          , F
          , Begin
          , Args...
        >
          : BOOST_PP_CAT(
                BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME),
                _impl)<
                F
              , Args...
            >
        {};
#endif

        template<typename F,typename Result>
        struct BOOST_FUSION_INVOKE_NAME
        {
            template<typename Begin, typename... Args>
            static Result
            call(mpl::int_<0>, F f,Begin const&, Args&&... args)
            {
                return BOOST_PP_CAT(BOOST_FUSION_INVOKE_NAME,_call_impl)<
                    Result
                >(std::forward<F>(f),std::forward<Args>(args)...);
            }

            template<int NumArgsLeft,typename Begin, typename... Args>
            static Result
            call(mpl::int_<NumArgsLeft>, F f, Begin const& begin,Args&&... args)
            {
                return call(
                        mpl::int_<NumArgsLeft-1>(),
                        f,
                        begin,
                        fusion::deref(fusion::advance_c<NumArgsLeft-1>(begin)),
                        std::forward<Args>(args)...);
            }
        };
    }

    template<typename F, typename SeqRef>
    struct BOOST_FUSION_INVOKE_NAME
    {
#ifdef BOOST_FUSION_RETURN_VOID
        typedef void type;
#else
        typedef
            mpl::eval_if<
                traits::is_bidirectional<SeqRef>
              , bidirectional_impl::
                    BOOST_PP_CAT(result_of_unrolled_,BOOST_FUSION_INVOKE_NAME)<
                    result_of::size<SeqRef>::value
                  , F
                  , typename result_of::end<SeqRef>::type
                >
              , forward_impl::
                    BOOST_PP_CAT(result_of_,BOOST_FUSION_INVOKE_NAME)<
                    result_of::size<SeqRef>::value
                  , F
                  , typename result_of::begin<SeqRef>::type
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
        call_impl(mpl::false_ /*is_bidirectional*/,
                    F f,
                    SeqRef seq)
        {
            return forward_impl::BOOST_FUSION_INVOKE_NAME<
                F
              , type
           >::call(mpl::int_<result_of::size<SeqRef>::value>(),
                   f,
                   fusion::begin(static_cast<SeqRef>(seq)));
        }

        static type
        call_impl(mpl::true_ /*is_bidirectional*/,
                    F f,
                    SeqRef seq)
        {
            return bidirectional_impl::BOOST_PP_CAT(unrolled_,
                BOOST_FUSION_INVOKE_NAME)<
                F
              , type
            >::call(mpl::int_<result_of::size<SeqRef>::value>(),
                    f,
                    fusion::end(static_cast<SeqRef>(seq)));
        }

        static type
        call(F f, SeqRef seq)
        {
            return call_impl(
                traits::is_bidirectional<SeqRef>(),f,seq);
        }
    };
}}}

