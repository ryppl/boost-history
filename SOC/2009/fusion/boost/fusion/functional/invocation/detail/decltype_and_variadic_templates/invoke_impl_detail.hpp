// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_DECLTYPE_AND_VARIADIC_TEMPLATES_INVOKE_IMPL_DETAIL_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_DECLTYPE_AND_VARIADIC_TEMPLATES_INVOKE_IMPL_DETAIL_HPP

#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/result_of.hpp>
#include <boost/fusion/functional/invocation/detail/that_ptr.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename FRef, typename... Args>
    struct invoke_impl_result
    {
        typedef typename
            boost::result_of<
                FRef(Args...)
            >::type
        type;
    };

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
        return (that_ptr<typename support::preevaluate<F>::class_type>::
                    get(std::forward<ClassInstance>(instance))->*f)(
                            std::forward<Args>(args)...);
    }

    namespace bidirectional_traversal
    {
        template<
            typename FRef
          , typename ArgsSeq
          , bool Empty
          , typename... Args
        >
        struct invoke_impl_result
        {
            typedef typename result_of::pop_back<ArgsSeq>::type new_args_seq;

            typedef typename
                invoke_impl_result<
                    FRef
                  , new_args_seq
                  , result_of::empty<new_args_seq>::value
                  , typename result_of::back<ArgsSeq>::type
                  , Args...
                >::type
            type;
        };

        template<typename FRef, typename ArgsSeq, typename... Args>
        struct invoke_impl_result<FRef,ArgsSeq,true,Args...>
          : detail::invoke_impl_result<FRef,Args...>
        {};

        template<typename FRef,typename SeqRef>
        struct invoke_impl
        {
            typedef typename
                mpl::eval_if<
                    typename support::is_preevaluable<FRef>::type
                  , support::preevaluate<FRef>
                  , invoke_impl_result<
                        FRef
                      , SeqRef
                      , result_of::empty<SeqRef>::value
                    >
                >::type
            type;

            template<typename Seq,typename... Args>
            static typename
                enable_if<
                    typename result_of::empty<Seq&&>::type
                  , type
                >::type
                call_impl(FRef f,Seq&&, Args&&... args)
            {
                return detail::invoke_impl_call<type>(
                        std::forward<FRef>(f),std::forward<Args>(args)...);
            }

            template<typename Seq,typename... Args>
            static typename
                disable_if<
                    typename result_of::empty<Seq&&>::type
                  , type
                >::type
                call_impl(FRef f,Seq&& seq, Args&&... args)
            {
                return call_impl(
                        f,
                        fusion::pop_back(std::forward<Seq>(seq)),
                        fusion::back(std::forward<Seq>(seq)),
                        std::forward<Args>(args)...);
            }

            static type
            call(FRef f,SeqRef seq)
            {
                return call_impl(f,std::forward<SeqRef>(seq));
            }
        };
    }

    namespace forward_traversal
    {
        template<
            typename FRef
          , typename SeqRef
          , int NumArgsLeft
          , typename... Args
        >
        struct invoke_impl_result
          : invoke_impl_result<
                FRef
              , SeqRef
              , NumArgsLeft-1
              , typename result_of::deref<
                    typename result_of::advance_c<
                        typename result_of::begin<SeqRef>::type
                      , NumArgsLeft-1
                    >::type
                >::type
              , Args...
            >
        {};

        template<typename FRef, typename SeqRef, typename... Args>
        struct invoke_impl_result<FRef,SeqRef,0,Args...>
          : detail::invoke_impl_result<FRef,Args...>
        {
        };

        template<typename FRef,typename SeqRef>
        struct invoke_impl
        {
            typedef typename
                mpl::eval_if<
                    typename support::is_preevaluable<FRef>::type
                  , support::preevaluate<FRef>
                  , invoke_impl_result<
                        FRef
                      , SeqRef
                      , result_of::size<SeqRef>::value
                    >
                >::type
            type;

            template<int NumArgsLeft,typename... Args>
            static typename enable_if_c<!NumArgsLeft,type>::type
            call_impl(FRef f,SeqRef, Args&&... args)
            {
                return detail::invoke_impl_call<type>(
                        std::forward<FRef>(f),std::forward<Args>(args)...);
            }

            template<int NumArgsLeft,typename... Args>
            static typename enable_if_c<NumArgsLeft, type>::type
            call_impl(FRef f,SeqRef seq, Args&&... args)
            {
                return call_impl<NumArgsLeft-1>(
                        f,
                        seq,
                        deref(advance_c<NumArgsLeft-1>(
                                fusion::begin(std::forward<SeqRef>(seq))
                        )),
                        std::forward<Args>(args)...);
            }

            static type
            call(FRef f,SeqRef seq)
            {
                return call_impl<result_of::size<SeqRef>::value>(
                        f,
                        std::forward<SeqRef>(seq));
            }
        };
    }

    template<typename FRef,typename SeqRef>
    struct invoke_impl
      : mpl::if_<
            typename is_base_of<
                bidirectional_traversal_tag
              , typename traits::category_of<SeqRef>::type
            >::type
          , bidirectional_traversal::invoke_impl<FRef,SeqRef>
          , forward_traversal::invoke_impl<FRef,SeqRef>
        >::type
    {};
}}}

#endif
