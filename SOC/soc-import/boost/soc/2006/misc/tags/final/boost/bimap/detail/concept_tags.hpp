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

/// \file detail/concept_tags.hpp
/// \brief Bimap tags and concepts

#ifndef BOOST_BIMAP_DETAIL_CONCEPT_TAGS_HPP
#define BOOST_BIMAP_DETAIL_CONCEPT_TAGS_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost {
namespace bimap {
namespace detail {

/// \brief Tag of {SetType}_of definition classes
/**
The {SetType}_of classes are derived from this class so it is easy to construct
metafunctions. For example now is easy to create a is_set_type_of metafunction.
                                                                                **/

struct set_type_of_tag          {};

/// \brief Tag of {SetType}_of_relation defition classes

struct set_type_of_relation_tag {};

/// \brief Tag of {Side}_based identifiers

struct side_based_tag : set_type_of_relation_tag {};

} // namespace detail


/** \struct boost::bimap::left_based
    \brief Tag to indicate that the set of relation will be based on the left side.

This is convenient because the multi-index core will be more efficient.
If possible use this options or the right based one.

See also right_based.
                                                                            **/

/** \struct boost::bimap::right_based
    \brief Tag to indicate that the set of relation will be based on the right side.

This is convenient because the multi-index core will be more efficient.
If possible use this options or the right based one.

See also left_based.
                                                                            **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

struct left_based : ::boost::bimap::detail::side_based_tag
{
    // TODO
    // I run into troubles if I do not define bind for side based tags.
    // Maybe a more coherent way of binding the relation can be developped.
    template< class Relation > struct bind_to { typedef void type; };
};

struct right_based : ::boost::bimap::detail::side_based_tag
{
    // TODO
    // I run into troubles if I do not define bind for side based tags.
    // Maybe a more coherent way of binding the relation can be developped.
    template< class Relation > struct bind_to { typedef void type; };
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

typedef mpl::_ _relation;

} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_CONCEPT_TAGS_HPP

