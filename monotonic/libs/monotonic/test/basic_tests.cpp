#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/monotonic/allocator.h>
#include <string>
#include <vector>
#include <set>
#include <map>

#define BOOST_TEST_MODULE basic_test test
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;

BOOST_AUTO_TEST_CASE(test_splice)
{
	monotonic::storage<> store1;
	std::list<int, monotonic::allocator<int> > list1(store1);
	{
		monotonic::storage<> store2;
		std::list<int, monotonic::allocator<int> > list2(store2);
		list2.push_back(1);
		list1.splice(list1.begin(), list2);
	}
	BOOST_CHECK_EQUAL(*list1.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_ctors)
{
	monotonic::storage<> storage;
	string foo = "foo";
	vector<char, monotonic::allocator<char> > v1(foo.begin(), foo.end(), storage);
	BOOST_CHECK(v1.size() == 3);
	BOOST_CHECK(equal(v1.begin(), v1.end(), "foo"));

	vector<char, monotonic::allocator<char> > v2(6, 'x', storage);
	BOOST_CHECK(v2.size() == 6);
	BOOST_CHECK(equal(v2.begin(), v2.end(), "xxxxxx"));

	set<char, less<char>, monotonic::allocator<char> > s2(foo.begin(), foo.end(), less<char>(), storage);
	BOOST_CHECK(s2.size() == 2);
	BOOST_CHECK(s2.find('f') != s2.end());
	BOOST_CHECK(s2.find('o') != s2.end());

	vector<pair<int, string>, monotonic::allocator<pair<int, string> > > v(storage);
	v.push_back(make_pair(42,"foo"));
	v.push_back(make_pair(123,"bar"));

	map<int, string, less<int>, monotonic::allocator<int> > m1(v.begin(), v.end(), less<int>(), storage);
	BOOST_CHECK(m1.find(42) != m1.end());
	BOOST_CHECK(m1.find(123) != m1.end());

	list<int, monotonic::allocator<int> > l1(foo.begin(), foo.end(), storage);
	BOOST_CHECK(equal(l1.begin(), l1.end(), "foo"));
}

BOOST_AUTO_TEST_CASE( test_copy )
{
	monotonic::storage<> storage;
	std::vector<int, monotonic::allocator<int> > v1(storage);

	for (int n = 0; n < 100; ++n)
		v1.push_back(n);

	size_t rem1 = storage.fixed_remaining();
	std::vector<int, monotonic::allocator<int> > v2(v1);
	size_t rem2 = storage.fixed_remaining();

	BOOST_CHECK(v2 == v1);
	//BOOST_CHECK_EQUAL(rem1 - rem2, 12 + 100*sizeof(int));
}

BOOST_AUTO_TEST_CASE(test_shared_allocators)
{
	monotonic::storage<> sa, sb;
	typedef monotonic::allocator<int> Al;
	{
		std::vector<int, Al> v0(sa), v1(sa);
		std::vector<int, Al> v2(sb), v3(sb);
		std::list<int, Al> l0(sa), l1(sb);

		BOOST_CHECK(v0.get_allocator() == v1.get_allocator());
		BOOST_CHECK(v2.get_allocator() == v3.get_allocator());
		BOOST_CHECK(v0.get_allocator() != v2.get_allocator());
		BOOST_CHECK(v3.get_allocator() != v1.get_allocator());

		for (int n = 0; n < 10; ++n)
			v0.push_back(n);

		v1 = v0;
		v1.swap(v2);	// swap from different allocators means they are copied
		BOOST_CHECK(v1.empty() && v3.empty() && v1 == v3);

		BOOST_CHECK(v2 == v0); // both are now [0..9]

		v1.swap(v0);	// swap from same allocators means no copying
		BOOST_CHECK(v2 == v1);
		BOOST_CHECK(v0 == v3);

		l0.assign(v0.begin(), v0.end());
		l1 = l0;
		BOOST_CHECK(l0 == l1);
	}
}

BOOST_AUTO_TEST_CASE(test_basic)
{
	monotonic::storage<> storage1;
	vector<int, monotonic::allocator<int> > v1(storage1);

	for(int i = 0; i < 100; ++i)
		v1.push_back(i);

	size_t len = storage1.fixed_remaining();
	vector<int, monotonic::allocator<int> > copy(v1);
	size_t len2 = storage1.fixed_remaining();

	BOOST_CHECK(copy == v1);
	//BOOST_CHECK(len - len2 == 12 + 100*sizeof(int)); // 12 has no right being there, it is for the extra gunk in vector and is STL-impl-dependant

	// create the storage that will be used for the various monotonic containers.
	// while it is on the stack here, it could be on the heap as well.
	monotonic::storage<> storage;

	// create a list that uses inline, monotonically-increasing storage
	list<int, monotonic::allocator<int> > list(storage);
	list.push_back(100);
	list.push_back(400);
	list.erase(list.begin());

	// a map from the same storage
	map<int, float, less<int>, monotonic::allocator<int> > map(less<int>(), storage);
	map[42] = 3.14f;
	BOOST_CHECK(map[42] == 3.14f);

	// a set...
	set<float, less<float>, monotonic::allocator<float> > set(less<float>(), storage);
	set.insert(3.14f);
	set.insert(-123.f);
	BOOST_CHECK(set.size() == 2);
}

//EOF
