/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP

#ifdef BOOST_FUSION_ENABLE_STATIC_ASSERTS
#   include <boost/fusion/sequence/intrinsic/size.hpp>
#endif
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/deduce.hpp>
#include <boost/fusion/support/internal/sequence_assign.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/call_traits.hpp>
#endif
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/fusion/view/single_view/detail/single_view_fwd.hpp>
#include <boost/fusion/view/single_view/detail/single_view_iterator.hpp>
#include <boost/fusion/view/single_view/detail/begin_impl.hpp>
#include <boost/fusion/view/single_view/detail/end_impl.hpp>
#include <boost/fusion/view/single_view/detail/deref_impl.hpp>
#include <boost/fusion/view/single_view/detail/next_impl.hpp>
#include <boost/fusion/view/single_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/single_view/detail/equal_to_impl.hpp>

namespace boost { namespace fusion
{
    struct single_view_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    template <typename T>
    struct single_view
      : sequence_base<single_view<T> >
    {
        typedef T value_type;

        typedef single_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; 
        typedef forward_traversal_tag category;
        typedef mpl::true_ is_view;
        typedef mpl::int_<1> size;

#define BOOST_FUSION_SINGLE_VIEW_CTOR(COMBINATION,_)\
        single_view(single_view COMBINATION view)\
          : val(BOOST_FUSION_FORWARD(single_view COMBINATION,view).val)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_SINGLE_VIEW_CTOR,_)

#undef BOOST_FUSION_SINGLE_VIEW_CTOR

#ifdef BOOST_NO_VARIADIC_TEMPLATES
        single_view()
          : val()
        {}

#   ifdef BOOST_NO_RVALUE_REFERENCES
        explicit
        single_view(typename call_traits<T>::param_type val)
          : val(val)
        {}
#   else
        template<typename Arg>
        explicit
        single_view(Arg&& val);
          : val(std::forward<Arg>(val))
        {}
#   endif
#else
        template<typename... Args>
        explicit
        single_view(BOOST_FUSION_R_ELSE_CLREF(Args)... args)
          : val(BOOST_FUSION_FORWARD(Args,args)...)
        {}
#endif

        template<typename Seq>
        single_view(
            BOOST_FUSION_R_ELSE_CLREF(Seq) seq,
            typename disable_if<
                is_convertible<BOOST_FUSION_R_ELSE_CLREF(Seq), value_type>
            >::type* =0)
          : val(fusion::front(BOOST_FUSION_FORWARD(Seq,seq)))
        {
            BOOST_FUSION_STATIC_ASSERT((result_of::size<Seq>::value==1));
        }

#define BOOST_FUSION_SINGLE_VIEW_ASSIGN_CTOR(COMBINATION,_)\
        template<typename SeqRef>\
        single_view(\
            detail::sequence_assign_type<SeqRef> COMBINATION seq_assign)\
          : val(fusion::front(seq_assign.get()))\
        {\
            BOOST_FUSION_STATIC_ASSERT((result_of::size<SeqRef>::value==1));\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_SINGLE_VIEW_ASSIGN_CTOR,_)

#undef BOOST_FUSION_SINGLE_VIEW_ASSIGN_CTOR

        template<typename OtherT>
        single_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherT) val)
        {
            val=BOOST_FUSION_FORWARD(OtherT,val);
            return *this;
        }

#define BOOST_FUSION_SINGLE_VIEW_SEQ_ASSIGN(COMBINATION,_)\
        template<typename SeqRef>\
        single_view&\
        operator=(detail::sequence_assign_type<SeqRef> COMBINATION seq_assign)\
        {\
            BOOST_FUSION_STATIC_ASSERT((result_of::size<SeqRef>::value==1));\
            \
            val=fusion::front(seq_assign.get());\
            return *this;\
        }

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_SINGLE_VIEW_SEQ_ASSIGN,_)

#undef BOOST_FUSION_SINGLE_VIEW_SEQ_ASSIGN

        value_type val;
    };

    namespace result_of
    {
        template<typename T>
        struct make_single_view
        {
            typedef
                single_view<
                    typename traits::deduce<BOOST_FUSION_R_ELSE_CLREF(T)>::type
                >
            type;
        };
    };

    template <typename T>
    inline typename result_of::make_single_view<T>::type
    make_single_view(BOOST_FUSION_R_ELSE_CLREF(T) val)
    {
        return typename result_of::make_single_view<T>::type(
                BOOST_FUSION_FORWARD(T,val));
    }
}}

#endif
