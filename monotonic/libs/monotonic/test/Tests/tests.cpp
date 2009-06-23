#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <string>
#include <boost/monotonic/monotonic.hpp>

//#include <boost/monotonic/shared_allocator.hpp>
#include <boost/monotonic/shared_storage.hpp>
#include <boost/monotonic/thread_local_storage.hpp>
#include <boost/monotonic/shared_allocator.hpp>
#include <boost/monotonic/local.hpp>
#include <boost/monotonic/allocator.hpp>

#define BOOST_TEST_MODULE basic_test test
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;

#ifdef WIN32
// warning C4996: 'std::fill_n': Function call with parameters that may be unsafe
#pragma warning(disable:4996)
#endif


struct region0 {};
struct region1 {};

BOOST_AUTO_TEST_CASE(test_local)
{
	monotonic::local<region0> storage0;
	monotonic::local<region1> storage1;
	{
		std::list<int, monotonic::allocator<int, region0> > list0;
		std::list<int, monotonic::allocator<int, region1> > list1;
		fill_n(back_inserter(list0), 100, 42);
		fill_n(back_inserter(list1), 100, 42);

		std::basic_string<char, std::char_traits<char>, monotonic::allocator<char, region0> > str("foo");
		str += "bar";
		BOOST_ASSERT(str == "foobar");
	}
}

BOOST_AUTO_TEST_CASE(test_shared_allocation)
{
	typedef std::list<int, monotonic::allocator<int, region0, monotonic::shared_access_tag> > List0;
	List0 list0;

	// same thing, less typing:
	typedef std::list<int, monotonic::shared_allocator<int, region0> > List2;
	List2 list2;

	typedef std::list<int, monotonic::allocator<int, region0, monotonic::thread_local_access_tag> > List1;
	List1 list1;
}

BOOST_AUTO_TEST_CASE(test_regional_allocation)
{
	typedef std::list<int, monotonic::allocator<int, region0> > List0;
	typedef std::list<int, monotonic::allocator<int, region1> > List1;
	{
		List0 list0;
		generate_n(back_inserter(list0), 10, rand);
		list0.sort();

		List1 list1;
		generate_n(back_inserter(list1), 10, rand);
		list1.sort();
	}
}


BOOST_AUTO_TEST_CASE(test_local_storage)
{
	monotonic::storage<10*1024> storage;
	{
		storage.allocate_bytes(123);
		BOOST_ASSERT(storage.fixed_used() == 123);
		BOOST_ASSERT(storage.heap_used() == 0);
		BOOST_ASSERT(storage.used() == 123);

		storage.reset();
		BOOST_ASSERT(storage.fixed_used() == 0);
		BOOST_ASSERT(storage.heap_used() == 0);
		BOOST_ASSERT(storage.used() == 0);

		// test alignment
		storage.reset();
		storage.allocate_bytes(12, 16);
		BOOST_ASSERT(storage.fixed_used() == 16);

		storage.allocate_bytes(12, 64);
		BOOST_ASSERT(storage.fixed_used() == 64 + 12);
		storage.reset();
	}
}

BOOST_AUTO_TEST_CASE(test_local_storage_to_heap)
{
	monotonic::storage<16> storage;
	{
		storage.allocate_bytes(16);
		BOOST_ASSERT(storage.fixed_used() == 16);
		BOOST_ASSERT(storage.heap_used() == 0);

		storage.allocate_bytes(200);
		BOOST_ASSERT(storage.fixed_used() == 16);
		BOOST_ASSERT(storage.heap_used() == 200);

		storage.release();

		BOOST_ASSERT(storage.used() == 0);
		storage.allocate_bytes<2000>();
		BOOST_ASSERT(storage.fixed_used() == 0);
		BOOST_ASSERT(storage.heap_used() == 2000);

		storage.allocate_bytes<10>();
		BOOST_ASSERT(storage.fixed_used() == 10);
		BOOST_ASSERT(storage.heap_used() == 2000);
	}
}

BOOST_AUTO_TEST_CASE(test_local_storage_iter)
{
	size_t length = 4;
	// storage starts on the stack (in this case, 10k of it), then merges into the heap as needed
	monotonic::storage<10*1024> storage;
	for (size_t n = 0; n < length; ++n)
	{
		// create a new int from storage
		int &n0 = storage.create<int>();

		// create a new string (uses correct alignment)
		string const &s1 = storage.create<string>("foo");
		BOOST_ASSERT(s1 == "foo");

		// allocate 37 bytes with alignment 1
		char *array0 = storage.allocate_bytes(37);
		fill_n(array0, 37, 42);

		// allocate 2537 bytes with 64-byte alignment
		char *array1 = storage.allocate_bytes(2537, 64);
		fill_n(array1, 2537, 123);

		// allocate 1283 bytes with machine alignment
		char *array2 = storage.allocate_bytes<1283>();
		fill_n(array2, 1283, 42);

		array<int, 42> &array3 = storage.create<array<int, 42> >();

		// destroy objects. this only calls the destructors; it does not release memory
		storage.destroy(s1);

		//cout << "storage.fixed, heap, total used: " << storage.fixed_used() << ", " << storage.heap_used() << ", " << storage.used() << endl;
	}
	// storage is released. if this was only ever on the stack, no work is done
}

BOOST_AUTO_TEST_CASE(test_ctors)
{
	monotonic::storage<> storage;
	string foo = "foo";
	{
		monotonic::vector<char> v1(foo.begin(), foo.end(), storage);
		BOOST_CHECK(v1.size() == 3);
		BOOST_CHECK(equal(v1.begin(), v1.end(), "foo"));

		monotonic::vector<char> v2(6, 'x', storage);
		BOOST_CHECK(v2.size() == 6);
		BOOST_CHECK(equal(v2.begin(), v2.end(), "xxxxxx"));

		monotonic::set<char> s2(foo.begin(), foo.end(), storage);
		BOOST_CHECK(s2.size() == 2);
		BOOST_CHECK(s2.find('f') != s2.end());
		BOOST_CHECK(s2.find('o') != s2.end());

		monotonic::vector<pair<int, string> > v(storage);
		v.push_back(make_pair(42,"foo"));
		v.push_back(make_pair(123,"bar"));

		monotonic::map<int, string> m1(v.begin(), v.end(), storage);
		BOOST_CHECK(m1.find(42) != m1.end());
		BOOST_CHECK(m1.find(123) != m1.end());

		monotonic::list<int> l1(foo.begin(), foo.end(), storage);
		BOOST_CHECK(equal(l1.begin(), l1.end(), "foo"));
	}
}

BOOST_AUTO_TEST_CASE( test_copy )
{
	monotonic::storage<> storage;
	monotonic::vector<int> v1(storage);

	for (int n = 0; n < 100; ++n)
		v1.push_back(n);

	size_t rem1 = storage.fixed_remaining();
	monotonic::vector<int> v2(v1);
	size_t rem2 = storage.fixed_remaining();

	//BOOST_CHECK(v2 == v1);
	//BOOST_CHECK_EQUAL(rem1 - rem2, 12 + 100*sizeof(int));
}

BOOST_AUTO_TEST_CASE(test_shared_allocators)
{
	monotonic::storage<> sa, sb;
	{
		monotonic::vector<int> v0(sa), v1(sa);
		monotonic::vector<int> v2(sb), v3(sb);
		monotonic::list<int> l0(sa), l1(sb);

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

		//l0.assign(v0.begin(), v0.end());
		//l1 = l0;
		//BOOST_CHECK(l0 == l1);
	}
}

BOOST_AUTO_TEST_CASE(test_basic)
{
	monotonic::storage<> storage;
	{
		monotonic::vector<int> v1(storage);

		for(int i = 0; i < 100; ++i)
			v1.push_back(i);

		monotonic::vector<int> copy(storage);
		size_t len = storage.used();
		copy = v1;
		size_t len2 = storage.used();

		BOOST_CHECK(copy == v1);
		BOOST_CHECK(len2 - len == 100*sizeof(int));

		// create a list that uses inline, monotonically-increasing storage
		monotonic::list<int> list(storage);
		list.push_back(100);
		list.push_back(400);
		list.erase(list.begin());

		// a map from the same storage
		monotonic::map<int, float> map(storage);
		map[42] = 3.14f;
		BOOST_CHECK(map[42] == 3.14f);

		// a set...
		monotonic::set<float> set(storage);
		set.insert(3.14f);
		set.insert(-123.f);
		BOOST_CHECK(set.size() == 2);
	}
}

BOOST_AUTO_TEST_CASE(test_string)
{
	monotonic::storage<> storage;
	{
		monotonic::string str(storage);
	}
}

//EOF
