// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/support/get.hpp
/// \brief get<tag>(r) function

#ifndef BOOST_BIMAP_RELATION_SUPPORT_GET_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_GET_HPP

#include <boost/bimap/relation/standard_pair_view.hpp>
#include <boost/bimap/detail/mpl/bool_result.hpp>
#include <boost/bimap/relation/support/value_type_of.hpp>
#include <boost/bimap/relation/detail/access_builder.hpp>

//#include <boost/bimap/relation/standard_pair_view.hpp>

namespace boost {
namespace bimap {
namespace relation {
namespace support {

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
    struct get<Tag,Relation>;
    {
        typedef -UNDEFINED- type;
    };

    } // namespace result_of

*/

BOOST_BIMAP_SYMMETRIC_ACCESS_RESULT_OF_BUILDER
(
    get,
    value_type_of
);



// Implementation
// -------------------------------------------------------------------------

BOOST_BIMAP_SYMMETRIC_ACCESS_IMPLEMENTATION_BUILDER
(
    get,
    Relation,
    rel,
    return rel.get_left(),
    return rel.get_right()
);


// Interface
// --------------------------------------------------------------------------

/// \brief Gets the value of one of the members in a symmetrical class.
/**
\param r is a symmetrical type
\return Convertible to \c value_type_of<Tag,Relation>::type
Instead of using \c value_type_of<Tag,Relation>::type this functions use
\b Boost.call_traits to find the best way to return this value. To help
the user of this function the following metafunction is provided
\code

namespace result_of {

template< class Tag, class Relation >
struct get<Tag,Relation>;
{
    typedef -UNDEFINED- type;
};

} // namespace result_of

\endcode

See also member_at, value_type_of.
\ingroup relation_group
                                                                        **/



template< class Type >
struct is_standard_pair_view
{
    BOOST_BIMAP_MPL_BOOL_RESULT(false)
};

template< class FirstType, class SecondType, class Layout >
struct is_standard_pair_view< standard_pair_view<FirstType,SecondType,Layout> >
{
    BOOST_BIMAP_MPL_BOOL_RESULT(true)
};

template< class Tag, class Symmetric >
typename enable_if< is_standard_pair_view< Symmetric >,

typename result_of::get< Tag, Symmetric >::type

>::type
get( Symmetric s )
{
    typedef typename ::boost::bimap::relation::support::member_with_tag
    <
        Tag, Symmetric

    >::type member_at_tag;

    return detail::get(member_at_tag(),s);
}


BOOST_BIMAP_SYMMETRIC_ACCESS_INTERFACE_BUILDER
(
    get
);



} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_SUPPORT_GET_HPP

