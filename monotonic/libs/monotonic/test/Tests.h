// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/

#pragma once

struct test_vector_create
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector(length*rand()/RAND_MAX);
		return vector.size();
	}
};

struct test_vector_dupe
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::vector<int, typename Rebind<Alloc, int>::type> Vector;
		Vector vector(count*rand()/RAND_MAX);
		Vector dupe = vector;
		return dupe.size();
	}
};

template <class Ty>
struct test_vector_sort
{
	template <class Alloc>
	int test(Alloc, size_t count) const
	{
		std::vector<Ty, typename Rebind<Alloc, Ty>::type> vector(count);
		for (size_t n = 0; n < count; ++n)
			vector[n] = count - n;
		sort(vector.begin(), vector.end());
		return 0;
	}
};

struct test_vector_accumulate
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector(length*rand()/RAND_MAX);
		return accumulate(vector.begin(), vector.end(), 0);
	}
};

struct test_vector_accumulate_unaligned
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<Unaligned, typename Rebind<Alloc, Unaligned>::type> vector(length);
		int total = 0;
		BOOST_FOREACH(Unaligned const &val, vector)
		{
			total += val.c[2];
		}
		return total;
	}
};

template <class Ty>
struct test_list_create
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		std::list<Ty, typename Rebind<Alloc, Ty>::type> list;
		fill_n(back_inserter(list), count, 42);
		return 0;
	}
};

struct test_list_dupe
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::list<int, typename Rebind<Alloc, int>::type> List;
		List list;
		fill_n(back_inserter(list), count, 42);
		List dupe = list;
		return dupe.size();
	}
};

template <class Ty>
struct test_list_sort
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		std::list<Ty, typename Rebind<Alloc, Ty>::type> list;
		for (size_t n = 0; n < count; ++n)
			list.push_back(count - n);
		list.sort();
		return 0;
	}
};

struct test_set_vector
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::vector<int, typename Rebind<Alloc, int>::type> Vector;
		typedef std::set<Vector, std::less<Vector>, typename Rebind<Alloc, Vector>::type> Set;
		int dummy = 0;
		Set set;
		for (size_t n = 0; n < count; ++n)
		{
			size_t size = count*rand()/RAND_MAX;
			Vector vector(size);
			generate_n(vector.begin(), size, rand);
			set.insert(vector);
			dummy += set.size();
		}
		return dummy;
	}
};

template <class Map>
int test_map_vector_impl(size_t length)
{
	Map map;
	size_t mod = length/10;
	if (mod == 0)
		mod = 5;
	for (size_t n = 0; n < length; ++n)
	{
		int random = rand() % mod;
		map[random].push_back(n);
	}
	return 0;
}

template <class Ty>
struct test_map_vector
{
	template <class Alloc>
	int test(Alloc, size_t length) const
	{
		return test_map_vector_impl<std::map<int
			, std::vector<Ty, typename Rebind<Alloc, Ty>::type>
			, std::less<int>
			, typename Rebind<Alloc, int>::type> >(length);
	}
};

struct test_map_list_unaligned
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::map<int
			, std::list<Unaligned, typename Rebind<Alloc, Unaligned>::type>
			, std::less<int>
			, typename Rebind<Alloc, int>::type
		> map;
		size_t mod = length/10;
		for (size_t n = 0; n < length; ++n)
		{
			int random = rand() % mod;
			map[random].push_back(n);
		}
		return 0;
	}
};

//EOF
