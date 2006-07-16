// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/symmetrical_base.hpp
/// \brief Base class for symmetrical types

#ifndef BOOST_BIMAP_RELATION_SYMMETRICAL_BASE_HPP
#define BOOST_BIMAP_RELATION_SYMMETRICAL_BASE_HPP

// Boost.Bimap
#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/tas/support/default_tagged.hpp>

#include <boost/bimap/relation/member_at.hpp>


namespace boost {
namespace bimap {
namespace relation {

/// \brief Base of symetrical tagged types.
/**

                                                           **/

template< class TA, class TB >
class symmetrical_base
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

    //@{

        /// The type stored in the relation
        typedef typename tagged_left_type::value_type left_value_type;
        typedef typename tagged_right_type::value_type right_value_type;

    //@}

    //@{

        /// The tag of the member. By default it is \c member_at::{side}
        typedef typename tagged_left_type::tag left_tag;
        typedef typename tagged_right_type::tag right_tag;

    //@}
};



} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_SYMMETRICAL_BASE_HPP

