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
	
//TODO: lots. current implementation is dysfunctional.

template <typename SizeType>
class rank_data {
 public:
	typedef SizeType size_type;
	//get_rank - just show rank info	
	
//	size_type const get_rank() const
//	{
//		return m_rank;
//	}
//	
//	void set_rank(size_type priority)
//	{
//		m_rank = rank;
//	}
	
	rank_data(size_type r = 1)
	: m_rank(r) {}
	
	size_type& rank()
	{
		return m_rank;
	}

	size_type const& rank() const
	{
		return m_rank;
	}
		
	//operator[] ? rather not...
	
	
private:
	size_type m_rank;
};

template <typename SizeType = std::size_t>
class rank {
 public:
 	typedef SizeType size_type;
 	typedef rank_data<size_type> metadata_type;
 	
 protected:
	//or metadata as param-type?
	template <class Tree>
 	void pre_rotate(Tree&, typename Tree::cursor& q)
 	{
 		typename Tree::cursor p = q->parent();
		if (!q.parity())
			p.metadata().rank() -= q.metadata().rank();
		else
			q.metadata().rank() += p.metadata().rank();
 	}
 	
 	template <class Tree>
 	void pre_detach(Tree& t, typename Tree::cursor& y)
 	{
 		typename Tree::cursor p = y;
 		while (p.parent() != t.root()) {
 			if (!p.parity())
 				--p.parent().metadata().rank();
 			p = p.parent();
 		}
 	}
 	
 	template <class Tree>
 	void pre_detach(Tree& t, typename Tree::cursor& y, typename Tree::cursor& z)
 	{
 		typename Tree::cursor p = y;
 		while (p.parent() != t.root()) {
 			if (!p.parity())
 				--p.parent().metadata().rank();
 			p = p.parent();
 		}
 		if (z == t.root())
 			y.metadata().rank() = z.metadata().rank();
 	}

 	template <class Tree>
 	void descend(Tree&, typename Tree::cursor& p)
 	{
 		if (p.parity() == 0) {
 			++p.metadata().rank();
 		}
 	}
 	
};

} // namespace augmentors

// This is actually some kind of algorithm...
template <class Tree>
typename Tree::const_cursor select_rank(Tree const& tree, typename Tree::augmentor_type::size_type i)
{
	typename Tree::const_cursor p = tree.root().begin();
	if (p.metadata().rank() < i) {
		return p;
	}
	++i;
	p = p.begin();
	for (;;) {
		if (i < p.metadata().rank())
			p = p.begin();
		else {
			i -= p.metadata().rank();
			if (i == 0)
				return p;
			p = p.end();
		}
	}
}

template <class Tree>
typename Tree::cursor select_rank(Tree& tree, typename Tree::augmentor_type::size_type i)
{
	typename Tree::cursor p = tree.root().begin();
	if (p.metadata().rank() < i) {
		return p;
	}
	++i;
	p = p.begin();
	for (;;) {
		if (i < p.metadata().rank())
			p = p.begin();
		else {
			i -= p.metadata().rank();
			if (i == 0)
				return p;
			p = p.end();
		}
	}
}

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_AUGMENTORS_RANK_HPP


