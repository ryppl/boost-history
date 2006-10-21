// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/list_map_view.hpp
/// \brief View of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP

#include <boost/config.hpp>

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
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        list_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    ),
    public ::boost::bimap::detail::map_view_base< list_map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        list_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(list_map_view,Tag,BimapType);

    public:

    list_map_view(typename base_::base_type & c) :
        base_(c) {}

    list_map_view & operator=(const list_map_view & v) { this->base() = v.base(); return *this; }

    BOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION
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
    public BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        list_map_adaptor,
        Tag,BimapType,
        const_reverse_iterator_type_by
    )
{
    typedef BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        list_map_adaptor,
        Tag,BimapType,
        const_reverse_iterator_type_by

    ) base_;

    public:

    const_list_map_view(typename base_::base_type & c) :
        base_(c) {}

};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP

