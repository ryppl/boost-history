//----------------------------------------------------------------------------
// smart_ptr.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_SMART_PTR_HPP
#define BOOST_SMART_PTR_HPP
//----------------------------------------------------------------------------
// Headers
#include <stdexcept>
#include <boost/config.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/eval_if.hpp>
//----------------------------------------------------------------------------
// Policies
namespace boost
{
    //------------------------------------------------------------------------
    // Policy Tags
    struct copy_semantics_tag { };
    struct move_semantics_tag { };
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
# define BOOST_SP_DECLARE_TEMPLATE_FRIEND(Param, type)                       \
    template <typename Param> friend class type;
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
# define BOOST_SP_CONVERT_ARGUMENT(policy, obj, field)                       \
    (reinterpret_cast<policy const&>(obj).field)
# define BOOST_SP_DECLARE_TEMPLATE_FRIEND(Param, type)
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
        //--------------------------------------------------------------------
        // Cast Tags
#ifndef BOOST_SHARED_PTR_HPP_INCLUDED
        struct const_cast_tag { };
        struct static_cast_tag { };
        struct dynamic_cast_tag { };
#endif
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
    }   // namespace detail
    //------------------------------------------------------------------------
    // Type generator smart_ptr
    template <
        class P1 = detail::empty_policy,
        class P2 = detail::empty_policy,
        class P3 = detail::empty_policy,
        class P4 = detail::empty_policy
    >
    struct smart_ptr
    {
        template <typename T>
        struct to;
    };
    //------------------------------------------------------------------------
    namespace detail
    {
        //--------------------------------------------------------------------
        // Move semantics logic
        template <class Category, typename T>
        struct cant_move_from_const_base
        { };

        template <typename T>
        struct cant_move_from_const_base<move_semantics_tag, T>
        {
            typedef typename T::error type;
        };

        template <class SmartPtr>
        struct cant_move_from_const_base_f
        {
            typedef cant_move_from_const_base<
                typename SmartPtr::ownership_policy::ownership_category,
                typename SmartPtr::element_type
            > type;
        };
            
        template <class SmartPtr>
        struct cant_move_from_const;

        template <class P1, class P2, class P3, class P4, typename T>
        struct cant_move_from_const<typename smart_ptr<P1, P2, P3, P4>::template to<T> const>
            : mpl::apply<
                cant_move_from_const_base_f<
                    typename smart_ptr<P1, P2, P3, P4>::template to<T>
                >
            >
        { };
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
    // Nested smart_ptr
    template <class P1, class P2, class P3, class P4>
    template <typename T>
    class smart_ptr<P1, P2, P3, P4>::to
        : public detail::conversion_policy_<                                     \
            T, mpl::list<P1, P2, P3, P4>, BOOST_SP_DEFAULT_CONVERSION_POLICY     \
        >::type
    {
    public:             // Public Types
        typedef T                               element_type;
        typedef T                               value_type;
        typedef typename detail::conversion_policy_<                                     \
            T, mpl::list<P1, P2, P3, P4>, BOOST_SP_DEFAULT_CONVERSION_POLICY     \
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
            get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(smart_ptr const&)");
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, typename SmartPtr::pointer_type = 0)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs));
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&)"
            );
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, detail::const_cast_tag)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                const_cast<stored_type>(get_impl_ref(rhs))
            );
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this,
                "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&, const_cast_tag)"
            );
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, detail::static_cast_tag)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                static_cast<stored_type>(get_impl_ref(rhs))
            );
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this,
                "smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const&, "
                    "static_cast_tag)"
            );
        }

        template <class SmartPtr>
        to(SmartPtr const& rhs, detail::dynamic_cast_tag)
            : base_type(static_cast<typename SmartPtr::base_type const&>(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                dynamic_cast<stored_type>(get_impl_ref(rhs))
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

        to(stored_param p)
            : base_type(p)
        {
            checking_policy::on_init(get_impl(*this));
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "smart_ptr(stored_param)");
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

        template <class U>
        to(U&, typename detail::cant_move_from_const<U>::type = 0);

    public:             // Ownership modifiers
        to&             operator=(this_type rhs)
        {
            swap(rhs);
            return *this;
        }

        void            swap(this_type& rhs)
        {
            base_type::swap(*this, rhs);
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
            return get_impl(*this) < get_impl(rhs);
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

    public:             // Conversion to pointer_type
                        operator automatic_conversion_result(void) const
        {
            return get_impl(*this);
        }

    public:             // Concept Interface
        template <typename U>
        friend inline
        to<U>           const_pointer_cast(to const& p)
        {
            return to<U>(p, detail::const_cast_tag());
        }

        template <typename U>
        friend inline
        to<U>           static_pointer_cast(to const& p)
        {
            return to<U>(p, detail::static_cast_tag());
        }

        template <typename U>
        friend inline
        to<U>           dynamic_pointer_cast(to const& p)
        {
            return to<U>(p, detail::dynamic_cast_tag());
        }

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
#endif // BOOST_SMART_PTR_HPP
