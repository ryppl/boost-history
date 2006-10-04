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

#ifndef BOOST_TREE_BALANCERS_UNBALANCED_HPP
#define BOOST_TREE_BALANCERS_UNBALANCED_HPP

#include <boost/tree/detail/cursor/binary.hpp>
#include <boost/tree/inorder.hpp>

using boost::tree::access_rotate;

namespace boost {
namespace tree {
namespace balancers {

//struct unbalance_metadata {};

class unbalanced : public access_rotate {
 public:
	unbalanced() {}

	struct metadata_type {};
	metadata_type metadata;

	template <class Cursor>
	void rotate(Cursor& c)
	{
		access_rotate::rotate(c);
	}

 protected:	
	template <class Cursor>
	void add(Cursor&, Cursor const&)
	{ }
	  
	template <class Cursor>
	void remove(Cursor& x, Cursor& y)
	{
		y = x;
		if (x.begin().empty() || x.end().empty())
 			return;
 		inorder::forward(x);
 		return;
	}
	
	template <class Cursor>
	void touch(Cursor&, Cursor const&)
	{ }
};

} // namespace balancers
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BALANCERS_UNBALANCED_HPP


