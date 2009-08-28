/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/functional/invocation/detail/0x/invoke_impl_detail.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename F, typename Seq>
        struct BOOST_FUSION_INVOKE_NAME
          : detail::invoke_impl<F, Seq>
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    //TODO doc
    template <typename F, typename Seq>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<F&&, Seq&&>::type
    BOOST_FUSION_INVOKE_NAME(F&& f, Seq&& seq)
    {
        return result_of::BOOST_FUSION_INVOKE_NAME<F&&,Seq&&>::call(f,seq);
    }
}}

