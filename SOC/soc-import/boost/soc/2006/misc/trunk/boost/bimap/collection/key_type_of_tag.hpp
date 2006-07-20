// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/key_type_of_tag.hpp
/// \brief Collection idiom tags and concepts

#ifndef BOOST_BIMAP_COLLECTION_KEY_TYPE_OF_TAG_HPP
#define BOOST_BIMAP_COLLECTION_KEY_TYPE_OF_TAG_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost {
namespace bimap {

/// \brief The set type of idiom is lives in this namespace

namespace collection {

/// \brief Tag of {SetType}_of definition classes
/**
The {SetType}_of classes are derived from this class so it is easy to construct
metafunctions. For example now is easy to create a is_set_type_of metafunction.
                                                                                **/

struct key_type_of_tag          {};

/// \brief Tag of {SetType}_of_relation defition classes

struct set_type_of_relation_tag {};

/** \struct boost::bimap::collection::use_default
    \brief Empty struct used to specify defaults parameters.
                                                                            **/

/** \struct boost::bimap::collection::left_based
    \brief Tag to indicate that the set of relation will be based on the left side.

This is convenient because the multi index core will be more efficient.
If possible use this options or the right based one.

See also right_based.
                                                                            **/

/** \struct boost::bimap::collection::right_based
    \brief Tag to indicate that the set of relation will be based on the right side.

This is convenient because the multi index core will be more efficient.
If possible use this options or the right based one.

See also left_based.
                                                                            **/


#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

struct use_default
{
    // TODO
    // I have tried lazy evaluation, with no results.
    // There have to be a way were this definition is not necesary:
    template< class T > struct apply { typedef void type; };
};

struct side_based_tag : set_type_of_relation_tag {};

struct left_based  : side_based_tag
{
    // TODO
    // I run into troubles if I do not define bind for side based tags.
    // Maybe a more coherent way of binding the relation can be developped.
    template< class Relation > struct bind_to { typedef void type; };
};

struct right_based : side_based_tag
{
    // TODO
    // I run into troubles if I do not define bind for side based tags.
    // Maybe a more coherent way of binding the relation can be developped.
    template< class Relation > struct bind_to { typedef void type; };
};

struct independent_index_tag    {};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

} // namespace collection
} // namespace bimap
} // namespace boost

/** \namespace boost::bimap::collection::support
\brief Metafunctions to help using the set of type idiom.
                                                                                **/

#endif // BOOST_BIMAP_COLLECTION_KEY_TYPE_OF_TAG_HPP

