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
#include <boost/tree/balancers/red_black.hpp>
#include <boost/tree/augmentors/unaugmented.hpp>

#include <boost/tree/searcher.hpp>

#include "helpers.hpp"

#include <boost/test/minimal.hpp>

using boost::tree::binary_tree;

//TODO: Make this a test suite.

void test_red_black_tree()
{
	using namespace boost::tree;
	
	typedef binary_tree<int, balancers::red_black> tree_t;
	typedef searcher<false, tree_t> searcher_t;
	searcher_t my_searcher;
	
	searcher_t::iterator it, c1, c2, c3, c4, c5;
	searcher_t::const_iterator cit;

	it = my_searcher.end();
//	BOOST_CHECK(!c.has_child());
//	
	//c1 = my_searcher.insert(c, 8);
	c1 = my_searcher.insert(it, 8);
//	

//	cit = my_searcher.begin();
	BOOST_CHECK(*c1 == 8);

//	my_searcher.insert(5);
//	my_searcher.insert(it, 7);
//	my_searcher.insert(it, 12);
//	my_searcher.insert(it, 14);
//	my_searcher.insert(it, 2);
//	my_searcher.insert(it, 11);
	
//	searcher_t::container_type core = my_searcher.get_container();
//	searcher_t::container_type::cursor c = core.root().begin();
//	BOOST_CHECK(*c == 8);

//	BOOST_CHECK(c1.parent() == c); //Maybe factor out specifc tests (parent(), begin(), ...)	
//	BOOST_CHECK(c1 == my_searcher.end().begin()); //FIXME. end means root
//	BOOST_CHECK(c1 != my_searcher.end().end()); //ditto
//	BOOST_CHECK(*c1 == 8);
//	BOOST_CHECK(!c1.has_child());
//	
//	BOOST_CHECK(c.has_child());
//	BOOST_CHECK(c == my_searcher.end());
	
//	ret = key_lower_bound(my_searcher.end(), 18, 
//									   lower_bound_wrapper<mycursor, int, std::less<int> >(),
//									   boost::multi_index::identity<int>(), std::less<int>());
									   
//	BOOST_CHECK(ret.first == my_searcher.root().end());
//	BOOST_CHECK(ret.second.first);
//	//BOOST_CHECK(ret.first.m_pos == 1);	
//	c2 = my_searcher.insert(c, 18); //so where does that go?
//	//BOOST_CHECK(c2.parent() == c1);
//	//BOOST_CHECK(c1.end() == c2);	
//	//BOOST_CHECK(c2 != my_searcher.root().end());	
//		
//	BOOST_CHECK(*c2 == 18);
//	BOOST_CHECK(!c2.has_child());
//
//	BOOST_CHECK(c2 == my_searcher.root().end().begin());		
//	//BOOST_CHECK(c2.m_parent == c.m_parent);	
//	//BOOST_CHECK(c2 == c.m_parent);	
//	//BOOST_CHECK(c1.has_child());  ///also fails!!!
//	
//
//	
//	c2 = my_searcher.insert(c, 31);
//	
//	c2 = my_searcher.insert(c, 412);
//	c3 = my_searcher.insert(c, 39);
//	
//	c4 = my_searcher.insert(c, 7);
//	
//	BOOST_CHECK(*(c2.parent()) == 31);
//	BOOST_CHECK(*(c2.begin()) == 39);
//	
//	//BOOST_CHECK(c4.parent() == c1);
//
//	BOOST_CHECK(*c3 == 39);
//	BOOST_CHECK(c4.parent() == c1);
//	BOOST_CHECK(c1.begin() == c4);

		
	//*(mytree.root()) = -5;
	//c = my_searcher.root();
	//my_searcher.c();
//	//*c = 5;
////	//BOOST_CHECK(*c == 5);
////	
//	c1 = mytree.insert(c, 1);
//	BOOST_CHECK(*c == 1);
//	c2 = mytree.insert(c1, 2);
//	BOOST_CHECK(*c1 == 1);
//	BOOST_CHECK(*c2 == 2);


//	*c1 = 14; //how can we forbid this? by setting key to int const&
//	BOOST_CHECK(*c1 == 14);
//	BOOST_CHECK(*c2 == 2);
//	BOOST_CHECK(c2.parent() == c1);
//	BOOST_CHECK(c1.parent() == c);
//	
//	c3 = c1.end();
//	--c3;
//	BOOST_CHECK(*c3 == 2);
//	++c3;
//	c4 = mytree.insert(c3, 4);
//	BOOST_CHECK(*c4 == 4);
//	c4 = c4.parent();
//	--c4;
//	BOOST_CHECK(*c4 == 2);
//	
//	//BOOST_CHECK(c4.parent() == c1);
		
}


int test_main(int, char* [])
{
	test_red_black_tree();
	return 0;
}
