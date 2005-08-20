//////////////////////////////////////////////////////////////////////////////
//
// smart_ptr.hpp
//
//////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copy-
//     right notice appear in all copies and that both that copyright notice
//     and this permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
//////////////////////////////////////////////////////////////////////////////
// Last update (Andrei Alexandrescu): February 19, 2001
//
// Boostified by David Held on April 18, 2002
// 
// Additional suggestions/ideas contributed by:
//     Gennadiy Rozental
//     David Abrahams
//     Giovanni Bajo
//     Beman Dawes
//     Phil Nash
//     Other Boost members
//     And, of course, Andrei Alexandrescu
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SMART_PTR_HPP
#define BOOST_SMART_PTR_HPP

//////////////////////////////////////////////////////////////////////////////
// IMPORTANT NOTE
// Due to threading issues, the OwnershipPolicy has been changed as follows:
//     Release() returns a boolean saying if that was the last release
//        so the pointer can be deleted by the StoragePolicy
//     IsUnique() was removed
//////////////////////////////////////////////////////////////////////////////

// This should get moved to config/compiler/borland.hpp
// bcc has flaky support for non-type class template parameters
#ifdef __BORLANDC__
# define BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
#endif

#define BOOST_DEBUG 1

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/ct_if.hpp>

#ifdef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
#include <boost/type_traits/same_traits.hpp>
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

// Use boost::pool?
//#include "SmallObj.h"

#include <algorithm>                    // for std::less
#include <functional>                   // for std::binary_function
#include <stdexcept>                    // for std::runtime_error

//////////////////////////////////////////////////////////////////////////////
// Used standard assert instead of boost::assert, because the implicit
// behaviour of a Checking Policy is to assert by default, and disable
// assertions with NDEBUG, while boost::assert turns off assertions by
// default, and only enables them with BOOST_DEBUG, which would probably
// suprise many people who select, say, assert_check.
//////////////////////////////////////////////////////////////////////////////
#include <cassert>

namespace boost
{

////////////////////////////////////////////////////////////////////////////////
// class template by_ref
// Transports a reference as a value
// Serves to implement the Colvin/Gibbons trick for smart_ptr
////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template <typename T>
        class by_ref
        {
        public:
            by_ref(T& v) : value_(v) {}
            operator T&() { return value_; }
            operator const T&() const { return value_; }
        private:
            T& value_;
        };

        template <class Policy>
        class policy_forwarder : public Policy { };
    }   // namespace detail

////////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (declaration)
// The reason for all the fuss below
// Default policy declarations
////////////////////////////////////////////////////////////////////////////////

    template <typename> class default_storage_traits;
    template <class> class default_storage;
    template <class> class ref_counted;
    template <class> class disallow_conversion;
    template <class> class assert_check;
/*
    template
    <
        template <typename> class StorageTraits = default_storage_traits
        template <class> class Policy1 = detail::policy_forwarder,
        template <class> class Policy2 = detail::policy_forwarder,
        template <class> class Policy3 = detail::policy_forwarder,
        template <class> class Policy4 = detail::policy_forwarder
    >
    class policy_builder
    {
        template <typename T>
        class basic_policy
        : public StorageTraits<T>, Policy1, Policy2, Policy3, Policy4
        {
        };

        typedef typename ct_if<no_owner,
    }
*/

    template
    <
        typename T,
        class Policy =
            assert_check<
            disallow_conversion<
            ref_counted<
            default_storage<
            default_storage_traits<T
        > > > > >
    >
    class smart_ptr;

////////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (definition)
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy>
    class smart_ptr : public Policy
    {
    private:
        typedef Policy policy;
        typedef smart_ptr this_type;

    public:
        typedef typename policy::pointer_type           pointer_type;
        typedef typename policy::pointer_type const     const_pointer_type;
        typedef typename policy::stored_type            stored_type;
        typedef typename policy::stored_type const      const_stored_type;
        typedef typename policy::reference_type         reference_type;
        typedef typename policy::reference_type const   const_reference_type;

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef typename ct_if<
            policy::destructive_copy_tag, this_type, const this_type
        >::type copy_arg;
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef typename ct_if_t<
            typename policy::destructive_copy_tag, this_type,
            const this_type
        >::type copy_arg;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

        smart_ptr()
        : policy()
        { policy::on_default(get_impl(*this)); }

        smart_ptr(const stored_type& p)
        : policy(p)
        { policy::on_init(get_impl(*this)); }

        smart_ptr(copy_arg& rhs)
        : policy(rhs)
        { get_impl_ref(*this) = policy::clone(get_impl_ref(rhs)); }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U, class Policy1>
        smart_ptr(const smart_ptr<U, Policy1>& rhs)
        : policy(rhs)
        { get_impl_ref(*this) = policy::clone(get_impl_ref(rhs)); }

        template <typename U, class Policy1>
        smart_ptr(smart_ptr<U, Policy1>& rhs)
        : policy(rhs)
        { get_impl_ref(*this) = policy::clone(get_impl_ref(rhs)); }

#endif // BOOST_NO_MEMBER_TEMPLATES

        smart_ptr(detail::by_ref<smart_ptr> rhs)
        : policy(rhs)
        { }


        ~smart_ptr()
        {
            if (policy::release(get_impl(*this)))
            {
                policy::destroy();
            }
        }

        operator detail::by_ref<smart_ptr>()
        {
            return detail::by_ref<smart_ptr>(*this);
        }

        smart_ptr& operator=(copy_arg& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U, class Policy1>
        smart_ptr& operator=(const smart_ptr<U, Policy1>& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

        template <typename U, class Policy1>
        smart_ptr& operator=(smart_ptr<U, Policy1>& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

#else // BOOST_NO_MEMBER_TEMPLATES

        smart_ptr& operator=(const this_type& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

        smart_ptr& operator=(this_type& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

        void swap(smart_ptr& rhs)
        {
            policy::swap(rhs);
        }

        friend inline void release(smart_ptr& sp, stored_type& p)
        {
            policy::on_release(policy::storage());
            p = get_impl_ref(sp);
            get_impl_ref(sp) = policy::default_value();
            policy::init_ownership();
        }

        friend inline void reset(smart_ptr& sp, stored_type p)
        {
            smart_ptr(p).swap(sp);
        }

        pointer_type operator->()
        {
            policy::on_dereference(get_impl_ref(*this));
            return policy::get_pointer();
        }

        const_pointer_type operator->() const
        {
            policy::on_dereference(get_impl_ref(*this));
            return policy::get_pointer();
        }

        reference_type operator*()
        {
            policy::on_dereference(get_impl_ref(*this));
            return policy::get_reference();
        }

        const_reference_type operator*() const
        {
            policy::on_dereference(get_impl_ref(*this));
            return policy::get_reference();
        }

        // Accessors
        friend inline pointer_type get_impl(this_type& sp)
        { return sp.get_pointer(); }

        friend inline const_pointer_type get_impl(const this_type& sp)
        { return sp.get_pointer(); }

        friend inline stored_type& get_impl_ref(this_type& sp)
        { return sp.get_pointer_ref(); }

        friend inline const_stored_type& get_impl_ref(const this_type& sp)
        { return sp.get_pointer_ref(); }

        // Logical Operators
        bool operator!() const // Enables "if (!sp) ..."
        { return !policy::is_valid(); }

        inline friend bool operator==(const smart_ptr& lhs, const T* rhs)
        { return get_impl(lhs) == rhs; }

        inline friend bool operator==(const T* lhs, const smart_ptr& rhs)
        { return rhs == lhs; }

        inline friend bool operator!=(const smart_ptr& lhs, const T* rhs)
        { return !(lhs == rhs); }

        inline friend bool operator!=(const T* lhs, const smart_ptr& rhs)
        { return rhs != lhs; }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        // Ambiguity buster
        template <typename U, class Policy1>
        bool operator==(const smart_ptr<U, Policy1>& rhs) const
        { return *this == get_impl(rhs); }

        // Ambiguity buster
        template <typename U, class Policy1>
        bool operator!=(const smart_ptr<U, Policy1>& rhs) const
        { return !(*this == rhs); }

        // Ambiguity buster
        template <typename U, class Policy1>
        bool operator<(const smart_ptr<U, Policy1>& rhs) const
        { return *this < get_impl(rhs); }

#else // BOOST_NO_MEMBER_TEMPLATES

        // Ambiguity buster
        bool operator==(const smart_ptr& rhs) const
        { return *this == get_impl(rhs); }

        // Ambiguity buster
        bool operator!=(const smart_ptr& rhs) const
        { return !(*this == rhs); }

        // Ambiguity buster
        bool operator<(const smart_ptr& rhs) const
        { return *this < get_impl(rhs); }

#endif // BOOST_NO_MEMBER_TEMPLATES

    private:
        // Helper for enabling 'if (sp)'
        class tester
        {
#ifdef __GNUC__
        public:
            void avoid_warning_about_all_private_members();
#endif // __GNUC__
        private:
            void operator delete(void*);
        };

    public:
        // enable 'if (sp)'
        operator tester*() const
        {
            if (!*this) return 0;
            static tester t;
            return &t;
        }

    private:
#ifndef __BORLANDC__
        typedef typename policy::result_type automatic_conversion_result;

    public:
        operator automatic_conversion_result() const
        { return get_impl(*this); }
#else // __BORLANDC__

// I somehow broke conversion support on bcc, and I can't get it back, so we'll
// just ditch it for now.

        // bcc ICE's on conversion operators to a dependent type of a base
        // class.  To work around this, we use the original ct_if implemen-
        // tation and explicitly define the conversion to T*, which breaks
        // the genericity of the code, but covers the most common case.
//        struct disallow_conversion_result
//        {
//            disallow_conversion_result(pointer_type)
//            { }
//        };

//        typedef typename ct_if_t<
//            typename policy::allow_conversion, T*, disallow_conversion_result
//        >::type automatic_conversion_result;
//        BOOST_STATIC_ASSERT((
//            !policy::allow_conversion::value ||
//            is_same<policy::pointer_type, T*>::value
//        ));
#endif // __BORLANDC__

    };

////////////////////////////////////////////////////////////////////////////////
// class template default_storage_traits
// Implementation of the traits used by default_storage
////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class default_storage_traits
    {
    protected:
        typedef T*          stored_type;    // the type of the pointee_ object
        typedef T const*    const_stored_type;
        typedef T*          pointer_type;   // type returned by operator->
        typedef T const*    const_pointer_type;
        typedef T&          reference_type; // type returned by operator*
        typedef T const&    const_reference_type;

        default_storage_traits()
        { }
        
        default_storage_traits(const stored_type&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template default_storage
// Implementation of the StoragePolicy used by smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class default_storage : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type            stored_type;
        typedef typename policy::const_stored_type      const_stored_type;
        typedef typename policy::pointer_type           pointer_type;
        typedef typename policy::const_pointer_type     const_pointer_type;
        typedef typename policy::reference_type         reference_type;
        typedef typename policy::const_reference_type   const_reference_type;

        default_storage()
        : pointee_(default_value())
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        // The storage policy doesn't initialize the stored pointer
        // which will be initialized by the OwnershipPolicy's Clone fn
        template <class U>
        default_storage(const default_storage<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        default_storage(const default_storage&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        default_storage(const stored_type& p)
        : policy(p), pointee_(p)
        { }

        void swap(default_storage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        pointer_type get_pointer()
        { return pointee_; }

        const_pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference()
        { return *pointee_; }

        const_reference_type get_reference() const
        { return *pointee_; }

        stored_type& get_pointer_ref()
        { return pointee_; }

        stored_type const& get_pointer_ref() const
        { return pointee_; }

        bool is_valid()
        { return pointee_ != 0; }

        void destroy()
        { boost::checked_delete(pointee_); }

        static stored_type default_value()
        { return 0; }

    private:
        // Data
        stored_type pointee_;
    };

////////////////////////////////////////////////////////////////////////////////
// class template ref_counted
// Implementation of the OwnershipPolicy used by smart_ptr
// Provides a classic external reference counting implementation
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class ref_counted : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename Policy::pointer_type pointer_type;
        typedef typename Policy::stored_type stored_type;

        ref_counted()
        {
            init_ownership();
        }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        ref_counted(const ref_counted<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : count_(reinterpret_cast<const ref_counted&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        ref_counted(const ref_counted& rhs)
        : count_(rhs.count_)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        ref_counted(const stored_type& p)
        : policy(p)
        {
            init_ownership();
        }

        void init_ownership()
        {
//            count_ = static_cast<unsigned int*>(
//                SmallObject<>::operator new(sizeof(unsigned int)));
//            BOOST_ASSERT(pCount_);
//            *count_ = 1;
            count_ = new unsigned int(1);
        }

        pointer_type clone(const pointer_type& val)
        {
            ++*count_;
            return val;
        }

        bool release(const pointer_type&)
        {
            if (!--*count_)
            {
                //SmallObject<>::operator delete(pCount_, sizeof(unsigned int));
                delete count_;
                return true;
            }
            return false;
        }

        void swap(ref_counted& rhs)
        {
            std::swap(pCount_, rhs.count_);
        }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class ref_counted;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        unsigned int* count_;
    };

//////////////////////////////////////////////////////////////////////////////
// class template ref_counted_mt
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements external reference counting for multithreaded programs
//////////////////////////////////////////////////////////////////////////////

    // For some reason, gcc 3.0 demands a template argument for ref_counted_mt
    //     as a template argument below.  Giving it one pacifies it.
    template <template <class> class ThreadingModel>
    class ref_counted_mt
#ifndef __GNUC__
        : public ThreadingModel<ref_counted_mt>
#else // __GNUC__
        : public ThreadingModel<ref_counted_mt<ThreadingModel> >
#endif // __GNUC__
    {
    public:
        template <class Policy>
        class impl : public Policy
        {
        protected:
            typedef ThreadingModel<ref_counted_mt> threading_model;
            typedef typename threading_model::int_type int_type;
            typedef Policy policy;
            typedef typename policy::stored_type stored_type;
            typedef typename policy::pointer_type pointer_type;

            impl()
            {
                init_ownership();
            }

            impl(const impl& rhs)
            : count_(rhs.count_)
            { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

            template <typename U>
            impl(const impl<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            : count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            : count_(reinterpret_cast<const ref_counted&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            { }

#endif // BOOST_NO_MEMBER_TEMPLATES

            impl(const stored_type& p)
            : policy(p)
            {
                init_ownership();
            }

            void init_ownership()
            {
//                count_ = static_cast<unsigned int*>(
//                    SmallObject<ThreadingModel>::operator new(
//                        sizeof(unsigned int)));
//                BOOST_ASSERT(count_);
//                *count_ = 1;
                count_ = new unsigned int(1);
            }

            pointer_type clone(const pointer_type& val)
            {
                threading_model::atomic_increment(*count_);
                return val;
            }

            bool release(const pointer_type&)
            {
                if (!threading_model::atomic_decrement(*count_))
                {
//                    SmallObject<ThreadingModel>::operator delete(count_,
//                        sizeof(unsigned int));
                    delete count_;
                    return true;
                }
                return false;
            }

            void swap(impl& rhs)
            {
                std::swap(count_, rhs.count_);
            }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
            enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
            typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

        private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            template <typename U> friend class impl;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

            // Data
            volatile int_type* count_;
        };
    };

////////////////////////////////////////////////////////////////////////////////
// class template com_ref_counted
// Implementation of the OwnershipPolicy used by smart_ptr
// Adapts COM intrusive reference counting to OwnershipPolicy-specific syntax
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class com_ref_counted : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        com_ref_counted()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        com_ref_counted(const com_ref_counted<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        com_ref_counted(const com_ref_counted&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        com_ref_counted(const stored_type& p)
        : policy(p)
        { }

        void init_ownership()
        { }

        static pointer_type clone(const pointer_type& val)
        {
            val->AddRef();
            return val;
        }

        static bool release(const pointer_type& val)
        {
            val->Release();
            return false;
        }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

        static void swap(com_ref_counted&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template ref_linked
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements reference linking
////////////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        class ref_linked_base
        {
        protected:
            ref_linked_base()
            {
                prev_ = next_ = this;
            }

            ref_linked_base(const ref_linked_base& rhs)
            {
                prev_ = &rhs;
                next_ = rhs.next_;
                prev_->next_ = this;
                next_->prev_ = this;
            }

            bool release()
            {
                if (next_ == this)
                {
                    BOOST_ASSERT(prev_ == this);
                    return true;
                }
                prev_->next_ = next_;
                next_->prev_ = prev_;
                return false;
            }

            void swap(ref_linked_base& rhs)
            {
                if (next_ == this)
                {
                    BOOST_ASSERT(prev_ == this);
                    if (rhs.next_ == &rhs)
                    {
                        BOOST_ASSERT(rhs.prev_ == &rhs);
                        // both lists are empty, nothing to do
                        return;
                    }
                    prev_ = rhs.prev_;
                    next_ = rhs.next_;
                    prev_->next_ = next_->prev_ = this;
                    rhs.next_ = rhs.prev_ = &rhs;
                    return;
                }
                if (rhs.next_ == &rhs)
                {
                    rhs.swap(*this);
                    return;
                }
                std::swap(prev_, rhs.prev_);
                std::swap(next_, rhs.next_);
                std::swap(prev_->next_, rhs.prev_->next_);
                std::swap(next_->prev_, rhs.next_->prev_);
            }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
            enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
            typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

        private:
            mutable const ref_linked_base* prev_;
            mutable const ref_linked_base* next_;
        };
    }   // namespace detail

    template <class Policy>
    class ref_linked : public detail::ref_linked_base, Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        ref_linked()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        ref_linked(const ref_linked<U>& rhs)
        : detail::ref_linked_base(rhs)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        ref_linked(const ref_linked& rhs)
        : detail::ref_linked_base(rhs)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        ref_linked(const stored_type& p)
        : policy(p)
        { }

        void init_ownership()
        { }

        static pointer_type clone(const pointer_type& val)
        { return val; }

        static bool release(const pointer_type&)
        { return detail::ref_linked_base::release(); }
    };

////////////////////////////////////////////////////////////////////////////////
// class template destructive_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements destructive copy semantics (a la std::auto_ptr)
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class destructive_copy : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        destructive_copy()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        destructive_copy(const destructive_copy<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        destructive_copy(const destructive_copy&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        destructive_copy(const stored_type& p)
        : policy(p)
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        void init_ownership()
        { }

        template <typename U>
        static pointer_type clone(U& val)
        {
            pointer_type result(val);
            val = U();
            return result;
        }

#else // BOOST_NO_MEMBER_TEMPLATES

// ???

#endif // BOOST_NO_MEMBER_TEMPLATES

        static bool release(const pointer_type&)
        { return true; }

        static void swap(destructive_copy&)
        { }

        // If this flag is called destructive_copy, bcc complains that it has
        //     the same name as the class.
#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = true };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef true_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
    };

////////////////////////////////////////////////////////////////////////////////
// class template deep_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements deep copy semantics, assumes existence of a clone() member
//     function of the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class deep_copy : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        deep_copy()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        deep_copy(const deep_copy<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        deep_copy(const deep_copy&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        deep_copy(const stored_type& p)
        : policy(p)
        { }

        void init_ownership()
        { }

        static pointer_type clone(const pointer_type& val)
        { return val->clone(); }

        static bool release(const pointer_type& val)
        { return true; }

        static void swap(deep_copy&)
        { }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
    };

////////////////////////////////////////////////////////////////////////////////
// class template no_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements a policy that doesn't allow copying objects
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class no_copy : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        no_copy()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        no_copy(const no_copy<U>&)
        { }

#endif

        no_copy(const stored_type& p)
        : policy(p)
        { }

        void init_ownership()
        { }

        static pointer_type clone(const pointer_type&)
        {
            BOOST_STATIC_ASSERT(false);
            // This_Policy_Disallows_Value_Copying
        }

        static bool release(const pointer_type&)
        { return true; }

        static void swap(no_copy&)
        { }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
    };

////////////////////////////////////////////////////////////////////////////////
// class template allow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Allows implicit conversion from smart_ptr to the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class allow_conversion : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

#ifndef __BORLANDC__
        typedef pointer_type result_type;
#else // __BORLANDC__
        typedef true_type allow;
#endif // __BORLANDC__

        allow_conversion()
        { }

        allow_conversion(const stored_type& p)
        : policy(p)
        { }

        static void swap(allow_conversion&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template disallow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Does not allow implicit conversion from smart_ptr to the pointee type
// You can initialize a disallow_conversion with an allow_conversion
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class disallow_conversion : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

#ifndef __BORLANDC__
        struct disallow_conversion_result
        {
            disallow_conversion_result(const pointer_type&)
            { }
        };

        typedef disallow_conversion_result result_type;
#else // __BORLANDC__
        typedef false_type allow;
#endif // __BORLANDC__

        disallow_conversion()
        { }

        disallow_conversion(const allow_conversion<policy>&)
        { }

        disallow_conversion(const stored_type& p)
        : policy(p)
        { }

        static void swap(disallow_conversion&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class null_pointer_error
// Used by some implementations of the CheckingPolicy used by smart_ptr
////////////////////////////////////////////////////////////////////////////////

    struct null_pointer_error : public std::runtime_error
    {
        null_pointer_error() : std::runtime_error("Null Pointer Exception")
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template no_check
// Implementation of the CheckingPolicy used by smart_ptr
// Well, it's clear what it does :o)
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class no_check : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        no_check()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        no_check(const no_check<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        no_check(const no_check&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        no_check(const stored_type& p)
        : policy(p)
        { }

        static void on_default(const pointer_type&)
        { }

        static void on_init(const pointer_type&)
        { }

        static void on_dereference(const pointer_type&)
        { }

        static void on_release(const pointer_type&)
        { }

        static void swap(no_check&)
        { }
    };


////////////////////////////////////////////////////////////////////////////////
// class template assert_check
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class assert_check : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        assert_check()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        assert_check(const assert_check<U>&)
        { }

        template <typename U>
        assert_check(const no_check<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        assert_check(const assert_check&)
        { }

        assert_check(const no_check&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        assert_check(const stored_type& p)
        : policy(p)
        { }

        static void on_default(const pointer_type&)
        { }

        static void on_init(const pointer_type&)
        { }

        static void on_dereference(pointer_type val)
        { assert(val); }

        static void on_release(const pointer_type&)
        { }

        static void swap(assert_check&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template assert_check_strict
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer against zero upon initialization and before dereference
// You can initialize an assert_check_strict with an assert_check
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class assert_check_strict : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        assert_check_strict()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        assert_check_strict(const assert_check_strict<U>&)
        { }

        template <typename U>
        assert_check_strict(const assert_check<U>&)
        { }

        template <typename U>
        assert_check_strict(const no_check<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        assert_check_strict(const assert_check_strict&)
        { }

        assert_check_strict(const assert_check&)
        { }

        assert_check_strict(const no_check&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        assert_check_strict(const stored_type& p)
        : policy(p)
        { }

        static void on_default(pointer_type val)
        { assert(val); }

        static void on_init(pointer_type val)
        { assert(val); }

        static void on_dereference(pointer_type val)
        { assert(val); }

        static void on_release(const pointer_type&)
        { }

        static void swap(assert_check_strict&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null_static
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class reject_null_static : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        reject_null_static()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        reject_null_static(const reject_null_static<U>&)
        { }

        template <typename U>
        reject_null_static(const no_check<U>&)
        { }

        template <typename U>
        reject_null_static(const assert_check<U>&)
        { }

        template <typename U>
        reject_null_static(const assert_check_strict<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        reject_null_static(const reject_null_static&)
        { }

        reject_null_static(const no_check&)
        { }

        reject_null_static(const assert_check&)
        { }

        reject_null_static(const assert_check_strict&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        reject_null_static(const stored_type& p)
        : policy(p)
        { }

        static void on_default(const pointer_type&)
        {
            BOOST_STATIC_ASSERT(false);
            // This_Policy_Does_Not_Allow_Default_Initialization
        }

        static void on_init(pointer_type val)
        { if (!val) throw null_pointer_error(); }

        static void on_dereference(pointer_type val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(const pointer_type&)
        { }

        static void swap(reject_null_static&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class reject_null : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        reject_null()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        reject_null(const reject_null<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        reject_null(const reject_null&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        reject_null(const stored_type& p)
        : policy(p)
        { }

        static void on_default(const pointer_type&)
        { }

        static void on_init(const pointer_type&)
        { }

        static void on_dereference(pointer_type val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(const pointer_type&)
        { }

        static void swap(reject_null&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null_strict
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    class reject_null_strict : public Policy
    {
    protected:
        typedef Policy policy;
        typedef typename policy::stored_type stored_type;
        typedef typename policy::pointer_type pointer_type;

        reject_null_strict()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        reject_null_strict(const reject_null_strict<U>&)
        { }

        template <typename U>
        reject_null_strict(const reject_null<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        reject_null_strict(const reject_null_strict&)
        { }

        reject_null_strict(const reject_null&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        reject_null_strict(const stored_type& p)
        : policy(p)
        { }

        static void on_init(pointer_type val)
        { if (!val) throw null_pointer_error(); }

        static void on_default(pointer_type val)
        {
            BOOST_STATIC_ASSERT(false);
            // This_Policy_Does_Not_Allow_Default_Initialization
        }

        static void on_dereference(pointer_type val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(const pointer_type&)
        { }

        static void swap(reject_null_strict&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// free comparison operators for class template smart_ptr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator==(const smart_ptr<T, Policy>& lhs, const U* rhs)
    { return get_impl(lhs) == rhs; }

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator==(const U* lhs, const smart_ptr<T, Policy>& rhs)
    { return rhs == lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator!=(const smart_ptr<T, Policy>& lhs, const U* rhs)
    { return !(lhs == rhs); }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator!=(const U* lhs, const smart_ptr<T, Policy>& rhs)
    { return rhs != lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = smart_ptr, rhs = raw pointer -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator<(const smart_ptr<T, Policy>& lhs, const U* rhs);

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = raw pointer, rhs = smart_ptr -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator<(const U* lhs, const smart_ptr<T, Policy>& rhs);

////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = smart_ptr, rhs = raw pointer -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator>(const smart_ptr<T, Policy>& lhs, const U* rhs)
    { return rhs < lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator>(const U* lhs, const smart_ptr<T, Policy>& rhs)
    { return rhs < lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator<=(const smart_ptr<T, Policy>& lhs, const U* rhs)
    { return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator<=(const U* lhs, const smart_ptr<T, Policy>& rhs)
    { return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator>=(const smart_ptr<T, Policy>& lhs, const U* rhs)
    { return !(lhs < rhs); }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, class Policy, typename U>
    inline bool operator>=(const U* lhs, const smart_ptr<T, Policy>& rhs)
    { return !(lhs < rhs); }

} // namespace boost

////////////////////////////////////////////////////////////////////////////////
// specialization of std::less for smart_ptr
////////////////////////////////////////////////////////////////////////////////

namespace std
{
    template <typename T, class Policy>
    struct less< boost::smart_ptr<T, Policy> > : public binary_function<
        boost::smart_ptr<T, Policy>, boost::smart_ptr<T, Policy>, bool
    >
    {
        bool operator()(
            const boost::smart_ptr<T, Policy>& lhs,
            const boost::smart_ptr<T, Policy>& rhs
        ) const
        {
            return less<T*>()(get_impl(lhs), get_impl(rhs));
        }
    };
}

#endif // BOOST_SMART_PTR_HPP

