// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/vector_map_view.hpp
/// \brief View of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP

#include <boost/bimap/container_adaptor/vector_map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core.

See also const_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class vector_map_view
:
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        vector_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    ),

    public ::boost::bimap::detail::map_view_base< vector_map_view<Tag,BimapType>,Tag,BimapType >
{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        vector_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(vector_map_view,Tag,BimapType);

    public:

    vector_map_view(typename base_::base_type & c) :
        base_(c) {}

};

/// \brief Constant view of a side of a bimap that is signature compatible with std::map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::map.

See also map_view.
                                                                                    **/
template< class Tag, class BimapType >
class const_vector_map_view
:
    public BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        vector_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    )
{
    typedef BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        vector_map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    public:

    const_vector_map_view() {}
    const_vector_map_view(typename base_::base_type & c) :
        base_(c) {}
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP

