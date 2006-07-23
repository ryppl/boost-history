// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/list_map_view.hpp
/// \brief View of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP

#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/list_map_adaptor.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core.

See also const_list_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class list_map_view
:
    public container_adaptor::list_map_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,

        typename bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        typename bimap::detail::map_view_iterator_to_base<Tag,BimapType>::type,

        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >,

    public bimap::detail::map_view_base< list_map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef list_map_view this_type;

    public:

    list_map_view() {}
    list_map_view(typename this_type::base_type & c) :
        this_type::list_map_adaptor_(c) {}
};

/// \brief Constant view of a side of a bimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core.

See also list_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class const_list_map_view
:
    public container_adaptor::list_map_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    public:

    const_list_map_view() {}
    const_list_map_view(typename const_list_map_view::base_type & c) :
        const_list_map_view::list_map_adaptor_(c) {}
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP

