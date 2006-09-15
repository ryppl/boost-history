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

#include <boost/tree/binary_tree.hpp>

#include <boost/tree/searcher.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_unbalance_binary_tree()
{
	using boost::tree::binary_tree;
	using boost::tree::binary_node;
	using boost::tree::tree_cursor;
	using boost::tree::lower_bound;
	
	typedef binary_tree<int> tree_type;
	tree_type mytree;

	tree_type::cursor c, c1, c2, c3, c4;
	tree_type::iterator it;
	
	c = mytree.root();

	BOOST_CHECK(c.empty());
	BOOST_CHECK(c.is_root());
	
	c1 = mytree.insert(c, 1);
	BOOST_CHECK(*c1 == 1);
	BOOST_CHECK(!c1.is_root());
	
	BOOST_CHECK(!c.empty());
	BOOST_CHECK(c.is_root());
	
	BOOST_CHECK(c1.parent() == c);
	
	c2 = mytree.insert(c1, 2);
	BOOST_CHECK(!c.empty());
	BOOST_CHECK(c2.empty());
	BOOST_CHECK(*c1 == 1);
	BOOST_CHECK(*c2 == 2);
	*c1 = 14;
	BOOST_CHECK(*c1 == 14);
	BOOST_CHECK(*c2 == 2);
	BOOST_CHECK(c2.parent() == c1);
	BOOST_CHECK(c1.parent() == c);
	
	c3 = c1.end();
	BOOST_CHECK(c3 == c1.end());
	--c3;
	BOOST_CHECK(c3.parity() == 0);
	BOOST_CHECK(c3.parent() != c3);
	BOOST_CHECK(c3.parent() == c1);
	BOOST_CHECK(c3 == c1.begin());
	BOOST_CHECK(*c3 == 2);
	++c3;
	c4 = mytree.insert(c3, 4);
	BOOST_CHECK(*c4 == 4);
	c4 = c4.parent();
	--c4;
	BOOST_CHECK(*c4 == 2);
	BOOST_CHECK(c4.parent() == c1);
	c = boost::tree::lower_bound(mytree.root(), mytree.shoot(), 2, std::less<int>());
	BOOST_CHECK(*c == 2);
	BOOST_CHECK(c4.empty());

	BOOST_CHECK(*c1 == 14);
	
	BOOST_CHECK(c1.begin().empty() || c1.end().empty());
	
	//c1 = mytree.erase(c1);
	//BOOST_CHECK(*c1 == 2);

	mytree.clear();	
	BOOST_CHECK(mytree.empty());
	
}



int test_main(int, char* [])
{
	test_unbalance_binary_tree();
	return 0;
}
