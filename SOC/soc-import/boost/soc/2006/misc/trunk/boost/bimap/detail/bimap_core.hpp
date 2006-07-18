// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file detail/bimap_core.hpp
/// \brief Bimap base definition.

#ifndef BOOST_BIMAP_DETAIL_BIMAP_CORE_HPP
#define BOOST_BIMAP_DETAIL_BIMAP_CORE_HPP

// Boost.MPL
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_same.hpp>


// Boost.MultiIndex
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>

// Boost.Bimap.Relation
#include <boost/bimap/relation/relation.hpp>
#include <boost/bimap/relation/member_at.hpp>

// Boost.Bimap.Tags
#include <boost/bimap/tags/support/apply_to_value_type.hpp>

// Boost.Bimap
#include <boost/bimap/detail/manage_bimap_key.hpp>
#include <boost/bimap/detail/manage_additional_parameters.hpp>
#include <boost/bimap/detail/get_value_type.hpp>
#include <boost/bimap/views/detail/map_view_iterator.hpp>
#include <boost/bimap/views/detail/set_view_iterator.hpp>

#include <boost/bimap/collection/set_of.hpp>

namespace boost {
namespace bimap {

/// \brief Library details

namespace detail {

// In detail, we are free to import same namespaces to augment the
// readability of the code. No one is supposed to use this namespace.

using namespace mpl::placeholders;

/// \brief Base for the bimap class.
/**


See also bimap.
                                                                        **/


template< class LeftKeyType, class RightKeyType, class AP1, class AP2, class AP3 >
struct bimap_core
{
    // Manage bimap key instantiation
    // --------------------------------------------------------------------
    private:

    typedef typename manage_bimap_key
    <
        LeftKeyType,
        relation::member_at::left

    >::type left_tagged_key_type;

    typedef typename manage_bimap_key
    <
        RightKeyType,
        relation::member_at::right

    >::type right_tagged_key_type;


    // Construct the relation type to be used
    // --------------------------------------------------------------------
    public:

    /// \brief Relation type stored by the bimap.

    typedef typename relation::select_relation
    <
        typename tags::support::apply_to_value_type
        <
            get_value_type<_>,
            left_tagged_key_type

        >::type,

        typename tags::support::apply_to_value_type
        <
            get_value_type<_>,
            right_tagged_key_type

        >::type

    >::type relation;

    //@{

        typedef typename relation::left_tag  left_tag;
        typedef typename relation::right_tag right_tag;

    //@}

    //@{

        typedef typename relation::left_value_type    left_data_type;
        typedef typename relation::right_value_type   right_data_type;

    //@}

    //@{

        typedef typename relation::left_pair  left_value_type;
        typedef typename relation::right_pair right_value_type;

    //@}

    //@{

        typedef typename left_tagged_key_type::value_type  left_key_type;
        typedef typename right_tagged_key_type::value_type right_key_type;

    //@}

    // Manage the additional parameters
    // --------------------------------------------------------------------
    private:

    typedef typename manage_additional_parameters<AP1,AP2,AP3>::type parameters;

    // Bind the member of the relation, so multi_index can manage them
    // --------------------------------------------------------------------
    private:

    typedef multi_index::member
    <
        relation,
        left_data_type,
        &relation::left

    > left_member_extractor;

    typedef multi_index::member
    <
        relation,
        right_data_type,
        &relation::right

    > right_member_extractor;

    // Use type hiding to get better symbol names

    struct basic_core_indices :

        multi_index::indexed_by
        <

            typename collection::support::compute_index_type
            <
                left_key_type,
                left_member_extractor,
                left_tag

            >::type,

            typename collection::support::compute_index_type
            <
                right_key_type,
                right_member_extractor,
                right_tag

            >::type
        >

    {};

    // The multi index core can have two or three indices depending on the set
    // type of the relation. If it is based either on the left or on the right,
    // then only two indices are needed. But the set type of the relation
    // can be completely diferent from the onew used for the sides in wich case
    // we have to add yet another index to the core.

    typedef typename mpl::if_<
        is_same< typename parameters::set_type_of_relation, collection::left_based >,
    // {
            left_tagged_key_type,
    // }
    /* else */ typename mpl::if_<
            is_same< typename parameters::set_type_of_relation, collection::right_based >,
    // {
            right_tagged_key_type,
    // }
    // else
    // {
            tags::tagged
            <
                typename parameters::set_type_of_relation::template bind_to
                <
                    relation

                >::type,
                collection::independent_index_tag
            >
    // }
        >::type

    >::type tagged_set_of_relation_type;

    protected:

    typedef typename tagged_set_of_relation_type::tag relation_set_tag;

    private:

    typedef typename mpl::if_<

        is_same< relation_set_tag, collection::independent_index_tag >,
    // {
            typename mpl::push_front
            <
                basic_core_indices,

                typename collection::support::compute_index_type
                <
                    typename tagged_set_of_relation_type::value_type,
                    multi_index::identity<relation>,
                    collection::independent_index_tag

                >::type

            >::type,
    // }
    // else
    // {
            basic_core_indices
    // }

    >::type core_indices;

    // Define the core using compute_index_type to translate the
    // set type to an multi index specification
    // --------------------------------------------------------------------
    public:

    typedef multi_index::multi_index_container
    <
        relation,
        core_indices,
        typename parameters::allocator::template rebind<relation>::other

    > core_type;

    // Core metadata
    // --------------------------------------------------------------------
    public:

    typedef typename core_type::template index<left_tag >::type left_index;
    typedef typename core_type::template index<right_tag>::type right_index;

    typedef typename left_index::iterator       left_core_iterator;
    typedef typename left_index::const_iterator left_core_const_iterator;

    typedef typename right_index::iterator       right_core_iterator;
    typedef typename right_index::const_iterator right_core_const_iterator;

    // Map by {side} iterator metadata
    // --------------------------------------------------------------------
    public:

    //@{

        typedef typename views::detail::map_view_iterator
        <
            left_tag,
            relation,
            left_core_iterator,
            const left_value_type // Due to the multiIndex core the "const" is forced

        >::type left_iterator;

        typedef typename views::detail::map_view_iterator
        <
            right_tag,
            relation,
            right_core_iterator,
            const right_value_type // Due to the multiIndex core the "const" is forced

        >::type right_iterator;

    //@}

    //@{

        typedef typename views::detail::map_view_iterator
        <
            left_tag,
            relation,
            left_core_const_iterator,
            const left_value_type

        >::type left_const_iterator;

        typedef typename views::detail::map_view_iterator
        <
            right_tag,
            relation,
            right_core_const_iterator,
            const right_value_type

        >::type right_const_iterator;

    //@}

    // Relation set view

    typedef typename collection::support::compute_set_view_type
    <
        typename tagged_set_of_relation_type::value_type,
        typename core_type::template index< relation_set_tag >::type

    >::type relation_set;

    public:

    typedef bimap_core bimap_core_;
};

} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_BIMAP_CORE_HPP
