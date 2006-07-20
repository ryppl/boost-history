// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file detail/generate_relation_binder.hpp
/// \brief Define macros to help building the set type of definitions

#ifndef BOOST_BIMAP_DETAIL_GENERATE_RELATION_BINDER_HPP
#define BOOST_BIMAP_DETAIL_GENERATE_RELATION_BINDER_HPP

#include <boost/mpl/apply.hpp>

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_RELATION_BINDER_0CP(                             \
                                                                              \
        SET_TYPE_OF                                                           \
    )                                                                         \
                                                                              \
    template< class Relation >                                                \
    struct bind_to                                                            \
    {                                                                         \
        typedef SET_TYPE_OF<Relation> type;                                   \
                                                                              \
    };

/*///////////////////////////////////////////////////////////////////////////*/



/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_RELATION_BINDER_1CP(                             \
                                                                              \
        SET_TYPE_OF,                                                          \
        CP1                                                                   \
    )                                                                         \
                                                                              \
    template< class Relation >                                                \
    struct bind_to                                                            \
    {                                                                         \
        typedef SET_TYPE_OF                                                   \
        <                                                                     \
            Relation,                                                         \
            typename mpl::apply<CP1,Relation>::type                           \
                                                                              \
        > type;                                                               \
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
    template< class Relation >                                                \
    struct bind_to                                                            \
    {                                                                         \
        typedef SET_TYPE_OF                                                   \
        <                                                                     \
            Relation,                                                         \
            typename mpl::apply<CP1,Relation>::type,                          \
            typename mpl::apply<CP2,Relation>::type                           \
                                                                              \
        > type;                                                               \
                                                                              \
    };

/*///////////////////////////////////////////////////////////////////////////*/



#endif // BOOST_BIMAP_DETAIL_GENERATE_RELATION_BINDER_HPP
