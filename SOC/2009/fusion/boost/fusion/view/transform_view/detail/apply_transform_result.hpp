/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_APPLY_TRANSFORM_RESULT_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_APPLY_TRANSFORM_RESULT_HPP

#include <boost/utility/result_of.hpp>

namespace boost { namespace fusion
{
    struct void_;

    namespace detail
    {
        template <typename F>
        struct apply_transform_result
        {
            //TODO cschmidt: !!!
            template <typename T0, typename T1 = void_>
            struct apply
              : //boost::result_of<F(T0, T1)>
                detail::remove_reference<F>::type::template result<
                    typename detail::identity<F>::type(T0, T1)>
            {};

            template <typename T0>
            struct apply<T0, void_>
              : //boost::result_of<F(T0)>
                detail::remove_reference<F>::type::template result<
                    typename detail::identity<F>::type(T0)>
            {};
        };
    }
}}

#endif
