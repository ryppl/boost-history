//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/monotonic/vector.h>
#include <boost/monotonic/list.h>
#include <boost/monotonic/map.h>
#include <boost/monotonic/set.h>

#include <boost/timer.hpp>
#include <boost/foreach.hpp>
#include <iostream>

using namespace std;
using namespace boost;

// #define BOOST_MONOTONIC_USE_AUTOBUFFER before including <boost/monotonic/inline_storage.h> to
// try this at home.
void test_auto_buffer()
{
	monotonic::inline_storage<10> storage;

	// this fails because the storage that the vector uses
	// will be moved when the buffer resizes...
	//monotonic::vector<int> vec(storage);
	//for (size_t n = 0; n < 100; ++n)
	//{
	//	vec.push_back(n);
	//}

	// this fails because at the end of the buffer, just before
	// it may be resized and possibly put onto heap, the following asserts
	// on MSVC in <xtree>:
	//		if (max_size() - 1 <= _Mysize)
	//			_THROW(length_error, "map/set<T> too long");
	//
	//monotonic::map<int, int> map(storage);
	//for (int n = 0; n < 100; ++n)
	//	map[n] = n;
}

void test_basic()
{
	monotonic::inline_storage<1000*sizeof(int)> storage1;   // create local storage on the stack
	monotonic::vector<int> v1(storage1);   // create a vector that uses this storage

	for(int i = 0; i < 100; ++i)
		v1.push_back(i);

	size_t len = storage1.remaining();
	monotonic::vector<int> copy(v1);
	size_t len2 = storage1.remaining();

	assert(copy == v1);
	assert(len - len2 == 100*sizeof(int));


	// create the storage that will be used for the various monotonic containers.
	// while it is on the stack here, it could be on the heap as well.
	monotonic::inline_storage<1000> storage;

	// create a list that uses inline, monotonically-increasing storage
	monotonic::list<int> list(storage);
	list.push_back(100);
	list.push_back(400);
	list.erase(list.begin());

	// a map from the same storage
	monotonic::map<int, float> map(storage);
	map[42] = 3.14f;
	assert(map[42] == 3.14f);

	// a set...
	monotonic::set<float> set(storage);
	set.insert(3.14f);
	set.insert(-123.f);
}

void test_copy()
{
	monotonic::inline_storage<1000*sizeof(int)> storage;
	monotonic::vector<int> v1(storage);

	for (int n = 0; n < 100; ++n)
		v1.push_back(n);

	size_t rem1 = storage.remaining();
	monotonic::vector<int> v2(v1);
	size_t rem2 = storage.remaining();

	assert(v2 == v1);
	assert(rem1 - rem2 == 100*sizeof(int));
}

void test_ctors()
{
	monotonic::inline_storage<1000*sizeof(int)> storage;
	string foo = "foo";
	monotonic::vector<char> v1(foo.begin(), foo.end(), storage);
	assert(v1.size() == 3);
	assert(equal(v1.begin(), v1.end(), "foo"));

	monotonic::vector<char> v2(6, 'x', storage);
	assert(v2.size() == 6);
	assert(equal(v2.begin(), v2.end(), "xxxxxx"));

	monotonic::set<char> s2(foo.begin(), foo.end(), storage);
	assert(s2.size() == 2);
	assert(s2.find('f') != s2.end());
	assert(s2.find('o') != s2.end());

	vector<pair<int, string> > v;
	v.push_back(make_pair(42,"foo"));
	v.push_back(make_pair(123,"bar"));
	monotonic::map<int, string> m1(v.begin(), v.end(), storage);
	assert(m1.find(42) != m1.end());
	assert(m1.find(123) != m1.end());

	monotonic::list<int> l1(foo.begin(), foo.end(), storage);
	assert(equal(l1.begin(), l1.end(), "foo"));
}

void test_speed()
{
	typedef monotonic::map<int, monotonic::list<int> > map_with_list;
	monotonic::inline_storage<1000000> storage;
	map_with_list m(storage);
	size_t count = 10000;
	boost::timer timer;
	for (size_t i = 0; i < count; ++i)
	{
		int random = rand() % 100;
		map_with_list::iterator iter = m.find(random);
		if (iter == m.end())
			m.insert(make_pair(random, monotonic::list<int>(storage)));
		else
			iter->second.push_back(i);
	}
	double elapsed = timer.elapsed();
	cout << "monotonic: " << elapsed << endl;

	// do the same thing, with std::containers
	{
		typedef std::map<int, std::list<int> > map_with_list;
		map_with_list m;
		boost::timer timer;
		for (size_t i = 0; i < count; ++i)
		{
			int random = rand() % 100;
			map_with_list::iterator iter = m.find(random);
			if (iter == m.end())
				m[random] = std::list<int>();
			else
				iter->second.push_back(i);
		}
		double elapsed = timer.elapsed();
		cout << "std: " << elapsed << endl;
	}
}

void test_speed_heap()
{
	size_t num_iterations = 100000;

	typedef monotonic::map<int, monotonic::list<int> > map_with_list;
	monotonic::inline_storage<10000000> *storage = new monotonic::inline_storage<10000000>;

	// do the test with monotonic containers and heap-based storage
	{
		map_with_list m(*storage);
		boost::timer timer;
		for (size_t i = 0; i < num_iterations; ++i)
		{
			int random = rand() % 100;
			map_with_list::iterator iter = m.find(random);
			if (iter == m.end())
				m.insert(make_pair(random, monotonic::list<int>(*storage)));
			else
				iter->second.push_back(i);
		}
		double elapsed = timer.elapsed();
		cout << "monotonic: " << elapsed << endl;
	}
	delete storage;

	// do the same thing, with std::containers
	{
		typedef std::map<int, std::list<int> > map_with_list;
		map_with_list m;
		boost::timer timer;
		for (size_t i = 0; i < num_iterations; ++i)
		{
			int random = rand() % 100;
			map_with_list::iterator iter = m.find(random);
			if (iter == m.end())
				m[random] = std::list<int>();
			else
				iter->second.push_back(i);
		}
		double elapsed = timer.elapsed();
		cout << "std: " << elapsed << endl;
	}
}

namespace
{

	template<typename C>
	struct Foo 
	{
		long ord;
		C c;
	};

	const int LOOP_COUNT = 100000000;
	const int ELEM_COUNT = 1000;

	template<typename C>
	void test_loop_monotonic()
	{
		boost::monotonic::inline_storage<100000> storage;
		boost::monotonic::vector<Foo<C> > vec(storage);
		Foo<C> orig = { 'A', 65 };      
		vec.assign(ELEM_COUNT, orig);
		boost::timer timer;
		for (int i = 0; i < LOOP_COUNT; ++i)
			++vec[1 + i % (ELEM_COUNT - 2)].ord;
		double elapsed = timer.elapsed();
		std::cout << "Incrementing ord = " << 1000000000*elapsed/LOOP_COUNT << " ps per iteration" << std::endl;
	}

	template <class C>
	void test_loop_std()
	{
		Foo<C> orig = { 'A', 65 };      
		std::vector<Foo<C> > vec;
		vec.assign(ELEM_COUNT, orig);
		boost::timer timer;
		for (int i = 0; i < LOOP_COUNT; ++i)
			++vec[1 + i % (ELEM_COUNT - 2)].ord;
		double elapsed = timer.elapsed();
		std::cout << "STD: Incrementing ord = " << 1000000000*elapsed/LOOP_COUNT << " ps per iteration" << std::endl;
	}

} // namespace

void test_alignment()
{
	monotonic::inline_storage<10000> storage;

	// the two arguments to storage.allocate are the size, and the required alignment
	void *P = storage.allocate(3, 4);
	assert(P == storage.begin() + 0);
	
	P = storage.allocate(3, 4);
	assert(P == storage.begin() + 4);
	
	P = storage.allocate(11, 4);
	assert(P == storage.begin() + 8);

	P = storage.allocate(11, 16);
	assert(P == storage.begin() + 32);

	typedef boost::array<char, 3> c0;
	typedef boost::array<char, 6> c1;
	typedef boost::array<char, 11> c2;
	typedef boost::array<char, 31> c3;
	typedef boost::array<char, 33> c4;
	typedef boost::array<char, 57> c5;
	typedef boost::array<char, 111> c6;

	monotonic::vector<c0> v0(storage);
	monotonic::vector<c1> v1(storage);
	monotonic::vector<c2> v2(storage);
	monotonic::vector<c3> v3(storage);
	monotonic::vector<c4> v4(storage);
	monotonic::vector<c5> v5(storage);
	monotonic::vector<c6> v6(storage);

	v0.resize(5);
	v1.resize(5);
	v2.resize(5);
	v3.resize(5);
	v4.resize(5);
	v5.resize(5);
	v6.resize(5);
#define write_cn(n) \
	BOOST_FOREACH(c ## n &c, v ## n) \
		c = c ## n();
	write_cn(0);
	write_cn(1);
	write_cn(2);
	write_cn(3);
	write_cn(4);
	write_cn(5);
	write_cn(6);
#undef write_cn


	test_loop_monotonic<char>();
	test_loop_monotonic<long>();

	test_loop_std<char>();
	test_loop_std<short>();

}

template <class List>
double test_list_speed_impl(size_t count, List &l0, List &l1, List &l2, List &both)
{
	boost::timer timer;
	std::generate_n(std::back_inserter(l0), count, rand);
	std::generate_n(std::back_inserter(l1), count, rand);
	std::generate_n(std::back_inserter(l2), count, rand);
	BOOST_FOREACH(int n, l0)
	{
		BOOST_FOREACH(int m, l1)
		{
			BOOST_FOREACH(int p, l2)
			{
				if (m == n && m == p)
					both.push_back(m);
			}
		}
	}
	double elapsed = timer.elapsed();
	//cout << "test_list_speed[" << count << "]" << elapsed << endl;
	return elapsed;
}

void test_list_speed()
{
	monotonic::inline_storage<1000000> *s = new monotonic::inline_storage<1000000>();
	size_t count = 50;
	double mt = 0;
	double st = 0;

	for (size_t n = 0; n < 10; ++n, count += 100)
	{
		srand(42);
		// use monotonic::
		boost::timer t0;
		{
			monotonic::list<int> l0(*s), l1(*s), l2(*s), both(*s);
			mt += test_list_speed_impl(count, l0, l1, l2, both);
		}
		mt += t0.elapsed();

		srand(42);
		// use std::
		boost::timer t1;
		{
			std::list<int> l0, l1, l2, both;
			st += test_list_speed_impl(count, l0, l1, l2, both);
		}
		st += t1.elapsed();

		s->reset();
	}
	cout << "test_list_speed: mono: " << mt << endl;
	cout << "test_list_speed: std : " << st << endl;

	delete s;
}

// part of the test_map_list_realtime test.
// this is to be like running part of a simulation.
void test_map_list_impl_mono(size_t count, monotonic::storage_base &storage)
{
	typedef monotonic::list<int> List;
	typedef monotonic::map<int, List> Map;
	Map map(storage);
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		Map::iterator iter = map.find(random);
		if (iter == map.end())
		{
			map.insert(make_pair(random, List(storage)));
		}
		else
		{
			iter->second.push_back(n);
		}
	}
}

// same as test_map_list_impl_mono, but using std::containers
void test_map_list_impl_std(size_t count)
{
	typedef std::list<int> List;
	typedef std::map<int, List> Map;
	Map map;
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		Map::iterator iter = map.find(random);
		if (iter == map.end())
		{
			map.insert(make_pair(random, List()));
		}
		else
		{
			iter->second.push_back(n);
		}
	}
}

// the purpose of this test is to simulate a `realtime system`
// which does an amount of work each frame, then does a similar 
// amount of work the next frame.
void test_map_list_realtime()
{
	monotonic::inline_storage<1000000> storage;
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	boost::timer t0;
	for (int n = 0; n < outter_loops; ++n)
	{
		test_map_list_impl_mono(inner_loops, storage);	
		storage.reset(); ///<<< reset the memory usage to zero
	}
	double e0 = t0.elapsed();
	cout << "test_map_list: mono: " << e0 << endl;

	boost::timer t1;
	for (int n = 0; n < outter_loops; ++n)
	{
		test_map_list_impl_std(inner_loops);	
	}
	double e1 = t1.elapsed();
	cout << "test_map_list:  std: " << e1 << endl;
}

int main()
{
	test_alignment();
	test_auto_buffer();
	test_speed();
	test_speed_heap();
	test_map_list_realtime();
	test_basic();
	test_copy();
	test_ctors();
}

//EOF
