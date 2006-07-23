// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/unordered_multimap_view.hpp
/// \brief View of a side of a bimap that is signature compatible with tr1::unordered_multimap.

#ifndef BOOST_BIMAP_VIEWS_UNOREDERED_MULTIMAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_UNOREDERED_MULTIMAP_VIEW_HPP

#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/unordered_multimap_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with tr1::unordered_multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a tr1::unordered_multimap.

See also const_unordered_multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class unordered_multimap_view
:
    public container_adaptor::unordered_multimap_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,

        typename bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_iterator_type_by<Tag,BimapType>::type,

        typename bimap::support::local_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_local_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // local iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >,

    public bimap::detail::map_view_base< unordered_multimap_view<Tag,BimapType>,Tag,BimapType >

{
    // Interface ------------------------------------------------------------

    public:

    unordered_multimap_view() {}
    unordered_multimap_view(typename unordered_multimap_view::base_type & c)
        : unordered_multimap_view::unordered_multimap_adaptor_(c) {}

};

/// \brief Constant view of a side of a bimap that is signature compatible with tr1::unordered_multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a tr1::unordered_multimap.

See also unordered_multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class const_unordered_multimap_view
:
    public container_adaptor::unordered_multimap_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::local_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_local_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // local iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    // Interface ------------------------------------------------------------

    public:

    const_unordered_multimap_view() {}
    const_unordered_multimap_view(typename const_unordered_multimap_view::base_type & c)
        : const_unordered_multimap_view::unordered_multimap_adaptor_(c) {}

};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_UNOREDERED_MULTIMAP_VIEW_HPP


