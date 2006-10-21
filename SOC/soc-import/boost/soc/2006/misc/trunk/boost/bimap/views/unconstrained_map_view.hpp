// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/unconstrained_map_view.hpp
/// \brief Unconstrained view of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_UNCONSTRAINED_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_UNCONSTRAINED_MAP_VIEW_HPP

namespace boost {
namespace bimap {
namespace views {

/// \brief Unconstrained view of a side of a bimap.

template< class Tag, class BimapType>
class unconstrained_map_view
{
    public:
    template< class T >
    unconstrained_map_view(const T & t) {}
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_UNCONSTRAINED_MAP_VIEW_HPP

