//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// the documentation is at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html

// the sandbox is at https://svn.boost.org/svn/boost/sandbox/monotonic/

#include <boost/monotonic/vector.h>
#include <boost/monotonic/list.h>
#include <boost/monotonic/map.h>
#include <boost/monotonic/set.h>

#include <boost/iterator/counting_iterator.hpp>

#include <boost/monotonic/rope.h>

#include <boost/timer.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <deque>

#include <boost/range.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>

template <class T
, size_t C = 64
, class Al = std::allocator<T>
, class Link = std::vector<T, Al>
, class Links = std::deque<Link, Al>
>
struct chain;


using namespace std;
using namespace boost;

void test_deque()
{
	monotonic::inline_storage<4000> storage;   // create local storage on the stack
	{
		{
			std::list<int, boost::monotonic::allocator<int> > list(storage);
			fill_n(back_inserter(list), 100, 42);
			cout << "list: " << storage.used() << endl;
		}
		storage.reset();
		{
			std::deque<int, boost::monotonic::allocator<int> > deque(storage);
			fill_n(back_inserter(deque), 100, 42);
			cout << "deque: " << storage.used() << endl;
		}
		storage.reset();

		{
			std::vector<int, boost::monotonic::allocator<int> > vector(storage);
			fill_n(back_inserter(vector), 100, 42);
			cout << "vector: " << storage.used() << endl;
		}
		storage.reset();

		{
			monotonic::chain<int> rope(storage);
			fill_n(back_inserter(rope), 100, 42);
			cout << "default chain: " << storage.used() << endl;
		}
		storage.reset();

		{
			monotonic::chain<int, 100> rope(storage);
			fill_n(back_inserter(rope), 100, 42);
			cout << "chain<100>: " << storage.used() << endl;
		}
		storage.reset();
	}
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
	for (size_t n = 0; n < outter_loops; ++n)
	{
		test_map_list_impl_mono(inner_loops, storage);
		storage.reset(); ///<<< reset the memory usage to zero
	}
	double e0 = t0.elapsed();
	cout << "test_map_list: mono: " << e0 << endl;

	boost::timer t1;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		test_map_list_impl_std(inner_loops);
	}
	double e1 = t1.elapsed();
	cout << "test_map_list:  std: " << e1 << endl;
}

template <class T>
pair<boost::counting_iterator<T>, boost::counting_iterator<T> > range(T start, T end)
{
	typedef boost::counting_iterator<T> cit;
	return std::make_pair(cit(start), cit(end));
}


void test_shared_allocators()
{
	monotonic::inline_storage<500> sa, sb;
	typedef monotonic::allocator<int> Al;
	{
		std::vector<int, Al> v0(sa), v1(sa);
		std::vector<int, Al> v2(sb), v3(sb);
		std::list<int, Al> l0(sa), l1(sb);

		assert(v0.get_allocator() == v1.get_allocator());
		assert(v2.get_allocator() == v3.get_allocator());
		assert(v0.get_allocator() != v2.get_allocator());
		assert(v3.get_allocator() != v1.get_allocator());

		for (int n = 0; n < 10; ++n)
			v0.push_back(n);

		v1 = v0;
		v1.swap(v2);	// swap from different allocators means they are copied
		assert(v1.empty() && v3.empty() && v1 == v3);

		assert(v2 == v0); // both are now [0..9]

		v1.swap(v0);	// swap from same allocators means no copying
		assert(v2 == v1);
		assert(v0 == v3);

		l0.assign(v0.begin(), v0.end());
		l1 = l0;
		assert(l0 == l1);
	}
}

void test_rope();
void test_chain();

template <class List>
void test_bubble_sort_impl(size_t length, List &list)
{
	for (size_t n = 0; n < length; ++n)
		list.push_back(length - n);
	bool swapped = false;
	do
	{
		swapped = false;
		typename List::iterator A = list.begin(), B = --list.end();
		for (--B; A != B; ++A)
		{
			typename List::iterator C = A;
			++C;
			if (*A > *C)
			{
				std::swap(*A, *C);
				swapped = true;
			}
		}
	}
	while (swapped);
}

void test_bubble_sort()
{
	monotonic::inline_storage<100000> storage;
	const size_t count = 50000;
	const size_t length = 20;

	boost::timer mono_timer;
	for (size_t n = 0; n < count; ++n)
	{
	    std::list<int, monotonic::allocator<int> > list(storage);
		test_bubble_sort_impl(length, list);
		storage.reset();
	}
	double mono_total = mono_timer.elapsed();
	cout << "mono bubble sort: " << 1000*1000*mono_total/count << "us" << endl;

	boost::timer std_timer;
	for (size_t n = 0; n < count; ++n)
	{
	    std::list<int> list;
		test_bubble_sort_impl(length, list);
	}
	double std_total = std_timer.elapsed();
	cout << "std  bubble sort: " << 1000*1000*std_total/count << "us" << endl;
}

int main()
{
	test_bubble_sort();
	//test_map_list_realtime();
	return 0;
	//test_chain();
	test_deque();
	//test_rope();
	test_shared_allocators();
	test_alignment();
	test_auto_buffer();
	test_speed();
	test_speed_heap();
	test_basic();
	test_copy();
	test_ctors();
}

//EOF
