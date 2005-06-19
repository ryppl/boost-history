//----------------------------------------------------------------------------
// smart_ptr.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
// Change log(latest at top):
// - 02 June 2005: Larry Evans 
//   WHAT:
//     1) added:
//          template<class U> smart_ptr<...>::to<T>::to(to<U>&).
//   WHY:
//     1) to enable operator=(this_type rhs) to work where rhs was created
//        from to<U>, where U is derived from T, as in
//        libs/policy_ptr/smart_ptr_test.cpp:assign_test.
//
//        NOTE:
//          For some unknown reasons, the previous changes caused the
//          assign_test to fail.
//
// - 01 June 2005: Larry Evans 
//   WHAT:
//     1) added:
//          template<class U> smart_ptr<...>::to<T>::to(U&).
//     2) used newly added non-member function, get_less_comparator,
//        in less than comparison operations.
//   WHY:
//     1) to allow derived class pointer to be used as arg, and thus allow
//        tests in:
//          n_constructors::copy_constructor(void)
//        in:
//          policy_ptr/test/std_ptrs_shared_ptr_test.cpp
//        to pass.
//     2) Allow operator< on std_ptrs to work correctly.  This function
//        serves a similar purpose to member method, _internal_less, in 
//        shared_ptr.hpp.
// - 31 May 2005: Larry Evans 
//   WHAT:
//     added smart_ptr<...>::to<T>::operator bool
//   WHY:
//     to avoid ambiguity compiler diagnostic when std_ptrs uses 'allow_conversions'
//     and test/std_ptrs_shared_ptr_test.cpp is compiled.
// - 14 May 2005: Larry Evans 
//   removed extraneous \\n 's around smart_ptr superclass specification.
// - 29 Oct 2004: Jonathan Turkanis added VC7.1 workarounds and move_ptr 
//   emulation
//----------------------------------------------------------------------------
#ifndef BOOST_SMART_PTR_HPP
#define BOOST_SMART_PTR_HPP
//----------------------------------------------------------------------------
// Headers
#include <functional>
#include <stdexcept>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#if defined(BOOST_MSVC) && \
    BOOST_WORKAROUND(_MSC_FULL_VER, BOOST_TESTED_AT(140040607)) \
    /**/
# pragma warning(push)
# pragma warning(disable:4521) // Multiple copy constructors
#endif

//----------------------------------------------------------------------------
// Policies
namespace boost
{
    //------------------------------------------------------------------------
    // Policy Tags
    struct copy_rvalue_tag { }; // Included for clarity.
    struct copy_mutable_lvalue_tag { };
    struct copy_const_lvalue_tag { };
    struct copy_lvalue_tag 
        : copy_mutable_lvalue_tag, 
          copy_const_lvalue_tag
        { };
    struct copy_semantics_tag 
        : copy_rvalue_tag, 
          copy_lvalue_tag 
        { };
    struct move_semantics_tag : copy_rvalue_tag { };
    struct auto_semantics_tag 
        : copy_rvalue_tag, 
          copy_mutable_lvalue_tag 
        { };
    struct no_copy_semantics_tag { };

    struct storage_policy_tag { };
    struct ownership_policy_tag { };
    struct conversion_policy_tag { };
    struct checking_policy_tag { };
    //------------------------------------------------------------------------
    // Exceptions
    struct null_pointer_error
        : public std::runtime_error
    {
                        null_pointer_error(void)
                            : std::runtime_error("Null Pointer Exception")
                                                { }
    };
    //------------------------------------------------------------------------
    struct bad_release
        : public std::runtime_error
    {
                        bad_release(void)
                            : std::runtime_error(
                                "Bad Release: Too many references"
                            )                   { }
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
// Utility Macros
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
# define BOOST_SP_CONVERT_ARGUMENT(Policy, obj, field)                       \
    ((obj).field)
# define BOOST_SP_DECLARE_TEMPLATE_FRIEND(Param, type, arity)                \
    template <BOOST_PP_ENUM_PARAMS(arity, class Param)>                      \
    friend class type;                   
# define BOOST_SP_PRIVATE private
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
# define BOOST_SP_CONVERT_ARGUMENT(policy, obj, field)                       \
    (reinterpret_cast<policy const&>(obj).field)
# define BOOST_SP_DECLARE_TEMPLATE_FRIEND(Param, type, arity)
# define BOOST_SP_PRIVATE public
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
//----------------------------------------------------------------------------
// Configuration Macros
#ifdef BOOST_SP_INVARIANT_CHECKING
# define BOOST_SP_INVARIANT(x)                          BOOST_ASSERT(x)
#else  // BOOST_SP_INVARIANT_CHECKING
# define BOOST_SP_INVARIANT(x)
#endif // BOOST_SP_INVARIANT_CHECKING

// Debug hooks
#ifdef BOOST_SP_DEBUG_MODE
# define BOOST_SP_CONSTRUCTOR_HOOK(p, s)                sp_constructor_hook(p, s)
# define BOOST_SP_DESTRUCTOR_HOOK(p, s)                 sp_destructor_hook(p, s)
#else
# define BOOST_SP_CONSTRUCTOR_HOOK(p, s)
# define BOOST_SP_DESTRUCTOR_HOOK(p, s)
#endif // BOOST_SP_DEBUG_MODE

#ifndef BOOST_SP_DEFAULT_STORAGE_POLICY
# define BOOST_SP_DEFAULT_STORAGE_POLICY                scalar_storage
# include "policy/scalar_storage.hpp"
#endif // BOOST_SP_DEFAULT_STORAGE_POLICY
#ifndef BOOST_SP_DEFAULT_OWNERSHIP_POLICY
# define BOOST_SP_DEFAULT_OWNERSHIP_POLICY              ref_counted
# include "policy/ref_counted.hpp"
#endif // BOOST_SP_DEFAULT_OWNERSHIP_POLICY
#ifndef BOOST_SP_DEFAULT_CHECKING_POLICY
# define BOOST_SP_DEFAULT_CHECKING_POLICY               assert_check
# include "policy/assert_check.hpp"
#endif // BOOST_SP_DEFAULT_CHECKING_POLICY
#ifndef BOOST_SP_DEFAULT_CONVERSION_POLICY
# define BOOST_SP_DEFAULT_CONVERSION_POLICY             disallow_conversion
# include "policy/conversion.hpp"
#endif // BOOST_SP_DEFAULT_CONVERSION_POLICY
//----------------------------------------------------------------------------
// smart_ptr
namespace boost
{
    //------------------------------------------------------------------------
    namespace detail
    {
    namespace policy_ptr 
    {

        //--------------------------------------------------------------------
        // Cast Tags
        struct const_cast_tag { };
        struct static_cast_tag { };
        struct dynamic_cast_tag { };

        //--------------------------------------------------------------------
        // Explicit move helper
        template<typename Ptr>
        class move_source {
        public:
            explicit move_source(Ptr& ptr) : ptr_(ptr) {}
            Ptr& ptr() const { return ptr_; }
        private:
            Ptr& ptr_;
            move_source(const Ptr&);
        };

        //--------------------------------------------------------------------
        struct empty_policy
        {
            typedef mpl::void_ policy_category;
        };

        //--------------------------------------------------------------------
        // Policy Adaptor
        using mpl::_;

        template <class Policy>
        struct get_category
        {
            typedef typename Policy::policy_category type;
        };

        template <class Sequence, class Category, class Default>
        struct get_policy
        {
            typedef typename mpl::find_if<
                Sequence, is_same<get_category<_>, Category>
            >::type iter_;
            typedef typename mpl::end<Sequence>::type last_;
            typedef typename mpl::eval_if<
                is_same<iter_, last_>,
                mpl::identity<Default>,
                mpl::deref<iter_>
            >::type type;
        };

	    template <typename T, class Policies, class Default>
	    struct storage_policy_
            : public mpl::apply1<
                typename get_policy<
                    Policies, storage_policy_tag, Default
                >::type,
                T
            >
	    { };

        template <typename T, class Policies, class Default>
        struct ownership_policy_
            : public mpl::apply1<
                typename get_policy<
                    Policies, ownership_policy_tag, Default
                >::type,
                typename storage_policy_<
                    T, Policies, BOOST_SP_DEFAULT_STORAGE_POLICY
                >::type
            >
        { };

        template <typename T, class Policies, class Default>
        struct checking_policy_
            : public mpl::apply1<
                typename get_policy<
                    Policies, checking_policy_tag, Default
                >::type,
                typename ownership_policy_<
                    T, Policies, BOOST_SP_DEFAULT_OWNERSHIP_POLICY
                >::type
            >
        { };

        template <typename T, class Policies, class Default>
        struct conversion_policy_
            : public mpl::apply1<
                typename get_policy<
                    Policies, conversion_policy_tag, Default
                >::type,
                typename checking_policy_<
                    T, Policies, BOOST_SP_DEFAULT_CHECKING_POLICY
                >::type
            >
        { };
        //--------------------------------------------------------------------
    }   // namespace policy_ptr
    }   // namespace detail

    //------------------------------------------------------------------------
    // Type generator smart_ptr
    template <
        class P1 = detail::policy_ptr::empty_policy,
        class P2 = detail::policy_ptr::empty_policy,
        class P3 = detail::policy_ptr::empty_policy,
        class P4 = detail::policy_ptr::empty_policy
    >
    struct smart_ptr
    {
        template <typename T>
        class to;
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(to, T, 1)
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(smart_ptr, Q, 4)
    BOOST_SP_PRIVATE:
        template <typename SmartPtr>
        static typename SmartPtr::stored_type 
        clone(const SmartPtr& ptr, const typename SmartPtr::stored_type& p)
        {
            return ptr.do_clone(p);
        }

        template <typename SmartPtr>
        static typename SmartPtr::stored_type 
        clone(const SmartPtr& ptr, typename SmartPtr::stored_type& p)
        {
            return ptr.do_clone(p);
        }
    };
    //------------------------------------------------------------------------
    namespace detail
    {
    namespace policy_ptr 
    {
        //BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_enclosing_class, enclosing_class, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_first_policy, first_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_second_policy, second_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_third_policy, third_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_fourth_policy, fourth_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_element_type, element_type, true)

        template<class SmartPtr>
        struct is_smart_ptr_impl 
            : is_same<
                  SmartPtr,
                  typename 
                  smart_ptr<
                      typename SmartPtr::first_policy,
                      typename SmartPtr::second_policy,
                      typename SmartPtr::third_policy,
                      typename SmartPtr::fourth_policy
                  >::template to<typename SmartPtr::element_type>
              >
            { };

        template<class SmartPtr>
        struct is_smart_ptr
            : mpl::and_< // Default arity of and_ is 5.
                  mpl::and_<
                      has_first_policy<SmartPtr>,
                      has_second_policy<SmartPtr>,
                      has_third_policy<SmartPtr>,
                      has_fourth_policy<SmartPtr>,
                      has_element_type<SmartPtr>
                  >,
                  is_smart_ptr_impl<typename remove_cv<SmartPtr>::type>
              >
            { };

        template<class P1, class P2, class P3, class P4, class SmartPtr>
        struct is_const_smart_ptr_impl 
            : is_same<
                  SmartPtr,
                  const typename smart_ptr<P1, P2, P3, P4>::template 
                      to<typename SmartPtr::element_type>
              >
            { };

        template<class P1, class P2, class P3, class P4, class SmartPtr>
        struct is_const_smart_ptr
            : mpl::and_< 
                  has_element_type<SmartPtr>,
                  is_const_smart_ptr_impl<P1, P2, P3, P4, SmartPtr>
              >
            { };

        //--------------------------------------------------------------------
        // Move semantics logic
        template <class SmartPtr, class Tag>
        struct is_ownership_category_convertible
            : is_convertible<
                  typename SmartPtr::ownership_policy::ownership_category,
                  Tag
              >
        { };

        template <class SmartPtr>
        struct cant_move_from_const_lvalue
        {   // Indicates improper use of move semantics. 
            typedef typename SmartPtr::error type;
        };

        template <class P1, class P2, class P3, class P4, class SmartPtr>
        struct enforce_move_semantics
            : mpl::if_<
                  mpl::and_<
                      is_const_smart_ptr<P1, P2, P3, P4, SmartPtr>,
                      mpl::not_<
                          is_ownership_category_convertible<
                              SmartPtr, copy_const_lvalue_tag
                          >
                      >
                  >,
                  cant_move_from_const_lvalue<SmartPtr>,
                  mpl::empty_base
              >::type
            { };

        //--------------------------------------------------------------------
    }   // namespace detail
    }   // namespace policy_ptr
    //------------------------------------------------------------------------
    // Nested smart_ptr
    template <class P1, class P2, class P3, class P4>
    template <typename T>
    class smart_ptr<P1, P2, P3, P4>::to
        : public detail::policy_ptr::conversion_policy_<
            T, mpl::list<P1, P2, P3, P4>, BOOST_SP_DEFAULT_CONVERSION_POLICY
        >::type
    {
    public:             // Public Types
        typedef T                               element_type;
        typedef T                               value_type;
        typedef typename detail::policy_ptr::conversion_policy_<
            T, mpl::list<P1, P2, P3, P4>, BOOST_SP_DEFAULT_CONVERSION_POLICY
        >::type                                 conversion_policy;
        typedef typename conversion_policy::checking_policy
                                                checking_policy;
        typedef typename checking_policy::ownership_policy
                                                ownership_policy;
        typedef typename ownership_policy::storage_policy
                                                storage_policy;
        typedef to                              this_type;
        typedef conversion_policy               base_type;

        typedef typename storage_policy::pointer_type
                                                pointer_type;
        typedef typename storage_policy::const_pointer_type
                                                const_pointer_type;
        typedef typename storage_policy::stored_type
                                                stored_type;
        typedef typename storage_policy::const_stored_type
                                                const_stored_type;
        typedef typename storage_policy::reference_type
                                                reference_type;
        typedef typename storage_policy::const_reference_type
                                                const_reference_type;
        typedef typename storage_policy::stored_param
                                                stored_param;
        typedef typename storage_policy::pointer_param
                                                pointer_param;

        typedef P1 first_policy;
        typedef P2 second_policy;
        typedef P3 third_policy;
        typedef P4 fourth_policy;
        typedef smart_ptr<P1, P2, P3, P4> enclosing_class;

        template <typename U>
        struct rebind
        {
            typedef to<U> other;
        };

    private:            // Helper Types
        class unspecified_bool_type
        {
        public:
            void avoid_gcc_warning_about_all_private_members(void);
        private:
            void operator delete(void*);
        };

        typedef typename conversion_policy::result_type
                                                automatic_conversion_result;

    public:             // Structors
        to(void)
        {
            checking_policy::on_default(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(void)");
        }

        to(this_type const& rhs)
            : base_type(static_cast<base_type const&>(rhs))
        {
            get_impl_ref(*this) = 
                enclosing_class::clone(rhs, get_impl_ref(rhs));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(smart_ptr const&)");
        }

        template <class U>
        to(to<U> const& rhs)
            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "to(to<U> const&)");
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, typename SmartPtr::pointer_type = 0)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = 
                enclosing_class::clone(rhs, get_impl_ref(rhs));
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&)"
            );
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, detail::policy_ptr::const_cast_tag)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = 
                const_cast<stored_type>(
                    enclosing_class::clone(rhs, get_impl_ref(rhs))
                );
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this,
                "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&, const_cast_tag)"
            );
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, detail::policy_ptr::static_cast_tag)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = 
                static_cast<stored_type>(
                    enclosing_class::clone(rhs, get_impl_ref(rhs))
                );
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this,
                "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&, "
                    "static_cast_tag)"
            );
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, detail::policy_ptr::dynamic_cast_tag)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = 
                dynamic_cast<stored_type>(
                    enclosing_class::clone(rhs, get_impl_ref(rhs))
                );
            if (!*this)
            {
                // dynamic_cast<> failed; reset count
                this_type().swap(*this);
            }
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this,
                "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&, "
                    "dynamic_cast_tag)"
            );
        }

        template <class SmartPtr>
        to(detail::policy_ptr::move_source<SmartPtr> const& rhs)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs.ptr()))
        {
            BOOST_STATIC_ASSERT((
                !is_convertible<
                    typename ownership_policy::ownership_category,
                    copy_lvalue_tag
                >::value
            ));
            get_impl_ref(*this) = 
                enclosing_class::clone(rhs.ptr(), get_impl_ref(rhs.ptr()));
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&)"
            );
        }

        to(stored_param p)
            : base_type(p)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(stored_param)");
        }

        template <typename U>
        to(U& p)
            : base_type(p)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(U&)");
        }

        template <typename U, typename V>
        to(U& p, V& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(U&, V&)");
        }

        template <typename U, typename V>
        to(U const& p, V& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(U const&, V&)");
        }

        template <typename U, typename V>
        to(U& p, V const& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(U&, V const&)");
        }

        template <typename U, typename V>
        to(U const& p, V const& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(U const&, V const&)");
        }

        ~to(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~smart_ptr(void)");
        }
    BOOST_SP_PRIVATE: // Cloning machinery
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(Q, smart_ptr, 4)
        stored_type do_clone(const stored_type& p) const
        {
            return ownership_policy::clone(p);
        }

        stored_type do_clone(stored_type& p) const
        {
            return ownership_policy::clone(p);
        }
    public: // Move semantics machinery
        to(this_type& rhs)
            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_STATIC_ASSERT((
                is_convertible<
                    typename ownership_policy::ownership_category,
                    copy_mutable_lvalue_tag
                >::value
            ));
            get_impl_ref(*this) = 
                enclosing_class::clone(rhs, get_impl_ref(rhs));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(smart_ptr const&)");
        }

        template <class U>        
        to(to<U>& rhs)
            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_STATIC_ASSERT((
                is_convertible<
                    typename ownership_policy::ownership_category,
                    copy_mutable_lvalue_tag
                >::value
            ));
            get_impl_ref(*this) = 
                enclosing_class::clone(rhs, get_impl_ref(rhs));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "to(to<U>&)");
        }

    private: // More move semantics machinery
        template <class SmartPtr>
        to( SmartPtr&,
            typename 
            enable_if< 
                mpl::and_<
                    detail::policy_ptr::is_smart_ptr<SmartPtr>,
                    mpl::not_<
                        is_convertible<
                            typename ownership_policy::ownership_category,
                            copy_mutable_lvalue_tag
                        >
                    >
                >
            >::type* = 0 );

        template <class U>
        to( U&, 
            typename detail::policy_ptr::enforce_move_semantics<
                P1, P2, P3, P4, U
            >::type = 0 );
    public:             // Ownership modifiers
        void            swap(this_type& rhs)
        {
            base_type::swap(*this, rhs);
        }

        to&             operator=(this_type rhs)
        {
            swap(rhs);
            return *this;
        }

        friend inline
        void            release(this_type& sp, stored_type& p)
        {
            checking_policy::on_release(get_impl(sp));
            ownership_policy::on_release(sp);
            p = get_impl_ref(sp);
            get_impl_ref(sp) = storage_policy::default_value();
            ownership_policy::reset(sp);
        }

        friend inline
        void            reset(this_type& sp, stored_param p)
        {
            checking_policy::on_reset(get_impl(sp), p);
            this_type(p).swap(sp);
        }

    public:             // Dereference
        pointer_type    operator->() const
        {
            checking_policy::on_dereference(get_impl(*this));
            return get_impl(*this);
        }

        reference_type  operator*() const
        {
            checking_policy::on_dereference(get_impl(*this));
            return *get_impl(*this);
        }

    public:             // Comparison
        friend inline
        bool            operator==(to const& lhs, T const* rhs)
        {
            return get_impl(lhs) == rhs;
        }

        friend inline
        bool            operator==(T const* lhs, to const& rhs)
        {
            return rhs == lhs;
        }

        friend inline
        bool            operator!=(to const& lhs, T const* rhs)
        {
            return !(lhs == rhs);
        }

        friend inline
        bool            operator!=(T const* lhs, this_type const& rhs)
        {
            return rhs != lhs;
        }

        template <typename U>
        bool            operator==(U* rhs) const
        {
            return get_impl(*this) == rhs;
        }

        template <typename U>
        bool            operator==(U const* rhs) const
        {
            return get_impl(*this) == rhs;
        }

        template <class SmartPtr>
        bool            operator==(SmartPtr const& rhs) const
        {
            return get_impl(*this) == get_impl(rhs);
        }

        template <typename U>
        bool            operator!=(U* rhs) const
        {
            return !(get_impl(*this) == rhs);
        }

        template <typename U>
        bool            operator!=(U const* rhs) const
        {
            return !(get_impl(*this) == rhs);
        }

        template <class SmartPtr>
        bool            operator!=(SmartPtr const& rhs) const
        {
            return !(*this == rhs);
        }

        template <typename U>
        bool            operator<(U* rhs) const
        {
            return get_impl(*this) < rhs;
        }

        template <typename U>
        bool            operator<(U const* rhs) const
        {
            return get_impl(*this) < rhs;
        }

        template <class SmartPtr>
        bool            operator<(SmartPtr const& rhs) const
        {
            return get_less_comparator(*this) < get_less_comparator(rhs);
        }

        template <typename U>
        friend inline
        bool            operator==(to const& lhs, U const* rhs)
        {
            return get_impl(lhs) == rhs;
        }

        template <typename U>
        friend inline
        bool            operator==(U const* lhs, to const& rhs)
        {
            return rhs == lhs;
        }

        template <typename U>
        friend inline
        bool            operator!=(to const& lhs, U const* rhs)
        {
            return !(lhs == rhs);
        }

        template <typename U>
        friend inline
        bool            operator!=(U const* lhs, to const& rhs)
        {
            return rhs != lhs;
        }

    public:             // Conversion to bool
                        operator unspecified_bool_type*(void) const
        {
            if (!*this) return 0;
            static unspecified_bool_type b;
            return &b;
        }

        bool            operator!(void) const
        {
            return !storage_policy::is_valid();
        }

                        operator bool(void) const
        {
            return storage_policy::is_valid();
        }

    public:             // Conversion to pointer_type
                        operator automatic_conversion_result(void) const
        {
            return get_impl(*this);
        }

    public:             // Concept Interface
        friend inline
        pointer_type    get_pointer(to const& p)
        {
            return get_impl(p);
        }

        friend inline
        void                swap(to& lhs, to& rhs)
        {
            lhs.swap(rhs);
        }
    };

    template <typename U, typename Ptr>
    inline 
    typename enable_if<
        detail::policy_ptr::is_smart_ptr<Ptr>,
        typename Ptr::enclosing_class::template to<U>
    >::type
    const_pointer_cast(Ptr const& p)
    {
        typedef typename Ptr::enclosing_class::template to<U> result_type;
        return result_type(p, detail::policy_ptr::const_cast_tag());
    }

    template <typename U, typename Ptr>
    typename enable_if<
        detail::policy_ptr::is_smart_ptr<Ptr>,
        typename Ptr::enclosing_class::template to<U>
    >::type
    static_pointer_cast(Ptr const& p)
    {
        typedef typename Ptr::enclosing_class::template to<U> result_type;
        return result_type(p, detail::policy_ptr::static_cast_tag());
    }

    template <typename U, typename Ptr>
    typename enable_if<
        detail::policy_ptr::is_smart_ptr<Ptr>,
        typename Ptr::enclosing_class::template to<U>
    >::type
    dynamic_pointer_cast(Ptr const& p)
    {
        typedef typename Ptr::enclosing_class::template to<U> result_type;
        return result_type(p, detail::policy_ptr::dynamic_cast_tag());
    }

    template <typename Ptr>
    typename enable_if<
        detail::policy_ptr::is_smart_ptr<Ptr>,
        detail::policy_ptr::move_source<Ptr>
    >::type
    move(Ptr& p)
    {
        return detail::policy_ptr::move_source<Ptr>(p);
    }

    //------------------------------------------------------------------------
    template <typename T>
    struct less;

    template <class P1, class P2, class P3, class P4>
    struct less<smart_ptr<P1, P2, P3, P4> >
        : public std::binary_function<
            smart_ptr<P1, P2, P3, P4>, smart_ptr<P1, P2, P3, P4>, bool
        >
    {
        typedef smart_ptr<P1, P2, P3, P4> arg_type;

        template <typename T>
        bool            operator()(
                            typename arg_type::template to<T> const& lhs,
                            typename arg_type::template to<T> const& rhs
                        ) const
        {
            return std::less<
                typename arg_type::template to<T>::pointer_type
            >()(get_impl(lhs), get_impl(rhs));
        }
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------

#if defined(BOOST_MSVC) && \
    BOOST_WORKAROUND(_MSC_FULL_VER, BOOST_TESTED_AT(140040607)) \
    /**/
# pragma warning(pop)
#endif

#endif // BOOST_SMART_PTR_HPP
