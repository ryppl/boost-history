/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_DETAIL_VIEW_STORAGE_HPP
#define BOOST_FUSION_VIEW_DETAIL_VIEW_STORAGE_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/internal/ref.hpp>

namespace boost { namespace fusion { namespace detail
{
#ifdef BOOST_MSVC
#   define BOOST_FUSION_DETAIL_VIEW_STROAGE(SEQ)\
        detail::view_storage<SEQ, traits::is_view<SEQ>::type::value>

    template<typename T, bool IsView>
    struct view_storage;
#else
#   define BOOST_FUSION_DETAIL_VIEW_STROAGE(SEQ) detail::view_storage<SEQ>

    template<typename T, bool IsView=traits::is_view<T>::type::value>
    struct view_storage;
#endif

    template<typename View>
    struct view_storage<View, /*IsView*/true>
    {
        typedef typename
            remove_const<typename remove_reference<View>::type>::type
        type;
#ifdef BOOST_NO_RVALUE_REFERENCES
        typedef type const& call_param;
#endif

#define BOOST_FUSION_VIEW_STORAGE_CTOR(MODIFIER,_)\
        template<typename OtherT, bool IsView>\
        view_storage(\
            view_storage<OtherT, IsView> MODIFIER storage)\
          : view(storage.template get<view_storage<OtherT, IsView> MODIFIER>())\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VIEW_STORAGE_CTOR,_)

#undef BOOST_FUSION_VIEW_STORAGE_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
        view_storage(call_param view)
          : view(view)
        {}
#else
        template<typename OtherView>
        view_storage(OtherView&& view)
          : view(std::forward<OtherView>(view))
        {}
#endif

        template<typename OtherViewStorage>
        view_storage&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherViewStorage) other_storage)
        {
            view=other_storage.template get<OtherViewStorage>();
            return *this;
        }

        //workaround until we get rvalue refs for this
        template<typename SelfType>
        typename forward_as<SelfType, type>::type
        get()
        {
            return static_cast<typename forward_as<SelfType, type>::type>(view);
        }

        template<typename SelfType>
        typename forward_as<SelfType, type const>::type
        get()const
        {
            return static_cast<
                typename forward_as<SelfType, type const>::type
            >(view);
        }

        type view;
    };

    template<typename Seq>
    struct view_storage<Seq, /*IsView*/false>
    {
        typedef typename add_lref<Seq>::type type;
#ifdef BOOST_NO_RVALUE_REFERENCES
        typedef type call_param;
#endif

#define BOOST_FUSION_VIEW_STORAGE_CTOR(MODIFIER,_)\
        template<typename OtherSeq>\
        view_storage(\
            view_storage<OtherSeq, false> MODIFIER storage)\
          : seq(storage.seq)\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VIEW_STORAGE_CTOR,_)

#undef BOOST_FUSION_VIEW_STORAGE_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
        view_storage(call_param seq)
          : seq(&seq)
        {}
#else
        template<typename OtherSeq>
        view_storage(OtherSeq&& seq)
          : seq(&seq)
        {}
#endif

        view_storage&
        operator=(view_storage const& other_storage)
        {
            seq=other_storage.seq;
            return *this;
        }

        template<typename OtherViewStorage>
        view_storage&
        operator=(OtherViewStorage const& other_storage)
        {
            seq=other_storage.seq;
            return *this;
        }

        template<typename>
        type
        get()const
        {
            return static_cast<type>(*seq);
        }

        typename remove_reference<Seq>::type* seq;
    };
}}}

#endif
