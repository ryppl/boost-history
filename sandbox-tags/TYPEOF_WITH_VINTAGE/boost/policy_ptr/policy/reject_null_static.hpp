//----------------------------------------------------------------------------
// reject_null_static.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_REJECT_NULL_STATIC_HPP
#define BOOST_REJECT_NULL_STATIC_HPP
//----------------------------------------------------------------------------
// Headers
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include "no_check.hpp"
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class OwnershipPolicy>
    class reject_null_static_ : public OwnershipPolicy
    {
    public:             // Types
        typedef OwnershipPolicy                         ownership_policy;
        typedef ownership_policy                        base_type;
        typedef typename ownership_policy::pointer_type pointer_type;
        typedef typename ownership_policy::stored_param stored_param;
        typedef typename ownership_policy::pointer_param
                                                        pointer_param;

    protected:          // Protected Interface
                        reject_null_static_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null_static(void)");
        }

                        reject_null_static_(reject_null_static_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(reject_null_static const&)"
            );
        }

                        template <typename U>
                        reject_null_static_(reject_null_static_<U> const& rhs)
                            : base_type(static_cast<typename
                                reject_null_static_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(reject_null_static<U> const&)"
            );
        }

                        template <typename U>
                        reject_null_static_(no_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_check_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(no_check<U> const&)"
            );
        }

                        reject_null_static_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(stored_param)"
            );
        }

                        template <typename U>
                        reject_null_static_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(U const&)"
            );
        }

                        template <typename U, typename V>
                        reject_null_static_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(U&, V&)"
            );
        }

                        template <typename U, typename V>
                        reject_null_static_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(U const&, V&)"
            );
        }

                        template <typename U, typename V>
                        reject_null_static_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(U&, V const&)"
            );
        }

                        template <typename U, typename V>
                        reject_null_static_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null_static(U const&, V const&)"
            );
        }

                       ~reject_null_static_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~reject_null_static(void)");
        }

        static
        void            on_default(pointer_param)
        {
            // This policy does not allow default initialization
            BOOST_STATIC_ASSERT(false);
        }

        static
        void            on_init(pointer_param p)
        {
            if (!p) throw null_pointer_error();
        }

        static
        void            on_dereference(pointer_param p)
        {
            if (!p) throw null_pointer_error();
        }

        static
        void            on_release(pointer_param)       { }

        static
        void            on_reset(pointer_param sp, pointer_param p)
        {
            BOOST_ASSERT(p == 0 || p != sp);
        }

        static
        void            swap(reject_null_static_& lhs, reject_null_static_& rhs)
        {
            base_type::swap(lhs, rhs);
        }
    };
    //------------------------------------------------------------------------
    struct reject_null_static
    {
        typedef checking_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef reject_null_static_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_REJECT_NULL_STATIC_HPP

