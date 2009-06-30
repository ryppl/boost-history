/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DETAIL_IS_VIEW_HPP
#define BOOST_FUSION_SUPPORT_DETAIL_IS_VIEW_HPP

namespace boost { namespace fusion { namespace detail
{
    template <typename T>
    struct fusion_is_view
    {
        typedef typename detail::remove_reference<T>::type::is_view type;
    };
}}}

#endif
