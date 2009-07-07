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
        /// common for all instances of the given base and allocator type
        template <class Base, class Alloc>
        struct instance_base
        {
            typedef Base base_type;
            typedef Alloc allocator_type;
            typedef instance_base<Base,Alloc> this_type;

        private:
            allocator_type *alloc;

        public:
            instance_base(allocator_type *al = 0) 
                : alloc(al) { }

            bool has_allocator() const
            {
                return alloc != 0;
            }
            allocator_type &get_allocator() const 
            { 
                if (!alloc)
                    throw empty_allocator(typeid(this_type).name());
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

        /// common for all instances
        template <class Abstract, class Derived, class Base, class Alloc>
        struct instance_common : instance_base<Base,Alloc>
        {
            typedef instance_base<Base,Alloc> parent_type;
            using parent_type::base_type;
            using parent_type::allocator_type;
            using parent_type::set_allocator;

            typedef Abstract abstract_type;
            typedef Derived derived_type;
            typedef detail::mixin<derived_type, base_type> is_derived_type;
            typedef instance_common<Abstract, Derived, Base, Alloc> this_type;

        protected:
            derived_type *ptr;

        public:
            instance_common(derived_type *p = 0) : ptr(p) { }
            instance_common(allocator_type &a, derived_type *p = 0) : parent_type(&a), ptr(p) { }

            this_type clone() const
            {
                if (empty())
                    return this_type(get_allocator());
                return this_type(get_allocator(), to_abstract()->clone_as<derived_type>(get_allocator()));
            }

            template <class Ty>
            instance_common<base<Ty,Base, unknown_construction_tag>, Derived, Base, Alloc> clone() const
            {
                typedef instance_common<base<Ty,Base, unknown_construction_tag>, Derived, Base, Alloc> that_type;
                if (empty())
                    return that_type(get_allocator());
                return that_type(get_allocator(), to_abstract()->clone_as<Ty>(get_allocator()));
            }

            void allocate()
            {
                if (!has_allocator())
                    return;
                if (exists())
                    release();
                derived_type *derived = cloneable::allocate<derived_type>(get_allocator());
                derived->is_derived_type::self_ptr = derived;
                ptr = derived;
            }
            void release()
            {
                if (!has_allocator())
                    return;
                cloneable::release(ptr, get_allocator());
                ptr = 0;
            }
            template <class A0>
            void construct(A0 a0)
            {
                allocate();
                new (to_derived()) derived_type(a0);
            }
            template <class A0, class A1>
            void construct(A0 a0, A1 a1)
            {
                allocate();
                new (to_derived()) derived_type(a0, a1);
            }
            template <class A0, class A1, class A2>
            void construct(A0 a0, A1 a1, A2 a2)
            {
                allocate();
                new (to_derived()) derived_type(a0, a1, a2);
            }

            const std::type_info &get_type() const
            {
                return typeid(derived_type);
            }

            bool exists() const
            {
                return ptr != 0;
            }

            bool empty() const
            {
                return ptr == 0;
            }

            base_type *to_base() const
            {
                return ptr;
            }
            abstract_type *to_abstract() const
            {
                return ptr;
            }
            derived_type *to_derived() const
            {
                return ptr;
            }

            derived_type &derived_ref()
            {
                ptr = to_derived();
                if (ptr == 0)
                    throw empty_object();
                return *ptr;
            }

            derived_type *operator->()
            {
                return &derived_ref();
            }
            derived_type &operator*()
            {
                return derived_ref();
            }
        };

        /// a pointer to a general instance
        template <class Derived, class Base, class Alloc, class Ctor>
        struct instance : instance_common<base<Derived,Base,Ctor>, Derived, Base, Alloc>
        {
            typedef abstract_base<Base> abstract_base_type;
            typedef base<Derived,Base,Ctor> base_type;
            typedef instance_common<abstract_type, Derived, Base, Alloc> parent_type;
            typedef instance<Derived, Base, Alloc, Ctor> this_type;

        public:
            instance() { }

            instance(allocator_type &al) 
                : parent_type(al
                    , detail::create_new<Derived, Ctor>::given(to_abstract(), al, abstract_type::alignment))
            {
            }

            template <class Other, class Ctor2>
            instance(const instance<Other,Base,Alloc,Ctor2> &other) 
                : parent_type(dynamic_cast<derived_type *>(other.to_base()))
            {
                if (other.has_allocator())
                    parent_type::set_allocator(other.get_allocator());
            }

            instance(base_type *ptr)
                : parent_type(dynamic_cast<derived_type *>(ptr))
            {
            }
            instance(abstract_base_type *ptr)
                : parent_type(dynamic_cast<derived_type *>(ptr))
            {
            }
            instance(abstract_base_type *abst, allocator_type &al)
                : parent_type(al, dynamic_cast<derived_type *>(abst))
            {
            }
            template <class A0>
            instance(allocator_type &al, A0 a0) : parent_type(al)
            {
                allocate();
                new (to_derived()) derived_type(a0);
            }
            template <class A0, class A1>
            instance(allocator_type &al, A0 a0, A1 a1) : parent_type(al)
            {
                allocate();
                new (to_derived()) derived_type(a0, a1);
            }
            template <class A0, class A1, class A2>
            instance(allocator_type &al, A0 a0, A1 a1, A2 a2) : parent_type(al)
            {
                allocate();
                new (to_derived()) derived_type(a0, a1, a2);
            }
            this_type clone() const
            {
                if (empty())
                    return this_type(get_allocator());
                return this_type(get_allocator(), to_abstract()->clone_as<derived_type>(get_allocator()));
            }
        };

    } // namespace cloneable

} // namespace boost

#endif // BOOST_CLONEABLE_DETAIL_POINTER_HPP

//EOF
