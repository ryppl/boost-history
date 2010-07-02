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

#define BOOST_CLONEABLE_SCOPE_EXIT(locals)    \
        this_type *self = this;            \
        BOOST_SCOPE_EXIT(locals(self))

#define BOOST_CLONEABLE_SCOPE_EXIT_END \
        BOOST_SCOPE_EXIT_END

namespace boost
{
    namespace cloneable
    {
        namespace detail
        {
            /// common functionality for all containers using the given base and allocator types
            template <class Cont, class Base, class Alloc>
            struct container_base
            {
                typedef Cont container_type;
                typedef Base base_type;
                typedef abstract_base<base_type> abstract_base_type;
                typedef typename detail::make_clone_allocator<Alloc>::type allocator_type;
                typedef instance_base<Base, allocator_type> instance_base_type;

                typedef typename container_type::value_type value_type;
                typedef typename container_type::reference reference;
                typedef typename container_type::const_reference const_reference;
                typedef typename container_type::iterator iterator;
                typedef typename container_type::const_iterator const_iterator;

                /// ensures that the given type T is a valid type for this container.
                /// this provides better information from errors than otherwise.
                template <class T>
                struct validate
                {
                    // can only add cloneable things to a heterogenous container.
                    // to make a type T cloneable, derive from cloneable::base<T>,
                    // or provide your own base-type Base and derive from cloneable::base<T,Base>.
                    // there is no restriction on the Base type.
                    BOOST_STATIC_ASSERT(is_cloneable<T>::value);

                    // you must ensure that base-types for objects that you add to a container
                    // is the same base used by the container
                    typedef is_convertible<T *, base_type *> has_base_type;
                    BOOST_STATIC_ASSERT(has_base_type::value);

                    typedef T type;
                };

                /// an instance of the given derived type suitable for this container
                template <class Derived>
                struct instance 
                    : cloneable::instance<
                        typename validate<Derived>::type
                        , Base
                        , allocator_type
                        , typename traits<typename validate<Derived>::type>::construction_tag>
                {
                    typedef typename validate<Derived>::type derived_type;
                    typedef cloneable::instance<derived_type, Base, allocator_type, typename traits<derived_type>::construction_tag> parent_type;
                    
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
                container_base()
                { 
                }
                container_base(allocator_type &a)
                    : alloc(a) 
                { 
                }

                const allocator_type &get_allocator() const
                {
                    return alloc;
                }
                allocator_type &get_allocator()
                {
                    return alloc;
                }
            };

        } // namespace detail

    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif //  BOOST_CLONEABLE_DETAIL_CONTAINER_BASE_HPP

//EOF