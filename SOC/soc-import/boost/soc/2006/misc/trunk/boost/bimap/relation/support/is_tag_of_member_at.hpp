// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/support/is_tag_of_member_at.hpp
/// \brief is_tag_of_member_at<tag,relation> metafunction

#ifndef BOOST_BIMAP_RELATION_SUPPORT_IS_TAG_OF_MEMBER_AT_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_IS_TAG_OF_MEMBER_AT_HPP

#include <boost/bimap/relation/member_at.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/bimap/relation/support/member_with_tag.hpp>
#include <boost/bimap/detail/mpl/bool_result.hpp>
#include <boost/bimap/detail/safe_enabler.hpp>


/** \struct boost::bimap::relation::support::is_tag_of_member_at_left

\brief Metafunction to test if a user tag is refering to the left member.

\code

template< class Tag, class Relation >
struct is_tag_of_member_at_left
{
    typedef {true_|false_} type;
    typedef typename type::value value;
};

\endcode

This metafunction is somewhat redundant with member_with_tag, but it is included
because it is a lot easier to metaprogram with it. The result type is the
same that:

\code

 is_same< member_with_tag<Tag,Relation>::type , member_at::left >::type

\endcode

See also member_with_tag, member_at, is_tag_of_member_at_right.
\ingroup relation_group
                                                                             **/


/** \struct boost::bimap::relation::support::is_tag_of_member_at_right

\brief Metafunction to test if a user tag is refering to the left member.

\code

template< class Tag, class Relation >
struct is_tag_of_member_at_right
{
    typedef {true_|false_} type;
    typedef typename type::value value;
};

\endcode

This metafunction is somewhat redundat with member_with_tag, but it is included
because it is a lot easier to metaprogram with it. The result type is the
same that:

\code

 is_same< member_with_tag<Tag,Relation>::type , member_at::right >::type

\endcode

See also member_with_tag, member_at, is_tag_of_member_at_left.
\ingroup relation_group
                                                                             **/


#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace relation {
namespace support {

// Metafunction is_tag_of_member_at_left
// Easiear metaprogramming

template
<
    class Tag,
    class Relation,
    class Enable = void // why bimap::detail::safe_enabler do not work?
>
struct is_tag_of_member_at_left
{
    BOOST_BIMAP_MPL_BOOL_RESULT(false);
};

template< class Tag, class Relation >
struct is_tag_of_member_at_left
<
    Tag, Relation,
    typename enable_if
    <
        is_same
        <
            typename member_with_tag<Tag,Relation>::type,
            member_at::left
        >

    >::type
>
{
    BOOST_BIMAP_MPL_BOOL_RESULT(true);
};

// Metafunction is_tag_of_member_at_right
// Easiear metaprogramming

template
<
    class Tag,
    class Relation,
    class Enable = void // why bimap::detail::safe_enabler do not work?
>
struct is_tag_of_member_at_right
{
    BOOST_BIMAP_MPL_BOOL_RESULT(false);
};

template< class Tag, class Relation >
struct is_tag_of_member_at_right
<
    Tag, Relation,
    typename enable_if
    <
        is_same
        <
            typename member_with_tag<Tag,Relation>::type,
            member_at::right
        >

    >::type
>
{
    BOOST_BIMAP_MPL_BOOL_RESULT(true);
};

} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_RELATION_SUPPORT_IS_TAG_OF_MEMBER_AT_HPP


