// Copyright 2006 Bernhard Reiter.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_AUGMENTORS_INTERVAL_HPP
#define SBOOST_TREE_AUGMENTORS_INTERVAL_HPP


#include "boost/multi_index/composite_key.hpp"
#include "boost/multi_index/member.hpp"


#include <utility> //std::pair

// Interval extractor (for use by key search)
//TODO: concept checks...
//use boost bind or lambda instead!!!
template<typename T, class Interval = std::pair<T,T>, 
		 class ExtractLeft = &std::pair<T,T>::first, 
		 class ExtractRight = &std::pair<T,T>::second,
		 class Left = member<Interval,T,ExtractLeft>, 
		 class Right = member<Interval,T,ExtractRight>
		>
struct interval_extract : public composite_key<Interval, Left, Right> {};


template <typename T>
struct interval_search
{
 public:
	typedef T value_type;
	value_type const& largest_endpoint() { return m_largest_endpoint; }
 protected:
	//update etc.
 private:
	value_type m_largest_endpoint;
};

template <class Cursor>
bool overlaps(Cursor c, typename Cursor::value_type val)
{
	//FIXME: implement.
	return true;
}

#endif // BOOST_TREE_AUGMENTORS_INTERVAL_HPP


