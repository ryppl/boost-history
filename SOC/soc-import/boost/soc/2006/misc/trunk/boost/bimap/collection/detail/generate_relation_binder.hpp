// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/detail/generate_relation_binder.hpp
/// \brief Define macros to help building metafunctions

#ifndef BOOST_BIMAP_COLLECTION_DETAIL_GENERATE_RELATION_BINDER_HPP
#define BOOST_BIMAP_COLLECTION_DETAIL_GENERATE_RELATION_BINDER_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/bimap/collection/key_type_of_tag.hpp>

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_RELATION_BINDER_1CP(                             \
                                                                              \
        SET_TYPE_OF,                                                          \
        CP1                                                                   \
    )                                                                         \
                                                                              \
    template<class Relation,class DefaultParameter1 = use_default >           \
    struct bind_to                                                            \
    {                                                                         \
        typedef typename mpl::if_                                             \
        <                                                                     \
            is_same<DefaultParameter1,use_default>,                           \
            typename mpl::if_                                                 \
            <                                                                 \
                is_same<CP1,use_default>,                                     \
                SET_TYPE_OF<Relation>,                                        \
                SET_TYPE_OF<Relation,CP1>                                     \
                                                                              \
            >::type,                                                          \
            SET_TYPE_OF<Relation,DefaultParameter1>                           \
                                                                              \
        >::type type;                                                         \
                                                                              \
    };

/*///////////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_RELATION_BINDER_2CP(                             \
                                                                              \
        SET_TYPE_OF,                                                          \
        CP1,                                                                  \
        CP2                                                                   \
    )                                                                         \
                                                                              \
    template                                                                  \
    <                                                                         \
        class Relation,                                                       \
        class DefaultParameter1 = use_default,                                \
        class DefaultParameter2 = use_default                                 \
    >                                                                         \
    struct bind_to                                                            \
    {                                                                         \
        typedef typename mpl::if_                                             \
        <                                                                     \
            is_same<DefaultParameter1,use_default>,                           \
                                                                              \
            typename mpl::if_                                                 \
            <                                                                 \
                is_same<CP1,use_default>,                                     \
                SET_TYPE_OF<Relation>,                                        \
                typename mpl::if_                                             \
                <                                                             \
                    is_same<CP2,use_default>,                                 \
                    SET_TYPE_OF<Relation,CP1>,                                \
                    SET_TYPE_OF<Relation,CP1,CP2>                             \
                                                                              \
                >::type                                                       \
                                                                              \
            >::type,                                                          \
                                                                              \
            typename mpl::if_                                                 \
            <                                                                 \
                is_same<DefaultParameter2,use_default>,                       \
                typename mpl::if_                                             \
                <                                                             \
                    is_same<CP2,use_default>,                                 \
                    SET_TYPE_OF<Relation,DefaultParameter1>,                  \
                    SET_TYPE_OF<Relation,DefaultParameter1,CP2>               \
                                                                              \
                >::type,                                                      \
                                                                              \
                SET_TYPE_OF<Relation,DefaultParameter1,DefaultParameter2>     \
                                                                              \
            >::type                                                           \
                                                                              \
        >::type type;                                                         \
                                                                              \
    };

/*///////////////////////////////////////////////////////////////////////////*/



#endif // BOOST_BIMAP_COLLECTION_DETAIL_GENERATE_RELATION_BINDER_HPP
