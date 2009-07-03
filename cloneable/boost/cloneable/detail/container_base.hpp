// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_CONTAINER_BASE_HPP
#define BOOST_CLONEABLE_DETAIL_CONTAINER_BASE_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/make_clone_allocator.hpp>
#include <boost/cloneable/instance.hpp>
#include <boost/cloneable/traits.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace detail
		{
			/// common functionality for all containers using the given base and allocator types
			template <class Base, class Alloc>
			struct container_base
			{
				typedef Base base_type;
				typedef abstract_base<base_type> abstract_base_type;
				typedef typename detail::make_clone_allocator<Alloc>::type allocator_type;
				typedef instance_base<Base, allocator_type> instance_base_type;

				/// ensures that the given type T is a valid type for this container.
				/// this provides better information from errors than otherwise.
				template <class T>
				struct validate
				{
					// can only add cloneable things to a heterogenous container
					BOOST_STATIC_ASSERT(is_cloneable<T>::value);

					// you must ensure that base-types for objects that you add to a container
					// is the same base used by the container
					typedef is_convertible<T *, base_type *> has_base_type;
					BOOST_STATIC_ASSERT(has_base_type::value);

					typedef T type;
				};

				/// an instance of the given derived type U suitable for this container
				template <class Derived>
				struct instance 
					: cloneable::instance<Derived, Base, Alloc, typename traits<Derived>::construction_tag>
				{
					typedef cloneable::instance<Derived, Base, Alloc, typename traits<Derived>::construction_tag> parent_type;
					instance(allocator_type &a) : parent_type(a) { }
					template <class A0>
					instance(allocator_type &a, A0 a0) : parent_type(a, a0) { }
					template <class A0, class A1>
					instance(allocator_type &a, A0 a0, A1 a1) : parent_type(a, a0, a1) { }
					template <class A0, class A1, class A2>
					instance(allocator_type &a, A0 a0, A1 a1, A2 a2) : parent_type(a, a0, a1, a2) { }
				};

			private:
				allocator_type alloc;

			protected:
				template <class U>
				instance<U> new_instance()
				{
					return instance<U>(get_allocator());
				}
				template <class U, class A0>
				instance<U> new_instance(A0 a0)
				{
					return instance<U>(get_allocator(), a0);
				}
				template <class U, class A0, class A1>
				instance<U> new_instance(A0 a0, A1 a1)
				{
					return instance<U>(get_allocator(), a0, a1);
				}
				template <class U, class A0, class A1, class A2>
				instance<U> new_instance(A0 a0, A1 a1, A2 a2)
				{
					return instance<U>(get_allocator(), a0, a1, a2);
				}

			public:
				container_base() { }
				container_base(allocator_type &a)
					: alloc(a) { }

				const allocator_type &get_allocator() const
				{
					return alloc;
				}
				allocator_type &get_allocator()
				{
					return alloc;
				}
			};

			template <class Cont, class Base, class Alloc>
			struct sequence_container_base : container_base<Base,Alloc>
			{
				typedef Cont container_type;
				typedef container_base<Base,Alloc> parent_type;
				using parent_type::base_type;
				using parent_type::abstract_base_type;
				using parent_type::allocator_type;
				using parent_type::validate;
				using parent_type::new_instance;
				using parent_type::instance;
				typedef typename container_type::value_type value_type;
				typedef typename container_type::reference reference;
				typedef typename container_type::const_reference const_reference;
				typedef typename container_type::iterator iterator;
				typedef typename container_type::const_iterator const_iterator;

			private:
				container_type container;

			protected:
				const container_type &impl() const
				{
					return container;
				}
				container_type &impl()
				{
					return container;
				}

			public:
				sequence_container_base() 
					: container(get_allocator())
				{ 
				}
				sequence_container_base(allocator_type &a)
					: parent_type(a), container(get_allocator())
				{
				}

				size_t size() const
				{
					return container.size();
				}

				template <class Ty, class Fun>
				Fun for_each(Fun fun)
				{
					BOOST_FOREACH(base_type &b, *this)
					{
						if (Ty *ptr = dynamic_cast<Ty *>(&b))
						{
							fun(*ptr);
						}
					}
					return fun;
				}

				template <class Ty, class Fun>
				Fun for_each(Fun fun) const
				{
					BOOST_FOREACH(const base_type &base, *this)
					{
						if (Ty *ptr = dynamic_cast<Ty *>(&base))
						{
							fun(*ptr);
						}
					}
					return fun;
				}

				bool empty() const
				{
					return impl().empty();
				}

				iterator begin()
				{
					return impl().begin();
				}
				iterator end()
				{
					return impl().end();
				}
				const_iterator begin() const
				{
					return impl().begin();
				}
				const_iterator end() const
				{
					return impl().end();
				}

				reference back()
				{
					return impl().back();
				}
				const_reference back() const
				{
					return impl().back();
				}
				reference front()
				{
					return impl().front();
				}
				const_reference front() const
				{
					return impl().front();
				}

				template <class Other>
				Other &back_as()
				{
					BOOST_STATIC_ASSERT(is_cloneable<Other>::value);
					Other *ptr = dynamic_cast<Other *>(back());
					if (ptr == 0)
						throw std::bad_cast();
					return *ptr;
				}
				template <class Other>
				Other &front_as()
				{
					BOOST_STATIC_ASSERT(is_cloneable<Other>::value);
					Other *ptr = dynamic_cast<Other *>(front());
					if (ptr == 0)
						throw std::bad_cast();
					return *ptr;
				}

				// TODO: use variadic arguments or BOOST_PP to pass ctor args
				template <class U>
				void push_back()
				{
					impl().push_back(new_instance<typename validate<U>::type>().to_abstract());
				}
				template <class U, class A0>
				void push_back(A0 a0)
				{
					impl().push_back(new_instance<typename validate<U>::type>(a0).to_abstract());
				}
				template <class U, class A0, class A1>
				void push_back(A0 a0, A1 a1)
				{
					impl().push_back(new_instance<typename validate<U>::type>(a0,a1).to_abstract());
				}
				template <class U, class A0, class A1, class A2>
				void push_back(A0 a0, A1 a1, A2 a2)
				{
					impl().push_back(new_instance<typename validate<U>::type>(a0,a1,a2).to_abstract());
				}


				template <class U>
				void push_front()
				{
					impl().push_front(new_instance<typename validate<U>::type>().to_abstract());
				}
				template <class U, class A0>
				void push_front(A0 a0)
				{
					impl().push_front(new_instance<typename validate<U>::type>(a0).to_abstract());
				}
				template <class U, class A0, class A1>
				void push_front(A0 a0, A1 a1)
				{
					impl().push_front(new_instance<typename validate<U>::type>(a0,a1).to_abstract());
				}
				template <class U, class A0, class A1, class A2>
				void push_front(A0 a0, A1 a1, A2 a2)
				{
					impl().push_front(new_instance<typename validate<U>::type>(a0,a1,a2).to_abstract());
				}
			};

			template <class Cont, class Base, class Alloc>
			bool operator==(const sequence_container_base<Cont,Base,Alloc> &left, const sequence_container_base<Cont,Base,Alloc> &right)
			{
				return left.size() == right.size() && std::equal(left.begin(), left.end(), right.begin());
			}

			template <class Cont, class Base, class Alloc>
			bool operator<(const sequence_container_base<Cont,Base,Alloc> &left, const sequence_container_base<Cont,Base,Alloc> &right)
			{
				return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
			}
		} // namespace detail

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif //  BOOST_CLONEABLE_DETAIL_CONTAINER_BASE_HPP

//EOF
