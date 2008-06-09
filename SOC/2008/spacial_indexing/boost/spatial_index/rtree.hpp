// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_RTREE_HPP
#define BOOST_SPATIAL_INDEX_RTREE_HPP

// #include "quadtree_node.hpp"

// #include <boost/thread/xtime.hpp>

namespace boost {
namespace spatial_index {

  template<typename Point, typename Value>
  class rtree_node
  {
  };


    template<typename Point, typename Value>
    class rtree : public spatial_index<Point, Value>
    {
    private:
      rtree_node<Point,Value> root;
      unsigned int element_count;

      // minimum number of points in each node
      unsigned int m_;
      // maximum number of points in each node
      unsigned int M_;

    public:
      rtree(const unsigned int &m, const unsigned int &M) : element_count(0), m_(m), M_(M) {}
      
      virtual void insert(const Point &k, const Value &v)
      {
	element_count++;
      }

      virtual void bulk_insert(Value &v_begin, Value &v_end, std::vector<Point> &v)
      {
      }

      virtual Value find(const Point &k)
      {
	return Value();
      }


      virtual std::deque<Value> find(const geometry::box<Point> &r)
      {
	std::deque<Value> result;
	return result;
      }

      virtual unsigned int elements(void) { return element_count; }


      virtual ~rtree() {}
    };


} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_RTREE_HPP

