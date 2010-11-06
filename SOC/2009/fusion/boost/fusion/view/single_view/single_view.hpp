/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP

#include <boost/fusion/support/internal/base.hpp>
#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#endif
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/support/internal/sequence_base.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/fusion/support/internal/sequence_assign.hpp>
#include <boost/fusion/support/internal/is_explicitly_convertible.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/utility/enable_if.hpp>
#if defined(BOOST_NO_VARIADIC_TEMPLATES) && defined(BOOST_FUSION_NO_RVALUE_REFERENCES)
#   include <boost/call_traits.hpp>
#endif

#include <boost/fusion/view/single_view/detail/single_view_fwd.hpp>
#include <boost/fusion/view/single_view/detail/at_impl.hpp>
#include <boost/fusion/view/single_view/detail/value_at_impl.hpp>
#include <boost/fusion/view/single_view/detail/single_view_iterator.hpp>
#include <boost/fusion/view/single_view/detail/begin_impl.hpp>
#include <boost/fusion/view/single_view/detail/deref_impl.hpp>
#include <boost/fusion/view/single_view/detail/distance_impl.hpp>
#include <boost/fusion/view/single_view/detail/end_impl.hpp>
#include <boost/fusion/view/single_view/detail/equal_to_impl.hpp>
#include <boost/fusion/view/single_view/detail/next_impl.hpp>
#include <boost/fusion/view/single_view/detail/prior_impl.hpp>
#include <boost/fusion/view/single_view/detail/value_of_impl.hpp>

BOOST_FUSION_DEFINE_IS_SEQUENCE_IS_VIEW_IMPL(single_view_tag, 1)

namespace boost { namespace fusion
{
    struct single_view_tag;
    struct fusion_sequence_tag;

#ifdef BOOST_MSVC
#   pragma warning(push)
    //'class' : multiple copy constructors specified
#   pragma warning(disable:4521)
#endif

    template<typename T>
    struct single_view
    {
        typedef T value_type;

        typedef single_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; 
        typedef random_access_traversal_tag category;
        typedef mpl::int_<1> size;

#define BOOST_FUSION_SINGLE_VIEW_CTOR(MODIFIER,_)\
        single_view(single_view MODIFIER view)\
          : val(static_cast<single_view MODIFIER>(view).val)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_SINGLE_VIEW_CTOR,_)

#undef BOOST_FUSION_SINGLE_VIEW_CTOR

        single_view()
          : val()
        {}

#if defined(BOOST_NO_VARIADIC_TEMPLATES) && defined(BOOST_FUSION_NO_RVALUE_REFERENCES)
        explicit
        single_view(typename call_traits<T>::param_type val)
          : val(val)
        {}
#else
        template<typename Arg>
        explicit
        single_view(BOOST_FUSION_R_ELSE_CLREF(Arg) arg
          , typename enable_if<
                detail::is_explicitly_convertible<
                    BOOST_FUSION_R_ELSE_CLREF(Arg)
                  , value_type
                >
            >::type* =0)
          : val(BOOST_FUSION_FORWARD(Arg,arg))
        {}
#endif

#ifndef BOOST_NO_VARIADIC_TEMPLATES
        template<typename Arg1, typename Arg2, typename... Args>
        explicit
        single_view(BOOST_FUSION_R_ELSE_CLREF(Arg1) arg1
          , BOOST_FUSION_R_ELSE_CLREF(Arg2) arg2
          , BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : val(BOOST_FUSION_FORWARD(Arg1,arg1),
                BOOST_FUSION_FORWARD(Arg2,arg2),
                BOOST_FUSION_FORWARD(Args,args)...)
        {}
#endif

        template<typename Seq>
        single_view(
            BOOST_FUSION_R_ELSE_CLREF(Seq) seq
          , typename disable_if<
                detail::is_explicitly_convertible<
                    BOOST_FUSION_R_ELSE_CLREF(Seq)
                  , value_type
                >
            >::type* =0)
          : val(fusion::front(BOOST_FUSION_FORWARD(Seq,seq)))
        {
            BOOST_FUSION_STATIC_ASSERT((result_of::size<Seq>::value==1))
        }

#define BOOST_FUSION_SINGLE_VIEW_ASSIGN_CTOR(MODIFIER,_)\
        template<typename SeqRef>\
        single_view(\
            detail::sequence_assign_type<SeqRef> MODIFIER seq_assign)\
          : val(fusion::front(seq_assign.get()))\
        {\
            BOOST_FUSION_STATIC_ASSERT((result_of::size<SeqRef>::value==1))\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(
            BOOST_FUSION_SINGLE_VIEW_ASSIGN_CTOR,_)

#undef BOOST_FUSION_SINGLE_VIEW_ASSIGN_CTOR

        single_view&
        operator=(single_view const& other_view)
        {
            val=other_view.val;
            return *this;
        }

        template<typename OtherT>
        single_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherT) val)
        {
            val=BOOST_FUSION_FORWARD(OtherT,val);
            return *this;
        }

#define BOOST_FUSION_SINGLE_VIEW_SEQ_ASSIGN(MODIFIER,_)\
        template<typename SeqRef>\
        single_view&\
        operator=(detail::sequence_assign_type<SeqRef> MODIFIER seq_assign)\
        {\
            BOOST_FUSION_STATIC_ASSERT((result_of::size<SeqRef>::value==1))\
            \
            val=fusion::front(seq_assign.get());\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(
            BOOST_FUSION_SINGLE_VIEW_SEQ_ASSIGN,_)

#undef BOOST_FUSION_SINGLE_VIEW_SEQ_ASSIGN

        value_type val;
    };

    namespace result_of
    {
        template<typename T>
        struct make_single_view
        {
            typedef single_view<typename traits::deduce<T>::type> type;
        };
    }

    template<typename T>
    inline typename
        result_of::make_single_view<BOOST_FUSION_R_ELSE_CLREF(T)>::type
    make_single_view(BOOST_FUSION_R_ELSE_CLREF(T) val)
    {
        return typename result_of::make_single_view<
            BOOST_FUSION_R_ELSE_CLREF(T)
        >::type(BOOST_FUSION_FORWARD(T,val));
    }

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
    template<typename T, typename Seq>
    //cschmidt: see https://svn.boost.org/trac/boost/ticket/3305
#   if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) || BOOST_WORKAROUND(__GNUC__,<4)
    typename lazy_disable_if<
        is_const<T>
      , result_of::make_single_view<T&>
    >::type
#   else
    typename result_of::make_single_view<T&>::type
#   endif
    make_single_view(T& val)
    {
        return typename result_of::make_single_view<T&>::type(val);
    }
#endif

#ifdef BOOST_MSVC
#   pragma warning(pop)
#endif
}}

#endif
