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

#include <boost/tree/nary_tree.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_nary_tree()
{
	using namespace boost::tree;
	
	typedef nary_tree<int> tree_type;
	tree_type mytree;
	tree_type::cursor c = mytree.shoot();
	BOOST_CHECK(mytree.root().empty());
	BOOST_CHECK(c.empty());

	c = mytree.insert(c, 4);
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c == mytree.root());
	BOOST_CHECK(c.empty());
	
	c = mytree.insert(c, 5);
	BOOST_CHECK(*c == 5);
	BOOST_CHECK(c == mytree.root());
	++c;
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c != mytree.root());
	BOOST_CHECK(c.empty());
	BOOST_CHECK(c.m_parent != tree_type::node_type::nil());

	mytree.insert(c.end(), 3);
	BOOST_CHECK(*(c.begin()) == 3);
	BOOST_CHECK(!c.empty());
	BOOST_CHECK(c == c.begin().parent());

}

int test_main(int, char* [])
{
	test_nary_tree();
	return 0;
}
