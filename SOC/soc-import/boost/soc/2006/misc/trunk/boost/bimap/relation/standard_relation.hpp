// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/standard_relation.hpp
/// \brief Includes the relation class

#ifndef BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP
#define BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP

// Boost
#include <boost/serialization/nvp.hpp>

// Boost.Bimap
#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/tags/support/default_tagged.hpp>
#include <boost/bimap/tags/support/tag_of.hpp>
#include <boost/bimap/tags/support/value_type_of.hpp>

#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>
#include <boost/bimap/relation/standard_pair_view.hpp>
#include <boost/bimap/relation/const_standard_pair_view.hpp>
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

template< class TA, class TB >
class standard_relation :

    public symmetrical_base<TA,TB>

{
    typedef symmetrical_base<TA,TB> base_;

    public:

    //@{
        /// A signature compatible std::pair that is a view of the relation.

        typedef structured_pair<TA,TB,normal_layout> left_pair ;
        typedef structured_pair<TB,TA,mirror_layout> right_pair;

        typedef standard_pair_view<TA,TB,normal_layout> left_pair_reference ;
        typedef standard_pair_view<TB,TA,mirror_layout> right_pair_reference;

        typedef const_standard_pair_view<TA,TB,normal_layout> const_left_pair_reference ;
        typedef const_standard_pair_view<TB,TA,mirror_layout> const_right_pair_reference;

    //@}


    //@{

        /// data, exposed for easy manipulation
        typename base_:: left_value_type  left;
        typename base_::right_value_type right;

    //@}

    standard_relation() {}

    standard_relation(typename ::boost::call_traits<
                          typename base_:: left_value_type>::param_type l,
                      typename ::boost::call_traits<
                          typename base_::right_value_type>::param_type r) :
        left (l),
        right(r)
    {}

    standard_relation(const standard_relation & rel) :

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
        typename base_::left_value_type,
        typename base_::right_value_type

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

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        left,right,

        standard_relation,
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


