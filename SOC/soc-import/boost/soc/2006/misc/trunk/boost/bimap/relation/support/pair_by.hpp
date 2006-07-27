// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/support/pair_by.hpp
/// \brief pair_by<member>(r) function

#ifndef BOOST_BIMAP_RELATION_SUPPORT_PAIR_BY_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_PAIR_BY_HPP

#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/bimap/relation/support/pair_type_by.hpp>
#include <boost/bimap/relation/detail/access_builder.hpp>

namespace boost {
namespace bimap {
namespace relation {
namespace support {


/** \fn boost::bimap::relation::support::pair_by

\brief Gets a pair view of the relation.

\code

template< class Tag, class Relation >
typename result_of::pair_by<Tag,Relation>::type
    pair_by( Relation & );

\endcode

\ingroup relation_group
                                                                        **/


#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

// Since it is very error-prone to directly write the hole bunch
// of relation accesor. They are buil from litle macro blocks that
// are both more readable, leading to self docummenting code and a
// lot more easier to understand and mantain.
// It is very important to note that the three building blocks have
// to laid in the same namespace in order to work. There is also
// important to keep them in order.
// The forward declaration are not necesary but they help a lot to
// the reader, as they undercover what is the signature of the
// result code.
// In the end, it is not quicker to do it in this way because you
// write a lot. But this code has no complexity at all and almost
// every word writed is for documentation.

// Result of
// -------------------------------------------------------------------------
/*
    namespace result_of {

    template< class Tag, class Relation >
    struct pair_by<Tag,Relation>;
    {
        typedef -UNDEFINED- type;
    };

    } // namespace result_of

*/


// It can not be used directly BOOST_BIMAP_SYMMETRIC_ACCESS_RESULT_OF_BUILDER
// here because all the standard and mutant problem.

namespace result_of {

template< class Tag, class Relation >
struct pair_by
{
    typedef typename mpl::if_< is_const<Relation>,
    // {
           typename const_pair_reference_type_by< Tag,Relation >::type,
    // }
    // else
    // {
           typename pair_reference_type_by< Tag,Relation >::type
    // }
    >::type type;
};

} // namespace result_of


// Implementation
// -------------------------------------------------------------------------

BOOST_BIMAP_SYMMETRIC_ACCESS_IMPLEMENTATION_BUILDER
(
    pair_by,
    Relation,
    rel,
    return rel.get_left_pair(),
    return rel.get_right_pair()
);

// Interface
// --------------------------------------------------------------------------

BOOST_BIMAP_SYMMETRIC_ACCESS_INTERFACE_BUILDER
(
    pair_by
);


#endif // BOOST_BIMAP_DOXIGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES



} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_SUPPORT_PAIR_BY_HPP
