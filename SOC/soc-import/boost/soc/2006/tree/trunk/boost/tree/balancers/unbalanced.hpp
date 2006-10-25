//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_BALANCERS_UNBALANCED_HPP
#define BOOST_TREE_BALANCERS_UNBALANCED_HPP

#include <boost/tree/detail/cursor/nary.hpp>
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
	template <class Tree>
	void add(Tree&, typename Tree::cursor&)
	{ }
	  
	template <class Tree>
	void remove(Tree& t, typename Tree::cursor& x)
	{
		typename Tree::cursor y = x;
		if (x.begin().empty() || x.end().empty())
 			return;
 		inorder::forward(x);
 		return;
	}
	
	template <class Tree>
	void touch(Tree&, typename Tree::cursor&)
	{ }
};

} // namespace balancers
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BALANCERS_UNBALANCED_HPP


