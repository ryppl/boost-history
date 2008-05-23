// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_SPATIAL_INDEX_SPATIAL_INDEX_HPP
#define BOOST_SPATIAL_INDEX_SPATIAL_INDEX_HPP

#include <vector>
#include <deque>

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

template<typename Key, typename Value>
class spatial_index
{

public:

	/// insert data 'v' with key 'k'
	virtual void insert(const Key &k, const Value &v) = 0;
	
	/// bulk insert data in (v_begin, v_end)
	virtual void bulk_insert(Value &v_begin, Value &v_end, std::vector<Key> &v) = 0;

	/// find element with key 'k'
	virtual Value find(const Key &k) = 0;

	/// find element in the defined rectangle
	/// TODO: change container
	/// TODO: use rectangle from the Geometry Library
	virtual std::deque<Value> find(const double x1, const double x2, const double y1, const double y2) = 0;

	/// element count in the index
	virtual unsigned int elements(void) = 0;
	      
	/// destructor
	virtual ~spatial_index(void) {}
};

} // namespace spatial_index
} // namespace boost

#endif // BOOST_SPATIAL_INDEX_SPATIAL_INDEX_HPP

