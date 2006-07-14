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

/// \brief Empty struct used to specify defaults parameters.

struct use_default              {};

//@{
/// \brief The set of relations will be based on one of the sides.
/**
This is convenient because the multi index core will be more efficient.
If possible use this two options.
                                                                                **/

struct set_type_based_on_left   {};
struct set_type_based_on_right  {};

//@}

} // namespace collection
} // namespace bimap
} // namespace boost

/** \namespace boost::bimap::collection::support
\brief Metafunctions to help using the set of type idiom.
                                                                                **/

#endif // BOOST_BIMAP_COLLECTION_KEY_TYPE_OF_TAG_HPP

