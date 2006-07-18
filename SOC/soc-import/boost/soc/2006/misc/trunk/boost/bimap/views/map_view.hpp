// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/map_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::map.

#ifndef BOOST_BIMAP_VIEWS_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

#include <boost/bimap/views/detail/map_view_iterator.hpp>
#include <boost/bimap/views/detail/to_pair_from_relation.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/map_adaptor.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with std::map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::map.

See also const_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class map_view
:
    public container_adaptor::map_adaptor
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
    typedef map_view this_type;

    public:

    map_view() {}
    map_view(typename this_type::base_type & c) : this_type::map_adaptor_(c) {}

    typename this_type::data_type & operator[](const typename this_type::key_type & k)
    {
        return ( * ( ( this_type::base().insert(
                typename this_type::value_type( k, typename this_type::data_type() )
                ) ).first ) ).second;
    }
};

/// \brief Constant view of a side of a bimap that is signature compatible with std::map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::map.

See also map_view.
                                                                                    **/
template< class Tag, class BimapType >
class const_map_view
:
    public container_adaptor::map_adaptor
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

    const_map_view() {}
    const_map_view(typename const_map_view::base_type & c) : const_map_view::map_adaptor_(c) {}
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

