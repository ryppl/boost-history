#ifndef BOOST_GUIGL_EXAMPLE_NODE_TYPES_HPP
#define BOOST_GUIGL_EXAMPLE_NODE_TYPES_HPP

#include <boost/guigl/types.hpp>
#include <boost/array.hpp>
#include <geometry/geometries/polygon.hpp>
#include <boost/guigl/ggl.hpp>

typedef boost::guigl::position_type point_type;
typedef boost::guigl::segment_type line_type;
typedef geometry::polygon<point_type> plane_type;

template<class T>
struct base_result
  {
  typedef T result_type;
  result_type result;

  base_result():result(){}
  explicit base_result(T const& r):result(r){}
  };

struct source_point : public base_result<point_type>
  {
  source_point(point_type const& pt):base_result<point_type>(pt){}
  };

struct line_from_two_points : public base_result<line_type>
  {
  point_type const& pt1, pt2;

  line_from_two_points(
    point_type const& pt1_, point_type const& pt2_)
    : pt1(pt1_), pt2(pt2_)
    {}
  };

struct plane_from_three_points : public base_result<plane_type>
  {
  point_type const& pt1, pt2, pt3;

  plane_from_three_points(
    point_type const& pt1_, point_type const& pt2_, point_type const& pt3_)
    : pt1(pt1_), pt2(pt2_), pt3(pt3_)
    {}
  };

#endif // BOOST_GUIGL_EXAMPLE_NODE_TYPES_HPP
