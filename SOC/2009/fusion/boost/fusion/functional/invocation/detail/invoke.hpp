/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/ref.hpp>
#if defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES)
#   include <boost/fusion/functional/invocation/detail/pp/invoke_impl.hpp>
#else
#   include <boost/fusion/functional/invocation/detail/variadic_templates/invoke_impl.hpp>
#endif
#ifndef BOOST_NO_RVALUE_REFERENCES
#   include <boost/mpl/if.hpp>
#   include <boost/type_traits/is_same.hpp>
#endif

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename F, typename Seq>
        struct BOOST_FUSION_INVOKE_NAME
          : detail::BOOST_FUSION_INVOKE_NAME<
                typename unwrap_reference<F>::type
              , typename detail::add_lref<Seq>::type
            >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename F, typename Seq>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<F, Seq const&>::type
    BOOST_FUSION_INVOKE_NAME(F f, Seq const& seq)
    {
         return result_of::BOOST_FUSION_INVOKE_NAME<F, Seq const&>::call(f,seq);
    }

    template <typename F, typename Seq>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<F,Seq&>::type
    BOOST_FUSION_INVOKE_NAME(F f,Seq& seq)
    {
        return result_of::BOOST_FUSION_INVOKE_NAME<F,Seq&>::call(f,seq);
    }
#else
    template <typename FQualified=void,typename F=void, typename Seq=void>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<
        typename mpl::if_<
            is_same<FQualified, void>
          , F&&
          , FQualified
        >::type
      , Seq&&
    >::type
    BOOST_FUSION_INVOKE_NAME(F&& f, Seq&& seq)
    {
         return result_of::BOOST_FUSION_INVOKE_NAME<
             typename mpl::if_<
                 is_same<FQualified, void>
               , F&&
               , FQualified
             >::type
           , Seq&&
         >::call(static_cast<F&&>(f),std::forward<Seq>(seq));
    }
#endif
}}

