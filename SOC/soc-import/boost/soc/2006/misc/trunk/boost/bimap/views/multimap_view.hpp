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

#include <boost/bimap/container_adaptor/multimap_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

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
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        multimap_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    ),
    public ::boost::bimap::detail::map_view_base< multimap_view<Tag,BimapType>,Tag,BimapType >

{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        multimap_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(multimap_view,Tag,BimapType);

    public:

    multimap_view(typename base_::base_type & c)
        : base_(c) {}

    BOOST_BIMAP_MAP_VIEW_RANGE_IMPLEMENTATION(base_)

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
    public BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        multimap_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    )
{
    typedef BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        multimap_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    public:

    const_multimap_view(typename base_::base_type & c) : base_(c) {}

    BOOST_BIMAP_MAP_VIEW_RANGE_IMPLEMENTATION(base_)
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

