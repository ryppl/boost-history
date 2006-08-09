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
#include <boost/bimap/relation/member_at.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>


/******************************************************************************
                   BIMAP SYMMETRIC ACCESS RESULT OF
*******************************************************************************

namespace result_of {

template< class Tag, class SymmetricType >
struct NAME
{
    typedef -unspecified- type;
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
        typedef typename mpl::if_< is_const<SymmetricType>,                   \
                                                                              \
            typename call_traits<value_type>::const_reference,                \
                                                                              \
            typename call_traits<value_type>::reference                       \
                                                                              \
        >::type type;                                                         \
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
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename enable_if< mpl::not_< is_const<TP_SYMMETRIC> >,                  \
                                                                              \
    typename result_of::NAME                                                  \
    <                                                                         \
        ::boost::bimap::relation::member_at::left,TP_SYMMETRIC                \
                                                                              \
    >::type                                                                   \
                                                                              \
    >::type NAME( ::boost::bimap::relation::member_at::left,                  \
                  TP_SYMMETRIC & PARAMETER_NAME )                             \
    {                                                                         \
        LEFT_BODY;                                                            \
    }                                                                         \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename enable_if< mpl::not_< is_const<TP_SYMMETRIC> >,                  \
                                                                              \
    typename result_of::NAME                                                  \
    <                                                                         \
        ::boost::bimap::relation::member_at::right,TP_SYMMETRIC               \
                                                                              \
    >::type                                                                   \
                                                                              \
    >::type NAME( ::boost::bimap::relation::member_at::right,                 \
                  TP_SYMMETRIC & PARAMETER_NAME )                             \
    {                                                                         \
        RIGHT_BODY;                                                           \
    }                                                                         \
                                                                              \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename enable_if< is_const<TP_SYMMETRIC> ,                              \
                                                                              \
    typename result_of::NAME                                                  \
    <                                                                         \
        ::boost::bimap::relation::member_at::left,TP_SYMMETRIC                \
                                                                              \
    >::type                                                                   \
                                                                              \
    >::type NAME( ::boost::bimap::relation::member_at::left,                  \
                  TP_SYMMETRIC & PARAMETER_NAME )                             \
    {                                                                         \
        LEFT_BODY;                                                            \
    }                                                                         \
                                                                              \
    template< class TP_SYMMETRIC >                                            \
    typename enable_if< is_const<TP_SYMMETRIC> ,                              \
                                                                              \
    typename result_of::NAME                                                  \
    <                                                                         \
        ::boost::bimap::relation::member_at::right,TP_SYMMETRIC               \
                                                                              \
    >::type                                                                   \
                                                                              \
    >::type NAME( ::boost::bimap::relation::member_at::right,                 \
                  TP_SYMMETRIC & PARAMETER_NAME )                             \
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
    typename enable_if< mpl::not_< is_const<SymmetricType> >,                 \
                                                                              \
    typename result_of::NAME<Tag,SymmetricType>::type                         \
                                                                              \
    >::type NAME( SymmetricType & s )                                         \
    {                                                                         \
        typedef typename ::boost::bimap::relation::support::member_with_tag   \
        <                                                                     \
            Tag,SymmetricType                                                 \
                                                                              \
        >::type member_at_tag;                                                \
                                                                              \
        return detail::NAME(member_at_tag(),s);                               \
    }                                                                         \
                                                                              \
    template< class Tag, class SymmetricType >                                \
    typename enable_if< is_const<SymmetricType>,                              \
                                                                              \
    typename result_of::NAME<Tag,SymmetricType>::type                         \
                                                                              \
    >::type NAME( SymmetricType & s )                                         \
    {                                                                         \
        typedef typename ::boost::bimap::relation::support::member_with_tag   \
        <                                                                     \
            Tag,SymmetricType                                                 \
                                                                              \
        >::type member_at_tag;                                                \
                                                                              \
        return detail::NAME(member_at_tag(),s);                               \
    }

/*///////////////////////////////////////////////////////////////////////////*/


#endif // BOOST_BIMAP_RELATION_ACCESS_BUILDER_HPP

