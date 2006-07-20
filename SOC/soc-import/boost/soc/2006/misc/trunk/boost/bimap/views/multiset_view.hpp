// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/multiset_view.hpp
/// \brief View of a bimap that is signature compatible with std::multiset.

#ifndef BOOST_BIMAP_VIEWS_MULTISET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MULTISET_VIEW_HPP

#include <boost/bimap/container_adaptor/multiset_adaptor.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::multiset.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multiset.

See also const_multiset_view.
                                                                                    **/

template< class IndexType >
class multiset_view
:
    public container_adaptor::multiset_adaptor
    <
        IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::reverse_iterator,
        typename IndexType::const_reverse_iterator,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        typename IndexType::key_from_value
    >
{
    public:

    multiset_view() {}
    multiset_view(typename multiset_view::base_type & c) : multiset_view::multiset_adaptor_(c) {}

};

/// \brief Constant view of a bimap that is signature compatible with std::multiset.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multiset.

See also multiset_view.
                                                                                    **/

template< class IndexType >
class const_multiset_view
:
    public container_adaptor::multiset_adaptor
    <
        const IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::reverse_iterator,
        typename IndexType::const_reverse_iterator,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        typename IndexType::key_from_value
    >
{
    public:

    const_multiset_view() {}
    const_multiset_view(typename const_multiset_view::base_type & c) : const_multiset_view::multiset_adaptor_(c) {}

};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MULTISET_VIEW_HPP

