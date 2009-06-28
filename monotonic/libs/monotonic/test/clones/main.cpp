// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// documentation at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html
// sandbox at https://svn.boost.org/svn/boost/sandbox/monotonic/

//
// test cases for ptr_container and clone_allocator issues
//

#include <string>
#include <boost/abstract_allocator.hpp>
#include <boost/cloneable.hpp>
#include <boost/monotonic/allocator.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

using namespace std;
using namespace boost;

struct derived : cloneable::base<derived>
{
	int num;
	derived() : num(0) { }
	explicit derived(int n) : num(n) { }
};

struct derived2 : cloneable::base<derived2>
{
	std::string str;

	derived2() { }
	explicit derived2(std::string const &n) : str(n) { }
};


struct derived3 : cloneable::base<derived3>
{
	float real;
	int num;
	std::string str;

	derived3() { }
	explicit derived3(float f, int n, std::string const &s) : real(f), num(n), str(s) { }
};

namespace boost
{
	namespace heterogenous
	{
		template <class Alloc = std::allocator<char> >
		struct vector
		{
			typedef ptr_vector<cloneable::common_base, cloneable::allocator, Alloc> implementation;
			typedef Alloc allocator_type;
			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference_type;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;

		private:
			implementation impl;

		public:

			size_t size() const
			{
				return impl.size();
			}

			iterator begin()
			{
				return impl.begin();
			}
			iterator end()
			{
				return impl.end();
			}
			const_iterator begin() const
			{
				return impl.begin();
			}
			const_iterator end() const
			{
				return impl.end();
			}
			reference_type at(size_t n)
			{
				return impl.at(n);
			}
			reference_type operator[](size_t n)
			{
				return impl[n];
			}
			template <class Other>
			bool is_type_at(size_t n) const
			{
				return ptr_at<Other>(n) != 0;
			}
			template <class Other>
			Other &ref_at(size_t n)
			{
				Other *ptr = ptr_at<Other>(n);
				if (ptr == 0)
					throw std::bad_cast();
				return *ptr;
			}
			template <class Other>
			Other *ptr_at(size_t n)
			{
				return dynamic_cast<Other *>(&at(n));
			}
			template <class U>
			void push_back()
			{
				U *ptr = construct_type<U>();
				base().push_back(ptr);
			}
			template <class U, class A0>
			void push_back(A0 a0)
			{
				U *ptr = allocate_type<U>();
				new (ptr) U(a0);
				impl.push_back(ptr);
			}
			template <class U, class A0, class A1>
			void push_back(A0 a0, A1 a1)
			{
				U *ptr = allocate_type<U>();
				new (ptr) U(a0, a1);
				impl.push_back(ptr);
			}
			template <class U, class A0, class A1, class A2>
			void push_back(A0 a0, A1 a1, A2 a2)
			{
				U *ptr = allocate_type<U>();
				new (ptr) U(a0, a1, a2);
				impl.push_back(ptr);
			}

			allocator_type get_allocator()
			{
				return impl.get_allocator();
			}
		private:
			template <class U>
			U *allocate_type()
			{
				typename allocator_type::template rebind<U>::other alloc(get_allocator());
				return alloc.allocate(1);
			}
			template <class U>
			U *construct_type()
			{
				typename allocator_type::template rebind<U>::other alloc(get_allocator());
				U *ptr = alloc.allocate(1);
				alloc.construct(ptr);
				return ptr;
			}

		};
	}
}

int main()
{
	// there is a problem with static_move_ptr<>
	//typedef cloneable::make_cloneable_allocator<std::allocator<int> >::type alloc_type;
	typedef cloneable::make_cloneable_allocator<monotonic::allocator<int> >::type alloc_type;

	typedef heterogenous::vector<alloc_type> vec;

	{
		vec bases;
		bases.push_back<derived>(42);
		bases.push_back<derived2>("foo");
		bases.push_back<derived3>(3.14f, -123, "spam");

		BOOST_ASSERT(bases.size() == 3);
		vec copy = bases;
		BOOST_ASSERT(copy.size() == 3);

		derived &p1 = copy.ref_at<derived>(0);
		derived2 *p2 = copy.ptr_at<derived2>(1);
		derived3 *p3 = copy.ptr_at<derived3>(1);
		
		BOOST_ASSERT(p2);
		BOOST_ASSERT(p3);
		
		BOOST_ASSERT(p1.num == 42);
		BOOST_ASSERT(p2->str == "foo");
		BOOST_ASSERT(p3->real == 3.14f);
		BOOST_ASSERT(p3->num == -123);
		BOOST_ASSERT(p3->str == "spam");

	}
	monotonic::static_storage<>::release();

	return 0;
}

//EOF
