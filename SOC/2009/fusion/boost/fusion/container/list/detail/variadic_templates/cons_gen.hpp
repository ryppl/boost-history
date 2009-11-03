/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_VARIADIC_TEMPLATES_CONS_GEN_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_VARIADIC_TEMPLATES_CONS_GEN_HPP

namespace boost { namespace fusion { namespace detail
{
    template<typename... Args>
    struct cons_gen;

    template<typename Head, typename... Args>
    struct cons_gen<Head, Args...>
    {
    public:
        typedef cons<Head, typename cons_gen<Args...>::type> type;
    };

    template<>
    struct cons_gen<>
    {
    public:
        typedef nil type;
    };
}}}

#endif
