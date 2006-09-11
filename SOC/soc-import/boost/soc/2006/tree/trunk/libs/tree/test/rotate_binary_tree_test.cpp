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

#include "helpers.hpp"

#include <boost/test/minimal.hpp>

using boost::tree::binary_tree;

//TODO: Make this a test suite.

void test_rotate()
{
	binary_tree<int> mytree;
	test_data_tree(mytree);
	binary_tree<int>::cursor c = mytree.root().begin().end();
	BOOST_CHECK(*c.begin() == 6);

	BOOST_CHECK(*c.parent() == 8);
	BOOST_CHECK(*c.parent().begin() == 3); // invariant candidate
	
	BOOST_CHECK(*--c == 3); // differently (not invariantly!) spoken
	BOOST_CHECK(*c.begin() == 1);
	BOOST_CHECK(*((++c).begin()).begin() == 4);
	BOOST_CHECK(*(++c.begin()).begin() == 7);
	
	BOOST_CHECK(*c.begin() == 6);
	
	mytree.rotate(c); // Left rotate

	BOOST_CHECK(*c.begin() == 6);
	BOOST_CHECK(*c.parent().begin() == 8);
	
	BOOST_CHECK(*c.end().begin() == 7);
	BOOST_CHECK(*c.begin().begin() == 3);
	BOOST_CHECK(*c.begin().begin().begin() == 1);

	BOOST_CHECK(*c.begin().end().begin() == 4);

	c = c.begin();
	BOOST_CHECK(*c.begin() == 3);
	
	mytree.rotate(c); // Right rotate
	
	BOOST_CHECK(*c.begin() == 3);
	c = c.end();

	BOOST_CHECK(*c.begin() == 6);

	BOOST_CHECK(*c.parent() == 8);
	BOOST_CHECK(*c.parent().begin() == 3); // other invariant candidate
	
	BOOST_CHECK(*--c == 3);
	BOOST_CHECK(*c.begin() == 1);
	BOOST_CHECK(*((++c).begin()).begin() == 4);
	BOOST_CHECK(*(++c.begin()).begin() == 7);
	
	BOOST_CHECK(*c.begin() == 6);
//	BOOST_CHECK(*c.parent().parent().begin() == 6);
//	BOOST_CHECK(*c.parent().parent().end().begin() == 7);
	
//	BOOST_CHECK(*c.begin() == 1);
//	BOOST_CHECK(*c.parent().begin() == 3); // invariant?
//	
//	BOOST_CHECK(*c.parent().parent().begin() == 6);
//	BOOST_CHECK(*c.parent().parent().parent().begin() == 8);
//	BOOST_CHECK(*c.parent().parent().end().begin() == 7);
	
}


int test_main(int, char* [])
{
	test_rotate();
	return 0;
}
