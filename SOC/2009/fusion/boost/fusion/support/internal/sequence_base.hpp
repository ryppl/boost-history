/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_BASE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_SEQUENCE_BASE_HPP

#include <boost/preprocessor/control/iif.hpp>
#include <boost/mpl/bool.hpp>

#define BOOST_FUSION_DEFINE_IS_SEQUENCE_IS_VIEW_IMPL(TAG, IS_VIEW)              \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template<>                                                                  \
    struct is_sequence_impl<TAG>                                                \
    {                                                                           \
        template<typename>                                                      \
        struct apply                                                            \
          : mpl::true_                                                          \
        {};                                                                     \
    };                                                                          \
                                                                                \
    template<>                                                                  \
    struct is_view_impl<TAG>                                                    \
    {                                                                           \
        template<typename>                                                      \
        struct apply                                                            \
          : BOOST_PP_IIF(IS_VIEW, mpl::true_, mpl::false_)                      \
        {};                                                                     \
    };                                                                          \
}}}

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct is_sequence_impl;

    template<typename>
    struct is_view_impl;
}}}

#endif
