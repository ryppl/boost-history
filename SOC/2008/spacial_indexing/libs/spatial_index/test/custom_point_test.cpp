//
// Boost.SpatialIndex - example showing the use of a user-defined point class
//
// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/ for latest version.
//


#include <boost/spatial_index.hpp>

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/shared_ptr.hpp>

#include <geometry/geometry.hpp>

#include <vector>
#include <string>


#include <boost/tuple/tuple.hpp>
#include <geometry/distance.hpp>
#include <geometry/aswkt.hpp>


// we define our own point
struct my_2d_point: boost::tuple < float, float >
{
  typedef float coordinate_type;
  enum { coordinate_count = 2 };

  my_2d_point(void)
  {
    get<0>() = 0.0;
    get<1>() = 0.0;
  }

  my_2d_point(float x, float y)
  {
    get<0>() = x;
    get<1>() = y;
  }

  bool operator<(const my_2d_point & o) const
  {
    return get<0> () <o. get <0> ();
  }

  // Because the geometry::point concept shares the "get" methods with 
  // boost::tuple, no more methods are needed.
};


// point traits as in the geometry library proposal
namespace geometry
{
  template <> 
  struct strategy_traits < my_2d_point, my_2d_point >
  {
    typedef strategy::distance::pythagoras < my_2d_point, my_2d_point > 
      point_distance;
    typedef strategy::not_implemented point_segment_distance;
    typedef strategy::not_implemented area;
    typedef strategy::not_implemented within;
    typedef strategy::not_implemented centroid;
    typedef strategy::not_implemented envelope;
  };

  template <> struct wkt_traits < my_2d_point >
  {
    typedef impl::wkt::stream_point < my_2d_point > stream_type;
  };
}                               // namespace geometry

// point operator<<
template < typename CH, typename TR >
inline std::basic_ostream < CH,TR > &
operator<<(std::basic_ostream < CH, TR > &os, const my_2d_point & p)
{
  os << geometry::as_wkt < my_2d_point > (p);
  return os;
}



int test_main(int, char *[])
{
  std::vector < std::string > data;
  std::vector < my_2d_point > points;

  // insert data
  data.push_back("test0");
  data.push_back("test1");
  data.push_back("test2");
  data.push_back("test3");
  data.push_back("test4");
  data.push_back("test5");

  points.push_back(my_2d_point(1.0, 1.0));
  points.push_back(my_2d_point(2.0, 1.0));
  points.push_back(my_2d_point(5.0, 5.0));
  points.push_back(my_2d_point(1.0, 6.0));
  points.push_back(my_2d_point(9.0, 9.0));
  points.push_back(my_2d_point(9.0, 8.0));

  geometry::box < my_2d_point > b(my_2d_point(0.0, 0.0),
                                  my_2d_point(20.0, 20.0));

  typedef std::vector < std::string >::iterator value_type;

  // create index
  boost::spatial_index::spatial_index < my_2d_point, value_type,
    boost::spatial_index::quadtree < my_2d_point, value_type > >q(b, 1);

  //  std::cerr << " --> bulk insert" << std::endl;
  //  std::vector<std::string>::iterator b, e;
  //  b = data.begin();
  //  e = data.end();
  //  q.bulk_insert(b,e, points);

  std::vector < std::string >::iterator it = data.begin();

  // insert data
  std::cerr << " --> insert" << std::endl;
  q.insert(my_2d_point(1.0, 1.0), it++);
  std::cerr << " --> insert" << std::endl;
  q.insert(my_2d_point(2.0, 1.0), it++);
  std::cerr << " --> insert" << std::endl;
  q.insert(my_2d_point(5.0, 5.0), it++);
  std::cerr << " --> insert" << std::endl;
  q.insert(my_2d_point(1.0, 6.0), it++);
  std::cerr << " --> insert" << std::endl;
  q.insert(my_2d_point(9.0, 9.0), it++);
  std::cerr << " --> insert" << std::endl;
  q.insert(my_2d_point(9.0, 8.0), it++);


  std::vector < std::string >::iterator it1;

  // find data
  std::cerr << " --> find" << std::endl;
  it1 = q.find(my_2d_point(9.0, 9.0));
  BOOST_CHECK_EQUAL(*it1, "test4");
  std::cout << "  " << *it1 << std::endl;

  std::cerr << " --> find" << std::endl;
  it1 = q.find(my_2d_point(5.0, 5.0));
  BOOST_CHECK_EQUAL(*it1, "test2");
  std::cout << "  " << *it1 << std::endl;

  // expected result
  std::vector < std::string > res;
  res.push_back("test0");
  res.push_back("test1");
  res.push_back("test2");

  std::cerr << "Elements: " << q.elements() << std::endl;
  BOOST_CHECK_EQUAL(q.elements(), 6u);

  std::cerr << " --> find rectangle" << std::endl;
  geometry::box < my_2d_point > query_box(my_2d_point(0.0, 0.0),
                                          my_2d_point(5.0, 5.0));
  std::deque < std::vector < std::string >::iterator > d = q.find(query_box);
  BOOST_CHECK_EQUAL(d.size(), 3u);
  unsigned int
    i = 0;
  for (std::deque < std::vector <
       std::string >::iterator >::const_iterator dit = d.begin();
       dit != d.end(); ++dit) {
    std::cerr << "Value: " << *(*dit) << std::endl;
    BOOST_CHECK_EQUAL(*(*dit), res[i++]);
  }

  return 0;
};
