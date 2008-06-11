// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_SPATIAL_INDEX_HPP
#define BOOST_SPATIAL_INDEX_SPATIAL_INDEX_HPP

#include <vector>
#include <deque>

#include <geometry/geometry.hpp>
#include <geometry/box.hpp>

namespace boost {
namespace spatial_index {

/// The spatial_index class does nothing.
/** \ingroup group_nothing
    \param T The type on which spatial_index does nothing.
    \warning This class does nothing.
    \todo Make this class do something.
    \bug This class does do something.

    Since spatial_index does nothing, it is not very useful.
*/

template<typename Point, typename Value>
class spatial_index
{

public:

  /// insert data 'v' with key 'k'
  virtual void insert(const Point &k, const Value &v) = 0;

  /// insert data with envelope 'e' with key 'k'
  virtual void insert(const geometry::box<Point> &e, const Value &v) = 0;
	
  /// bulk insert data from (v_begin, v_end)
  virtual void bulk_insert(Value &v_begin, Value &v_end, std::vector<Point> &v) = 0;

  /// find element with key 'k'
  virtual Value find(const Point &k) = 0;

  /// find element in the defined rectangle
  /// TODO: change container
  virtual std::deque<Value> find(const geometry::box<Point> &r) = 0;

  /// element count in the index
  virtual unsigned int elements(void) = 0;

  /// debug print
  virtual void print(void) const = 0;
	      
  /// destructor
  virtual ~spatial_index(void) {}
};

} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_SPATIAL_INDEX_HPP

