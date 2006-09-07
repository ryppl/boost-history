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
 * @file treap.hpp
 * Treap balancer and metadata
 */
//TODO: maybe have treap really model binary_heap?

#ifndef BOOST_TREE_BALANCERS_TREAP_HPP
#define BOOST_TREE_BALANCERS_TREAP_HPP

#include <limits.h>
#include <stdlib.h>


namespace boost {
namespace tree {

class treap_metadata {
public:	
	treap_metadata()
	: m_priority(0) {}
	
	int const get_priority() const
	{
		return m_priority;
	}
	
	void set_priority(int priority)
	{
		m_priority = priority;
	}
private:
	int m_priority;
};

//make the following part of tree? that is, derive tree from this (as templ. arg)?
//template <class Tree> //tentative approach using CRTP.
class treap_balance {
public:
	typedef treap_metadata metadata_type;
	
	template <class Cursor>
	static void add(Cursor x, Cursor header)
	{
		x.metadata().set_priority((lrand48() >> 1) + 1);
		while (!x.is_root() && (x.metadata().get_priority() > x.parent().metadata().get_priority()))
			x.rotate();
	}
	  
	template <class Cursor>
	static void remove(Cursor p) //why not tree instead of header?
	{
		Cursor q;
		while (empty(q = p.begin() + 
			   (p.begin().metadata().get_priority() > 
				p.end().metadata().get_priority())))
			q.metadata().rotate();
		p.metadata().splice_out(p.parity());
	}
	
	template <class Cursor>
	static void read(Cursor)
	{ }
  
};

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_BALANCERS_TREAP_HPP


