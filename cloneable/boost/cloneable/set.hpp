// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_SET_HPP
#define BOOST_CLONEABLE_SET_HPP

#include <boost/ptr_container/ptr_set.hpp>
#include <boost/foreach.hpp>

#include <boost/cloneable/detail/make_clone_allocator.hpp>
#include <boost/cloneable/allocator.hpp>
#include <boost/cloneable/adaptor.hpp>

namespace boost 
{
	namespace cloneable
	{
		/// a vector of heterogenous objects
		// TODO: move to boost/heterogenous/set.hpp, or boost/cloneable/containers/set.hpp
		template <class Base, class Pred, class Alloc>
		struct set
		{
			typedef typename detail::make_clone_allocator<Alloc>::type allocator_type;
			typedef Base base_type;
			typedef Pred predicate_type;
			typedef abstract_base<Base> abstract_base_type;

			typedef ptr_set<abstract_base_type, predicate_type, allocator, allocator_type> implementation;

			typedef typename implementation::value_type value_type;
			typedef typename implementation::reference reference;
			typedef typename implementation::const_reference const_reference;
			typedef typename implementation::iterator iterator;
			typedef typename implementation::const_iterator const_iterator;
			typedef set<Base, Pred, Alloc> this_type;

		//private:
			implementation impl;

		public:
			set()
			{
			}
			set(allocator_type a) 
				: impl(a)
			{
			}

			/* purposefully elided
			template <class II>
			set(II F, II L, allocator_type a = allocator_type());
			*/


		public:
			typedef std::pair<iterator, bool> emplace_result;
			template <class U>
			emplace_result emplace_insert()
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator()).to_abstract();
				return impl.insert(instance);
			}

			// TODO: use variadic arguments or BOOST_PP to pass ctor args
			template <class U, class A0>
			emplace_result emplace_insert(A0 a0)
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator(), a0).to_abstract();
				return impl.insert(instance);
			}
			template <class U, class A0, class A1>
			emplace_result emplace_insert(A0 a0, A1 a1)
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator(), a0, a1).to_abstract();
				return impl.insert(instance);
			}
			template <class U, class A0, class A1, class A2>
			emplace_result emplace_insert(A0 a0, A1 a1, A2 a2)
			{
				abstract_base_type *instance = detail::construct<U,base_type>(get_allocator(), a0, a1, a2).to_abstract();
				return impl.insert(instance);
			}

			template <class Fun>
			Fun for_each(Fun fun)
			{
				BOOST_FOREACH(value_type &value, *this)
				{
					fun(value);
				}
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

			struct impl
			{
				template <class U>
				struct find
				{
					template <class A0>
					static iterator given(this_type *cont, A0 a0)
					{
						abstract_base_type *instance = 0;
						instance = detail::construct<U,base_type>(cont->get_allocator(), a0).to_abstract();
						BOOST_SCOPE_EXIT((instance)(cont))
						{
							cloneable::release(instance, cont->get_allocator());
						}
						BOOST_SCOPE_EXIT_END
						return cont->find_instance<U>(instance);
					}
					template <class A0, class A1>
					static iterator given(this_type *cont, A0 a0, A1 a1)
					{
						abstract_base_type *instance = 0;
						instance = detail::construct<U,base_type>(cont->get_allocator(), a0, a1).to_abstract();
						BOOST_SCOPE_EXIT((instance)(cont))
						{
							cloneable::release(instance, cont->get_allocator());
						}
						BOOST_SCOPE_EXIT_END
						return cont->find_instance<U>(instance);
					}
				};
				struct default_key : base<default_key, base_type>
				{
					default_key() { }
					default_key(base_type const &X) : base<default_key, base_type>::base_type(X) { }
				};

				template <>
				struct find<base_type>
				{
					template <class A0>
					static iterator given(this_type *cont, A0 a0)
					{
						return cont->impl.find(default_key(base_type(a0)));
					}
					template <class A0, class A1>
					static iterator given(this_type *cont, A0 a0, A1 a1)
					{
						return cont->impl.find(default_key(base_type(a0, a1)));
					}
				};
			};
			template <class U>
			iterator find_instance(value_type instance)
			{
				iterator found = impl.find(*instance);
				if (found == impl.end())
					return found;
				if (U *ptr = dynamic_cast<U *>(&*found))
					return found;
				return impl.end();
			}

			template <class U, class A0>
			iterator find(A0 a0)
			{
				return impl::find<U>::given(this, a0);
			}

			typename implementation::allocator_type get_allocator()
			{
				return impl.get_allocator();
			}
		};
	
	} // namespace heterogenous

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_SET_HPP

//EOF
