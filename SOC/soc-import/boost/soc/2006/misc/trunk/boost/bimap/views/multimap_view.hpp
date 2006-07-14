// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/multimap_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::multimap.

#ifndef BOOST_BIMAP_VIEWS_MULTIMAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MULTIMAP_VIEW_HPP

#include <boost/bimap/views/detail/map_view_iterator.hpp>
#include <boost/bimap/views/detail/to_pair_from_relation.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/multimap_adaptor.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with std::multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multimap.

See also const_multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class multimap_view
:
    public container_adaptor::multimap_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,

        typename support::iterator_type_by<Tag,BimapType>::type,
        typename support::const_iterator_type_by<Tag,BimapType>::type,
        typename support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename support::iterator_type_by<Tag,BimapType>::type,
            typename support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        detail::to_pair_view_from_relation<Tag, typename BimapType::relation >
    >
{
    public:

    multimap_view() {}
    multimap_view(typename multimap_view::base_type & c)
        : multimap_view::multimap_adaptor_(c) {}

};

/// \brief Constant view of a side of a bimap that is signature compatible with std::multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multimap.

See also multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class const_multimap_view
:
    public container_adaptor::multimap_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename support::iterator_type_by<Tag,BimapType>::type,
        typename support::const_iterator_type_by<Tag,BimapType>::type,
        typename support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename support::iterator_type_by<Tag,BimapType>::type,
            typename support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        detail::to_pair_view_from_relation<Tag, typename BimapType::relation >
    >
{
    public:

    const_multimap_view() {}
    const_multimap_view(typename const_multimap_view::base_type & c) : const_multimap_view::map_adaptor_(c) {}

};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

