// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/detail/access_builder.hpp
/// \brief Define macros to help building metafunctions

#ifndef BOOST_BIMAP_RELATION_ACCESS_BUILDER_HPP
#define BOOST_BIMAP_RELATION_ACCESS_BUILDER_HPP

#include <boost/bimap/relation/support/member_with_tag.hpp>
#include <boost/call_traits.hpp>


/******************************************************************************
                   BIMAP SYMMETRIC ACCESS RESULT OF
*******************************************************************************

namespace result_of {

template< class Tag, class SymmetricType >
struct NAME
{
    typedef -UNDEFINED- type;
};

} // namespace result_of

******************************************************************************/

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_SYMMETRIC_ACCESS_RESULT_OF_BUILDER(                       \
                                                                              \
        NAME,                                                                 \
        METAFUNCTION_BASE                                                     \
    )                                                                         \
                                                                              \
    namespace result_of {                                                     \
                                                                              \
    template< class Tag, class SymmetricType >                                \
    struct NAME                                                               \
    {                                                                         \
        typedef typename METAFUNCTION_BASE                                    \
        <                                                                     \
            Tag,SymmetricType                                                 \
                                                                              \
        >::type value_type;                                                   \
                                                                              \
        typedef typename call_traits<value_type>::reference type;             \
    };                                                                        \
                                                                              \
    template< class Tag, class SymmetricType >                                \
    struct NAME< Tag, const SymmetricType >                                   \
    {                                                                         \
        typedef typename METAFUNCTION_BASE                                    \
        <                                                                     \
            Tag,SymmetricType                                                 \
                                                                              \
        >::type value_type;                                                   \
                                                                              \
        typedef typename call_traits<value_type>::const_reference type;       \
    };                                                                        \
                                                                              \
    }

/*///////////////////////////////////////////////////////////////////////////*/


/******************************************************************************
                   BIMAP SYMMETRIC ACCESS IMPLEMENTATION
*******************************************************************************

namespace detail {

template< class Tag, class SymmetricType >
typename result_of::NAME<Tag,SymmetricType>::type
    NAME( Tag , const Relation & );

} // namespace detail

******************************************************************************/

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_SYMMETRIC_ACCESS_IMPLEMENTATION_BUILDER(                  \
                                                                              \
        NAME,                                                                 \
        TP_SYMMETRIC,                                                         \
        PARAMETER_NAME,                                                       \
        LEFT_BODY,                                                            \
        RIGHT_BODY                                                            \
    )                                                                         \
                                                                              \
    namespace detail {                                                        \
                                                                              \
    template< class Tag, class TP_SYMMETRIC >                                 \
    typename result_of::NAME<Tag,TP_SYMMETRIC>::type                          \
        NAME( Tag, TP_SYMMETRIC & PARAMETER_NAME );                           \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename result_of::NAME<member_at::left,TP_SYMMETRIC>::type              \
        NAME( member_at::left, TP_SYMMETRIC & PARAMETER_NAME )                \
    {                                                                         \
        LEFT_BODY;                                                            \
    }                                                                         \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename result_of::NAME<member_at::right,TP_SYMMETRIC>::type             \
        NAME( member_at::right, TP_SYMMETRIC & PARAMETER_NAME )               \
    {                                                                         \
        RIGHT_BODY;                                                           \
    }                                                                         \
                                                                              \
    template< class Tag, class TP_SYMMETRIC >                                 \
    typename result_of::NAME<Tag,const TP_SYMMETRIC>::type                    \
        NAME( Tag, const TP_SYMMETRIC & PARAMETER_NAME );                     \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename result_of::NAME<member_at::left,const TP_SYMMETRIC>::type        \
        NAME( member_at::left, const TP_SYMMETRIC & PARAMETER_NAME )          \
    {                                                                         \
        LEFT_BODY;                                                            \
    }                                                                         \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename result_of::NAME<member_at::right,const TP_SYMMETRIC>::type       \
        NAME( member_at::right, const TP_SYMMETRIC & PARAMETER_NAME )         \
    {                                                                         \
        RIGHT_BODY;                                                           \
    }                                                                         \
                                                                              \
    }

/*///////////////////////////////////////////////////////////////////////////*/


/******************************************************************************
                   BIMAP RELATION ACCESS INTERFACE
*******************************************************************************

template< class Tag, class SymmetricType >
typename result_of::NAME<Tag,SymmetricType>::type
    NAME( const SymmetricType & );

******************************************************************************/

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_SYMMETRIC_ACCESS_INTERFACE_BUILDER(                       \
                                                                              \
        NAME                                                                  \
    )                                                                         \
                                                                              \
    template< class Tag, class SymmetricType >                                \
    typename result_of::NAME<Tag,SymmetricType>::type                         \
    NAME( SymmetricType & s )                                                 \
    {                                                                         \
        typedef typename member_with_tag                                      \
        <                                                                     \
            Tag,SymmetricType                                                 \
                                                                              \
        >::type member_at_tag;                                                \
                                                                              \
        return detail::NAME(member_at_tag(),s);                               \
    }                                                                         \
                                                                              \
    template< class Tag, class SymmetricType >                                \
    typename result_of::NAME<Tag,const SymmetricType>::type                   \
    NAME( const SymmetricType & s )                                           \
    {                                                                         \
        typedef typename member_with_tag                                      \
        <                                                                     \
            Tag,SymmetricType                                                 \
                                                                              \
        >::type member_at_tag;                                                \
                                                                              \
        return detail::NAME(member_at_tag(),s);                               \
    }

/*///////////////////////////////////////////////////////////////////////////*/


#endif // BOOST_BIMAP_RELATION_ACCESS_BUILDER_HPP

