// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/unordered_multiset_view.hpp
/// \brief View of a bimap that is signature compatible with tr1::unordered_multiset.

#ifndef BOOST_BIMAP_VIEWS_UNORDERED_MULTISET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_UNORDERED_MULTISET_VIEW_HPP

#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/unordered_multiset_adaptor.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::unordered_multiset.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::unordered_multiset.

See also const_unordered_multiset_view.
                                                                                    **/

template< class IndexType >
class unordered_multiset_view
:
    public container_adaptor::unordered_multiset_adaptor
    <
        IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::local_iterator,
        typename IndexType::const_local_iterator,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        typename IndexType::key_from_value
    >
{
    public:

    unordered_multiset_view() {}
    unordered_multiset_view(typename unordered_multiset_view::base_type & c)
        : unordered_multiset_view::unordered_multiset_adaptor_(c) {}

};

/// \brief Constant view of a bimap that is signature compatible with std::unordered_multiset.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::unordered_multiset.

See also unordered_multiset_view.
                                                                                    **/

template< class IndexType >
class const_unordered_multiset_view
:
    public container_adaptor::unordered_multiset_adaptor
    <
        const IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::local_iterator,
        typename IndexType::const_local_iterator,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        typename IndexType::key_from_value
    >
{
    public:

    const_unordered_multiset_view() {}
    const_unordered_multiset_view(typename const_unordered_multiset_view::base_type & c)
        : const_unordered_multiset_view::unordered_multiset_adaptor_(c) {}

};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_UNORDERED_MULTISET_VIEW_HPP


