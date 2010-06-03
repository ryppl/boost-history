/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_GET_MPL_IT_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_GET_MPL_IT_HPP

namespace boost { namespace fusion { namespace detail
{
    template<typename It, typename Active = void>
    struct get_mpl_it
    {
        typedef It type;
    };

    template<typename It>
    struct get_mpl_it<It, typename It::void_>
    {
        typedef typename It::it_type type;
    };
}}}

#endif
