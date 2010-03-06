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

    template <typename F, typename Seq>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<
        BOOST_FUSION_RREF_ELSE_OBJ(F)
      , BOOST_FUSION_R_ELSE_CLREF(Seq)
    >::type
    BOOST_FUSION_INVOKE_NAME(
        BOOST_FUSION_RREF_ELSE_OBJ(F) f,
        BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return result_of::BOOST_FUSION_INVOKE_NAME<
            BOOST_FUSION_RREF_ELSE_OBJ(F)
          , BOOST_FUSION_R_ELSE_CLREF(Seq)
        >::call(f,seq);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename F, typename Seq>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<F,Seq&>::type
    BOOST_FUSION_INVOKE_NAME(F f,Seq& seq)
    {
        return result_of::BOOST_FUSION_INVOKE_NAME<F,Seq&>::call(f,seq);
    }
#endif
}}

