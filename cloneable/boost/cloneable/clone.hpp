// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_CLONE_HPP
#define BOOST_CLONEABLE_CLONE_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/traits.hpp>

namespace boost
{
	namespace cloneable
	{
		namespace impl
		{
			/// implementation of cloning when using non-Cloneable types
			template <bool>
			struct clone
			{
				template <class T>
				static T *given(const T &original)
				{
					return new T(original); 
				}
				template <class T, class Alloc>
				static T *given(const T &original, Alloc &alloc)
				{
					return create<T>(alloc, original);
				}
				template <class Derived, class Base, class Alloc>
				Derived *from_base(const Base &base, Alloc &alloc)
				{
					const Derived *derived = dynamic_cast<const Derived *>(&base);
					if (mixin == 0)
						throw std::bad_cast();
					return create<Derived>(alloc, *derived);
				}
				template <class Derived, class Base>
				static bool can_clone_as(const Base &base)
				{
					const Derived *derived = dynamic_cast<const Derived *>(&base);
					return derived != 0;
				}
			};

			/// implementation of cloneing using Cloneable types
			template <>
			struct clone<true>
			{
				template <class T>
				static T *given(const T &original)
				{
					return original.clone_as<T>();
				}
				template <class T, class Alloc>
				static T *given(const T &original, Alloc &alloc)
				{
					return original.clone_as<T>(alloc);
				}
				template <class Derived, class Base, class Alloc>
				static Derived *from_base(const Base &base, Alloc &alloc)
				{
					typedef detail::mixin<Derived,Base> mixin_type;
					const mixin_type *mixin = dynamic_cast<const mixin_type *>(&base);
					if (mixin == 0)
						throw std::bad_cast();
					return mixin->clone_as<Derived>(alloc);
				}
				template <class Derived, class Base>
				static Derived *from_base(const Base &base)
				{
					make_clone_allocator<default_allocator>::type alloc;
					return from_base<Derived>(base, alloc);
				}

				template <class Derived, class Base>
				static bool can_clone_as(const Base &base)
				{
					typedef is_same<Derived, Base> same;
					if (same::value)
						return true;
					if (clone<false>::can_clone_as<Derived>(base))
						return true;
					typedef detail::mixin<Derived,Base> mixin_type;
					return dynamic_cast<const mixin_type *>(&base) != 0;
				}
				template <class Derived, class Base>
				static bool can_create_as(const Base &base)
				{
					typedef is_same<Derived, Base> same;
					if (same::value)
						return true;
					if (clone<false>::can_create_as<Derived>(base))
						return true;
					return dynamic_cast<abstract_base<Base> const &>(base).can_create_as<Derived>();
				}
			};

		} // namespace impl

		/// free-function clone that works for cloneable and also default types
		template <class T>
		T *clone(const T &original)
		{
			return impl::clone<is_cloneable<T>::value>::given(original);
		}

		/// free-function clone that works for cloneable and also default types, given allocator
		template <class T, class Alloc>
		T *clone(const T &original, Alloc &alloc)
		{
			return impl::clone<is_cloneable<T>::value>::given(original, alloc);
		}

		template <class Derived, class Base>
		bool can_clone_as(const Base &base)
		{
			return impl::clone<is_cloneable<Base>::value>::can_clone_as<Derived>(base);
		}

		template <class Derived, class Base>
		bool can_create_as(const Base &base)
		{
			return impl::clone<is_cloneable<Base>::value>::can_create_as<Derived>(base);
		}

		/// clone as a specified derived type, from a base type
		template <class Derived, class Base>
		Derived *clone_as(const Base &original)
		{
			return impl::clone<true>::from_base<Derived>(original);
		}

		/// clone as a specified derived type, from a base type, given allocator
		template <class Derived, class Base, class Alloc>
		Derived *clone_as(const Base &original, Alloc &alloc)
		{
			return impl::clone<true>::from_base<Derived>(original, alloc);
		}

		template <class Base>
		Base *create_new(const Base &base)
		{
			const abstract_base<Base> *abst = dynamic_cast<const abstract_base<Base> *>(&base);
			if (abst == 0)
				throw not_cloneable();
			return abst->create();
		}
		
		template <class Base, class Alloc>
		Base *create_new(const Base &base, Alloc &alloc)
		{
			const abstract_base<Base> *abst = dynamic_cast<const abstract_base<Base> *>(&base);
			if (abst == 0)
				throw not_cloneable();
			return abst->create_new(alloc);
		}

	} // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_CLONE_HPP

//EOF
