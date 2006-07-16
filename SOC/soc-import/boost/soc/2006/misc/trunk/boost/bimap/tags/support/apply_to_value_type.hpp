// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file tags/support/apply_to_value_type.hpp
/// \brief Similar to mpl::apply but for tagged types.

#ifndef BOOST_BIMAP_TAGS_SUPPORT_APPLY_TO_VALUE_TYPE_HPP
#define BOOST_BIMAP_TAGS_SUPPORT_APPLY_TO_VALUE_TYPE_HPP

#include <boost/bimap/tags/tagged.hpp>
#include <boost/mpl/apply.hpp>

/** \struct boost::bimap::tags::support::apply_to_value_type
\brief Higger order metafunction similar to mpl::apply but for tagged types.

\code
template< class Metafunction, class TaggedType >
struct apply_to_value_type
{
    typedef tagged
    <
        Metafuntion< value_type_of< TaggedType >::type >::type,
        tag_of< TaggedType >::type

    > type;
};
\endcode

This higher order metafunctions is very useful, and it can be used with lambda
expresions.

See also tagged.
                                                                                **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace tags {
namespace support {

template < class F, class TaggedType >
struct apply_to_value_type;

template < class F, class ValueType, class Tag >
struct apply_to_value_type<F, tagged<ValueType,Tag> >
{
    typedef typename mpl::apply< F, ValueType >::type new_value_type;
    typedef tagged< new_value_type, Tag > type;
};

} // namespace support
} // namespace tags
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_TAGS_SUPPORT_APPLY_TO_VALUE_TYPE_HPP
