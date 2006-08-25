// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file unordered_set_of.hpp
/// \brief Include support for unordered_set constrains for the bimap container

#ifndef BOOST_BIMAP_UNORDERED_SET_OF_HPP
#define BOOST_BIMAP_UNORDERED_SET_OF_HPP

#ifdef BOOST_BIMAP_DISABLE_SERIALIZATION
    #define BOOST_MULTI_INDEX_DISABLE_SERIALIZATION
#endif

#include <functional>
#include <boost/functional/hash.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/bimap/detail/concept_tags.hpp>

#include <boost/bimap/detail/generate_index_binder.hpp>
#include <boost/bimap/detail/generate_view_binder.hpp>
#include <boost/bimap/detail/generate_relation_binder.hpp>

#include <boost/multi_index/hashed_index.hpp>

#include <boost/bimap/views/unordered_map_view.hpp>
#include <boost/bimap/views/unordered_set_view.hpp>

namespace boost {
namespace bimap {

/// \brief Set Type Specification
/**
This struct is used to specify an unordered_set specification.
It is not a container, it is just a metaprogramming facility to
express the type of a set. Generally, this specification will
be used in other place to create a container.
It has the same syntax that an tr1::unordered_set instantiation,
except that the allocator cannot be specified. The rationale behind
this difference is that the allocator is not part of the
unordered_set type specification, rather it is a container
configuration parameter.
The first parameter is the type of the objects in the set, the
second one is a Hash Functor that takes objects of this type, and
the third one is a Functor that compares them for equality.
Bimap binding metafunctions can be used with this class in
the following way:

\code
using namespace support;

BOOST_STATIC_ASSERT( is_set_type_of< unordered_set_of<Type> >::value );

BOOST_STATIC_ASSERT
(
     is_same
     <
        unordered_set_of<Type,HashFunctor,EqualKey>::index_bind
        <
            KeyExtractor,
            Tag

        >::type,

        hashed_unique< tag<Tag>, KeyExtractor, HashFunctor, EqualKey >

    >::value
);

typedef bimap
<
    unordered_set_of<Type>, RightKeyType

> bimap_with_left_type_as_unordered_set;

BOOST_STATIC_ASSERT
(
    is_same
    <
        unordered_set_of<Type>::map_view_bind
        <
            member_at::left,
            bimap_with_left_type_as_unordered_set

        >::type,

        unordered_map_view
        <
            member_at::left,
            bimap_with_left_type_as_unordered_set
        >

    >::value
);

\endcode

See also unordered_set_of_relation.
                                                                        **/

template
<
    class KeyType,
    class HashFunctor   = hash< KeyType >,
    class EqualKey      = std::equal_to< KeyType >
>
struct unordered_set_of : public ::boost::bimap::detail::set_type_of_tag
{
    /// The type that will be stored in the container
    typedef KeyType         value_type;

    /// Hash Functor that takes value_type objects
    typedef HashFunctor     hasher;

    /// Functor that compare two value_type objects for equality
    typedef EqualKey        key_equal;


    BOOST_BIMAP_GENERATE_INDEX_BINDER_2CP(

        // binds to
        multi_index::hashed_unique,

        // with
        hasher,
        key_equal
    );

    BOOST_BIMAP_GENERATE_MAP_VIEW_BINDER(

        // binds to
        views::unordered_map_view
    );

    BOOST_BIMAP_GENERATE_SET_VIEW_BINDER(

        // binds to
        views::unordered_set_view
    );

};


/// \brief Set Of Relation Specification
/**
This struct is similar to unordered_set_of but it is bind logically to
a relation. It is used in the bimap instantiation to specify the
desired type of the main view. This struct implements internally
a metafunction named bind_to that manages the quite complicated
task of finding the right type of the set for the relation.

\code
template<class Relation>
struct bind_to
{
    typedef -unspecified- type;
};
\endcode

See also unordered_set_of, is_set_type_of_relation.
                                                                **/

template
<
    class HashFunctor   = hash< _relation >,
    class EqualKey      = std::equal_to< _relation >
>
struct unordered_set_of_relation : public ::boost::bimap::detail::set_type_of_relation_tag
{
    /// Hash Functor that takes value_type objects
    typedef HashFunctor     hasher;

    /// Functor that compare two value_type objects for equality
    typedef EqualKey        key_equal;


    BOOST_BIMAP_GENERATE_RELATION_BINDER_2CP(

        // binds to
        unordered_set_of,

        // with
        hasher,
        key_equal
    );
};


} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_UNORDERED_SET_OF_HPP

