/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_DECLTYPE_AND_VARIADIC_TEMPLATES_INVOKE_IMPL_DETAIL_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_DECLTYPE_AND_VARIADIC_TEMPLATES_INVOKE_IMPL_DETAIL_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>
#include <boost/fusion/functional/invocation/detail/that_ptr.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename F, typename... Args>
    struct invoke_impl_result
      : boost::result_of<typename get_func_base<F>::type(Args...)>
    {};

    template<typename Result, typename F, typename... Args>
    typename
        disable_if<
            is_member_function_pointer<
                typename detail::remove_reference<F>::type
            >
          , Result
        >::type
    invoke_impl_call(F&& f, Args&&... args)
    {
        return f(std::forward<Args>(args)...);
    }

    template<
        typename Result
      , typename F
      , typename ClassInstance
      , typename... Args
    >
    typename
        enable_if<
            is_member_function_pointer<
                typename detail::remove_reference<F>::type
            >
          , Result
        >::type
    invoke_impl_call(F&& f, ClassInstance&& instance,Args&&... args)
    {
        return (that_ptr<typename detail::preevaluate<F>::gen::class_type>::
                    get(std::forward<ClassInstance>(instance))->*f)(
                        std::forward<Args>(args)...);
    }

    namespace bidirectional_traversal
    {
        template<
            typename F
          , typename ArgsSeq
          , bool Empty
          , typename... Args
        >
        struct invoke_impl_result
        {
            typedef typename result_of::pop_back<ArgsSeq>::type new_args_seq;

            typedef typename
                invoke_impl_result<
                    F
                  , new_args_seq
                  , result_of::empty<new_args_seq>::value
                  , typename result_of::back<ArgsSeq>::type
                  , Args...
                >::type
            type;
        };

        template<typename F, typename ArgsSeq, typename... Args>
        struct invoke_impl_result<F,ArgsSeq,true,Args...>
          : detail::invoke_impl_result<F,Args...>
        {};

        template<typename F,typename Seq>
        struct invoke_impl
        {
            typedef detail::preevaluate<F> preevaluater;

            typedef typename
                mpl::eval_if<
                    typename preevaluater::is_preevaluable
                  , preevaluater
                  , invoke_impl_result<
                        F
                      , Seq
                      , result_of::empty<Seq>::value
                    >
                >::type
            type;

            template<typename LeftSeq,typename... Args>
            static type
            call_impl(F f,LeftSeq&&, mpl::true_/*SeqEmpty*/,Args&&... args)
            {
                return detail::invoke_impl_call<type>(
                        std::forward<F>(f),std::forward<Args>(args)...);
            }

            template<typename LeftSeq,typename... Args>
            static type
            call_impl(F f,LeftSeq&& seq, mpl::false_/*SeqEmpty*/,Args&&... args)
            {
                return call_impl(
                        f,
                        fusion::pop_back(std::forward<LeftSeq>(seq)),
                        mpl::bool_<!(result_of::size<LeftSeq>::value-1)>(),
                        fusion::back(std::forward<LeftSeq>(seq)),
                        std::forward<Args>(args)...);
            }

            static type
            call(F f,Seq seq)
            {
                return call_impl(f,
                        std::forward<Seq>(seq),
                        typename result_of::empty<Seq>::type());
            }
        };
    }

    namespace forward_traversal
    {
        template<
            typename F
          , typename Seq
          , int NumArgsLeft
          , typename... Args
        >
        struct invoke_impl_result
          : invoke_impl_result<
                F
              , Seq
              , NumArgsLeft-1
              , typename result_of::deref<
                    typename result_of::advance_c<
                        typename result_of::begin<Seq>::type
                      , NumArgsLeft-1
                    >::type
                >::type
              , Args...
            >
        {};

        template<typename F, typename Seq, typename... Args>
        struct invoke_impl_result<F,Seq,0,Args...>
          : detail::invoke_impl_result<F,Args...>
        {};

        template<typename F,typename Seq>
        struct invoke_impl
        {
            typedef detail::preevaluate<F> preevaluater;

            typedef typename
                mpl::eval_if<
                    typename preevaluater::is_preevaluable
                  , preevaluater
                  , invoke_impl_result<
                        F
                      , Seq
                      , result_of::size<Seq>::value
                    >
                >::type
            type;

            template<typename... Args>
            static type
            call_impl(F f,Seq, mpl::int_<0>, Args&&... args)
            {
                return detail::invoke_impl_call<type>(
                        std::forward<F>(f),std::forward<Args>(args)...);
            }

            template<int NumArgsLeft,typename... Args>
            static type
            call_impl(F f,Seq seq, mpl::int_<NumArgsLeft>, Args&&... args)
            {
                return call_impl(
                        f,
                        seq,
                        mpl::int_<NumArgsLeft-1>(),
                        deref(advance_c<NumArgsLeft-1>(
                            fusion::begin(std::forward<Seq>(seq))
                        )),
                        std::forward<Args>(args)...);
            }

            static type
            call(F f,Seq seq)
            {
                return call_impl(f,
                        seq,
                        mpl::int_<result_of::size<Seq>::value>());
            }
        };
    }

    template<typename F,typename Seq>
    struct invoke_impl
      : mpl::if_<
            traits::is_bidirectional<Seq>
         , bidirectional_traversal::invoke_impl<F,Seq>
         , forward_traversal::invoke_impl<F,Seq>
        >::type
    {};
}}}

#endif
