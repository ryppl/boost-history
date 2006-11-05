//  Copyright (c) 2006, Bernhard Reiter
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

#include <boost/tree/detail/cursor/nary.hpp>

#include <limits.h>
#include <stdlib.h>

using boost::tree::access_rotate;

namespace boost {
namespace tree {
namespace balancers {

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


class treap : public access_rotate {
 public:
	typedef treap_metadata metadata_type;
	
// protected:
	// TODO: do we ever want to modify the second par directly?
	template <class Tree>
	static void add(Tree& t, typename Tree::cursor& x) 
	{
		x->metadata().set_priority((lrand48() >> 1) + 1); // Move this to metadata ctor?
		while ((x != t.root()) && 
			   (x->metadata().get_priority() > 
			   	x.parent()->metadata().get_priority()))
			t.rotate(x);
	}
	  
	template <class Tree>
	static void remove(Tree& t, typename Tree::cursor& p)
	{
		typename Tree::cursor q;
		while((q = ((p.begin()->metadata().get_priority()
					 > p.end()->metadata().get_priority())
					 ? p.begin() : p.end())).empty())
			access_rotate::rotate(q);
		q = p;
	}
	
	template <class Tree>
	static void touch(Tree&, typename Tree::cursor&)
	{ }
};

} // namespace balancers
} // namespace tree
} // namespace boost


#endif // BOOST_TREE_BALANCERS_TREAP_HPP


