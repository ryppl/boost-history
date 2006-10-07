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
 * @file unaugmented.hpp
 * Trivial augmentor (does nothing)
 */

#ifndef BOOST_TREE_AUGMENTORS_UNAUGMENTED_HPP
#define BOOST_TREE_AUGMENTORS_UNAUGMENTED_HPP

namespace boost {
namespace tree {
namespace augmentors {

/**
 * Trivial augmentor (does nothing)
 */
class unaugmented {
 public:
 	struct metadata_type {};
 protected:
	template <class Tree>
 	void pre_rotate(Tree&, typename Tree::cursor&)
 	{ }
 	
 	template <class Tree>
 	void pre_detach(Tree&, typename Tree::cursor&)
 	{ }
 	
 	template <class Tree>
 	void pre_detach(Tree&, typename Tree::cursor&, typename Tree::cursor&)
 	{ }
 	
 	template <class Tree>
 	void descend(Tree&, typename Tree::cursor&)
 	{ }
};

} // namespace augmentors
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_AUGMENTORS_UNAUGMENTED_HPP


