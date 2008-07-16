//
//	Spatial Index - QuadTree
//
//
// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef BOOST_SPATIAL_INDEX_QUADTREE_HPP
#define BOOST_SPATIAL_INDEX_QUADTREE_HPP

#include "quadtree_node.hpp"

// #include <boost/thread/xtime.hpp>

namespace boost {
namespace spatial_index {


template<typename Point, typename Value>
class quadtree
{
private:
  quadtree_node<Point,Value> root;
  unsigned int element_count;

  // number of points in each node
  unsigned int node_size_;

public:
  quadtree(const geometry::box<Point> &r)
    : root(r, 1), element_count(0), node_size_(1)  {}

  quadtree(const geometry::box<Point> &r, const unsigned int M)
    : root(r, M), element_count(0), node_size_(M)  {}

  quadtree(const geometry::box<Point> &r, const unsigned int m, const unsigned int M)
    : root(r, M), element_count(0), node_size_(M)  {}

  /// remove the element with key 'k'
  void remove(const Point &k)
  {
    root.remove(k);
    element_count--;
  }

  /// remove data with key 'k'
  void remove(const geometry::box<Point> &k)
  {
    std::cerr << "Boxes not implemented in quadtrees." << std::endl;
  }

	  
  void insert(const Point &k, const Value &v)
  {
    element_count++;
    root.insert(k, v);
  }

  void print(void) 
  {
    std::cerr << "=================================" << std::endl;
    std::cerr << "Elements: " << elements() << std::endl;
    root.print();
    std::cerr << "=================================" << std::endl;
  }

  /// insert data with envelope 'e' with key 'k'
  void insert(const geometry::box<Point> &e, const Value &v)
  {
    throw std::logic_error("Box insertion not implemented.");
  }


  void bulk_insert(std::vector<Value> &values,  std::vector<Point> &points) 
  {
    // boost::xtime xt1, xt2;
    // boost::xtime_get(&xt1, boost::TIME_UTC);

    // unsigned int counter = 0;

    typename std::vector<Point>::iterator it_point;
    typename std::vector<Value>::iterator it_value;
    it_point = points.begin();
    it_value = values.begin();
    while(it_value != values.end() && it_point != points.end()) {
      insert(*it_point, *it_value);
      
      it_point++;
      it_value++;

      // counter++;
      // if(counter%10000 == 0) {
      // std::cerr << "counter: [" << counter << "]" << std::endl;
      // }
    }
    // boost::xtime_get(&xt2, boost::TIME_UTC);
    // std::cerr << "secs: " << xt2.sec - xt1.sec;
    // std::cerr << " nsecs: " << xt2.nsec - xt1.nsec << std::endl;
  }

  Value find(const Point &k)
  {
    return root.find(k);
  }

  std::deque<Value> find(const geometry::box<Point> &r)
  {
    std::deque<Value> result;
    root.find(result, r);
    return result;
  }

  unsigned int elements(void) const  { return element_count; }
};


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_QUADTREE_HPP

