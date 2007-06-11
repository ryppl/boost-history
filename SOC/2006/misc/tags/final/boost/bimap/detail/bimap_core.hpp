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

/// \file detail/bimap_core.hpp
/// \brief Bimap base definition.

#ifndef BOOST_BIMAP_DETAIL_BIMAP_CORE_HPP
#define BOOST_BIMAP_DETAIL_BIMAP_CORE_HPP

// Boost.MPL
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/list.hpp>

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
#include <boost/bimap/detail/map_view_iterator.hpp>

#include <boost/bimap/set_of.hpp>
#include <boost/bimap/unconstrained_set_of.hpp>

namespace boost {
namespace bimap {

/// \brief Library details

namespace detail {

// In detail, we are free to import same namespaces to augment the
// readability of the code. No one is supposed to use this namespace.

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class Type >
struct get_value_type
{
    typedef typename Type::value_type type;
};

struct independent_index_tag {};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES


/// \brief Base for the bimap class.
/**


See also bimap.
                                                                        **/


template< class LeftSetType, class RightSetType, class AP1, class AP2, class AP3 >
struct bimap_core
{
    // Manage bimap key instantiation
    // --------------------------------------------------------------------
    private:

    typedef typename manage_bimap_key
    <
        LeftSetType,
        ::boost::bimap::relation::member_at::left

    >::type left_tagged_set_type;

    typedef typename manage_bimap_key
    <
        RightSetType,
        ::boost::bimap::relation::member_at::right

    >::type right_tagged_set_type;


    // Construct the relation type to be used
    // --------------------------------------------------------------------
    public:

    /// \brief Relation type stored by the bimap.

    typedef typename ::boost::bimap::relation::select_relation
    <
        typename ::boost::bimap::tags::support::apply_to_value_type
        <
            get_value_type< mpl::_>,
            left_tagged_set_type

        >::type,

        typename ::boost::bimap::tags::support::apply_to_value_type
        <
            get_value_type< mpl::_ >,
            right_tagged_set_type

        >::type

    >::type relation;

    //@{

        typedef typename relation::left_tag  left_tag;
        typedef typename relation::right_tag right_tag;

    //@}

    //@{

        typedef typename relation::left_value_type     left_key_type;
        typedef typename relation::right_value_type   right_key_type;

    //@}

    //@{

        typedef typename relation::right_value_type    left_data_type;
        typedef typename relation:: left_value_type   right_data_type;

    //@}

    //@{

        typedef typename relation::left_pair  left_value_type;
        typedef typename relation::right_pair right_value_type;

    //@}

    //@{

        typedef typename  left_tagged_set_type::value_type  left_set_type;
        typedef typename right_tagged_set_type::value_type right_set_type;

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
        left_key_type,
        &relation::left

    > left_member_extractor;

    typedef multi_index::member
    <
        relation,
        right_key_type,
        &relation::right

    > right_member_extractor;

    // The core indices are somewhat complicated to calculate, because they
    // can be zero, one, two or three indices, depending on the use of
    // {side}_based set type of relations and unconstrained_set_of and
    // unconstrained_set_of_relation specifications.

    typedef typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< left_set_type >,
    // {
            mpl::list<>,
    // }
    // else
    // {
            mpl::list
            <
                typename left_set_type::template index_bind
                <
                    left_member_extractor,
                    left_tag

                >::type
            >
    // }
    >::type left_core_indices;

    typedef typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< right_set_type >,
    // {
            left_core_indices,
    // }
    // else
    // {
            typename mpl::push_front
            <
                left_core_indices,

                typename right_set_type::template index_bind
                <
                    right_member_extractor,
                    right_tag

                >::type

            >::type
    // }
    >::type basic_core_indices;

    // If it is based either on the left or on the right, then only the side indices
    // are needed. But the set type of the relation can be completely diferent from
    // the onew used for the sides in wich case we have to add yet another index to
    // the core.

    // TODO
    // If all the set types are unsconstrained there must be readable compile time error.

    typedef typename mpl::if_<

        is_same< typename parameters::set_type_of_relation, left_based >,
    // {
            left_tagged_set_type,
    // }
    /* else */ typename mpl::if_<
            is_same< typename parameters::set_type_of_relation, right_based >,
    // {
            right_tagged_set_type,
    // }
    // else
    // {
            tags::tagged
            <
                typename parameters::set_type_of_relation::template bind_to
                <
                    relation

                >::type,
                independent_index_tag
            >
    // }
    >::type
    >::type tagged_set_of_relation_type;

    protected:

    typedef typename tagged_set_of_relation_type::tag           relation_set_tag;
    typedef typename tagged_set_of_relation_type::value_type    relation_set_type_of;

    // Logic tags
    // This is a necesary extra level of indirection to allow unconstrained sets to
    // be plug in the design. The bimap constructors use this logic tags.

    typedef typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< left_set_type >,
        typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< right_set_type >,
            independent_index_tag,
            right_tag
        >::type,
        left_tag

    >::type logic_left_tag;

    typedef typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< right_set_type >,
        typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< left_set_type >,
            independent_index_tag,
            left_tag
        >::type,
        right_tag

    >::type logic_right_tag;

    typedef typename mpl::if_< is_same< relation_set_tag, independent_index_tag >,
        typename mpl::if_< ::boost::bimap::detail::is_unconstrained_set_of< relation_set_type_of >,
            logic_left_tag,
            independent_index_tag
        >::type,
        typename mpl::if_< is_same< typename parameters::set_type_of_relation, left_based >,
            logic_left_tag,
            logic_right_tag
        >::type

    >::type logic_relation_set_tag;

    private:

    typedef typename mpl::if_<
        mpl::and_< is_same< relation_set_tag, independent_index_tag >,
                   mpl::not_< ::boost::bimap::detail::is_unconstrained_set_of< relation_set_type_of > >
        >,
    // {
            typename mpl::push_front
            <
                basic_core_indices,

                typename relation_set_type_of::template index_bind
                <
                    multi_index::identity<relation>,
                    independent_index_tag

                >::type

            >::type,
    // }
    // else
    // {
            basic_core_indices
    // }

    >::type complete_core_indices;

    struct core_indices : public complete_core_indices {};

    // Define the core using compute_index_type to translate the
    // set type to an multi-index specification
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

    typedef typename ::boost::multi_index::index<core_type, logic_left_tag>::type  left_index;
    typedef typename ::boost::multi_index::index<core_type,logic_right_tag>::type right_index;

    typedef typename left_index::iterator       left_core_iterator;
    typedef typename left_index::const_iterator left_core_const_iterator;

    typedef typename right_index::iterator       right_core_iterator;
    typedef typename right_index::const_iterator right_core_const_iterator;

    // Map by {side} iterator metadata
    // --------------------------------------------------------------------
    public:

    //@{

        typedef ::boost::bimap::detail::map_view_iterator
        <
            left_tag,
            relation,
            left_core_iterator,
            typename relation::const_left_pair_reference,
            const left_value_type

        > left_iterator;

        typedef ::boost::bimap::detail::map_view_iterator
        <
            right_tag,
            relation,
            right_core_iterator,
            typename relation::const_right_pair_reference,
            const right_value_type

        > right_iterator;

    //@}

    //@{

        typedef ::boost::bimap::detail::map_view_iterator
        <
            left_tag,
            relation,
            left_core_const_iterator,
            typename relation::const_left_pair_reference,
            const left_value_type

        > left_const_iterator;

        typedef ::boost::bimap::detail::map_view_iterator
        <
            right_tag,
            relation,
            right_core_const_iterator,
            typename relation::const_right_pair_reference,
            const right_value_type

        > right_const_iterator;

    //@}

    // Relation set view

    typedef typename relation_set_type_of::template set_view_bind
    <
        typename ::boost::multi_index::index<core_type, logic_relation_set_tag >::type

    >::type relation_set;

    public:

    typedef bimap_core bimap_core_;
};

} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_BIMAP_CORE_HPP
