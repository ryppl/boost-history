// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_CONS_FWD_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_CONS_FWD_HPP

namespace boost { namespace fusion
{
    struct cons_tag;
    struct forward_traversal_tag;

    struct nil;

    template <typename Car, typename Cdr>
    struct cons;
}}

#endif
