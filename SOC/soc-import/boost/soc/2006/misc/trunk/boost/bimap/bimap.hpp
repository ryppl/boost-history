// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file bimap.hpp
/// \brief Includes the basic bimap container

/** \mainpage notitle
\n
\image html http://matias.capeletto.googlepages.com/boost.bimap.reference.logo.png

\section Introduction

This is the complete reference of Boost.Bimap.

After getting a good understanding of the library from a user perspective
the next step will be:

    - Understand the tagged idiom. (boost::bimap::tags)
    - Understand the internals of the relation class (boost::bimap::relation)
    - Read the container_adaptor toolbox docs (boost::bimap::container_adaptor)
    - Understand the internals of the bimap class. (boost::bimap, boost::bimap::views
      and boost::bimap::detail)


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

#ifdef BOOST_BIMAP_DISABLE_SERIALIZATION
    #define BOOST_MULTI_INDEX_DISABLE_SERIALIZATION
#endif

#include <boost/config.hpp>

#ifndef BOOST_BIMAP_DISABLE_SERIALIZATION

    #include <boost/serialization/nvp.hpp>

#endif // BOOST_BIMAP_DISABLE_SERIALIZATION

// Boost.Bimap
#include <boost/bimap/detail/bimap_core.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>
#include <boost/bimap/relation/support/data_extractor.hpp>


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
    class AP1 = ::boost::bimap::detail::not_specified,
    class AP2 = ::boost::bimap::detail::not_specified,
    class AP3 = ::boost::bimap::detail::not_specified
>
class bimap
:
    public ::boost::bimap::detail::bimap_core<KeyTypeA,KeyTypeB,AP1,AP2,AP3>,
    public ::boost::bimap::detail::bimap_core<KeyTypeA,KeyTypeB,AP1,AP2,AP3>::relation_set

{
    typedef typename ::boost::bimap::detail::bimap_core<KeyTypeA,KeyTypeB,AP1,AP2,AP3> base_;

    typename base_::core_type core;

    public:

    // metadata --------------------------------------------------------

    /*
    // The rest is computed in the core, because it is quite difficult to
    // expose a nice interface with so many metaprogramming stuff.
    // Here it is the complete metadat list.

    // Map by {side} metadata

    typedef -unspecified- {side}_tag;
    typedef -unspecified- {side}_data_type;
    typedef -unspecified- {side}_value_type;
    typedef -unspecified- {side}_key_type;
    typedef -unspecified- {side}_iterator;
    typedef -unspecified- {side}_const_iterator;

    ------------------------------------------------------------------*/

    #if defined(BOOST_MSVC)
    typename base_::left_set_type left_set_type;
    typedef typename left_set_type
    #else
    typedef typename base_::left_set_type
    #endif

        ::template map_view_bind
        <
            typename base_::left_tag, base_

        >::type left_map_type;


    #if defined(BOOST_MSVC)
    typename base_::right_set_type right_set_type;
    typedef typename right_set_type
    #else
    typedef typename base_::right_set_type
    #endif

        ::template map_view_bind
        <
            typename base_::right_tag, base_

        >::type right_map_type;


    /// Left map view
    left_map_type  left;

    /// Right map view
    right_map_type right;

    bimap() :

        base_::relation_set(::boost::multi_index::get<typename base_::logic_relation_set_tag >(core) ),

        left     (::boost::multi_index::get<typename base_::logic_left_tag  >(core)),
        right    (::boost::multi_index::get<typename base_::logic_right_tag >(core))

    {}

    template< class InputIterator >
    bimap(InputIterator first,InputIterator last) :

        base_::relation_set(::boost::multi_index::get<typename base_::logic_relation_set_tag  >(core) ),

        core(first,last),

        left     (::boost::multi_index::get<typename base_::logic_left_tag  >(core)),
        right    (::boost::multi_index::get<typename base_::logic_right_tag >(core))

    {}

    #ifndef BOOST_BIMAP_DISABLE_SERIALIZATION

    // Serialization support

    private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & serialization::make_nvp("mi_core",core);
    }

    #endif // BOOST_BIMAP_DISABLE_SERIALIZATION
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



// Include basic tools for user commodity
#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/support/map_by.hpp>
#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/support/get.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>

// Bring the most used namespaces directly to the user main namespace
namespace boost {
namespace bimap {

using namespace boost::bimap::support;

using boost::bimap::tags::tagged;

using namespace boost::bimap::relation::support;
using namespace boost::bimap::relation;

} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_BIMAP_HPP
