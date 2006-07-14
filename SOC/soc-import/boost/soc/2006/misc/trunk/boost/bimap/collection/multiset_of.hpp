// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/multiset_of.hpp
/// \brief Include support for multiset constrains for the bimap container

#ifndef BOOST_BIMAP_COLLECTION_MULTISET_OF_HPP
#define BOOST_BIMAP_COLLECTION_MULTISET_OF_HPP

#include <functional>

#include <boost/bimap/collection/detail/is_key_type_of_builder.hpp>
#include <boost/bimap/collection/detail/register_key_type.hpp>
#include <boost/bimap/collection/detail/view_binder.hpp>
#include <boost/bimap/collection/detail/generate_relation_binder.hpp>
#include <boost/bimap/collection/key_type_of_tag.hpp>

#include <boost/multi_index/ordered_index.hpp>

#include <boost/bimap/views/multimap_view.hpp>
#include <boost/bimap/views/multiset_view.hpp>

namespace boost {
namespace bimap {
namespace collection {

/// \brief Set Type Specification
/**
This struct is used to specify a multiset specification.
It is not a container, it is just a metaprogramming facility to
express the type of a set. Generally, this specification will
be used in other place to create a container.
It has the same syntax that an std::set instantiation, except
that the allocator can not be specified. The rationale behind
this difference is that the allocator is not part of the set
type specification, rather it is a container configuration
parameter.
The first parameter is the type of the objects in the multiset,
and the second one is a Functor that compares them.
Bimap binding metafunctions can be used with this class in
the following way:

\code
using namespace support;

BOOST_STATIC_ASSERT( is_key_type_of< multiset_of<Type> >::value );

BOOST_STATIC_ASSERT
(
     is_same
     <
        compute_index_type
        <
            multiset_of<Type,KeyCompare>,
            KeyExtractor,
            Tag

        >::type
        ,
        ordered_nonunique< tag<Tag>, KeyExtractor, KeyCompare >

    >::value
);

typedef bimap
<
    multiset_of<Type>, RightKeyType

> bimap_with_left_type_as_multiset;

BOOST_STATIC_ASSERT
(
    is_same
    <
        compute_map_view_type
        <
            member_at::left,
            bimap_with_left_type_as_multiset

        >::type,
        multimap_view< member_at::left, bimap_with_left_type_as_multiset >

    >::value
);

\endcode

See also multiset_of_relation, is_multiset_of, compute_index_type,
compute_map_view_type, compute_set_type_view.
                                                                        **/

template
<
    class KeyType,
    class KeyCompare = std::less< KeyType >
>
struct multiset_of : public key_type_of_tag
{
     /// Type of the object that will be stored in the set
    typedef KeyType value_type;

    /// Functor that compare two keys
    typedef KeyCompare key_compare;
};


BOOST_BIMAP_IS_KEY_TYPE_OF_BUILDER_1CP
(
    // template< class Type > class
    is_multiset_of,
    // evaluates to true if type is a
    multiset_of
);


BOOST_BIMAP_REGISTER_KEY_TYPE_1CP
(
    support::is_multiset_of,  /* --------> */ multi_index::ordered_non_unique,
    KeyType,
    typename KeyType::key_comp
);


BOOST_BIMAP_COLLECTION_TO_MAP_VIEW_TYPE
(
    is_multiset_of, /* binds to */ views::multimap_view
);

BOOST_BIMAP_COLLECTION_TO_SET_VIEW_TYPE
(
    is_multiset_of, /* binds to */ views::multiset_view
);

/// \brief Set Of Relation Specification
/**
This struct is similar to multiset_of but it is bind logically to a
relation. It is used in the bimap instantiation to specify the
desired type of the main view. This struct implements internally
a metafunction named bind_to that manages the quite complicated
task of finding the right type of the set for the relation.

\code
template<class Relation,class DefaultKeyCompare = use_default >
struct bind_to
{
    typedef -UNDEFINED- type;
};
\endcode

See also multiset_of, is_set_type_of_relation.
                                                                **/

template< class KeyCompare = use_default >
struct multiset_of_relation : public set_type_of_relation_tag
{
    /// Functor that compare two keys
    typedef KeyCompare key_compare;

    /*
        template<class Relation,class DefaultKeyCompare = use_default >
        struct bind_to
        {
            typedef -UNDEFINED- type;
        }
    */
    BOOST_BIMAP_GENERATE_RELATION_BINDER_1CP(multiset_of,KeyCompare);
};

} // namespace collection
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_COLLECTION_MULTISET_OF_HPP
