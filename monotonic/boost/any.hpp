// See http://www.boost.org/libs/any for Documentation.

#ifndef BOOST_ANY_INCLUDED
#define BOOST_ANY_INCLUDED

// what:  variant type boost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// where: tested with BCC 5.5, MSVC 6.0, and g++ 2.95

#include <algorithm>
#include <typeinfo>

#include "boost/config.hpp"
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/throw_exception.hp>p
#include <boost/static_assert.hpp>

namespace boost
{
    template <class Alloc = std::allocator<char> >
    class any
    {
        typedef Alloc allocator_type;
        typedef any<allocator_type> any_type;

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
        template <class U, class A0>
        U *construct_type(A0 a0)
        {
            typename allocator_type::template rebind<U>::other alloc(get_allocator());
            U *ptr = alloc.allocate(1);
            new (ptr) U(a0);
            return ptr;
        }
        template <class U, class A0, class A1>
        U *construct_type(A0 a0, A1 a1)
        {
            typename allocator_type::template rebind<U>::other alloc(get_allocator());
            U *ptr = alloc.allocate(1);
            new (ptr) U(a0, a1);
            return ptr;
        }
        template <class U>
        void destroy_type(U *ptr)
        {
            typename allocator_type::template rebind<U>::other alloc(get_allocator());
            alloc.destroy(ptr);
        }
        template <class U>
        void deallocate_type(U *ptr)
        {
            typename allocator_type::template rebind<U>::other alloc(get_allocator());
            alloc.deallocate(ptr, 1);
        }
    public: // structors

        any()
          : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
        {
            content = construct_holder<ValueType>(value);
        }
        
        template<typename ValueType, class Al>
        any(const ValueType & value, Al al)
          : alloc(al)
        {
            content = construct_holder<ValueType>(value);
        }

        any(const any & other)
          : alloc(other.alloc)
        {
            content = other.content ? other.content->clone() : 0;
        }

        ~any()
        {
            destroy_type(content);
            deallocate_type(content);
        }

    public: // modifiers

        any & swap(any & rhs)
        {
            std::swap(content, rhs.content);
            //std::swap(alloc, rhs.alloc);
            return *this;
        }

        template<typename ValueType>
        any & operator=(const ValueType & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        any & operator=(any rhs)
        {
            rhs.swap(*this);
            return *this;
        }

    public: // queries

        bool empty() const
        {
            return !content;
        }

        allocator_type get_allocator() const
        {
            return alloc;
        }

        const std::type_info & type() const
        {
            return content ? content->type() : typeid(void);
        }

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    private: // types
#else
    public: // types (public so any_cast can be non-friend)
#endif

        class placeholder
        {
        public: // structors

            virtual ~placeholder()
            {
            }

        public: // queries

            virtual const std::type_info & type() const = 0;

            virtual placeholder * clone() const = 0;

        };

        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(const ValueType & value, any_type &p)
              : parent(&p), held(value)
            {
            }

        public: // queries

            virtual const std::type_info & type() const
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return parent->construct_holder<ValueType>(held);
            }

        public: // representation

            any_type *parent;
            ValueType held;

        private: // intentionally left unimplemented
            holder & operator=(const holder &);
        };

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

    private: // representation
        template <class U>
        holder<U> *construct_holder(U const &init)
        {
            typename allocator_type::template rebind<holder<U> >::other alloc(get_allocator());
            holder<U> *ptr = alloc.allocate(1);
            new (ptr) holder<U>(init, *this);
            return ptr;
        }

        template<typename ValueType, class Al9>
        friend ValueType * any_cast(any<Al9> *);

        template<typename ValueType, class Al10>
        friend ValueType * unsafe_any_cast(any<Al10> *);

#else

    public: // representation (public so any_cast can be non-friend)

#endif

        allocator_type alloc;
        placeholder * content;

    };

    class bad_any_cast : public std::bad_cast
    {
    public:
        virtual const char * what() const throw()
        {
            return "boost::bad_any_cast: "
                   "failed conversion using boost::any_cast";
        }
    };

    template<typename ValueType, class Al2>
    ValueType * any_cast(any<Al2> * operand)
    {
        typedef any<Al2> any_type;
        typedef typename any_type::template holder<ValueType> holder_type;
        return operand && operand->type() == typeid(ValueType)
                    ? &static_cast<holder_type *>
        (operand->content)->held
                    : 0;
    }

    template<typename ValueType, class Al3>
    inline const ValueType * any_cast(const any<Al3> * operand)
    {
        return any_cast<ValueType>(const_cast<any<Al3> *>(operand));
    }

    template<typename ValueType, class Al4>
    ValueType any_cast(any<Al4> & operand)
    {
        typedef BOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        // If 'nonref' is still reference type, it means the user has not
        // specialized 'remove_reference'.

        // Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
        // to generate specialization of remove_reference for your class
        // See type traits library documentation for details
        BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

        nonref * result = any_cast<nonref>(&operand);
        if(!result)
            boost::throw_exception(bad_any_cast());
        return *result;
    }

    template<typename ValueType, class Al5>
    inline ValueType any_cast(const any<Al5> & operand)
    {
        typedef BOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        // The comment in the above version of 'any_cast' explains when this
        // assert is fired and what to do.
        BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

        return any_cast<const nonref &>(const_cast<any<Al5> &>(operand));
    }

    // Note: The "unsafe" versions of any_cast are not part of the
    // public interface and may be removed at any time. They are
    // required where we know what type is stored in the any and can't
    // use typeid() comparison, e.g., when our types may travel across
    // different shared libraries.
    template<typename ValueType, class Al6>
    inline ValueType * unsafe_any_cast(any<Al6> * operand)
    {
        return &static_cast<typename any<Al6>::template holder<ValueType> *>(operand->content)->held;
    }

    template<typename ValueType, class Al7>
    inline const ValueType * unsafe_any_cast(const any<Al7> * operand)
    {
        return unsafe_any_cast<ValueType>(const_cast<any<Al7> *>(operand));
    }
}

// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif
