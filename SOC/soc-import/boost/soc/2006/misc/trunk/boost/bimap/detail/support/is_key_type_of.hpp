// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/support/is_key_type_of.hpp
/// \brief Is key type of metafunction definition.

#ifndef BOOST_BIMAP_COLLECTION_SUPPORT_IS_KEY_TYPE_OF_HPP
#define BOOST_BIMAP_COLLECTION_SUPPORT_IS_KEY_TYPE_OF_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/bimap/detail/safe_enabler.hpp>
#include <boost/bimap/detail/mpl/bool_result.hpp>

#include <boost/bimap/collection/key_type_of_tag.hpp>

/** \struct boost::bimap::collection::support::is_key_type_of

\brief Type trait to check if a class is a set_type_of specification

\code
template< class Type >
struct is_key_type_of
{
    typedef {mpl::true_/mpl::false_} type;
};
\endcode

                                                                                        **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace collection {
namespace support {

template< class Type, class Enable = void > // why it is not working with detail::safe_enabler?
struct is_key_type_of
{
    BOOST_BIMAP_MPL_BOOL_RESULT(false);
};

template
<
    class Type
>
struct is_key_type_of
<
    Type,
    typename enable_if< is_base_of< key_type_of_tag, Type > >::type
>
{
    BOOST_BIMAP_MPL_BOOL_RESULT(true);
};

} // namespace support
} // namespace collection
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_COLLECTION_SUPPORT_IS_KEY_TYPE_OF_HPP
