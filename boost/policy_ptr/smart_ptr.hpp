//////////////////////////////////////////////////////////////////////////////
//
// Multiple Inheritance implementation of policy_ptr/smart_ptr
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
// 10-01-2002: Added cast operators a la boost::shared_ptr<>
// 09-30-2002: Completed most of the documentation
// 09-20-2002: Implemented optimally_inherit, as suggested by Andrei
// 05-01-2002: Added VC6 support, array support,
//                 and boost::smart_ptr emulation
// 04-18-2002: Boostified by David Held
// 02-19-2001: Last update by Andrei Alexandrescu
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
//
// See http://www.boost.org/ for most recent version, including documentation.
//////////////////////////////////////////////////////////////////////////////
// smart_ptr is a policy-based smart pointer framework.  See
// libs/policy_ptr/doc/index.html
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SMART_PTR_20020920_HPP
#define BOOST_SMART_PTR_20020920_HPP

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
#include <boost/throw_exception.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/aux_/void_spec.hpp>
#include <boost/mpl/aux_/config/eti.hpp>

#include "../optimally_inherit.hpp"

//////////////////////////////////////////////////////////////////////////////
// Configuration macros
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_MSVC6_MEMBER_TEMPLATES
# error This library requires member template support.
#endif

#ifdef BOOST_NO_TEMPLATE_TEMPLATES
# undef BOOST_SMART_POINTER_LEGACY_INTERFACE
#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable:4181) // qualifier applied to reference type ignored
#endif // BOOST_MSVC

//////////////////////////////////////////////////////////////////////////////

// Use boost::pool?
//#include "SmallObj.h"

#include <stdexcept>                    // std::runtime_error
#include <typeinfo>                     // std::bad_cast
#include <algorithm>                    // std::less<>, std::swap()
#include <functional>                   // std::binary_function<>
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
            by_ref(T& value) : value_(value) { }
            operator T&() { return value_; }
            operator T const&() const { return value_; }
        private:
            T& value_;
        };

        struct static_cast_tag;
        struct dynamic_cast_tag;
        struct polymorphic_cast_tag;

    }   // namespace detail

//////////////////////////////////////////////////////////////////////////////
// Ownership categories that each OwnershipPolicy must declare
//////////////////////////////////////////////////////////////////////////////

    class copy_semantics_tag { };
    class move_semantics_tag { };
    class no_copy_semantics_tag { };

    class storage_policy_tag { };
    class ownership_policy_tag { };
    class conversion_policy_tag { };
    class checking_policy_tag { };

//////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (declaration)
// The reason for all the fuss below
// Default policy declarations
//////////////////////////////////////////////////////////////////////////////

    struct empty_policy
    { typedef void policy_category; };

    template <typename> class ref_counted;
    template <typename> struct disallow_conversion;
    template <typename> struct assert_check;
    template <typename> class scalar_storage;

    template
    <
        typename T,

#ifndef BOOST_SMART_POINTER_LEGACY_INTERFACE

        class P1 = empty_policy,
        class P2 = empty_policy,
        class P3 = empty_policy,
        class P4 = empty_policy

#else // BOOST_SMART_POINTER_LEGACY_INTERFACE

        template <typename> class OwnershipPolicy = ref_counted,
        template <typename> class ConversionPolicy = disallow_conversion,
        template <typename> class CheckingPolicy = assert_check,
        template <typename> class StoragePolicy = scalar_storage

#endif // BOOST_SMART_POINTER_LEGACY_INTERFACE

    >
    class smart_ptr;

//////////////////////////////////////////////////////////////////////////////
// policy adaptor
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SMART_POINTER_LEGACY_INTERFACE

}   // namespace boost

#include <boost/mpl/identity.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/is_placeholder.hpp>

namespace boost
{

    using mpl::_;

    namespace detail
    {

# define AUX_NESTED_TYPE_ACCESSOR_DEF(name)                                  \
    template <typename T> struct name                                        \
    { typedef typename T::name type; };

# ifndef BOOST_MPL_MSVC_ETI_BUG
#  define AUX_NESTED_TYPE_ACCESSOR(name)                                     \
    AUX_NESTED_TYPE_ACCESSOR_DEF(name)
# else // BOOST_MPL_MSVC_ETI_BUG
#  define AUX_NESTED_TYPE_ACCESSOR(name)                                     \
    AUX_NESTED_TYPE_ACCESSOR_DEF(name)                                       \
    template <> struct name<int>                                             \
    { typedef int type; };
# endif // BOOST_MPL_MSVC_ETI_BUG

        AUX_NESTED_TYPE_ACCESSOR(pointer_type)
        AUX_NESTED_TYPE_ACCESSOR(const_pointer_type)
        AUX_NESTED_TYPE_ACCESSOR(stored_type)
        AUX_NESTED_TYPE_ACCESSOR(const_stored_type)
        AUX_NESTED_TYPE_ACCESSOR(reference_type)
        AUX_NESTED_TYPE_ACCESSOR(const_reference_type)
        AUX_NESTED_TYPE_ACCESSOR(ownership_category)
        AUX_NESTED_TYPE_ACCESSOR(result_type)

# undef AUX_NESTED_TYPE_ACCESSOR
# undef AUX_NESTED_TYPE_ACCESSOR_DEF

        template <class F, typename T>
        struct apply_lambda
        {
            typedef typename mpl::lambda<F>::type f_;
            typedef typename mpl::apply1<f_, T>::type type;
        };

        template <class Policy>
        struct get_category_impl
        {
            typedef typename Policy::policy_category type;
        };

        template <class Policy>
        struct get_category : public mpl::if_<
            mpl::is_placeholder<Policy>,
            mpl::identity<Policy>,
            get_category_impl<Policy>
        >::type
        {
            BOOST_MPL_AUX_LAMBDA_SUPPORT(1, get_category, (Policy))
        };

        template <class Sequence, class Category, class Default>
        struct get_policy
        {
            typedef typename mpl::find_if<
                Sequence, is_same<get_category<_>, Category>
            >::type iter_;
            typedef typename mpl::end<Sequence>::type last_;
            typedef typename mpl::apply_if<
                typename is_same<iter_, last_>::type,
                mpl::identity<Default>, iter_
            >::type type;
        };

	    template <typename T, class Policies>
	    struct storage_policy_
	    {
            typedef typename apply_lambda<
                typename get_policy<
		            Policies, storage_policy_tag, scalar_storage<_>
	            >::type, T
            >::type type;
        };

        template <typename T, class Policies>
        struct ownership_policy_
        {
            typedef typename storage_policy_<T, Policies>::type policy_;
            typedef typename pointer_type<policy_>::type pointer_type_;
            typedef typename apply_lambda<
                typename get_policy<
                    Policies, ownership_policy_tag, ref_counted<_>
                >::type, pointer_type_
            >::type type;
        };

        template <typename T, class Policies>
        struct checking_policy_
        {
            typedef typename storage_policy_<T, Policies>::type policy_;
            typedef typename stored_type<policy_>::type stored_type_;
            typedef typename apply_lambda<
                typename get_policy<
                    Policies, checking_policy_tag, assert_check<_>
                >::type, stored_type_
            >::type type;
        };

        template <typename T, class Policies>
        struct conversion_policy_
        {
            typedef typename storage_policy_<T, Policies>::type policy_;
            typedef typename pointer_type<policy_>::type pointer_type_;
            typedef typename apply_lambda<
                typename get_policy<
                    Policies, conversion_policy_tag, disallow_conversion<_>
                >::type, pointer_type_
            >::type type;
        };

    }   // namespace detail

# define BOOST_SMART_POINTER_PARAMETERS                                      \
    class P1, class P2, class P3, class P4
# define BOOST_CONVERSION_PARAMETERS                                         \
    class Q1, class Q2, class Q3, class Q4
#define BOOST_SMART_POINTER_POLICIES                                         \
    P1, P2, P3, P4
#define BOOST_CONVERSION_POLICIES                                            \
    Q1, Q2, Q3, Q4

# define BOOST_STORAGE_POLICY                                                \
    typename detail::storage_policy_<T, mpl::list<P1, P2, P3, P4> >::type
# define BOOST_OWNERSHIP_POLICY                                              \
    typename detail::ownership_policy_<T, mpl::list<P1, P2, P3, P4> >::type
# define BOOST_CHECKING_POLICY                                               \
    typename detail::checking_policy_<T, mpl::list<P1, P2, P3, P4> >::type
# define BOOST_CONVERSION_POLICY                                             \
    typename detail::conversion_policy_<T, mpl::list<P1, P2, P3, P4> >::type

#else // BOOST_SMART_POINTER_LEGACY_INTERFACE

# define BOOST_SMART_POINTER_PARAMETERS                                      \
    template <typename> class OwnershipPolicy,                               \
    template <typename> class ConversionPolicy,                              \
    template <typename> class CheckingPolicy,                                \
    template <typename> class StoragePolicy
# define BOOST_CONVERSION_PARAMETERS                                         \
    template <typename> class OwnershipPolicy1,                              \
    template <typename> class ConversionPolicy1,                             \
    template <typename> class CheckingPolicy1,                               \
    template <typename> class StoragePolicy1
#define BOOST_SMART_POINTER_POLICIES                                         \
    OwnershipPolicy, ConversionPolicy, CheckingPolicy, StoragePolicy
#define BOOST_CONVERSION_POLICIES                                            \
    OwnershipPolicy1, ConversionPolicy1, CheckingPolicy1, StoragePolicy1

# define BOOST_STORAGE_POLICY       StoragePolicy<T>
# define BOOST_OWNERSHIP_POLICY     OwnershipPolicy<typename StoragePolicy<T>::pointer_type>
# define BOOST_CHECKING_POLICY      CheckingPolicy<typename StoragePolicy<T>::stored_type>
# define BOOST_CONVERSION_POLICY    ConversionPolicy<typename StoragePolicy<T>::pointer_type>

#endif // BOOST_SMART_POINTER_LEGACY_INTERFACE

//////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (definition)
//////////////////////////////////////////////////////////////////////////////

    template <typename T, class P1, class P2, class P3, class P4>
    class smart_ptr
        : public optimally_inherit<
            typename optimally_inherit<
                BOOST_STORAGE_POLICY,
                BOOST_OWNERSHIP_POLICY
            >::type,
            typename optimally_inherit<
                BOOST_CHECKING_POLICY,
                BOOST_CONVERSION_POLICY
            >::type
        >::type
    {
    public:     // Policy types
        typedef T                                           element_type;
        typedef BOOST_STORAGE_POLICY                        storage_policy;
        typedef BOOST_OWNERSHIP_POLICY                      ownership_policy;
        typedef BOOST_CHECKING_POLICY                       checking_policy;
        typedef BOOST_CONVERSION_POLICY                     conversion_policy;
        typedef typename optimally_inherit<
            typename optimally_inherit<
                BOOST_STORAGE_POLICY,
                BOOST_OWNERSHIP_POLICY
            >::type,
            typename optimally_inherit<
                BOOST_CHECKING_POLICY,
                BOOST_CONVERSION_POLICY
            >::type
        >::type                                             base_type;
//        typedef typename base_type::base1_type::base1_type  storage_base;
//        typedef typename base_type::base1_type::base2_type  ownership_base;
//        typedef typename base_type::base2_type::base1_type  checking_base;
//        typedef typename base_type::base2_type::base2_type  conversion_base;
        typedef smart_ptr                                   this_type;

    public:     // Pointer/Reference types
        typedef typename detail::pointer_type<storage_policy>::type
                                                            pointer_type;
        typedef typename detail::const_pointer_type<storage_policy>::type
                                                            const_pointer_type;
        typedef typename detail::stored_type<storage_policy>::type
                                                            stored_type;
        typedef typename detail::const_stored_type<storage_policy>::type
                                                            const_stored_type;
        typedef typename detail::reference_type<storage_policy>::type
                                                            reference_type;
        typedef typename detail::const_reference_type<storage_policy>::type
                                                            const_reference_type;

        typedef typename mpl::if_<
            ::boost::is_same<
                typename detail::ownership_category<ownership_policy>::type,
                move_semantics_tag
            >,
            this_type&, this_type const&
        >::type copy_arg;
        typedef typename mpl::if_<
            ::boost::is_same<
                typename detail::ownership_category<ownership_policy>::type,
                move_semantics_tag
            >,
            base_type&, base_type const&
        >::type copy_base;

    public:     // Constructors/Destructor
        smart_ptr()
        { checking_policy::on_default(get_impl(*this)); }

        smart_ptr(copy_arg rhs)
        : base_type(static_cast<copy_base>(rhs))
        { get_impl_ref(*this) = ownership_policy::clone(get_impl(rhs)); }

        template <typename U, BOOST_CONVERSION_PARAMETERS>
        smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs)
        : base_type(static_cast<
            typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
        >(rhs))
        { get_impl_ref(*this) = ownership_policy::clone(get_impl(rhs)); }

        template <typename U, BOOST_CONVERSION_PARAMETERS>
        smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES>& rhs)
        : base_type(static_cast<
            typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
        >(rhs))
        { get_impl_ref(*this) = ownership_policy::clone(get_impl(rhs)); }

        template <typename U, BOOST_CONVERSION_PARAMETERS>
        smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs,
            detail::static_cast_tag const&)
        : base_type(static_cast<
            typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
        >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                static_cast<pointer_type>(get_impl(rhs))
            );
        }

        template <typename U, BOOST_CONVERSION_PARAMETERS>
        smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs,
            detail::dynamic_cast_tag const&)
        : base_type(static_cast<
            typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
        >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                dynamic_cast<pointer_type>(get_impl(rhs))
            );
            if (!*this)
            {
                // dynamic_cast<> failed; reset count
                this_type().swap(*this);
            }
        }

        template <typename U, BOOST_CONVERSION_PARAMETERS>
        smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs,
            detail::polymorphic_cast_tag const&)
        : base_type(static_cast<
            typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
        >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                dynamic_cast<pointer_type>(get_impl(rhs))
            );
            if (!*this)
            {
                boost::throw_exception(std::bad_cast());
            }
        }

        smart_ptr(detail::by_ref<smart_ptr> rhs)
        : base_type(static_cast<base_type&>(static_cast<smart_ptr&>(rhs)))
        { }

        template <typename U>
        smart_ptr(U const& p)
        : base_type(p, detail::init_first_tag())
        { checking_policy::on_init(get_impl(*this)); }

        template <typename U, typename V>
        smart_ptr(U const& p, V const& v)
        : base_type(p, v, detail::init_first_tag())
        { checking_policy::on_init(get_impl(*this)); }

        ~smart_ptr()
        {
            if (ownership_policy::release(get_impl(*this)))
            {
                storage_policy::destroy();
            }
        }

    public:     // Ownership modifiers
        operator detail::by_ref<smart_ptr>()
        {
            return detail::by_ref<smart_ptr>(*this);
        }

        smart_ptr& operator=(copy_arg rhs)
        {
            smart_ptr(rhs).swap(*this);
            return *this;
        }

        template <typename U, BOOST_CONVERSION_PARAMETERS>
        smart_ptr& operator=(smart_ptr<U, BOOST_CONVERSION_POLICIES> rhs)
        {
            swap(rhs);
            return *this;
        }

        void swap(this_type& rhs)
        {
            base_type::swap(rhs);
        }

        friend inline void release(this_type& sp, stored_type& p)
        {
            checking_policy::on_release(storage_policy::storage());
            p = get_impl_ref(sp);
            get_impl_ref(sp) = storage_policy::default_value();
            ownership_policy& op = sp;
            op = ownership_policy();
        }

        friend inline void reset(this_type& sp, stored_type p)
        {
            this_type(p).swap(sp);
        }

    public:     // Dereference
        pointer_type operator->() const
        {
            checking_policy::on_dereference(get_impl(*this));
            return storage_policy::get_pointer();
        }

        reference_type operator*() const
        {
            checking_policy::on_dereference(get_impl(*this));
            return storage_policy::get_reference();
        }

    public:     // Comparison
        bool operator!() const // Enables "if (!sp) ..."
        { return !storage_policy::is_valid(); }

#ifndef __BORLANDC__
        inline friend bool operator==(smart_ptr const& lhs, T const* rhs)
        { return get_impl(lhs) == rhs; }

        inline friend bool operator==(T const* lhs, smart_ptr const& rhs)
        { return rhs == lhs; }

        inline friend bool operator!=(smart_ptr const& lhs, T const* rhs)
        { return !(lhs == rhs); }

        inline friend bool operator!=(T const* lhs, this_type const& rhs)
        { return rhs != lhs; }
#endif

        // Ambiguity buster
        template <typename U, BOOST_CONVERSION_PARAMETERS>
        bool operator==(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs) const
        { return *this == get_impl(rhs); }

        // Ambiguity buster
        template <typename U, BOOST_CONVERSION_PARAMETERS>
        bool operator!=(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs) const
        { return !(*this == rhs); }

        // Ambiguity buster
        template <typename U, BOOST_CONVERSION_PARAMETERS>
        bool operator<(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs) const
        { return *this < get_impl(rhs); }

    private:    // Helper for enabling 'if (sp)'
        class tester
        {
        public:
            void avoid_gcc_warning_about_all_private_members();
        private:
            void operator delete(void*);
        };

    public:     // enable 'if (sp)'
        operator tester*() const
        {
            if (!*this) return 0;
            static tester t;
            return &t;
        }

    private:
        typedef typename detail::result_type<conversion_policy>::type 
                                                automatic_conversion_result;

// VC6 gets horribly confused by the conversion operator
#ifndef BOOST_MSVC
    public:     // Implicit conversion
        operator automatic_conversion_result() const
        { return get_impl(*this); }
#endif
    };

//////////////////////////////////////////////////////////////////////////////
// Default policies
// These are the default policies provided for smart_ptr
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// class template scalar_storage
// Implementation of the StoragePolicy used by smart_ptr
//////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    struct storage_policy_base
    {
        typedef Policy type;
        typedef storage_policy_tag policy_category;
    };

    template <typename T>
    class scalar_storage : public storage_policy_base< scalar_storage<T> >
    {
    public:
        typedef T*          stored_type;        // the type of the pointee_
        typedef T const*    const_stored_type;  //   object
        typedef T*          pointer_type;       // type returned by operator->
        typedef T const*    const_pointer_type;
        typedef add_reference<T>::type
                            reference_type;     // type returned by operator*
        typedef add_reference<T const>::type
                            const_reference_type;

    protected:
        scalar_storage() : pointee_(default_value())
        { }

        scalar_storage(scalar_storage const&)
        : pointee_(default_value())
        { }

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        template <typename U>
        scalar_storage(scalar_storage<U> const& rhs)
        : pointee_(default_value())
        { }

        scalar_storage(stored_type const& p) : pointee_(p)
        { }

        void swap(scalar_storage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        friend inline pointer_type get_impl(scalar_storage const& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(scalar_storage& sp)
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

        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, scalar_storage, (T))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, scalar_storage) }
#endif

//////////////////////////////////////////////////////////////////////////////
// class template array_storage
// Implementation of an array-based StoragePolicy for smart_ptr
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class array_storage : public storage_policy_base< array_storage<T> >
    {
    public:
        typedef T*          stored_type;        // the type of the pointee_
        typedef T const*    const_stored_type;  //   object
        typedef T*          pointer_type;       // type returned by operator->
        typedef T const*    const_pointer_type;
        typedef add_reference<T>::type
                            reference_type;     // type returned by operator*
        typedef add_reference<T const>::type
                            const_reference_type;

    protected:
        array_storage() : pointee_(default_value())
        { }

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        template <typename U>
        array_storage(array_storage<U> const&)
        { }

        array_storage(stored_type const& p) : pointee_(p)
        { }

        void swap(array_storage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        const_pointer_type get_pointer() const
        { return pointee_; }

        const_reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        // Accessors
        friend inline pointer_type get_impl(array_storage const& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(array_storage const& sp)
        { return sp.pointee_; }

        // gcc gets confused if this is std::size_t
        const_reference_type operator[](int i) const
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

        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, array_storage, (T))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, array_storage) }
#endif

////////////////////////////////////////////////////////////////////////////////
// Ownership Policies
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class template ref_counted
// Implementation of the OwnershipPolicy used by smart_ptr
// Provides a classic external reference counting implementation
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class ref_counted
    {
    public:
        typedef ref_counted type;
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;

    protected:
        ref_counted()
        {
//            pCount_ = static_cast<unsigned int*>(
//                SmallObject<>::operator new(sizeof(unsigned int)));
//            BOOST_ASSERT(pCount_);
//            *pCount_ = 1;
            pCount_ = new unsigned(1);
        }

        template <typename U>
        ref_counted(ref_counted<U> const& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pCount_(rhs.pCount_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pCount_(reinterpret_cast<ref_counted const&>(rhs).pCount_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        // GCC incorrectly chooses the greedy c'tor below, instead of the
        // conversion c'tor above.  So this weaker pointer c'tor must be
        // used instead.
#ifdef __GCC__
        ref_counted(P const&)
#else
        template <typename U>
        ref_counted(U const&)
#endif
        {
//            pCount_ = static_cast<unsigned int*>(
//                SmallObject<>::operator new(sizeof(unsigned int)));
//            BOOST_ASSERT(pCount_);
//            *pCount_ = 1;
            pCount_ = new unsigned(1);
        }

        P clone(P const& val)
        {
            ++*pCount_;
            return val;
        }

        bool release(P const&)
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

    public:
        friend inline unsigned use_count(ref_counted const& p)
        { return *(p.pCount_); }

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class ref_counted;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        unsigned* pCount_;

        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, ref_counted, (P))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, ref_counted) }
#endif

//////////////////////////////////////////////////////////////////////////////
// class template ref_counted_mt
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements external reference counting for multithreaded programs
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SMART_POINTER_LEGACY_INTERFACE

    template <class ThreadingModel>
    class ref_counted_mt
    : public ThreadingModel

#else // BOOST_SMART_POINTER_LEGACY_INTERFACE

    template <template <class> class ThreadingModel>
    class ref_counted_mt
#ifndef __GNUC__
    : public ThreadingModel<ref_counted_mt>
#else // __GNUC__
    // For some reason, gcc 3.0 demands a template argument for ref_counted_mt
    //     as a template argument.  Supplying one pacifies it.
    : public ThreadingModel< ref_counted_mt<ThreadingModel> >
#endif // __GNUC__

#endif // BOOST_SMART_POINTER_LEGACY_INTERFACE

    {
    public:
        template <typename P>
        class impl
        {
        public:
            typedef ownership_policy_tag policy_category;
            typedef copy_semantics_tag ownership_category;

        protected:
#ifndef BOOST_SMART_POINTER_LEGACY_INTERFACE
            typedef ThreadingModel threading_model;
#else // BOOST_SMART_POINTER_LEGACY_INTERFACE
            typedef ThreadingModel<ref_counted_mt> threading_model;
#endif // BOOST_SMART_POINTER_LEGACY_INTERFACE
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

            template <typename U>
            impl(impl<U> const& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            : pCount_(rhs.pCount_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            : pCount_(reinterpret_cast<const impl&>(rhs).pCount_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
            { }

            template <typename U>
            impl(U const&)
            {
//                pCount_ = static_cast<unsigned int*>(
//                    SmallObject<ThreadingModel>::operator new(
//                        sizeof(unsigned int)));
//                BOOST_ASSERT(pCount_);
//                *pCount_ = 1;
                pCount_ = new unsigned int(1);
            }

            P clone(P const& val)
            {
                threading_model::atomic_increment(*pCount_);
                return val;
            }

            bool release(P const&)
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

#ifdef BOOST_SMART_POINTER_LEGACY_INTERFACE

    template <class ThreadingModel>
    class ref_counted_mt;

// Usage:
//   typedef BOOST_SMART_PTR1(int, BOOST_THREADING_MODEL(my_thread)) my_ptr;
#define BOOST_THREADING_MODEL(ThreadingModel) ThreadingModel<ref_counted_mt>::impl

#endif // BOOST_SMART_POINTER_LEGACY_INTERFACE

////////////////////////////////////////////////////////////////////////////////
// class template com_ref_counted
// Implementation of the OwnershipPolicy used by smart_ptr
// Adapts COM intrusive reference counting to OwnershipPolicy-specific syntax
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct com_ref_counted
    {
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;

        com_ref_counted()
        { }

        template <typename U>
        com_ref_counted(com_ref_counted<U> const&)
        { }

        template <typename U>
        com_ref_counted(U const&)
        { }

        static P clone(P const& val)
        {
            val->AddRef();
            return val;
        }

        static bool release(P const& val)
        {
            val->Release();
            return false;
        }

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
        public:
            ref_linked_base()
            { prev_ = next_ = this; }

            ref_linked_base(ref_linked_base const& rhs)
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

        private:
            mutable const ref_linked_base* prev_;
            mutable const ref_linked_base* next_;
        };
    }   // namespace detail

    template <typename P>
    struct ref_linked : public detail::ref_linked_base
    {
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;

        ref_linked()
        { }

        template <typename U>
        ref_linked(ref_linked<U> const& rhs)
        : detail::ref_linked_base(rhs)
        { }

        template <typename U>
        ref_linked(U const&)
        { }

        static P clone(P const& val)
        { return val; }

        static bool release(P const&)
        { return detail::ref_linked_base::release(); }
    };

////////////////////////////////////////////////////////////////////////////////
// class template destructive_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements destructive copy semantics (a la std::auto_ptr)
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct destructive_copy
    {
        typedef ownership_policy_tag policy_category;
        typedef move_semantics_tag ownership_category;

        destructive_copy()
        { }

        template <typename U>
        destructive_copy(destructive_copy<U> const&)
        { }

        template <typename U>
        destructive_copy(U const&)
        { }

        template <typename U>
        static P clone(U& val)
        {
            P result(val);
            val = U();
            return result;
        }

        static bool release(P const&)
        { return true; }

        static void swap(destructive_copy&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template deep_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements deep copy semantics, assumes existence of a clone() member
//     function of the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct deep_copy
    {
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;

        deep_copy()
        { }

        template <typename U>
        deep_copy(deep_copy<U> const&)
        { }

        template <typename U>
        deep_copy(U const&)
        { }

        static P clone(P const& val)
        { return val->clone(); }

        static bool release(P const&)
        { return true; }

        static void swap(deep_copy&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template no_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements a policy that doesn't allow copying objects
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct no_copy
    {
        typedef ownership_policy_tag policy_category;
        typedef no_copy_semantics_tag ownership_category;

        no_copy()
        { }

        template <typename U>
        no_copy(no_copy<U> const&)
        { }

        template <typename U>
        no_copy(U)
        { }

        static P clone(P const&)
        {
            // This Policy Disallows Value Copying
            BOOST_STATIC_ASSERT(false);
        }

        static bool release(P const&)
        { return true; }

        static void swap(no_copy&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template allow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Allows implicit conversion from smart_ptr to the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct allow_conversion
    {
        typedef conversion_policy_tag policy_category;

        typedef P result_type;

        allow_conversion()
        { }

        template <typename U>
        allow_conversion(allow_conversion<U> const&, long)
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

    template <typename P>
    struct disallow_conversion
    {
        typedef disallow_conversion type;
        typedef conversion_policy_tag policy_category;

        struct disallow_conversion_result
        {
            disallow_conversion_result(P const&)
            { }
        };

        typedef disallow_conversion_result result_type;

        disallow_conversion()
        { }

        template <typename U>
        disallow_conversion(disallow_conversion<U> const&)
        { }

        template <typename U>
        disallow_conversion(allow_conversion<U> const&)
        { }

        static void swap(disallow_conversion&)
        { }

        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, disallow_conversion, (P))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, disallow_conversion) }
#endif

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
    struct no_check
    {
        typedef checking_policy_tag policy_category;

        no_check()
        { }

        template <typename U>
        no_check(no_check<U> const&)
        { }

        static void on_default(P const&)
        { }

        static void on_init(P const&)
        { }

        static void on_dereference(P const&)
        { }

        static void on_release(P const&)
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
    struct assert_check
    {
        typedef checking_policy_tag policy_category;
        typedef assert_check type;

        assert_check()
        { }

        template <typename U>
        assert_check(assert_check<U> const&)
        { }

        template <typename U>
        assert_check(no_check<U> const&)
        { }

        static void on_default(P const&)
        { }

        static void on_init(P const&)
        { }

        static void on_dereference(P const& val)
        { assert(val); }

        static void on_release(P const&)
        { }

        static void swap(assert_check&)
        { }

        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, assert_check, (P))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, assert_check) }
#endif

////////////////////////////////////////////////////////////////////////////////
// class template assert_check_strict
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer against zero upon initialization and before dereference
// You can initialize an assert_check_strict with an assert_check
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct assert_check_strict
    {
        typedef checking_policy_tag policy_category;

        assert_check_strict()
        { }

        template <typename U>
        assert_check_strict(assert_check_strict<U> const&)
        { }

        template <typename U>
        assert_check_strict(assert_check<U> const&)
        { }

        template <typename U>
        assert_check_strict(no_check<U> const&)
        { }

        static void on_default(P const& val)
        { assert(val); }

        static void on_init(P const& val)
        { assert(val); }

        static void on_dereference(P const& val)
        { assert(val); }

        static void on_release(P const&)
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
    struct reject_null_static
    {
        typedef checking_policy_tag policy_category;

        reject_null_static()
        { }

        template <typename U>
        reject_null_static(reject_null_static<U> const&)
        { }

        template <typename U>
        reject_null_static(no_check<U> const&)
        { }

        template <typename U>
        reject_null_static(assert_check<U> const&)
        { }

        template <typename U>
        reject_null_static(assert_check_strict<U> const&)
        { }

        static void on_default(P const&)
        {
            // This Policy Does Not Allow Default Initialization
            BOOST_STATIC_ASSERT(false);
        }

        static void on_init(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_dereference(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(P const&)
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
    struct reject_null
    {
        typedef checking_policy_tag policy_category;

        reject_null()
        { }

        template <typename U>
        reject_null(reject_null<U> const&)
        { }

        static void on_default(P const&)
        { }

        static void on_init(P const&)
        { }

        static void on_dereference(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(P const&)
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
    struct reject_null_strict
    {
        typedef checking_policy_tag policy_category;

        reject_null_strict()
        { }

        template <typename U>
        reject_null_strict(reject_null_strict<U> const&)
        { }

        template <typename U>
        reject_null_strict(reject_null<U> const&)
        { }

        static void on_init(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_default(P const& val)
        {
            // This Policy Does Not Allow Default Initialization
            BOOST_STATIC_ASSERT(false);
        }

        static void on_dereference(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(P const&)
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

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator==(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return get_impl(lhs) == rhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator==(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return rhs == lhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator!=(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return !(lhs == rhs);
    }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator!=(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return rhs != lhs;
    }

////////////////////////////////////////////////////////////////////////////////
// Smart Pointer Concept Interface
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// C++-style cast operators
// These were shamelessly ripped off from boost::shared_ptr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// smart_static_cast
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline smart_ptr<T, BOOST_SMART_POINTER_POLICIES>
        smart_static_cast(smart_ptr<U, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        return smart_ptr<T, BOOST_SMART_POINTER_POLICIES>(
            p, detail::static_cast_tag()
        );
    }

////////////////////////////////////////////////////////////////////////////////
// smart_dynamic_cast
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline smart_ptr<T, BOOST_SMART_POINTER_POLICIES>
        smart_dynamic_cast(smart_ptr<U, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        return smart_ptr<T, BOOST_SMART_POINTER_POLICIES>(
            p, detail::dynamic_cast_tag()
        );
    }

////////////////////////////////////////////////////////////////////////////////
// smart_polymorphic_cast
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline smart_ptr<T, BOOST_SMART_POINTER_POLICIES>
        smart_polymorphic_cast(smart_ptr<U, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        return smart_ptr<T, BOOST_SMART_POINTER_POLICIES>(
            p, detail::polymorphic_cast_tag()
        );
    }

////////////////////////////////////////////////////////////////////////////////
// smart_polymorphic_downcast
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline smart_ptr<T, BOOST_SMART_POINTER_POLICIES>
        smart_polymorphic_downcast(smart_ptr<U, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        typedef typename smart_ptr<T, BOOST_SMART_POINTER_POLICIES>::pointer_type
            pointer_type;
        BOOST_ASSERT(dynamic_cast<pointer_type>(get_impl(p)) == get_impl(p));
        return smart_static_cast<T>(p);
    }

////////////////////////////////////////////////////////////////////////////////
// get_pointer() enables boost::mem_fn to recognize smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS>
    inline typename smart_ptr<T, BOOST_SMART_POINTER_POLICIES>::pointer_type
        get_pointer(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        return get_impl(p);
    }

    template <typename T, BOOST_SMART_POINTER_PARAMETERS>
    struct less
        : public std::binary_function<
            smart_ptr<T, BOOST_SMART_POINTER_POLICIES>,
            smart_ptr<T, BOOST_SMART_POINTER_POLICIES>,
            bool
        >
    {
        bool operator()(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs,
                        smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs) const
        {
            return less<T*>()(get_impl(lhs), get_impl(rhs));
        }
    };

    template <typename T, BOOST_SMART_POINTER_PARAMETERS>
    void swap(smart_ptr<T, BOOST_SMART_POINTER_POLICIES>& lhs,
              smart_ptr<T, BOOST_SMART_POINTER_POLICIES>& rhs)
    {
        return lhs.swap(rhs);
    }

}   // namespace boost

#ifdef BOOST_SMART_POINTER_ORDERING_OPERATORS
# include <boost/smart_ptr/ordering_operators.hpp>
#endif

#undef BOOST_SMART_POINTER_POLICIES
#undef BOOST_SMART_POINTER_PARAMETERS

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif // BOOST_MSVC

#endif // BOOST_SMART_PTR_20020920_HPP
