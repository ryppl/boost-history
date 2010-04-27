/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_REVERSE_VIEW_FWD_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_REVERSE_VIEW_FWD_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/internal/ref.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        //cschmidt: There is a nasty bug in gcc 4.5.0 which causes segfaults
        //when compiling forward_as in begin_impl and end_impl of reverse_view.
        //Using this thin wrapper seems to workaround the problem.
#if BOOST_WORKAROUND(__GNUC__,==4)&& \
    BOOST_WORKAROUND(__GNUC_MINOR__,==5)
    //cschmidt: the internals of BOOST_WORKAROUND do not allow comparing
    //with 0!
#   if __GNUC_PATCHLEVEL__==0
#       define BOOST_FUSION_REVERSE_VIEW_USE_FORWARD_AS_GCC_HELPER

        template <typename TestType,typename Type>
        struct reverse_view_forward_as_gcc_helper
          : forward_as<TestType, Type>
        {};
#   endif
#endif
    }

    struct reverse_view_tag;
}}

#endif
