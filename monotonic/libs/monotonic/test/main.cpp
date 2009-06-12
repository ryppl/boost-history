//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/monotonic/vector.h>
#include <boost/monotonic/list.h>
#include <boost/monotonic/map.h>
#include <boost/monotonic/set.h>

#include <boost/timer.hpp>
#include <iostream>

using namespace std;
using namespace boost;

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
	test_loop_monotonic<char>();
	test_loop_monotonic<long>();

	test_loop_std<char>();
	test_loop_std<short>();
}

int main()
{
	test_alignment();

	test_copy();
	test_ctors();
	test_speed();
	test_speed_heap();

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

//EOF
