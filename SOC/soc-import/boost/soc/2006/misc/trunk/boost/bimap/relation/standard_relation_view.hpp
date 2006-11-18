// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/standard_relation.hpp
/// \brief Includes the relation class

#ifndef BOOST_BIMAP_RELATION_STANDARD_RELATION_VIEW_HPP
#define BOOST_BIMAP_RELATION_STANDARD_RELATION_VIEW_HPP

#include <boost/bimap/relation/standard_relation_fwd.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/serialization/nvp.hpp>

// Boost.Bimap
#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/tags/support/default_tagged.hpp>
#include <boost/bimap/tags/support/tag_of.hpp>
#include <boost/bimap/tags/support/value_type_of.hpp>

#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>
#include <boost/bimap/relation/standard_pair_view.hpp>
#include <boost/bimap/relation/structured_pair.hpp>

#include <boost/bimap/relation/detail/totally_ordered_pair.hpp>


namespace boost {
namespace bimap {
namespace relation {

template< class TA, class TB >
class const_standard_relation_view;

/** \brief Standard relation above view.

See also standard_relation.
                                                           **/

template< class TA, class TB >
class standard_relation_view  :

    public symmetrical_base<TA,TB>
{

    typedef symmetrical_base<TA,TB> base_;

    public:

    //@{
        /// A signature compatible std::pair that is a view of the relation.

        typedef structured_pair<TA,TB,normal_layout> left_pair ;
        typedef structured_pair<TB,TA,mirror_layout> right_pair;

        typedef standard_pair_view<TA,TB,false,normal_layout> left_pair_reference ;
        typedef standard_pair_view<TB,TA,false,mirror_layout> right_pair_reference;

        typedef standard_pair_view<TA,TB,true,normal_layout> const_left_pair_reference ;
        typedef standard_pair_view<TB,TA,true,mirror_layout> const_right_pair_reference;

    //@}

    //@{

        /// data, exposed for easy manipulation
        typename base_:: left_value_type &  left;
        typename base_::right_value_type & right;

    //@}

    private:

    typedef standard_relation<TA,TB,true > relation_force_mutable;
    typedef standard_relation<TA,TB,false> relation_not_force_mutable;

    public:

    explicit standard_relation_view(relation_force_mutable & rel) :

        left (rel.left),
        right(rel.right)
    {}

    explicit standard_relation_view(relation_not_force_mutable & rel) :

        left (rel.left),
        right(rel.right)
    {}

    operator const relation_not_force_mutable ()
    {
        return relation_not_force_mutable(
            left, right
        );
    }

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        relation_force_mutable,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        relation_not_force_mutable,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        left_pair,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        right_pair,
        second,first
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        left_pair_reference,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        right_pair_reference,
        second,first
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        const_left_pair_reference,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        const_right_pair_reference,
        second,first
    );

    // The following functions are redundant if you only consider this class.
    // They are included to make easier the construction of the get and the
    // pair_by metafunction. Remember that not all compiler supports the mutant
    // idiom.

    typename base_::left_value_type & get_left()
    {
        return left;
    }

    const typename base_::left_value_type & get_left() const
    {
        return left;
    }

    typename base_::right_value_type & get_right()
    {
        return right;
    }

    const typename base_::right_value_type & get_right() const
    {
        return right;
    }

    left_pair_reference get_left_pair()
    {
        return left_pair_reference(*this);
    }
    const_left_pair_reference get_left_pair() const
    {
        return const_left_pair_reference(*this);
    }

    right_pair_reference get_right_pair()
    {
        return right_pair_reference(*this);
    }

    const_right_pair_reference get_right_pair() const
    {
        return const_right_pair_reference(*this);
    }
};

/** \brief Const Standard relation above view.

See also standard_relation_view, standard_relation.
                                                           **/

template< class TA, class TB >
class const_standard_relation_view  :

    public symmetrical_base<TA,TB>
{

    typedef symmetrical_base<TA,TB> base_;

    public:

    //@{
        /// A signature compatible std::pair that is a view of the relation.

        typedef structured_pair<TA,TB,normal_layout> left_pair ;
        typedef structured_pair<TB,TA,mirror_layout> right_pair;

        typedef standard_pair_view<TA,TB,false,normal_layout> left_pair_reference ;
        typedef standard_pair_view<TB,TA,false,mirror_layout> right_pair_reference;

        typedef standard_pair_view<TA,TB,true,normal_layout> const_left_pair_reference ;
        typedef standard_pair_view<TB,TA,true,mirror_layout> const_right_pair_reference;

    //@}

    //@{

        /// data, exposed for easy manipulation
        const typename base_:: left_value_type &  left;
        const typename base_::right_value_type & right;

    //@}

    private:

    typedef standard_relation<TA,TB,true > relation_force_mutable;
    typedef standard_relation<TA,TB,false> relation_not_force_mutable;
    typedef standard_relation_view<TA,TB> relation_view;

    public:

    explicit const_standard_relation_view(const relation_force_mutable & rel) :

        left (rel.left),
        right(rel.right)
    {}

    explicit const_standard_relation_view(const relation_not_force_mutable & rel) :

        left (rel.left),
        right(rel.right)
    {}

    const_standard_relation_view(const relation_view & rel) :

        left (rel.left),
        right(rel.right)
    {}

    operator const relation_not_force_mutable ()
    {
        return relation_not_force_mutable(
            left, right
        );
    }

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        const_standard_relation_view,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        relation_view,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        relation_force_mutable,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        relation_not_force_mutable,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        left_pair,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        right_pair,
        second,first
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        left_pair_reference,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        right_pair_reference,
        second,first
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        const_left_pair_reference,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        const_right_pair_reference,
        second,first
    );

    // The following functions are redundant if you only consider this class.
    // They are included to make easier the construction of the get and the
    // pair_by metafunction. Remember that not all compiler supports the mutant
    // idiom.

    const typename base_::left_value_type & get_left() const
    {
        return left;
    }

    const typename base_::right_value_type & get_right() const
    {
        return right;
    }

    const_left_pair_reference get_left_pair() const
    {
        return const_left_pair_reference(*this);
    }

    const_right_pair_reference get_right_pair() const
    {
        return const_right_pair_reference(*this);
    }
};

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace support
{

template< class Type >
struct is_standard_relation_view :
    ::boost::mpl::false_ {};

template< class TA, class TB >
struct is_standard_relation_view< standard_relation_view<TA,TB> > :
    ::boost::mpl::true_ {};

} // namespace support

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_STANDARD_RELATION_VIEW_HPP


