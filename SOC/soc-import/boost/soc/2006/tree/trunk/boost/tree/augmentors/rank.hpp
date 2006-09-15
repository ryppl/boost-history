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

#ifndef BOOST_TREE_AUGMENTORS_RANK_HPP
#define BOOST_TREE_AUGMENTORS_RANK_HPP

namespace boost {
namespace tree {
namespace augmentors {
	
//how do we know what size_type to use?
//1. from base_type
//2. from "instantiate node type with trivial augmentor" - size_type. cool.

template <typename SizeType = std::size_t>
class rank_search {
 public:
	typedef SizeType size_type;
	//get_rank - just show rank info	
	
	size_type const get_rank() const
	{
		return m_rank;
	}
	
	void set_rank(size_type priority)
	{
		m_rank = rank;
	}
	
	//operator[] ? rather not...

private:
	size_type m_rank;
};

class rank {
 protected:
	//or metadata as param-type?
	template <class Cursor>
 	void pre_rotate(Cursor& c)
 	{
 	}
 	
 	template <class Cursor>
 	void pre_detach(Cursor& c)
 	{
 	}
 	
 	template <class Cursor>
 	void pre_detach(Cursor& c, Cursor& c)
 	{
 	}
};
//cursor part: what is my rank?
//tree part: go to rank...
//pre_rotate, pre_detach.

} // namespace augmentors
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_AUGMENTORS_RANK_HPP


