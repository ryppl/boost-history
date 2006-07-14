// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.


#ifndef BOOST_BIMAP_COLLECTION_SUPPORT_COMPUTE_INDEX_TYPE_HPP
#define BOOST_BIMAP_COLLECTION_SUPPORT_COMPUTE_INDEX_TYPE_HPP

#include <boost/bimap/detail/safe_enabler.hpp>

namespace boost {
namespace bimap {
namespace collection {
namespace support {

/// \file collection/support/compute_index_type.hpp
/// \brief Compute Index Type metafunction declaration

template
<
    class KeyType,
    class KeyExtractor,
    class Tag,
    class Enable = void//detail::safe_enabler
>
struct compute_index_type;

/** \struct compute_index_type

\brief Computes the multi index type that correspond to a given set_type_of specification.

\code
template<class KeyType, class KeyExtractor, class Tag >
struct compute_index_type
{
    typedef multi_index::{IndexType}
    <
        multi_index::tag<Tag>,
        KeyExtractor,
        {ConfigParameters}

    > type;
};
\endcode

                                                                                        **/
} // namespace support
} // namespace collection
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_COLLECTION_SUPPORT_COMPUTE_INDEX_TYPE_HPP

