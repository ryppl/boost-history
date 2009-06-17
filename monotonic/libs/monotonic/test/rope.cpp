//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// the documentation is at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html

// the sandbox is at https://svn.boost.org/svn/boost/sandbox/monotonic/

#include <boost/monotonic/vector.hpp>
#include <boost/monotonic/list.hpp>
#include <boost/monotonic/map.hpp>
#include <boost/monotonic/set.hpp>

#include <boost/timer.hpp>
#include <boost/foreach.hpp>
#include <iostream>

#include <boost/range.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <boost/monotonic/chain.hpp>

using namespace boost;
using namespace std;

void test_iter_range()
{
	std::vector<int> v(10);
	boost::iter_range<std::vector<int> > r(v);
	*r = 10;
	*r++ = 10;
	//while (r)
	//{
	//	*r++ = 10;
	//}
	boost::const_iter_range<std::vector<int> > c(r);
}

struct Foo : boost::noncopyable
{
	int n;
	Foo(int N) : n(N) { }
};

void test_chain()
{
	{
		monotonic::storage<> storage;
		{
			monotonic::chain<int, 100> rope(storage);
			for (int n = 0; n < 200; ++n)
			{
				rope.push_back(n);
			}
		}
	}


	monotonic::storage<> storage;
	{
		typedef monotonic::chain<Foo, 2> Rope2;
		Rope2 foo(4, storage);

		typedef monotonic::chain<int, 2> Rope;
		Rope rope(storage);
		rope.push_back(0);
		rope.push_back(1);
		rope.push_back(2);
		rope.push_back(3);

		Rope::iterator A = rope.begin(), B = rope.end();
		for (; A != B; ++A)
		{
			*A *= 2;
		}
		Rope::iterator Q = rope.begin();
		*Q++ = 13;

		Rope::const_iterator C = rope.begin(), D = rope.end();
		for (; C != D; ++C)
		{
			cout << *C;
		}

		BOOST_FOREACH(int n, rope)
		{
			cout << n << endl;
		}

		BOOST_FOREACH(int &n, rope)
		{
			n *= 2;
		}
		BOOST_FOREACH(int n, rope)
		{
			cout << n << endl;
		}


		rope[0] = 0;
		rope[1] = 1;
		rope[2] = 2;
		rope[3] = 3;
		assert(*rope.begin() == 0);
		assert(rope.at(0) == 0);
		assert(rope.at(1) == 1);
		assert(rope.at(2) == 2);
		assert(rope.at(3) == 3);
	}
}

//EOF
