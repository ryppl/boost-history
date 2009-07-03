// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_POINTER_HPP
#define BOOST_CLONEABLE_DETAIL_POINTER_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/base.hpp>
#include <boost/cloneable/allocator.hpp>

namespace boost
{
	namespace cloneable
	{
		template <class Base, class Alloc>
		struct instance_base
		{
			typedef Base base_type;
			typedef Alloc allocator_type;

		private:
			allocator_type *alloc;

		public:
			instance_base(allocator_type *al = 0) : alloc(al) { }

			bool has_allocator() const
			{
				return alloc != 0;
			}
			allocator_type &get_allocator() const 
			{ 
				if (!alloc)
					throw std::exception("empty allocator");
				return *alloc; 
			}
			void set_allocator(allocator_type &al)
			{
				alloc = &al;
			}

			virtual const std::type_info &get_type() const = 0;
			virtual void allocate() = 0;
			virtual bool exists() const = 0;
			virtual base_type *to_base() const = 0;
			virtual void release() = 0;
		};

		/// a pointer store that can retrieve pointers from up and down 
		/// the inheritance tree. stores the allocator that made it.
		template <class Derived, class Base, class Alloc>
		struct instance : instance_base<Base, Alloc>
		{
			typedef instance_base<Base,Alloc> parent_type;
			using parent_type::base_type;
			using parent_type::allocator_type;

			typedef Derived derived_type;
			typedef base<derived_type, base_type> cloneable_type;
			typedef typename cloneable_type::abstract_base_type abstract_base_type;

		private:
			cloneable_type *ptr;

		public:
			instance() : ptr(0), alloc(0) { }

			template <class Other>
			instance(const instance<Other,base_type,allocator_type> &other) 
				: ptr(dynamic_cast<cloneable_type *>(other.to_base()))
			{
				if (other.has_allocator())
					parent_type::set_allocator(other.get_allocator());
			}
			instance(allocator_type &al) : parent_type(&al), ptr(0)
			{
				allocate();
			}
			template <class A0>
			instance(allocator_type &al, A0 a0) : parent_type(&al), ptr(0)
			{
				allocate();
				new (to_derived()) derived_type(a0);
			}
			template <class A0, class A1>
			instance(allocator_type &al, A0 a0, A1 a1) : parent_type(&al), ptr(0)
			{
				allocate();
				new (to_derived()) derived_type(a0, a1);
			}
			template <class A0, class A1, class A2>
			instance(allocator_type &al, A0 a0, A1 a1, A2 a2) : parent_type(&al), ptr(0)
			{
				allocate();
				new (to_derived()) derived_type(a0, a1, a2);
			}

			void allocate()
			{
				if (!has_allocator())
					return;
				if (exists())
					release();
				derived_type *derived = cloneable::allocate<derived_type>(get_allocator());
				derived->cloneable_type::self_ptr = derived;
				ptr = derived;
			}
			void release()
			{
				if (!has_allocator())
					return;
				cloneable::release(ptr, get_allocator());
				ptr = 0;
			}

			const std::type_info &get_type() const
			{
				return typeid(derived_type);
			}
			bool exists() const
			{
				return ptr != 0;
			}
			abstract_base_type *to_abstract() const
			{
				return ptr;
			}
			base_type *to_base() const
			{
				return ptr;
			}
			cloneable_type *to_cloneable() const
			{
				return ptr;
			}
			derived_type *to_derived() const
			{
				if (!ptr)
					return 0;
				return ptr->cloneable_type::self_ptr;
			}
		};

	} // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_DETAIL_POINTER_HPP

//EOF
