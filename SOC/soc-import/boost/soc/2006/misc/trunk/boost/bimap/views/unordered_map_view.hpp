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

#include <boost/bimap/container_adaptor/unordered_map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>
#include <boost/bimap/detail/operator_bracket_proxy.hpp>

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
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by
    ),

    public ::boost::bimap::detail::map_view_base< unordered_map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(unordered_map_view,Tag,BimapType);

    public:

    unordered_map_view(typename base_::base_type & c)
        : base_(c) {}

    ::boost::bimap::detail::operator_bracket_proxy<unordered_map_view>
        operator[](const typename base_::key_type & k)
    {
        return ::boost::bimap::detail::operator_bracket_proxy<unordered_map_view>(*this,k);
    }

    typename base_::data_type const &
        operator[](const typename base_::key_type & k) const
    {
        // TODO
        // Add index check?
        return this->find(k)->second;
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
    public BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by
    )
{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by

    ) base_;

    public:

    const_unordered_map_view(typename base_::base_type & c)
        : base_(c) {}

    typename base_::data_type const &
        operator[](const typename base_::key_type & k) const
    {
        // TODO
        // Add index check?
        return this->find(k)->second;
    }

};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP


