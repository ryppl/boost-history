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
#include <boost/cloneable/instance.hpp>

namespace boost 
{
	namespace cloneable
	{
		/// a vector of heterogenous objects
		// TODO: move to boost/heterogenous/set.hpp, or boost/cloneable/containers/set.hpp
		template <class Base, class Pred, class Alloc>
		struct set
		{
			typedef Base base_type;
			typedef Pred predicate_type;
			typedef abstract_base<Base> abstract_base_type;
			typedef typename detail::make_clone_allocator<Alloc>::type allocator_type;

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
			typedef std::pair<iterator, bool> insert_result;

			template <class U>
			struct emplace_result
			{
				typedef instance<U, base_type,allocator_type> instance_type;
				instance_type value;
				bool inserted;
				iterator where;
				emplace_result() : inserted(false) { }
				emplace_result(instance_type ins, const insert_result &result)
					: value(ins), inserted(result.second), where(result.first) { }
			};

			template <class U>
			emplace_result<U> emplace(instance<U,base_type,allocator_type> value)
			{
				insert_result result = impl.insert(value.to_abstract());
				if (!result.second)
					value.release();
				return emplace_result<U>(value, result);
			}

			template <class U>
			emplace_result<U> emplace()
			{
				return emplace(instance<U, base_type,allocator_type>(get_allocator()));
			}

			template <class U, class A0>
			emplace_result<U> emplace(A0 a0)
			{
				return emplace(instance<U, base_type,allocator_type>(get_allocator(), a0));
			}
			template <class U, class A0, class A1>
			emplace_result<U> emplace(A0 a0, A1 a1)
			{
				return emplace(instance<U, base_type,allocator_type>(get_allocator(), a0, a1));
			}
			template <class U, class A0, class A1, class A2>
			emplace_result<U> emplace(A0 a0, A1 a1, A2 a2)
			{
				return emplace(instance<U, base_type,allocator_type>(get_allocator(), a0, a1, a2));
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
					template <class Instance>
					static iterator search(this_type *cont, Instance &value)
					{
						BOOST_SCOPE_EXIT((&value))
						{
							value.release();
						}
						BOOST_SCOPE_EXIT_END
						return cont->find_instance<U>(value);
					}

					static iterator given(this_type *cont)
					{
						return search(cont, instance<U,base_type,allocator_type>(cont->get_allocator()));
					}

					template <class A0>
					static iterator given(this_type *cont, A0 a0)
					{
						return search(cont, instance<U,base_type,allocator_type>(cont->get_allocator(), a0));
					}
					template <class A0, class A1>
					static iterator given(this_type *cont, A0 a0, A1 a1)
					{
						return search(cont, instance<U,base_type,allocator_type>(cont->get_allocator(), a0, a1));
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
			iterator find_instance(instance<U,base_type,allocator_type> value)
			{
				if (!value.exists())
					return end();
				iterator found = impl.find(*value.to_abstract());
				if (found == impl.end())
					return found;
				std::type_info const &found_type = found->get_type();
				if (found_type == typeid(U))
					return found;
				return impl.end();
			}

			template <class U, class A0>
			iterator find(A0 a0)
			{
				return impl::find<U>::given(this, a0);
			}

			allocator_type get_allocator()
			{
				return impl.get_allocator();
			}
		};
	
	} // namespace heterogenous

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_SET_HPP

//EOF
