// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_HELPERS_HPP
#define BOOST_SPATIAL_INDEX_HELPERS_HPP

namespace boost {
namespace spatial_index {


  template<typename Point>
  bool overlaps(const geometry::box<Point> &b1, const geometry::box<Point> &b2)
  {

    bool overlaps_x, overlaps_y;
    overlaps_x = overlaps_y = false;

    if(geometry::get<0>(b1.min()) >= geometry::get<0>(b2.min()) && geometry::get<0>(b1.min()) <= geometry::get<0>(b2.max())) {
      overlaps_x = true;
    }
    if(geometry::get<0>(b1.max()) >= geometry::get<0>(b2.min()) && geometry::get<0>(b1.min()) <= geometry::get<0>(b2.max())) {
      overlaps_x = true;
    }

    if(geometry::get<1>(b1.min()) >= geometry::get<1>(b2.min()) && geometry::get<1>(b1.min()) <= geometry::get<1>(b2.max())) {
      overlaps_y = true;
    }
    if(geometry::get<1>(b1.max()) >= geometry::get<1>(b2.min()) && geometry::get<1>(b1.min()) <= geometry::get<1>(b2.max())) {
      overlaps_y = true;
    }

    return overlaps_x && overlaps_y;
  }


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_HELPERS_HPP
