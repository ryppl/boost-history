// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HETEROGENOUS_BASE_HPP
#define BOOST_HETEROGENOUS_BASE_HPP

#include <boost/heterogenous/detail/prefix.hpp>

namespace boost
{
	namespace heterogenous
	{

		/// base of the given derived type
		template <class Derived>
		struct base : common_base
		{
			typedef Derived derived_type;
			typedef base<derived_type> this_type;

		private:
			static size_t alignment;
			mutable derived_type *self_ptr;

			derived_type *&self(derived_type *ptr) const
			{
				return ptr->this_type::self_ptr;
			}

		public:
			base() : self_ptr(0) { }

			virtual base<Derived> *allocate(boost::abstract_allocator &alloc) const 
			{
				boost::abstract_allocator::pointer bytes = alloc.allocate_bytes(sizeof(derived_type), alignment);
				Derived *ptr = reinterpret_cast<Derived *>(bytes);
				self(ptr) = ptr;
				return ptr;
			}

			void deallocate(common_base *object, abstract_allocator &alloc) const
			{
				alloc.deallocate_bytes(reinterpret_cast<abstract_allocator::pointer>(object));
			}

			virtual base<Derived> *create(boost::abstract_allocator &alloc) const 
			{
				base<Derived> *ptr = allocate(alloc);
				new (ptr->self_ptr) Derived();
				return ptr;
			}

			virtual base<Derived> *copy_construct(const common_base &original, boost::abstract_allocator &alloc) const 
			{ 
				base<Derived> *ptr = allocate(alloc);
				new (ptr->self_ptr) Derived(static_cast<const Derived &>(original));
				return ptr;
			}
		};

		/// ensure correct alignment when allocating derived instances
		template <class Derived>
		size_t base<Derived>::alignment = aligned_storage<sizeof(Derived)>::alignment;


	} // namespace heterogenous

} // namespace boost

#include <boost/heterogenous/detail/suffix.hpp>

#endif // BOOST_HETEROGENOUS_BASE_HPP

//EOF
