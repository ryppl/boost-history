// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_DUMMY_TAG_HPP
#define BOOST_FUSION_SUPPORT_DUMMY_TAG_HPP

namespace boost { namespace fusion
{
    struct void_;

    namespace detail
    {
        template<typename>
        struct get_dummy_tag
        {
            typedef void_ type;
        };
    }
}}

#endif
