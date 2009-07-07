// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_ALLOCATOR_HPP
#define BOOST_CLONEABLE_ALLOCATOR_HPP

#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/detail/allocation.hpp>

namespace boost
{
    namespace cloneable
    {
        namespace impl
        {
            template <bool is_class>
            struct allocator
            {
                template <class Object, class Alloc>
                static Object *allocate_clone(const Object &orig, Alloc &alloc)
                {
                    typename Alloc::template rebind<Object>::other al(alloc);
                    ptr *clone = al.allocate(1);
                    new (clone) (orig);
                    return clone;
                }
                template <class Object, class Alloc>
                static void deallocate_clone(const Object *ptr, Alloc &alloc)
                {
                    if (!ptr)
                        return;
                    typename Alloc::template rebind<Object>::other al(alloc);
                    al.deallocate(const_cast<Object *>(ptr), 1);
                }
            };
            template <>
            struct allocator<true>
            {
                template <class Object, class Alloc>
                static Object *allocate_clone(const Object &orig, Alloc &alloc)
                {
                    return orig.clone(alloc);
                }

                template <class Object, class Alloc>
                static void deallocate_clone(const Object *ptr, Alloc &alloc)
                {
                    const_cast<Object *>(ptr)->deallocate(alloc);
                }
            };
        }

        /// a cloning allocator
        struct allocator
        {
            template <class Base>
            static Base* allocate_clone(const Base& orig)
            {
                make_clone_allocator<default_allocator> alloc;
                return impl::allocator<boost::is_class<Base>::value>::allocate_clone(orig, alloc);
            }

            template <class Base>
            static void deallocate_clone(const Base* clone)
            {
                if (!clone)
                    return;
                impl::allocator<boost::is_class<Base>::value>::deallocate_clone(clone, alloc);
            }

            template <class Base, class Alloc>
            static Base* allocate_clone(const Base& object, Alloc &alloc )
            {
                return impl::allocator<boost::is_class<Base>::value>::allocate_clone(object, alloc);
            }

            template <class Base, class Alloc>
            static void deallocate_clone(const Base *object, Alloc &alloc )
            {
                impl::allocator<boost::is_class<Base>::value>::deallocate_clone(object, alloc);
            }
        };

        template <class T, class Alloc>
        T *allocate(Alloc &alloc)
        {
            typename Alloc::template rebind<T>::other al(alloc);
            return al.allocate(1);
        }

        template <class T>
        T *allocate(abstract_allocator &alloc)
        {
            abstract_allocator::pointer ptr = alloc.allocate_bytes(sizeof(T), aligned_storage<sizeof(T)>::alignment);
            return reinterpret_cast<T *>(ptr);
        }

        template <class T, class Alloc>
        T *create(Alloc &alloc)
        {
            typename Alloc::template rebind<T>::other al(alloc);
            T *ptr = al.allocate(1);
            al.construct(ptr);
            return ptr;
        }

        template <class T>
        T *create(abstract_allocator &alloc)
        {
            T *ptr = allocate<T>(alloc);
            new (ptr) T();
            return ptr;
        }
        template <class T, class A0>
        T *create(abstract_allocator &alloc, A0 a0)
        {
            T *ptr = allocate<T>(alloc);
            new (ptr) T(a0);
            return ptr;
        }
        template <class T, class A0, class A1>
        T *create(abstract_allocator &alloc, A0 a0,  A1 a1)
        {
            T *ptr = allocate<T>(alloc);
            new (ptr) T(a0, a1);
            return ptr;
        }

        template <class T, class Alloc, class A0>
        T *create(Alloc &alloc, A0 a0)
        {
            typename Alloc::template rebind<T>::other al(alloc);
            T *ptr = al.allocate(1);
            new (ptr) T(a0);
            return ptr;
        }
        template <class T, class Alloc, class A0, class A1>
        T *create(Alloc &alloc, A0 a0, A1 a1)
        {
            typename Alloc::template rebind<T>::other al(alloc);
            T *ptr = al.allocate(1);
            new (ptr) T(a0, a1);
            return ptr;
        }
        template <class T, class Alloc>
        void release(T *ptr, Alloc &alloc)
        {
            typename Alloc::template rebind<T>::other al(alloc);
            al.destroy(ptr);
            al.deallocate(ptr, 1);
        }

    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_ALLOCATOR_HPP

//EOF
