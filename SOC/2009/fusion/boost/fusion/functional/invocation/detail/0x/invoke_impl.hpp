// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/functional/invocation/detail/0x/invoke_impl_detail.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template<typename F, typename Seq>
        struct BOOST_FUSION_INVOKE_NAME
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            typedef
                detail::invoke_impl<
                    typename detail::add_lref<F>::type
                  , typename detail::add_lref<Seq>::type
                >
            gen;

            typedef typename gen::type type;
        };
    }

    //TODO doc
    template <typename F, typename Seq>
    inline typename result_of::BOOST_FUSION_INVOKE_NAME<F, Seq&&>::type
    BOOST_FUSION_INVOKE_NAME(BOOST_FUSION_R_ELSE_CLREF(F) f, Seq&& seq)
    {
        return result_of::BOOST_FUSION_INVOKE_NAME<F, Seq&&>::gen::call(f,seq);
    }
}}

