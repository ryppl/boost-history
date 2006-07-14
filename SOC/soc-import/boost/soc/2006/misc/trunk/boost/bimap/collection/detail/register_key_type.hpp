// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/detail/register_key_type.hpp
/// \brief Define macros to help building metafunctions


#ifndef BOOST_BIMAP_DETAIL_REGISTER_KEY_TYPE_HPP
#define BOOST_BIMAP_DETAIL_REGISTER_KEY_TYPE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/multi_index/tag.hpp>

#include <boost/bimap/collection/support/compute_index_type.hpp>

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_REGISTER_KEY_TYPE_1CP(                                    \
                                                                              \
    IS_KEY_TYPE_METAFUNCTION,                                                 \
    MULTI_INDEX_TYPE,                                                         \
    KEY_TYPE_PARAMETER_NAME,                                                  \
    CONFIG_PARAMETER                                                          \
                                                                              \
)                                                                             \
                                                                              \
namespace support {                                                           \
                                                                              \
template< class KEY_TYPE_PARAMETER_NAME, class KeyExtractor, class Tag >      \
struct compute_index_type                                                     \
<                                                                             \
    KEY_TYPE_PARAMETER_NAME,                                                  \
    KeyExtractor,                                                             \
    Tag,                                                                      \
    typename enable_if                                                        \
    <                                                                         \
        IS_KEY_TYPE_METAFUNCTION                                              \
        <                                                                     \
            KEY_TYPE_PARAMETER_NAME                                           \
        >                                                                     \
                                                                              \
    >::type                                                                   \
>                                                                             \
{                                                                             \
    typedef MULTI_INDEX_TYPE                                                  \
    <                                                                         \
        multi_index::tag< Tag >,                                              \
        KeyExtractor,                                                         \
        CONFIG_PARAMETER                                                      \
                                                                              \
    > type;                                                                   \
};                                                                            \
                                                                              \
} // namespace support

/*///////////////////////////////////////////////////////////////////////////*/




/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_REGISTER_KEY_TYPE_2CP(                                    \
                                                                              \
    IS_KEY_TYPE_METAFUNCTION,                                                 \
    MULTI_INDEX_TYPE,                                                         \
    KEY_TYPE_PARAMETER_NAME,                                                  \
    CONFIG_PARAMETER_1,                                                       \
    CONFIG_PARAMETER_2                                                        \
)                                                                             \
                                                                              \
namespace support {                                                           \
                                                                              \
template< class KEY_TYPE_PARAMETER_NAME, class KeyExtractor, class Tag >      \
struct compute_index_type                                                     \
<                                                                             \
    KEY_TYPE_PARAMETER_NAME,                                                  \
    KeyExtractor,                                                             \
    Tag,                                                                      \
    typename enable_if                                                        \
    <                                                                         \
        IS_KEY_TYPE_METAFUNCTION                                              \
        <                                                                     \
            KEY_TYPE_PARAMETER_NAME                                           \
        >                                                                     \
                                                                              \
    >::type                                                                   \
>                                                                             \
{                                                                             \
    typedef MULTI_INDEX_TYPE                                                  \
    <                                                                         \
        multi_index::tag< Tag >,                                              \
        KeyExtractor,                                                         \
        CONFIG_PARAMETER_1,                                                   \
        CONFIG_PARAMETER_2                                                    \
                                                                              \
    > type;                                                                   \
                                                                              \
};                                                                            \
                                                                              \
} // namespace support

/*///////////////////////////////////////////////////////////////////////////*/





#endif // BOOST_BIMAP_DETAIL_REGISTER_KEY_TYPE_HPP
