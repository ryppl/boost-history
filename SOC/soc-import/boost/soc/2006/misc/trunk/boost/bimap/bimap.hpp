// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file bimap.hpp
/// \brief Includes the basic bimap container

/** \mainpage notitle
\n
\image html http://matias.capeletto.googlepages.com/boost.bimap.reference.logo.png

\section Introduction

This is the complete reference of Boost.Bimap.

2
\f[
    \mathcal{O}(I(n))
\f]

3
\f[
    \mathcal{O}(H(n))
\f]

4
\f[
    \mathcal{O}(m*H(n+m))
\f]

5
\f[
    \mathcal{O}(D(n))
\f]

6
\f[
    \mathcal{O}(log(n) + m*D(n))
\f]

7
\f[
    \mathcal{O}(R(n))
\f]

8
\f[
    \mathcal{O}(M(n))
\f]

9
\f[
    \mathcal{O}(log(n))
\f]

10
\f[
    \mathcal{O}(log(n)+count(x))
\f]

11
\f[
    \mathcal{O}(log(n) + count(x,comp))
\f]



\section GuideForTheUser Guide for the user

From a user point of view the best way to use this reference is to:

    - Understand the relation class. (boost::bimap::relation)
    - Read the bimap class documentation. (boost::bimap)
    - If user tags will be used, it is not necesary but it may be good to
      skim the tagged reference. (boost::bimap::tags)

\section GuideForTheDevelopper Guide for the developper

From a developer point of view all the information in this reference is
important. After getting a good understanding of the library from a user
perspective the next step will be:

    - Understand the mutant idiom. (boost::bimap::mutant)
    - Understand the tagged idiom. (boost::bimap::tags)
    - Understand the internals of the relation class (boost::bimap::relation)
    - Read the container_adaptor toolbox docs (boost::bimap::container_adaptor)
    - Understand the internals of the bimap class. (boost::bimap and boost::bimap::detail)


                                                                        **/

/** \defgroup mutant_group mutant idiom
\brief A safe wrapper around reinterpret_cast
                                                                        **/

/** \defgroup relation_group relation
\brief The relation
                                                                        **/

/** \defgroup tags_group tagged idiom
\brief The tagged idiom
                                                                        **/


#ifndef BOOST_BIMAP_BIMAP_HPP
#define BOOST_BIMAP_BIMAP_HPP

// Boost.Bimap
#include <boost/bimap/detail/bimap_core.hpp>

// Include basic tools for user commodity
#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/support/map_by.hpp>
#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/support/get.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>

// Bring the most used namespaces directly to the user main namespace
namespace boost {
namespace bimap {

using namespace boost::bimap::collection;
using namespace boost::bimap::support;
using namespace boost::bimap::tags;
using namespace boost::bimap::relation::support;
using namespace boost::bimap::relation;

} // namespace bimap
} // namespace boost

/// \brief The namespace where all the boost libraries lives.

namespace boost {

/// \brief Boost.Bimap library namespace
/**
All the entities in the library are defined in this namespace.
                                                                    **/
namespace bimap {

/// \brief The bimap class is the entry point to the library.
/**
This class manages the instantiation of the desired bimap type.
As there are several types of bidirectional maps that can be
created using it, the main job of it is to find the desired
type. This is done using metaprogramming to obtain the relation
type that will be stored, the map_view type of each side and
the set_view type of the general relationship. The instantiation
is kept simple using an extended standard set theory, where a
bidirectional map type is defined by the set types it relates.
For example, a bidirectional map that has multimap semantics
viewed from both sides is defined by specifying that the two
keys sets are of \c multiset_of<Key> type.
This allows the bimap class to support seamingless N-N, 1-N,
ordered/unordered and even vector-list types of mapping.
The three last parameters are used to specify the set type of
the relation, an inplace hooked data class and the allocator
type. As a help to the bimap user, these parameters support
default types but use a special idiom that allow them to be
specified without interleaving the usual use_default keyword.
The possible bimap instantiation are enumerated here:
\c {Side}KeyType can be directly a type, this is default to
\c set_of<{Side}KeyType>, or can be a \c {SetType}_of<Type>
specification. Additionally this two parameters can be tagged
to specify others tags instead of the usual \c member_at::{Side}
ones.

\code bimap<LeftKeyType,RightKeyType> \endcode
       - \b set_of_relation_type: based on the left key type
       - \b hook_data:            no additional data
       - \b allocator:            default allocator

\code bimap<LeftKeyType,RightKeyType> \endcode
       - \b set_of_relation_type: based on the left key type
       - \b hook_data:            no additional data
       - \b allocator:            \c Allocator

\code bimap<LeftKeyType,RightKeyType,SetOfRelationType> \endcode
       - \b set_of_relation_type: \c SetOfRelationType
       - \b hook_data:            no additional data
       - \b allocator:            default allocator

\code bimap<LeftKeyType,RightKeyType,SetOfRelationType,Allocator> \endcode
       - \b set_of_relation_type: \c SetOfRelationType
       - \b hook_data:            no additional data
       - \b allocator:            \c Allocator

\code bimap<LeftKeyType,RightKeyType,DataToHook> \endcode
       - \b set_of_relation_type: based on the left key type
       - \b hook_data:            \c DataToHook
       - \b allocator:            default allocator

\code bimap<LeftKeyType,RightKeyType,DataToHook,Allocator> \endcode
       - \b set_type_of_relation: based on the left key type
       - \b hook_data:            \c DataToHook
       - \b allocator:            \c Allocator

\code bimap<LeftKeyType,RightKeyType,SetOfRelationType,DataToHook> \endcode
       - \b set_of_relation_type: \c SetOfRelationType
       - \b hook_data:            \c DataToHook
       - \b allocator:            default allocator

\code bimap<LeftKeyType,RightKeyType,SetOfRelationType,DataToHook,Allocator> \endcode
       - \b set_of_relation_type: \c SetOfRelationType
       - \b hook_data:            \c DataToHook
       - \b allocator:            \c Allocator

                                                                            **/

template
<
    class KeyTypeA, class KeyTypeB,
    class AP1 = bimap::detail::not_specified,
    class AP2 = bimap::detail::not_specified,
    class AP3 = bimap::detail::not_specified
>
class bimap
:
    public bimap::detail::bimap_core<KeyTypeA,KeyTypeB,AP1,AP2,AP3>,
    public collection::support::compute_set_view_type
    <
        typename bimap::detail::bimap_core<KeyTypeA,KeyTypeB,AP1,AP2,AP3>
            ::set_type_of,

        typename bimap::detail::bimap_core<KeyTypeA,KeyTypeB,AP1,AP2,AP3>
            ::relation_index

    >::type
{
    private:

    typename bimap::core_type core;

    public:

    // metadata --------------------------------------------------------

    typedef typename collection::support::compute_set_view_type
    <
        typename bimap::set_type_of,
        typename bimap::relation_index

    >::type set_type;

    /*
    // The rest is computed in the core, because it is quite difficult to
    // expose a nice interface with so many metaprogramming stuff.
    // Here it is the complete metadat list.

    // Map by {side} metadata

    typedef -undefined- {side}_tag;
    typedef -undefined- {side}_data_type;
    typedef -undefined- {side}_value_type;
    typedef -undefined- {side}_key_type;
    typedef -undefined- {side}_iterator;
    typedef -undefined- {side}_const_iterator;
    typedef -undefined- {side}_reverse_iterator;
    typedef -undefined- {side}_const_reverse_iterator;

    ------------------------------------------------------------------*/

    typedef typename collection::support::compute_map_view_type
    <
        typename bimap::left_tag, typename bimap::bimap_core_

    >::type left_map_type;

    typedef typename collection::support::compute_map_view_type
    <
        typename bimap::right_tag, typename bimap::bimap_core_

    >::type right_map_type;

    /// Left map view
    typename bimap::left_map_type  left;

    /// Right map view
    typename bimap::right_map_type right;

    bimap() :

        set_type (core.get<typename bimap::relation_set_tag   >()),
        left     (core.get<typename bimap::left_tag           >()),
        right    (core.get<typename bimap::right_tag          >())

    {}
};

} // namespace bimap
} // namespace boost


/** \namespace boost::bimap::support
\brief Metafunctions to help working with bimaps.
                                                            **/

/** \namespace boost::bimap::views
\brief Bimap views.
                                                            **/

/** \namespace boost::bimap::views::detail
\brief Bimap views details.
                                                            **/


#endif // BOOST_BIMAP_BIMAP_HPP

