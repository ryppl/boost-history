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

#include <boost/tree/forest.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_forest()
{
	using namespace boost::tree;
	
	typedef forest<int> tree_type;
	tree_type mytree;
	
	tree_type::cursor c = mytree.root();
	tree_type::base_cursor cur = tree_type::base_cursor(c);
	BOOST_CHECK(!cur.parity());
	cur = cur.parent();
	BOOST_CHECK(cur.parity());
	
	c = mytree.insert(c, 6);	
	BOOST_CHECK(*c == 6);
	cur = tree_type::base_cursor(c);
	BOOST_CHECK(*cur == 6);
	
	BOOST_CHECK(cur == mytree.c.root().begin());
	
	c = mytree.insert(c, 5);	
	BOOST_CHECK(*c == 5);
	cur = tree_type::base_cursor(c);
	BOOST_CHECK(cur == mytree.c.root().begin());

	c = mytree.insert(c, 4);	
	BOOST_CHECK(*c == 4);
	BOOST_CHECK(c == mytree.root().begin());
	
	cur = tree_type::base_cursor(c);
	BOOST_CHECK(cur == mytree.c.root().begin());

//	++cur;
//	cur = cur.begin();	
//	BOOST_CHECK(*cur == 5);
//	cur = cur.parent();
//	--cur;

	++c;
	BOOST_CHECK(*c == 5);
	++c;
	BOOST_CHECK(*c == 6);
//	--c;
//	BOOST_CHECK(*c == 5);
//	--c;
//	BOOST_CHECK(*c == 4);
	
	
//	cur = tree_type::base_cursor(c);
//	BOOST_CHECK(*cur == 4);
//	//cur = cur.parent().parent().parent().begin();
//	BOOST_CHECK(*cur == 4);
	
	//BOOST_CHECK(*++c == 5);

}



int test_main(int, char* [])
{
	test_forest();
	return 0;
}
