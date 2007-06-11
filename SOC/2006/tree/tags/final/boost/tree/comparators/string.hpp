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

/** 
 * @file string.hpp
 * Optimising string comparator
 */

#ifndef BOOST_TREE_COMPARATORS_STRING_HPP
#define BOOST_TREE_COMPARATORS_STRING_HPP

#include <algorithm>

namespace boost {
namespace tree {

//concept check Cntnr1::value_type == Cntnr2::value_type
//ditto for size_type
//ForwardContainer? (cause we need quick positioning with size_type...)
template <class Cntnr1, class Cntnr2>
class container_lexicographical_compare : public std::binary_function<Cntnr1, Cntnr2, bool> {
public:
	container_lexicographical_compare(typename Cntnr1::size_type pos = 0) : m_pos(pos) {}
	bool operator() (Cntnr1 const& x, Cntnr2 const& y)
	{
		typename Cntnr1::const_iterator it1 = x.begin();
		typename Cntnr2::const_iterator it2 = y.begin();
		std::advance(it1, m_pos);
		std::advance(it2, m_pos);
		bool ret = std::lexicographical_compare(it1, x.end(), it2, y.end());
		m_pos = std::distance(x.begin(), it1);
		return ret;
	}
private:
 	typename Cntnr1::size_type m_pos;
};

//TODO: even more efficient version for strings (using their compare members)
      
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_COMPARATORS_STRING_HPP


