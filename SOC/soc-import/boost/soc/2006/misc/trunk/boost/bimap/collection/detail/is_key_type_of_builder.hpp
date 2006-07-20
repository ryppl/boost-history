// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/detail/is_key_type_of_builder.hpp
/// \brief Define macros to help building metafunctions

#ifndef BOOST_BIMAP_COLLECTION_DETAIL_IS_KEY_TYPE_OF_BUILDER_HPP
#define BOOST_BIMAP_COLLECTION_DETAIL_IS_KEY_TYPE_OF_BUILDER_HPP

#include <boost/bimap/detail/mpl/bool_result.hpp>
#include <boost/bimap/collection/support/is_key_type_of.hpp>
#include <boost/static_assert.hpp>


/*/////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_IS_KEY_TYPE_OF_BUILDER_0CP(                     \
                                                                    \
        METAFUNCTION_NAME,                                          \
        KEY_TYPE                                                    \
                                                                    \
    )                                                               \
                                                                    \
    namespace support {                                             \
                                                                    \
    template< class Type >                                          \
    struct METAFUNCTION_NAME                                        \
    {                                                               \
        BOOST_BIMAP_MPL_BOOL_RESULT(false);                         \
    };                                                              \
                                                                    \
    template<class T1>                                              \
    struct METAFUNCTION_NAME< KEY_TYPE<T1> >                        \
    {                                                               \
        BOOST_BIMAP_MPL_BOOL_RESULT(true);                          \
    };                                                              \
                                                                    \
    } // namespace support

/*/////////////////////////////////////////////////////////////////*/


/*/////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_IS_KEY_TYPE_OF_BUILDER_1CP(                     \
                                                                    \
        METAFUNCTION_NAME,                                          \
        KEY_TYPE                                                    \
                                                                    \
    )                                                               \
                                                                    \
    namespace support {                                             \
                                                                    \
    template< class Type >                                          \
    struct METAFUNCTION_NAME                                        \
    {                                                               \
        BOOST_BIMAP_MPL_BOOL_RESULT(false);                         \
    };                                                              \
                                                                    \
    template<class T1, class T2>                                    \
    struct METAFUNCTION_NAME< KEY_TYPE<T1,T2> >                     \
    {                                                               \
        BOOST_BIMAP_MPL_BOOL_RESULT(true);                          \
    };                                                              \
                                                                    \
    } // namespace support

/*/////////////////////////////////////////////////////////////////*/



/*/////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_IS_KEY_TYPE_OF_BUILDER_2CP(                     \
                                                                    \
        METAFUNCTION_NAME,                                          \
        KEY_TYPE                                                    \
                                                                    \
    )                                                               \
                                                                    \
    namespace support {                                             \
                                                                    \
    template< class Type >                                          \
    struct METAFUNCTION_NAME                                        \
    {                                                               \
        BOOST_BIMAP_MPL_BOOL_RESULT(false);                         \
    };                                                              \
                                                                    \
    template<class T1, class T2, class T3 >                         \
    struct METAFUNCTION_NAME< KEY_TYPE<T1,T2,T3> >                  \
    {                                                               \
        BOOST_BIMAP_MPL_BOOL_RESULT(true);                          \
    };                                                              \
                                                                    \
    } // namespace support

/*/////////////////////////////////////////////////////////////////*/



#endif // BOOST_BIMAP_COLLECTION_DETAIL_IS_KEY_TYPE_OF_BUILDER_HPP

