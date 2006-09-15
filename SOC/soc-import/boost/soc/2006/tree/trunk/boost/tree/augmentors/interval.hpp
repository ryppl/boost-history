//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_AUGMENTORS_INTERVAL_HPP
#define SBOOST_TREE_AUGMENTORS_INTERVAL_HPP

//#include <boost/numeric/interval.hpp> //useful ?

#include "boost/multi_index/composite_key.hpp"
#include "boost/multi_index/member.hpp"

namespace boost {
namespace tree {
namespace augmentors {

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

} // namespace augmentors
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_AUGMENTORS_INTERVAL_HPP


