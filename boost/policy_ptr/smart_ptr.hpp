//----------------------------------------------------------------------------
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See
// http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_SMART_PTR_HPP
#define BOOST_SMART_PTR_HPP
//----------------------------------------------------------------------------
// Headers
#include <boost/config.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/is_placeholder.hpp>
//----------------------------------------------------------------------------
// Configuration Macros
//   These macros must appear before the Policy Adaptor
#ifndef BOOST_SP_DEFAULT_STORAGE_POLICY
# define BOOST_SP_DEFAULT_STORAGE_POLICY                scalar_storage
#endif // BOOST_SP_DEFAULT_STORAGE_POLICY
#ifndef BOOST_SP_DEFAULT_OWNERSHIP_POLICY
# define BOOST_SP_DEFAULT_OWNERSHIP_POLICY              ref_counted
#endif // BOOST_SP_DEFAULT_OWNERSHIP_POLICY
#ifndef BOOST_SP_DEFAULT_CHECKING_POLICY
# define BOOST_SP_DEFAULT_CHECKING_POLICY               assert_check
#endif // BOOST_SP_DEFAULT_CHECKING_POLICY
#ifndef BOOST_SP_DEFAULT_CONVERSION_POLICY
# define BOOST_SP_DEFAULT_CONVERSION_POLICY             disallow_conversion
#endif // BOOST_SP_DEFAULT_CONVERSION_POLICY
//----------------------------------------------------------------------------
// Utility Macros
//   These macros are used by Ownership Policies, and thus must appear before
//   the policy headers
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
}   // namespace boost
//----------------------------------------------------------------------------
#include "policy/scalar_storage.hpp"
#include "policy/ref_counted.hpp"
#include "policy/assert_check.hpp"
#include "policy/conversion.hpp"
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
        struct static_cast_tag { };
        struct dynamic_cast_tag { };
        struct polymorphic_cast_tag { };
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
        struct get_category_impl
        {
            typedef typename Policy::policy_category type;
        };

        template <class Policy>
        struct get_category
            : public mpl::if_<
                mpl::is_placeholder<Policy>,
                mpl::identity<Policy>,
                get_category_impl<Policy>
            >::type
        { };

        template <class Sequence, class Category, class Default>
        struct get_policy
        {
            typedef typename mpl::find_if<
                Sequence, is_same<get_category<_>, Category>
            >::type iter_;
            typedef typename mpl::end<Sequence>::type last_;
            typedef typename mpl::apply_if<
                is_same<iter_, last_>,
                mpl::identity<Default>,
                iter_
            >::type type;
        };

	    template <typename T, class Policies, class Default>
	    struct storage_policy_
            : public mpl::apply1<
                typename get_policy<
                    Policies, storage_policy_tag, Default
                >::type,
                T
            >::type
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
            >::type
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
            >::type
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
            >::type type;
        { };
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
    // Debug hooks
#if defined(BOOST_SMART_PTR_DEBUG_MODE)
    void sp_constructor_hook(void* p, std::size_t size);
    void sp_destructor_hook(void* p, std::size_t size);
#endif
    //------------------------------------------------------------------------
    template <
        typename T,
        class P1 = detail::empty_policy,
        class P2 = detail::empty_policy,
        class P3 = detail::empty_policy,
        class P4 = detail::empty_policy
    >
    class smart_ptr
        : public detail::conversion_policy_<                                     \
            T, mpl::list<P1, P2, P3, P4>, BOOST_SP_DEFAULT_CONVERSION_POLICY     \
        >::type
    {
    public:             // Public Types
        typedef T                               element_type;
        typedef typename detail::conversion_policy_<                                     \
            T, mpl::list<P1, P2, P3, P4>, BOOST_SP_DEFAULT_CONVERSION_POLICY     \
        >::type                                 conversion_policy;
        typedef typename conversion_policy::checking_policy
                                                checking_policy;
        typedef typename checking_policy::ownership_policy
                                                ownership_policy;
        typedef typename ownership_policy::storage_policy
                                                storage_policy;
        typedef smart_ptr                       this_type;
        typedef conversion_policy               base_type;

        typedef typename storage_policy::pointer_type
                                                pointer_type;
        typedef typename storage_policy::const_pointer_type
                                                const_pointer_type;
        typedef typename storage_policy::stored_type::type
                                                stored_type;
        typedef typename storage_policy::const_stored_type<>::type
                                                const_stored_type;
        typedef typename storage_policy::reference_type
                                                reference_type;
        typedef typename storage_policy::const_reference_type
                                                const_reference_type;

        template <typename U>
        struct rebind
        {
            typedef smart_ptr<U, P1, P2, P3, P4> other;
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
        smart_ptr(void)
        {
            checking_policy::on_default(get_impl(*this));
        }

        smart_ptr(this_type const& rhs)
            : base_type(static_cast<base_type const&>(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs));
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const& rhs)
            : base_type(static_cast<
                typename smart_ptr<U, Q1, Q2, Q3, Q4>::base_type const&
            >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs));
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4>& rhs)
            : base_type(static_cast<
                typename smart_ptr<U, Q1, Q2, Q3, Q4>::base_type const&
            >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(get_impl_ref(rhs));
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const& rhs,
            detail::static_cast_tag const&)
            : base_type(static_cast<
                typename smart_ptr<U, Q1, Q2, Q3, Q4>::base_type const&
            >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                static_cast<pointer_type>(get_impl_ref(rhs))
            );
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        smart_ptr(smart_ptr<U, Q1, Q2, Q3, Q4> const& rhs,
            detail::dynamic_cast_tag const&)
            : base_type(static_cast<
                typename smart_ptr<U, Q1, Q2, Q3, Q4>::base_type const&
            >(rhs))
        {
            get_impl_ref(*this) = ownership_policy::clone(
                dynamic_cast<pointer_type>(get_impl_ref(rhs))
            );
            if (!*this)
            {
                // dynamic_cast<> failed; reset count
                this_type().swap(*this);
            }
        }

        template <typename U>
        smart_ptr(U& p)
            : base_type(p)
        {
            checking_policy::on_init(get_impl(*this));
        }

        template <typename U>
        smart_ptr(U const& p)
            : base_type(p)
        {
            checking_policy::on_init(get_impl(*this));
        }

        template <typename U, typename V>
        smart_ptr(U& p, V& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
        }

        template <typename U, typename V>
        smart_ptr(U const& p, V& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
        }

        template <typename U, typename V>
        smart_ptr(U& p, V const& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
        }

        template <typename U, typename V>
        smart_ptr(U const& p, V const& v)
            : base_type(p, v)
        {
            checking_policy::on_init(get_impl(*this));
        }

        template <class Category, typename U>
        struct cant_move_from_const_base
        { };

        template <typename U>
        struct cant_move_from_const_base<move_semantics_tag, U>
        {
            typedef typename smart_ptr<U>::error type;
        };

        template <typename U>
        struct cant_move_from_const;

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        struct cant_move_from_const<smart_ptr<U, Q1, Q2, Q3, Q4> const>
            : cant_move_from_const_base<
                typename detail::ownership_category<ownership_policy>::type, U
            >
        { };

        template <class U>
        smart_ptr(U&, typename cant_move_from_const<U>::type = 0);

    public:             // Ownership modifiers
        smart_ptr&      operator=(this_type rhs)
        {
            swap(rhs);
            return *this;
        }

        void            swap(this_type& rhs)
        {
            base_type::swap(rhs);
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
        void            reset(this_type& sp, stored_type p)
        {
            checking_policy::on_reset(sp, p);
            this_type(p).swap(sp);
        }

    public:             // Dereference
        pointer_type    operator->() const
        {
            checking_policy::on_dereference(get_impl(*this));
            return storage_policy::get_pointer();
        }

        reference_type  operator*() const
        {
            checking_policy::on_dereference(get_impl(*this));
            return storage_policy::get_reference();
        }

    public:             // Comparison
        friend inline
        bool            operator==(smart_ptr const& lhs, T const* rhs)
        {
            return get_impl(lhs) == rhs;
        }

        friend inline
        bool            operator==(T const* lhs, smart_ptr const& rhs)
        {
            return rhs == lhs;
        }

        friend inline
        bool            operator!=(smart_ptr const& lhs, T const* rhs)
        {
            return !(lhs == rhs);
        }

        friend inline
        bool            operator!=(T const* lhs, this_type const& rhs)
        {
            return rhs != lhs;
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        bool            operator==(smart_ptr<U, Q1, Q2, Q3, Q4> const& rhs) const
        {
            return get_impl(*this) == get_impl(rhs);
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        bool            operator!=(smart_ptr<U, Q1, Q2, Q3, Q4> const& rhs) const
        {
            return !(*this == rhs);
        }

        template <typename U, class Q1, class Q2, class Q3, class Q4>
        bool            operator<(smart_ptr<U, Q1, Q2, Q3, Q4> const& rhs) const
        {
            return get_impl(*this) < get_impl(rhs);
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
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#undef BOOST_SP_DECLARE_TEMPLATE_FRIEND
#undef BOOST_SP_CONVERT_ARGUMENT
//----------------------------------------------------------------------------
#endif // BOOST_SMART_PTR_HPP

