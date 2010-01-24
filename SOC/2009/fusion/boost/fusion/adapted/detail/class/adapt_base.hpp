/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_CLASS_ADAPT_BASE_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_CLASS_ADAPT_BASE_HPP

#include <boost/fusion/adapted/detail/struct/adapt_base.hpp>

#define BOOST_FUSION_ADAPT_CLASS_C_BASE(NAME, I, TUPLE, MAX_SIZE)               \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template <>                                                                 \
    struct struct_member<NAME, I>                                                \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(MAX_SIZE, 0, TUPLE) type;                   \
        typedef BOOST_PP_TUPLE_ELEM(MAX_SIZE, 1, TUPLE) get_type;               \
                                                                                \
        struct proxy                                                            \
        {                                                                       \
            typedef BOOST_PP_TUPLE_ELEM(MAX_SIZE, 0, TUPLE) type;               \
                                                                                \
            proxy(NAME& obj)                                                    \
              : obj(obj)                                                        \
            {}                                                                  \
                                                                                \
            template<class Arg>                                                 \
            proxy&                                                              \
            operator=(BOOST_FUSION_R_ELSE_CLREF(Arg) val)                       \
            {                                                                   \
                BOOST_PP_TUPLE_ELEM(MAX_SIZE, 3, TUPLE);                        \
                return *this;                                                   \
            }                                                                   \
                                                                                \
            operator type()                                                     \
            {                                                                   \
                return BOOST_PP_TUPLE_ELEM(MAX_SIZE, 2, TUPLE);                 \
            }                                                                   \
                                                                                \
            NAME& obj;                                                          \
        };                                                                      \
                                                                                \
        static get_type                                                         \
        call(NAME const& obj)                                                   \
        {                                                                       \
            return BOOST_PP_TUPLE_ELEM(MAX_SIZE, 2, TUPLE);                     \
        };                                                                      \
                                                                                \
        static proxy                                                            \
        call(NAME& obj)                                                         \
        {                                                                       \
            return proxy(obj);                                                  \
        };                                                                      \
    };                                                                          \
}}}

#endif
