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

// Boost.Bimap
#include <boost/bimap/tagged/tagged.hpp>
#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>

#include <boost/bimap/relation/standard_pair_view.hpp>
#include <boost/bimap/relation/const_standard_pair_view.hpp>

#include <boost/bimap/tagged/support/default_tagged.hpp>
#include <boost/bimap/tagged/support/tag_of.hpp>
#include <boost/bimap/tagged/support/value_type_of.hpp>

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
class standard_relation : public symmetrical_base<TA,TB>
{
    public:

    //@{
        /// A signature compatible std::pair that is a view of the relation.

        typedef standard_pair_view<TA,TB,normal_layout> left_pair ;
        typedef standard_pair_view<TB,TA,mirror_layout> right_pair;

        typedef left_pair  left_pair_reference ;
        typedef right_pair right_pair_reference;

        typedef const_standard_pair_view<TA,TB,normal_layout> const_left_pair ;
        typedef const_standard_pair_view<TB,TA,mirror_layout> const_right_pair;

        typedef const_left_pair  const_left_pair_reference ;
        typedef const_right_pair const_right_pair_reference;
    //@}


    //@{

        /// data, exposed for easy manipulation
        typename standard_relation::left_value_type  left;
        typename standard_relation::right_value_type right;

    //@}

    standard_relation() {}

    standard_relation(const typename standard_relation::left_value_type & l,
                      const typename standard_relation::right_value_type & r) :
        left (l),
        right(r)
    {}

    standard_relation(const standard_relation & rel) :

        left (rel.left),
        right(rel.right)
    {}

    // The following functions are redundant if you only consider this class.
    // They are included to make easier the construction of the get and the
    // pair_by metafunction. Remember that not all compiler supports the mutant
    // idiom.

    typename standard_relation::left_value_type & get_left()
    {
        return left;
    }

    const typename standard_relation::left_value_type & get_left() const
    {
        return left;
    }

    typename standard_relation::right_value_type & get_right()
    {
        return right;
    }

    const typename standard_relation::right_value_type & get_right() const
    {
        return right;
    }

    left_pair get_left_pair()
    {
        return left_pair(*this);
    }
    const_left_pair get_left_pair() const
    {
        return const_left_pair(*this);
    }

    right_pair get_right_pair()
    {
        return right_pair(*this);
    }

    const_right_pair get_right_pair() const
    {
        return const_right_pair(*this);
    }
};


} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP


