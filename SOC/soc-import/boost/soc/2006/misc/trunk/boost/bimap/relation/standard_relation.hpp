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

#include <boost/bimap/relation/structured_pair.hpp>

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

    public symmetrical_base<TA,TB>,

    totally_ordered<
        standard_relation<TA,TB>,
    totally_ordered<
        structured_pair<TA,TB,normal_layout>,
    totally_ordered<
        structured_pair<TB,TA,mirror_layout>,
    totally_ordered<
        standard_pair_view<TA,TB,normal_layout>,
    totally_ordered<
        standard_pair_view<TB,TA,mirror_layout>,
    totally_ordered<
        const_standard_pair_view<TA,TB,normal_layout>,
    totally_ordered<
        const_standard_pair_view<TB,TA,mirror_layout>

    > > > > > > >
{
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
        typename standard_relation::left_value_type  left;
        typename standard_relation::right_value_type right;

    //@}

    standard_relation() {}

    standard_relation(typename boost::call_traits<
                          typename standard_relation::left_value_type >::param_type l,
                      typename boost::call_traits<
                          typename standard_relation::right_value_type>::param_type r) :
        left (l),
        right(r)
    {}

    standard_relation(const standard_relation & rel) :

        left (rel.left),
        right(rel.right)
    {}

    // Allow to create relations from views

    standard_relation(const left_pair & lp) :

        left ( lp.first  ),
        right( lp.second )
    {}

    standard_relation(const right_pair & rp) :

        left ( rp.second ),
        right( rp.first  )
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

    // Operators required by Boost.Operators

    bool operator==(const standard_relation & rel) const
    {
        return ( ( left  == rel.left  ) &&
                 ( right == rel.right ) );
    }

    bool operator<(const standard_relation & rel) const
    {
        return ( ( left  < rel.left  ) &&
                 ( right < rel.right ) );
    }

    bool operator==(const left_pair & p) const
    {
        return ( ( left  == p.first  ) &&
                 ( right == p.second ) );
    }

    bool operator<(const left_pair & p) const
    {
        return ( ( left   < p.first   ) &&
                 ( right  < p.second  ) );
    }

    bool operator==(const right_pair & p) const
    {
        return ( ( left  == p.second  ) &&
                 ( right == p.first   ) );
    }

    bool operator<(const right_pair & p) const
    {
        return ( ( left   < p.second  ) &&
                 ( right  < p.first   ) );
    }

    bool operator==(const left_pair_reference & p) const
    {
        return ( ( left  == p.first  ) &&
                 ( right == p.second ) );
    }

    bool operator<(const left_pair_reference & p) const
    {
        return ( ( left   < p.first   ) &&
                 ( right  < p.second  ) );
    }

    bool operator==(const right_pair_reference & p) const
    {
        return ( ( left  == p.second  ) &&
                 ( right == p.first   ) );
    }

    bool operator<(const right_pair_reference & p) const
    {
        return ( ( left   < p.second  ) &&
                 ( right  < p.first   ) );
    }

    bool operator==(const const_left_pair_reference & p) const
    {
        return ( ( left  == p.first  ) &&
                 ( right == p.second ) );
    }

    bool operator<(const const_left_pair_reference & p) const
    {
        return ( ( left   < p.first   ) &&
                 ( right  < p.second  ) );
    }

    bool operator==(const const_right_pair_reference & p) const
    {
        return ( ( left  == p.second  ) &&
                 ( right == p.first   ) );
    }

    bool operator<(const const_right_pair_reference & p) const
    {
        return ( ( left   < p.second  ) &&
                 ( right  < p.first   ) );
    }

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


} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_STANDARD_RELATION_HPP


