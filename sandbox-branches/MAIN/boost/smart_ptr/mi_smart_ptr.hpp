//////////////////////////////////////////////////////////////////////////////
//
// Multiple Inheritance implementation of boost::loki::smart_ptr
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
// Copyright (C) 2002, David Held.
//
// 02-19-2001: Last update by Andrei Alexandrescu
// 04-18-2002: Boostified by David Held
// 05-01-2002: Added VC6 support, array support,
//                 and boost::smart_ptr emulation
//
// Additional suggestions/ideas contributed by:
//     Gennadiy Rozental
//     David Abrahams
//     Giovanni Bajo
//     Beman Dawes
//     Phil Nash
//     Peter Dimov
//     Greg Colvin
//     Other Boost members
//     And, of course, Andrei Alexandrescu
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SMART_PTR_20020501_HPP
#define BOOST_SMART_PTR_20020501_HPP

//////////////////////////////////////////////////////////////////////////////
// IMPORTANT NOTE
// Due to threading issues, the OwnershipPolicy has been changed as follows:
//     Release() returns a boolean saying if that was the last release
//        so the pointer can be deleted by the StoragePolicy
//     IsUnique() was removed
//////////////////////////////////////////////////////////////////////////////

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/ct_if.hpp>

//////////////////////////////////////////////////////////////////////////////
// Configuration macros
//////////////////////////////////////////////////////////////////////////////

// These should get moved to config/compiler
// bcc has flaky support for non-type class template parameters
#ifdef __BORLANDC__
# define BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
#endif // __BORLANDC__

#ifdef BOOST_MSVC
# define BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS
#endif // BOOST_MSVC

//////////////////////////////////////////////////////////////////////////////

#ifdef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
# include <boost/type_traits/same_traits.hpp>
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable:4181) // qualifier applied to reference type ignored
#endif // BOOST_MSVC

#ifdef BOOST_MSVC6_MEMBER_TEMPLATES
# undef BOOST_NO_MEMBER_TEMPLATES
#endif

//////////////////////////////////////////////////////////////////////////////

// Use boost::pool?
//#include "SmallObj.h"

#include <stdexcept>                    // for std::runtime_error
#include <algorithm>                    // for std::less
#include <functional>                   // for std::binary_function
//////////////////////////////////////////////////////////////////////////////
// Used standard assert instead of boost::assert, because the implicit
// behaviour of a Checking Policy is to assert by default, and disable
// assertions with NDEBUG, while boost::assert turns off assertions by
// default, and only enables them with BOOST_DEBUG, which would probably
// suprise many people who select, say, assert_check.
//////////////////////////////////////////////////////////////////////////////
#include <cassert>

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
# include <pshpack1.h>
#endif // defined(__BORLANDC__) || defined(BOOST_MSVC)

namespace boost
{

//////////////////////////////////////////////////////////////////////////////
// class template by_ref
// Transports a reference as a value
// Serves to implement the Colvin/Gibbons trick for smart_ptr
//////////////////////////////////////////////////////////////////////////////

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
    }   // namespace detail

//////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (declaration)
// The reason for all the fuss below
// Default policy declarations
//////////////////////////////////////////////////////////////////////////////

    template <typename> class ref_counted;
    template <typename> class disallow_conversion;
    template <typename> class assert_check;
    template <typename> class default_storage;

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        template <typename> class OwnershipPolicy = ref_counted,
        template <typename> class ConversionPolicy = disallow_conversion,
        template <typename> class CheckingPolicy = assert_check,
        template <typename> class StoragePolicy = default_storage

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        class OwnershipPolicy = ref_counted<default_storage<T>::pointer_type>,
        class ConversionPolicy = disallow_conversion<default_storage<T>::pointer_type>,
        class CheckingPolicy = assert_check<default_storage<T>::stored_type>,
        class StoragePolicy = default_storage<T>

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    >
    class smart_ptr;

//////////////////////////////////////////////////////////////////////////////
// Helper macros for compilers without template template support
//////////////////////////////////////////////////////////////////////////////

#ifdef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

// Usage:
//   typedef BOOST_SMART_PTR1(int, boost::ref_linked) my_ptr;
#define BOOST_SMART_PTR1(T, OwnershipPolicy)                                \
    boost::smart_ptr<T,                                                     \
        OwnershipPolicy<boost::default_storage<T>::pointer_type>,           \
        boost::disallow_conversion<boost::default_storage<T>::pointer_type>,\
        boost::assert_check<boost::default_storage<T>::stored_type>,        \
        boost::default_storage<T>                                           \
    >

// Usage:
//   typedef BOOST_SMART_PTR2(int, boost::ref_linked, boost::allow_conversion)
//       my_ptr;
#define BOOST_SMART_PTR2(T, OwnershipPolicy, ConversionPolicy)              \
    boost::smart_ptr<T,                                                     \
        OwnershipPolicy<boost::default_storage<T>::pointer_type>,           \
        ConversionPolicy<boost::default_storage<T>::pointer_type>,          \
        boost::assert_check<boost::default_storage<T>::stored_type>,        \
        boost::default_storage<T>                                           \
    >

// Usage:
//   typedef BOOST_SMART_PTR3(int, boost::ref_linked, boost::allow_conversion,
//       boost::reject_null) my_ptr;
#define BOOST_SMART_PTR3(T, OwnershipPolicy, ConversionPolicy,              \
            CheckingPolicy)                                                 \
    boost::smart_ptr<T,                                                     \
        OwnershipPolicy<boost::default_storage<T>::pointer_type>,           \
        ConversionPolicy<boost::default_storage<T>::pointer_type>,          \
        CheckingPolicy<boost::default_storage<T>::stored_type>,             \
        boost::default_storage<T>                                           \
    >

// Usage:
//   typedef BOOST_SMART_PTR4(int, boost::ref_linked, boost::allow_conversion,
//       boost::reject_null, boost::my_storage) my_ptr;
#define BOOST_SMART_PTR4(T, OwnershipPolicy, ConversionPolicy,              \
            CheckingPolicy, StoragePolicy)                                  \
    boost::smart_ptr<T,                                                     \
        OwnershipPolicy<StoragePolicy<T>::pointer_type>,                    \
        ConversionPolicy<StoragePolicy<T>::pointer_type>,                   \
        CheckingPolicy<StoragePolicy<T>::stored_type>,                      \
        StoragePolicy<T>                                                    \
    >

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

//////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (definition)
//////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        template <typename> class OwnershipPolicy,
        template <typename> class ConversionPolicy,
        template <typename> class CheckingPolicy,
        template <typename> class StoragePolicy

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        class OwnershipPolicy,
        class ConversionPolicy,
        class CheckingPolicy,
        class StoragePolicy

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    >
    class smart_ptr

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        : public StoragePolicy<T>
        , public CheckingPolicy<typename StoragePolicy<T>::stored_type>
        , public OwnershipPolicy<typename StoragePolicy<T>::pointer_type>
        , public ConversionPolicy<typename StoragePolicy<T>::pointer_type>

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        : public StoragePolicy
        , public CheckingPolicy
        , public OwnershipPolicy
        , public ConversionPolicy

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    {

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef StoragePolicy<T> storage_policy;
        typedef OwnershipPolicy<typename StoragePolicy<T>::pointer_type>
                                 ownership_policy;
        typedef CheckingPolicy<typename StoragePolicy<T>::stored_type>
                                 checking_policy;
        typedef ConversionPolicy<typename StoragePolicy<T>::pointer_type>
                                 conversion_policy;
        typedef smart_ptr this_type;

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef StoragePolicy storage_policy;
        typedef OwnershipPolicy ownership_policy;
        typedef CheckingPolicy checking_policy;
        typedef ConversionPolicy conversion_policy;
        typedef smart_ptr this_type;

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    public:
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::const_pointer_type
                                                        const_pointer_type;
        typedef typename storage_policy::stored_type    stored_type;
        typedef typename storage_policy::const_stored_type
                                                        const_stored_type;
        typedef typename storage_policy::reference_type reference_type;
        typedef typename storage_policy::const_reference_type
                                                        const_reference_type;

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef typename ct_if<
            ownership_policy::destructive_copy_tag, this_type, const this_type
        >::type copy_arg;
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef typename ct_if_t<
            typename ownership_policy::destructive_copy_tag, this_type,
            const this_type
        >::type copy_arg;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

        smart_ptr()
        { checking_policy::on_default(get_impl(*this)); }

        smart_ptr(copy_arg& rhs)
        : storage_policy(rhs), ownership_policy(rhs), checking_policy(rhs),
            conversion_policy(rhs)
        { get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs)); }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        smart_ptr(const smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs)
        : storage_policy(rhs), ownership_policy(rhs), checking_policy(rhs),
            conversion_policy(rhs)
        { get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs)); }

        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        smart_ptr(smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs)
        : storage_policy(rhs), ownership_policy(rhs), checking_policy(rhs),
            conversion_policy(rhs)
        { get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs)); }

#endif // BOOST_NO_MEMBER_TEMPLATES

        smart_ptr(detail::by_ref<smart_ptr> rhs)
        : storage_policy(rhs), ownership_policy(rhs), checking_policy(rhs),
            conversion_policy(rhs)
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        smart_ptr(U p) : storage_policy(p), ownership_policy(p)
        { checking_policy::on_init(get_impl(*this)); }

        template <typename U, typename V>
        smart_ptr(U p, V v) : storage_policy(p, v), ownership_policy(p, v)
        { checking_policy::on_init(get_impl(*this)); }

#else // BOOST_NO_MEMBER_TEMPLATES

        smart_ptr(const stored_type& p) : storage_policy(p)
        { checking_policy::on_init(get_impl(*this)); }

#endif // BOOST_NO_MEMBER_TEMPLATES

        ~smart_ptr()
        {
            if (ownership_policy::release(get_impl(*this)))
            {
                storage_policy::destroy();
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

        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        smart_ptr& operator=(const smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        smart_ptr& operator=(smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

        void swap(smart_ptr& rhs)
        {
            ownership_policy::swap(rhs);
            conversion_policy::swap(rhs);
            checking_policy::swap(rhs);
            storage_policy::swap(rhs);
        }

        friend inline void release(smart_ptr& sp, stored_type& p)
        {
            checking_policy::on_release(storage_policy::storage());
            p = get_impl_ref(sp);
            get_impl_ref(sp) = storage_policy::default_value();
            ownwership_policy& op = sp;
            op = ownership_policy();
        }

        friend inline void reset(smart_ptr& sp, stored_type p)
        {
            smart_ptr(p).swap(sp);
        }

        pointer_type operator->()
        {
            checking_policy::on_dereference(get_impl_ref(*this));
            return storage_policy::get_pointer();
        }

        const_pointer_type operator->() const
        {
            checking_policy::on_dereference(get_impl_ref(*this));
            return storage_policy::get_pointer();
        }

        reference_type operator*()
        {
            checking_policy::on_dereference(get_impl_ref(*this));
            return storage_policy::get_reference();
        }

        const_reference_type operator*() const
        {
            checking_policy::on_dereference(get_impl_ref(*this));
            return storage_policy::get_reference();
        }

        bool operator!() const // Enables "if (!sp) ..."
        { return !storage_policy::is_valid(); }

        inline friend bool operator==(const smart_ptr& lhs,
            const T* rhs)
        { return get_impl(lhs) == rhs; }

        inline friend bool operator==(const T* lhs,
            const smart_ptr& rhs)
        { return rhs == lhs; }

        inline friend bool operator!=(const smart_ptr& lhs,
            const T* rhs)
        { return !(lhs == rhs); }

        inline friend bool operator!=(const T* lhs,
            const smart_ptr& rhs)
        { return rhs != lhs; }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        // Ambiguity buster
        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        bool operator==(const smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs) const
        { return *this == get_impl(rhs); }

        // Ambiguity buster
        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        bool operator!=(const smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs) const
        { return !(*this == rhs); }

        // Ambiguity buster
        template
        <
            typename U,

# ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy1,
            template <typename> class ConversionPolicy1,
            template <typename> class CheckingPolicy1,
            template <typename> class StoragePolicy1

# else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy1,
            class ConversionPolicy1,
            class CheckingPolicy1,
            class StoragePolicy1

# endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        >
        bool operator<(const smart_ptr<U,
            OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1
        >& rhs) const
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
        typedef typename conversion_policy::result_type automatic_conversion_result;

    public:
        operator automatic_conversion_result() const
        { return get_impl(*this); }
#else // __BORLANDC__

// I somehow broke conversion support on bcc, and I can't get it back, so
// we'll just ditch it for now.

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
//            typename conversion_policy::allow, T*, disallow_conversion_result
//        >::type automatic_conversion_result;
//        BOOST_STATIC_ASSERT((
//            !conversion_policy::allow::value ||
//            is_same<storage_policy::pointer_type, T*>::value
//        ));
#endif // __BORLANDC__

    };

//////////////////////////////////////////////////////////////////////////////
// class template default_storage
// Implementation of the StoragePolicy used by smart_ptr
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class default_storage
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

        default_storage() : pointee_(default_value())
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        default_storage(const default_storage<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        default_storage(const default_storage&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        default_storage(const stored_type& p) : pointee_(p)
        { }

        void swap(default_storage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        // Accessors
        friend inline pointer_type get_impl(const default_storage& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(default_storage& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const default_storage& sp)
        { return sp.pointee_; }

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        void destroy()
        { boost::checked_delete(pointee_); }

        static stored_type default_value()
        { return 0; }

    private:
        // Data
        stored_type pointee_;
    };

//////////////////////////////////////////////////////////////////////////////
// class template array_storage
// Implementation of an array-based StoragePolicy for smart_ptr
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class array_storage
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

        array_storage() : pointee_(default_value())
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        array_storage(const array_storage<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        array_storage(const default_storage&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        array_storage(const stored_type& p) : pointee_(p)
        { }

        void swap(array_storage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        // Accessors
        friend inline pointer_type get_impl(const array_storage& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(array_storage& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const array_storage& sp)
        { return sp.pointee_; }

        // gcc gets confused if this is std::size_t
        reference_type operator[](int i)
        {
            BOOST_ASSERT(i >= 0);
            return get_pointer()[i];
        }

        const reference_type operator[](int i) const
        {
            BOOST_ASSERT(i >= 0);
            return get_pointer()[i];
        }

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        void destroy()
        { boost::checked_array_delete(pointee_); }

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

    template <typename P>
    class ref_counted
    {
    public:
        ref_counted()
        {
//            pCount_ = static_cast<unsigned int*>(
//                SmallObject<>::operator new(sizeof(unsigned int)));
//            BOOST_ASSERT(pCount_);
//            *pCount_ = 1;
            pCount_ = new unsigned int(1);
        }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        ref_counted(const ref_counted<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pCount_(rhs.pCount_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pCount_(reinterpret_cast<const ref_counted&>(rhs).pCount_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        ref_counted(const ref_counted& rhs)
        : pCount_(rhs.pCount_)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        ref_counted(U)
        {
//            pCount_ = static_cast<unsigned int*>(
//                SmallObject<>::operator new(sizeof(unsigned int)));
//            BOOST_ASSERT(pCount_);
//            *pCount_ = 1;
            pCount_ = new unsigned int(1);
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

        P clone(const P& val)
        {
            ++*pCount_;
            return val;
        }

        bool release(const P&)
        {
            if (!--*pCount_)
            {
                //SmallObject<>::operator delete(pCount_, sizeof(unsigned int));
                delete pCount_;
                return true;
            }
            return false;
        }

        void swap(ref_counted& rhs)
        {
            std::swap(pCount_, rhs.pCount_);
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
        unsigned int* pCount_;
    };

//////////////////////////////////////////////////////////////////////////////
// class template ref_counted_mt
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements external reference counting for multithreaded programs
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    template <template <class> class ThreadingModel>
    class ref_counted_mt
#ifndef __GNUC__
    : public ThreadingModel<ref_counted_mt>
#else // __GNUC__
    // For some reason, gcc 3.0 demands a template argument for ref_counted_mt
    //     as a template argument.  Supplying one pacifies it.
    : public ThreadingModel< ref_counted_mt<ThreadingModel> >
#endif // __GNUC__

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    template <class ThreadingModel>
    class ref_counted_mt
    : public ThreadingModel

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    {
    public:
        template <typename P>
        class impl
        {
        protected:
#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS
            typedef ThreadingModel<ref_counted_mt> threading_model;
#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS
            typedef ThreadingModel threading_model;
#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS
            typedef typename threading_model::int_type int_type;

        protected:
            impl()
            {
//                pCount_ = static_cast<unsigned int*>(
//                    SmallObject<ThreadingModel>::operator new(
//                        sizeof(unsigned int)));
//                BOOST_ASSERT(pCount_);
//                *pCount_ = 1;
                pCount_ = new unsigned int(1);
            }

#ifndef BOOST_NO_MEMBER_TEMPLATES

            template <typename U>
            impl(const impl<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            : pCount_(rhs.pCount_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            : pCount_(reinterpret_cast<const impl&>(rhs).pCount_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            { }

#else // BOOST_NO_MEMBER_TEMPLATES

            impl(const impl& rhs)
            : pCount_(rhs.pCount_)
            { }

#endif // BOOST_NO_MEMBER_TEMPLATES

#ifndef BOOST_NO_MEMBER_TEMPLATES

            template <typename U>
            impl(U)
            {
//                pCount_ = static_cast<unsigned int*>(
//                    SmallObject<ThreadingModel>::operator new(
//                        sizeof(unsigned int)));
//                BOOST_ASSERT(pCount_);
//                *pCount_ = 1;
                pCount_ = new unsigned int(1);
            }

#endif // BOOST_NO_MEMBER_TEMPLATES

            P clone(const P& val)
            {
                threading_model::atomic_increment(*pCount_);
                return val;
            }

            bool release(const P&)
            {
                if (!threading_model::atomic_decrement(*pCount_))
                {
//                    SmallObject<ThreadingModel>::operator delete(pCount_,
//                        sizeof(unsigned int));
                    delete pCount_;
                    return true;
                }
                return false;
            }

            void swap(impl& rhs)
            {
                std::swap(pCount_, rhs.pCount_);
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
            volatile int_type* pCount_;
        };
    };

////////////////////////////////////////////////////////////////////////////////
// Helper macro for compilers without template template support
////////////////////////////////////////////////////////////////////////////////

#ifdef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    template <class ThreadingModel>
    class ref_counted_mt;

// Usage:
//   typedef BOOST_SMART_PTR1(int, BOOST_THREADING_MODEL(my_thread)) my_ptr;
#define BOOST_THREADING_MODEL(ThreadingModel) ThreadingModel<ref_counted_mt>::impl

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

////////////////////////////////////////////////////////////////////////////////
// class template com_ref_counted
// Implementation of the OwnershipPolicy used by smart_ptr
// Adapts COM intrusive reference counting to OwnershipPolicy-specific syntax
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class com_ref_counted
    {
    protected:
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

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        com_ref_counted(U p)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        static P clone(const P& val)
        {
            val->AddRef();
            return val;
        }

        static bool release(const P& val)
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

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
    private:
        bool mi_ebo_hack_;
#endif // __BORLANDC__ || BOOST_MSVC
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
            { prev_ = next_ = this; }

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

    template <typename P>
    class ref_linked : public detail::ref_linked_base
    {
    protected:
        ref_linked()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        ref_linked(U)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        static P clone(const P& val)
        { return val; }

        static bool release(const P&)
        { return detail::ref_linked_base::release(); }
    };

////////////////////////////////////////////////////////////////////////////////
// class template destructive_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements destructive copy semantics (a la std::auto_ptr)
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class destructive_copy
    {
    protected:
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

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        destructive_copy(U)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        static P clone(U& val)
        {
            P result(val);
            val = U();
            return result;
        }

#else // BOOST_NO_MEMBER_TEMPLATES

        static P clone(P& val)
        {
            P result(val);
            val = P();
            return result;
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

        static bool release(const P&)
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

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
    private:
        bool mi_ebo_hack_;
#endif // __BORLANDC__ || BOOST_MSVC
    };

////////////////////////////////////////////////////////////////////////////////
// class template deep_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements deep copy semantics, assumes existence of a clone() member
//     function of the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class deep_copy
    {
    protected:
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

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        deep_copy(U)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        static P clone(const P& val)
        { return val->clone(); }

        static bool release(const P& val)
        { return true; }

        static void swap(deep_copy&)
        { }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
    private:
        bool mi_ebo_hack_;
#endif // __BORLANDC__ || BOOST_MSVC
    };

////////////////////////////////////////////////////////////////////////////////
// class template no_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements a policy that doesn't allow copying objects
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class no_copy
    {
    protected:
        no_copy()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        no_copy(const no_copy<U>&)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        no_copy(const no_copy&)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        no_copy(U)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        static P clone(const P&)
        {
            BOOST_STATIC_ASSERT(false);
            // This_Policy_Disallows_Value_Copying
        }

        static bool release(const P&)
        { return true; }

        static void swap(no_copy&)
        { }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
    private:
        bool mi_ebo_hack_;
#endif // __BORLANDC__ || BOOST_MSVC
    };

////////////////////////////////////////////////////////////////////////////////
// class template allow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Allows implicit conversion from smart_ptr to the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class allow_conversion
    {
    protected:
#ifndef __BORLANDC__
        typedef P result_type;
#else // __BORLANDC__
        typedef true_type allow;
#endif // __BORLANDC__

        static void swap(allow_conversion&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template disallow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Does not allow implicit conversion from smart_ptr to the pointee type
// You can initialize a disallow_conversion with an allow_conversion
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class disallow_conversion
    {
    protected:
#ifndef __BORLANDC__
        struct disallow_conversion_result
        {
            disallow_conversion_result(const P&)
            { }
        };

        typedef disallow_conversion_result result_type;
#else // __BORLANDC__
        typedef false_type allow;
#endif // __BORLANDC__

        disallow_conversion()
        { }

        disallow_conversion(const allow_conversion<P>&)
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

    template <typename P>
    class no_check
    {
    protected:
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

        static void on_default(const P&)
        { }

        static void on_init(const P&)
        { }

        static void on_dereference(const P&)
        { }

        static void on_release(const P&)
        { }

        static void swap(no_check&)
        { }
    };


////////////////////////////////////////////////////////////////////////////////
// class template assert_check
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class assert_check
    {
    protected:
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

        static void on_default(const P&)
        { }

        static void on_init(const P&)
        { }

        static void on_dereference(const P& val)
        { assert(val); }

        static void on_release(const P&)
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

    template <typename P>
    class assert_check_strict
    {
    protected:
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

        static void on_default(const P& val)
        { assert(val); }

        static void on_init(const P& val)
        { assert(val); }

        static void on_dereference(const P& val)
        { assert(val); }

        static void on_release(const P&)
        { }

        static void swap(assert_check_strict&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null_static
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class reject_null_static
    {
    protected:
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

        static void on_default(const P&)
        {
            BOOST_STATIC_ASSERT(false);
            // This_Policy_Does_Not_Allow_Default_Initialization
        }

        static void on_init(const P& val)
        { if (!val) throw null_pointer_error(); }

        static void on_dereference(const P& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(const P&)
        { }

        static void swap(reject_null_static&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class reject_null
    {
    protected:
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

        static void on_default(const P& val)
        { }

        static void on_init(const P& val)
        { }

        static void on_dereference(const P& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(const P&)
        { }

        static void swap(reject_null&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null_strict
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class reject_null_strict
    {
    protected:
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

        static void on_init(const P& val)
        { if (!val) throw null_pointer_error(); }

        static void on_default(const P& val)
        {
            BOOST_STATIC_ASSERT(false);
            // This_Policy_Does_Not_Allow_Default_Initialization
        }

        static void on_dereference(const P& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(const P&)
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

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator==(const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& lhs, const U* rhs)
    { return get_impl(lhs) == rhs; }

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator==(const U* lhs, const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& rhs)
    { return rhs == lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator!=(const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& lhs, const U* rhs)
    { return !(lhs == rhs); }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator!=(const U* lhs, const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& rhs)
    { return rhs != lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = smart_ptr, rhs = raw pointer -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator<(const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& lhs, const U* rhs);

////////////////////////////////////////////////////////////////////////////////
// operator< for lhs = raw pointer, rhs = smart_ptr -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator<(const U* lhs, const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& rhs);

////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = smart_ptr, rhs = raw pointer -- NOT DEFINED
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator>(const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& lhs, const U* rhs)
    { return rhs < lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator> for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator>(const U* lhs, const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& rhs)
    { return rhs < lhs; }

////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator<=(const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& lhs, const U* rhs)
    { return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
// operator<= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator<=(const U* lhs, const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& rhs)
    { return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator>=(const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& lhs, const U* rhs)
    { return !(lhs < rhs); }

////////////////////////////////////////////////////////////////////////////////
// operator>= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            template <typename> class OwnershipPolicy,
            template <typename> class ConversionPolicy,
            template <typename> class CheckingPolicy,
            template <typename> class StoragePolicy,

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

            class OwnershipPolicy,
            class ConversionPolicy,
            class CheckingPolicy,
            class StoragePolicy,

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typename U
    >
    inline bool operator>=(const U* lhs, const smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >& rhs)
    { return !(lhs < rhs); }

} // namespace boost

////////////////////////////////////////////////////////////////////////////////
// specialization of std::less for smart_ptr
////////////////////////////////////////////////////////////////////////////////

// MSVC gives a strange error about std::less already begin defined as a
// non-template class.
#ifndef BOOST_MSVC

namespace std
{
    template
    <
        typename T,

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        template <typename> class OwnershipPolicy,
        template <typename> class ConversionPolicy,
        template <typename> class CheckingPolicy,
        template <typename> class StoragePolicy

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        class OwnershipPolicy,
        class ConversionPolicy,
        class CheckingPolicy,
        class StoragePolicy

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    >
    struct less< boost::smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    > > : public binary_function<boost::smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >, boost::smart_ptr<
        T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
    >, bool>
    {
        bool operator()(const boost::smart_ptr<
            T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
        >& lhs, const boost::smart_ptr<
            T, OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
        >& rhs) const
        {
            return less<T*>()(get_impl(lhs), get_impl(rhs));
        }
    };
}

#endif // BOOST_MSVC

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
# include <poppack.h>
#endif

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif // BOOST_MSVC

#endif // BOOST_SMART_PTR_20020501_HPP
