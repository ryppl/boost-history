// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_DETAIL_VIEW_STORAGE_HPP
#define BOOST_FUSION_VIEW_DETAIL_VIEW_STORAGE_HPP

#include <boost/fusion/support/is_view.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename T>
    struct view_storage
    {
    private:
        template<typename OtherT>
        static BOOST_FUSION_R_ELSE_CLREF(OtherT)
        get_init_type(BOOST_FUSION_R_ELSE_LREF(OtherT) other_t, mpl::true_)
        {
            return other_t;
        }

        template<typename OtherT>
        static typename detail::remove_reference<OtherT>::type*
        get_init_type(BOOST_FUSION_R_ELSE_LREF(OtherT) other_t, mpl::false_)
        {
            return &other_t;
        }

    public:
        typedef typename
            mpl::if_<
                traits::is_view<T>
              , typename detail::remove_reference<T>::type
              , typename detail::add_lref<T>::type
            >::type
        type;

#ifdef BOOST_NO_RVALUE_REFERENCES
        typedef typename
            mpl::if_<
                traits::is_view<T>
              , typename detail::add_lref<typename add_const<T>::type>::type
              , type
            >::type
        call_param;
#endif

#define BOOST_FUSION_VIEW_STORAGE_CTOR(COMBINATION,_)\
        template<typename OtherT>\
        view_storage(view_storage<OtherT> COMBINATION storage)\
          : t(get_init_type(storage.get(), typename traits::is_view<T>::type()))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_VIEW_STORAGE_CTOR,_)

#undef BOOST_FUSION_VIEW_STORAGE_CTOR

#ifdef BOOST_NO_RVALUE_REFERENCES
        view_storage(call_param t)
          : t(get_init_type(t, typename traits::is_view<T>::type()))
        {}
#else
        template<typename OtherT>
        view_storage(OtherT&& t)
          : t(get_init_type(
                  std::forward<OtherT>(t),
                  typename traits::is_view<T>::type()))
        {}
#endif

        template<typename OtherViewStorage>
        view_storage&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherViewStorage) other_storage)
        {
            t=BOOST_FUSION_FORWARD(OtherViewStorage,other_storage).t;
            return *this;
        }

        typename mpl::if_<
            traits::is_view<T>
          , typename detail::add_lref<type>::type
          , type
        >::type
        get() const
        {
            return get(typename traits::is_view<T>::type());
        }

    private:
        typename detail::add_lref<type>::type
        get(mpl::true_ /*is_view*/)const
        {
            return t;
        }

        type
        get(mpl::false_ /*is_view*/)const
        {
            return *t;
        }

        typedef typename detail::remove_reference<T>::type non_ref_t;
        mutable typename
            mpl::if_<
                traits::is_view<T>
              , typename remove_const<non_ref_t>::type
              , non_ref_t*
            >::type
        t;
    };
}}}

#endif
