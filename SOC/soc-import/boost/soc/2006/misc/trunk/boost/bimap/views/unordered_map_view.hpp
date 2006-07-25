// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/unordered_map_view.hpp
/// \brief View of a side of a bimap that is signature compatible with tr1::unordered_map.

#ifndef BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP

#include <boost/config.hpp>

#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/unordered_map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with tr1::unordered_map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a tr1::unordered_map.

See also const_unordered_map_view.
                                                                                    **/


template< class Tag, class BimapType >
class unordered_map_view
:
    public container_adaptor::unordered_map_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,

        typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type,

        typename ::boost::bimap::support::local_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_local_iterator_type_by<Tag,BimapType>::type,

        typename ::boost::bimap::detail::map_view_iterator_to_base<Tag,BimapType>::type,

        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // local iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >,

    public ::boost::bimap::detail::map_view_base< unordered_map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef unordered_map_view this_type;

    #if defined(BOOST_MSVC)
        typedef ::boost::bimap::detail::map_view_base< unordered_map_view<Tag,BimapType>,Tag,BimapType >
            friend_map_view_base;
        friend class friend_map_view_base;
    #else
        friend class ::boost::bimap::detail::map_view_base< unordered_map_view<Tag,BimapType>,Tag,BimapType >;
    #endif

    public:

    unordered_map_view() {}
    unordered_map_view(typename unordered_map_view::base_type & c)
        : unordered_map_view::unordered_map_adaptor_(c) {}

    typename this_type::data_type & operator[](const typename this_type::key_type & k)
    {
        return ( * ( ( this_type::base().insert(
                typename this_type::value_type( k, typename this_type::data_type() )
                ) ).first ) ).second;
    }

};

/// \brief Constant view of a side of a bimap that is signature compatible with tr1::unordered_map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a tr1::unordered_map.

See also unordered_map_view.
                                                                                    **/

template< class Tag, class BimapType >
class const_unordered_map_view
:
    public container_adaptor::unordered_map_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::local_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_local_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // local iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    public:

    const_unordered_map_view() {}
    const_unordered_map_view(typename const_unordered_map_view::base_type & c)
        : const_unordered_map_view::const_unordered_map_adaptor_(c) {}

};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP


