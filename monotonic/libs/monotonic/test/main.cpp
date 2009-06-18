//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// the documentation is at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html

// the sandbox is at https://svn.boost.org/svn/boost/sandbox/monotonic/

// commented-out till i add the correct libs for boost::mutex
//#include <boost/monotonic/shared_storage.hpp>
//#include <boost/monotonic/shared_allocator.hpp>

#include <boost/monotonic/local.hpp>
#include <boost/monotonic/static_storage.hpp>

#include <boost/monotonic/vector.hpp>
#include <boost/monotonic/list.hpp>
#include <boost/monotonic/map.hpp>
#include <boost/monotonic/set.hpp>


#include <boost/iterator/counting_iterator.hpp>


#include <boost/timer.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <deque>
#include <sstream>

#include <boost/range.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/array.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/monotonic/chain.hpp>
#include <boost/monotonic/storage.hpp>
////#include <boost/monotonic/shared_allocator.hpp>

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
	monotonic::local<> storage;
	{
		{
			std::list<int, boost::monotonic::allocator<int> > list;
			fill_n(back_inserter(list), 100, 42);
			cout << "list: " << storage.used() << endl;
		}
		storage.reset();
		{
			std::deque<int, boost::monotonic::allocator<int> > deque;
			fill_n(back_inserter(deque), 100, 42);
			cout << "deque: " << storage.used() << endl;
		}
		storage.reset();

		{
			std::vector<int, boost::monotonic::allocator<int> > vector;
			fill_n(back_inserter(vector), 100, 42);
			cout << "vector: " << storage.used() << endl;
		}
		storage.reset();

		{
			monotonic::chain<int> chain;
			fill_n(back_inserter(chain), 100, 42);
			cout << "default chain: " << storage.used() << endl;
		}
		storage.reset();

		{
			monotonic::chain<int, 100> chain;
			fill_n(back_inserter(chain), 100, 42);
			cout << "chain<100>: " << storage.used() << endl;
		}
		storage.reset();
	}
}

void test_speed()
{
	monotonic::local<monotonic::storage<1000000> > storage;
	typedef monotonic::map<int, monotonic::list<int> > map_with_list;
	{
		map_with_list m;
		size_t count = 10000;
		boost::timer timer;
		for (size_t i = 0; i < count; ++i)
		{
			int random = rand() % 100;
			map_with_list::iterator iter = m.find(random);
			if (iter == m.end())
				m.insert(make_pair(random, monotonic::list<int>()));
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
		monotonic::local<> storage;
		std::vector<Foo<C>, monotonic::allocator<Foo<C> > > vec;
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
	monotonic::fixed_storage<10000> storage;

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

	monotonic::vector<c0> v0;
	monotonic::vector<c1> v1;
	monotonic::vector<c2> v2;
	monotonic::vector<c3> v3;
	monotonic::vector<c4> v4;
	monotonic::vector<c5> v5;
	monotonic::vector<c6> v6;

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



template <class T>
pair<boost::counting_iterator<T>, boost::counting_iterator<T> > range(T start, T end)
{
	typedef boost::counting_iterator<T> cit;
	return std::make_pair(cit(start), cit(end));
}


void test_chain();

#include "test_map_list.cpp"
#include "test_bubble_sort.cpp"
#include "test_dupe.cpp"
#include "test_chained_storage.cpp"
#include "test_shared_storage.cpp"
#include "compare_memory_pool.cpp"

namespace boost 
{ 
	namespace monotonic 
	{
		static_storage_base<> default_static_storage;
		storage_base *static_storage = &default_static_storage;

		//storage<> static_storage_base<storage<> >::global;
	}
}

void test_static_storage()
{
	// reset the global static storage to zero use
	boost::monotonic::get_storage().reset();

	typedef std::list<int, monotonic::allocator<int> > List;
	typedef std::map<int, List, std::less<int>, monotonic::allocator<int> > Map;
	{
		List list0, list1;
		Map map;
		list0.push_back(1);
		list1.push_back(2);
		list0.splice(list0.begin(), list1);
		map[42] = list0;
		map[123] = list1;
	}
	cout << monotonic::get_storage().used() << endl;
	boost::monotonic::get_storage().reset();
}

void run_all_tests()
{
	test_static_storage();
	test_shared_storage();
	test_chained_storage();
	test_map_list_heap_stack();
	test_dupe();
	graph_bubble_sort();
	test_bubble_sort();
	//test_chain();
	test_deque();
	test_alignment();
	//test_speed();
	//test_speed_heap();
}

void test_mono_map()
{
	monotonic::local<> store;
/*
// commented out till i add the required libraries for boost::mutex :/

	monotonic::shared_storage<> shared_store;
	{
		typedef std::list<int, monotonic::shared_allocator<int> > List;
		typedef std::vector<List, monotonic::shared_allocator<List> > Vector;

		BOOST_STATIC_ASSERT(monotonic::detail::is_monotonic<List>::value);
		BOOST_STATIC_ASSERT(monotonic::detail::is_monotonic<Vector>::value);

		Vector vec(shared_store);
		
		vec.resize(1);
		BOOST_ASSERT(vec[0].get_allocator().get_storage() == vec.get_allocator().get_storage());
		vec[0].push_back(42);
	}
*/

	// this can't work in general:
	//{
	//	typedef std::list<int, monotonic::allocator<int> > List;
	//	BOOST_STATIC_ASSERT(monotonic::detail::is_monotonic<List>::value);
	//	typedef std::map<int, List, std::less<int>, monotonic::allocator<int> > Map;
	//	BOOST_STATIC_ASSERT(monotonic::detail::is_monotonic<Map>::value);
	//	Map map(less<int>(), store);
	//	//map[42].push_back(123);
	//	map.insert(make_pair(42, List(store)));
	//	map[42].push_back(123);
	//	BOOST_ASSERT(map[42].get_allocator().get_storage() == map.get_allocator().get_storage());
	//}

	// but this can:
	{
		typedef monotonic::list<int> List;
		BOOST_STATIC_ASSERT(monotonic::detail::is_monotonic<List>::value);
		typedef monotonic::map<int, List > Map;
		BOOST_STATIC_ASSERT(monotonic::detail::is_monotonic<Map>::value);
		Map map;
		map[42].push_back(123);
		BOOST_ASSERT(map[42].get_allocator().get_storage() == map.get_allocator().get_storage());
	}

	{
		typedef monotonic::map<int, monotonic::map<int, monotonic::list<int> > > Map;
		Map map;
		map[42][64].push_back(13);
		BOOST_ASSERT(map[42][64].get_allocator().get_storage() == map.get_allocator().get_storage());
	}
}

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//warning C4297: 'straight_to_debugger' : function assumed not to throw an exception but does
#pragma warning(disable:4297)
extern "C" void straight_to_debugger(unsigned int, EXCEPTION_POINTERS*)
{
	throw;
}
#endif


int main()
{
#ifdef WIN32
	_set_se_translator(straight_to_debugger);
#endif

	//test_chained_storage();
	test_map_list_heap_stack();
	//compare_memory_pool();
	//test_mono_map();
	//test_mono_map();
	//test_static_storage();
	//run_all_tests();
}

//EOF
