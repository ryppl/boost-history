// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file support/iterator_type_by.hpp
/// \brief Metafunctions to access the iterator types of a bimap

#ifndef BOOST_BIMAP_SUPPORT_ITERATOR_TYPE_BY_HPP
#define BOOST_BIMAP_SUPPORT_ITERATOR_TYPE_BY_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>
#include <boost/bimap/relation/detail/static_access_builder.hpp>

#include <boost/bimap/relation/support/pair_type_by.hpp>

#include <boost/bimap/detail/map_view_iterator.hpp>

/** \struct boost::bimap::support::iterator_type_by

\brief Metafunction to obtain the iterator type of the map view by one of the sides.

\code

template< class Tag, class Bimap >
struct iterator_type_by
{
    typedef -UNDEFINED- type;
};

template< class Tag, class Bimap >
struct const_iterator_type_by
{
    typedef -UNDEFINED- type;
};

template< class Tag, class Bimap >
struct reverse_iterator_type_by
{
    typedef -UNDEFINED- type;
};

template< class Tag, class Bimap >
struct const_reverse_iterator_type_by
{
    typedef -UNDEFINED- type;
};

template< class Tag, class Bimap >
struct local_iterator_type_by
{
    typedef -UNDEFINED- type;
};

template< class Tag, class Bimap >
struct const_local_iterator_type_by
{
    typedef -UNDEFINED- type;
};

\endcode

See also member_at.
\ingroup bimap_group
                                                                    **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace support {


// Implementation of iterator type by metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    iterator_type_by,
    left_iterator,
    right_iterator
);

// Implementation of const iterator type by metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    const_iterator_type_by,
    left_const_iterator,
    right_const_iterator
);


// Implementation of reverse iterator type by metafunction

BOOST_BIMAP_SYMMETRIC_STATIC_ACCESS_BUILDER
(
    core_reverse_iterator_type_by,
    BimapCore,

    typedef typename BimapCore::core_type::template index<typename BimapCore::left_tag>
        ::type::reverse_iterator type,

    typedef typename BimapCore::core_type::template index<typename BimapCore::right_tag>
        ::type::reverse_iterator type
);

template< class Tag, class BimapCore >
struct reverse_iterator_type_by
{
    typedef ::boost::bimap::detail::map_view_iterator
    <
        Tag,
        typename BimapCore::relation,
        typename core_reverse_iterator_type_by<Tag,BimapCore>::type,
        typename ::boost::bimap::relation::support::const_pair_reference_type_by<Tag,typename BimapCore::relation>::type,
        const typename ::boost::bimap::relation::support::pair_type_by<Tag,typename BimapCore::relation>::type

    > type;
};

// Implementation of const reverse iterator type by metafunction

BOOST_BIMAP_SYMMETRIC_STATIC_ACCESS_BUILDER
(
    core_const_reverse_iterator_type_by,
    BimapCore,

    typedef typename BimapCore::core_type::template index<typename BimapCore::left_tag>
        ::type::const_reverse_iterator type,

    typedef typename BimapCore::core_type::template index<typename BimapCore::right_tag>
        ::type::const_reverse_iterator type
);

template< class Tag, class BimapCore >
struct const_reverse_iterator_type_by
{

    typedef ::boost::bimap::detail::map_view_iterator
    <
        Tag,
        typename BimapCore::relation,
        typename core_const_reverse_iterator_type_by<Tag,BimapCore>::type,
        typename ::boost::bimap::relation::support::const_pair_reference_type_by<Tag,typename BimapCore::relation>::type,
        const typename ::boost::bimap::relation::support::pair_type_by<Tag,typename BimapCore::relation>::type

    > type;
};


// Implementation of local iterator type by metafunction

BOOST_BIMAP_SYMMETRIC_STATIC_ACCESS_BUILDER
(
    core_local_iterator_type_by,
    BimapCore,

    typedef typename BimapCore::core_type::template index<typename BimapCore::left_tag>
        ::type::local_iterator type,

    typedef typename BimapCore::core_type::template index<typename BimapCore::right_tag>
        ::type::local_iterator type
);


template< class Tag, class BimapCore >
struct local_iterator_type_by
{

    typedef ::boost::bimap::detail::map_view_iterator
    <
        Tag,
        typename BimapCore::relation,
        typename core_local_iterator_type_by<Tag,BimapCore>::type,
        typename ::boost::bimap::relation::support::const_pair_reference_type_by<Tag,typename BimapCore::relation>::type,
        const typename ::boost::bimap::relation::support::pair_type_by<Tag,typename BimapCore::relation>::type

    > type;
};


// Implementation of const local iterator type by metafunction

BOOST_BIMAP_SYMMETRIC_STATIC_ACCESS_BUILDER
(
    core_const_local_iterator_type_by,
    BimapCore,

    typedef typename BimapCore::core_type::template index<typename BimapCore::left_tag>
        ::type::const_local_iterator type,

    typedef typename BimapCore::core_type::template index<typename BimapCore::right_tag>
        ::type::const_local_iterator type
);

template< class Tag, class BimapCore >
struct const_local_iterator_type_by
{

    typedef ::boost::bimap::detail::map_view_iterator
    <
        Tag,
        typename BimapCore::relation,
        typename core_const_local_iterator_type_by<Tag,BimapCore>::type,
        typename ::boost::bimap::relation::support::const_pair_reference_type_by<Tag,typename BimapCore::relation>::type,
        const typename ::boost::bimap::relation::support::pair_type_by<Tag,typename BimapCore::relation>::type

    > type;
};


} // namespace support
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_SUPPORT_ITERATOR_TYPE_BY_HPP

