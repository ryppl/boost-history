// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_MAP_HPP
#define BOOST_HETEROGENOUS_MAP_HPP

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/monotonic/allocator.hpp>
#include <boost/foreach.hpp>

#include <boost/heterogenous/detail/prefix.hpp>
#include <boost/heterogenous/base.hpp>
#include <boost/heterogenous/make_clone_allocator.hpp>

namespace boost 
{
	namespace heterogenous
	{
		/// a vector of heterogenous objects
		template <class Pred, class Base = common_base, class Alloc = monotonic::allocator<int> >
		struct map
		{
			typedef typename make_clone_allocator<Alloc>::type allocator_type;
			typedef Base base_type;
			//typedef ptr_map<Base, Base, Pred, allocator, allocator_type> implementation;

			typedef std::map<Base *, Base *, Pred, allocator_type> implementation;

			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference;
			typedef typename implementation::const_reference const_reference;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;
			typedef typename implementation::key_type key_type;
			typedef typename implementation::mapped_type mapped_type;
			typedef map<Pred,Base,Alloc> this_type;

		private:
			implementation impl;

		public:
			map()
			{
				//insert.key.parent = this;
			}
			map(allocator_type a) 
				: impl(a)
			{
				//insert.key.parent = this;
			}

			/* purposefully elided
			template <class II>
			vector(II F, II L, allocator_type a = allocator_type());
			vector(size_t reserved);
			*/

			/*struct inserter
			{
				struct key_adder
				{
					this_type *parent;

				*/	struct value_adder
					{
						this_type *parent;
						base_type *key_instance;

						value_adder(this_type *P, base_type *K) 
						{ 
							parent = P;
							key_instance = K;
						}

						template <class U>
						/*std::pair<iterator,bool> */void value()
						{
							U *val = parent->construct_type<U>();
							parent->insert(std::make_pair(key_instance, val));
						}
					};

					template <class U>
					//value_adder /*operator()*/set() const
					value_adder key()
					{
						U *key_instance = this->construct_type<U>();
						return value_adder(this, key_instance);
					}
			//	} key;
			//} insert;

			template <class Ty, class Fun>
			Fun for_each_key(Fun fun)
			{
				//BOOST_FOREACH(common_base &b, *this)
				//{
				//	if (Ty *ptr = dynamic_cast<Ty *>(&b))
				//	{
				//		fun(*ptr);
				//	}
				//}
				return fun;
			}

			void insert(value_type const &x)// key_type& key, mapped_type x )
			{
				impl.insert(x);
			}


			template <class Ty, class Fun>
			Fun for_each_mapped(Fun fun) const
			{
				//BOOST_FOREACH(const common_base &base, *this)
				//{
				//	if (Ty *ptr = dynamic_cast<Ty *>(&base))
				//	{
				//		fun(*ptr);
				//	}
				//}
				return fun;
			}

			size_t size() const
			{
				return impl.size();
			}
			bool empty() const
			{
				return impl.empty();
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

			//reference operator[](size_t n)
			//{
			//	return impl[n];
			//}
			//const_reference operator[](size_t n) const
			//{
			//	return impl[n];
			//}

			//template <class Other>
			//bool is_type_at(size_t n) const
			//{
			//	return ptr_at<Other>(n) != 0;
			//}
			//
			//template <class Other>
			//Other &ref_at(size_t n)
			//{
			//	Other *ptr = ptr_at<Other>(n);
			//	if (ptr == 0)
			//		throw std::bad_cast();
			//	return *ptr;
			//}
			//template <class Other>
			//const Other &ref_at(size_t n) const
			//{
			//	const Other *ptr = ptr_at<const Other>(n);
			//	if (ptr == 0)
			//		throw std::bad_cast();
			//	return *ptr;
			//}

			//template <class Other>
			//Other *ptr_at(size_t n)
			//{
			//	return dynamic_cast<Other *>(&at(n));
			//}
			//template <class Other>
			//const Other *ptr_at(size_t n) const
			//{
			//	return dynamic_cast<const Other *>(&at(n));
			//}

			//// TODO: use variadic arguments or BOOST_PP to pass ctor args
			//template <class U>
			//void push_back()
			//{
			//	U *ptr = construct_type<U>();
			//	impl.push_back(ptr);
			//}
			//template <class U, class A0>
			//void push_back(A0 a0)
			//{
			//	U *ptr = allocate_type<U>();
			//	new (ptr) U(a0);
			//	impl.push_back(ptr);
			//}
			//template <class U, class A0, class A1>
			//void push_back(A0 a0, A1 a1)
			//{
			//	U *ptr = allocate_type<U>();
			//	new (ptr) U(a0, a1);
			//	impl.push_back(ptr);
			//}
			//template <class U, class A0, class A1, class A2>
			//void push_back(A0 a0, A1 a1, A2 a2)
			//{
			//	U *ptr = allocate_type<U>();
			//	new (ptr) U(a0, a1, a2);
			//	impl.push_back(ptr);
			//}

			typename implementation::allocator_type get_allocator()
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
	
	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_MAP_HPP

//EOF
