// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/standard_relation.hpp
/// \brief Includes the relation class

#ifndef BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP
#define BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP

#include <boost/bimap/relation/standard_relation_fwd.hpp>

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
#include <boost/bimap/relation/standard_relation_view.hpp>
#include <boost/bimap/relation/structured_pair.hpp>

#include <boost/bimap/relation/detail/totally_ordered_pair.hpp>



namespace boost {
namespace bimap {
namespace relation {

/// \brief Abstraction of a related pair of values, that extends the std::pair class.
/**

This is a standard compliant suboptimal relation class.

See also standard_relation, mutant_relation.
\ingroup relation_group
                                                                           **/

template< class TA, class TB, bool force_mutable >
class standard_relation
{
    public:

    typedef typename tags::support::default_tagged
    <
        TA,
        member_at::left

    >::type tagged_left_type;

    typedef typename tags::support::default_tagged
    <
        TB,
        member_at::right

    >::type tagged_right_type;

    public:

    typedef       standard_relation<TA,TB>            above_view;
    typedef const standard_relation<TA,TB>      const_above_view;

    typedef       standard_relation_view<TA,TB>       above_view_reference;
    typedef const_standard_relation_view<TA,TB> const_above_view_reference;

    //@{

        /// The type stored in the relation

        typedef typename ::boost::mpl::if_c< force_mutable,

            typename ::boost::remove_const< typename tagged_left_type::value_type >::type,
            typename tagged_left_type::value_type

        >::type left_value_type;

        typedef typename ::boost::mpl::if_c< force_mutable,

            typename ::boost::remove_const< typename tagged_right_type::value_type >::type,
            typename tagged_right_type::value_type

        >::type right_value_type;

    //@}

    //@{

        /// The tag of the member. By default it is \c member_at::{side}
        typedef typename  tagged_left_type::tag  left_tag;
        typedef typename tagged_right_type::tag right_tag;

    //@}

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
         left_value_type  left;
        right_value_type right;

    //@}

    private:

    typedef standard_relation<TA,TB,true > relation_force_mutable;
    typedef standard_relation<TA,TB,false> relation_not_force_mutable;

    public:

    standard_relation() {}

    standard_relation(typename ::boost::call_traits<
                           left_value_type>::param_type l,
                      typename ::boost::call_traits<
                          right_value_type>::param_type r) :
        left (l),
        right(r)
    {}

    standard_relation(const relation_force_mutable & rel) :

        left (rel.left),
        right(rel.right)
    {}

    standard_relation(const relation_not_force_mutable & rel) :

        left (rel.left),
        right(rel.right)
    {}

    // Allow to create relations from views

    explicit standard_relation(const left_pair & lp) :

        left ( lp.first  ),
        right( lp.second )
    {}

    explicit standard_relation(const right_pair & rp) :

        left ( rp.second ),
        right( rp.first  )
    {}

    // Allow to create a relation from a std pair
    // This allows to better integration with the stl

    typedef std::pair
    <
         left_value_type,
        right_value_type

    > std_pair;

    explicit standard_relation(const std_pair & p) :

        left ( p.first  ),
        right( p.second )
    {}

    // Operators

    standard_relation& operator=(const standard_relation & rel)
    {
        left  = rel.left ;
        right = rel.right;
        return *this;
    }

    standard_relation& operator=(const left_pair & p)
    {
        left  = p.first ;
        right = p.second;
        return *this;
    }

    standard_relation& operator=(const right_pair & p)
    {
        left  = p.second;
        right = p.first ;
        return *this;
    }

    public:

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

        above_view_reference,
        left,right
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        const_above_view_reference,
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

    left_value_type & get_left()
    {
        return left;
    }

    const left_value_type & get_left() const
    {
        return left;
    }

    right_value_type & get_right()
    {
        return right;
    }

    const right_value_type & get_right() const
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

    above_view_reference get_view()
    {
        return above_view_reference(*this);
    }

    const_above_view_reference get_view() const
    {
        return const_above_view_reference(*this);
    }


    #ifndef BOOST_BIMAP_DISABLE_SERIALIZATION

    // Serialization support

    private:

    friend class ::boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(left );
        ar & BOOST_SERIALIZATION_NVP(right);
    }

    #endif // BOOST_BIMAP_DISABLE_SERIALIZATION
};


} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP


