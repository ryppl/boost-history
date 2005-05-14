//----------------------------------------------------------------------------
// reject_null.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_REJECT_NULL_HPP
#define BOOST_REJECT_NULL_HPP
//----------------------------------------------------------------------------
// Headers
#include <boost/assert.hpp>
#include "no_check.hpp"
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class OwnershipPolicy>
    class reject_null_ : public OwnershipPolicy
    {
    public:             // Types
        typedef OwnershipPolicy                         ownership_policy;
        typedef ownership_policy                        base_type;
        typedef typename ownership_policy::pointer_type pointer_type;
        typedef typename ownership_policy::stored_param stored_param;
        typedef typename ownership_policy::pointer_param
                                                        pointer_param;

    protected:          // Protected Interface
                        reject_null_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(void)");
        }

                        reject_null_(reject_null_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(reject_null const&)");
        }

                        template <typename U>
                        reject_null_(reject_null_<U> const& rhs)
                            : base_type(static_cast<typename
                                reject_null_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "reject_null(reject_null<U> const&)"
            );
        }

                        template <typename U>
                        reject_null_(no_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_check_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(no_check<U> const&)");
        }

                        reject_null_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(stored_param)");
        }

                        template <typename U>
                        reject_null_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(U const&)");
        }

                        template <typename U, typename V>
                        reject_null_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(U&, V&)");
        }

                        template <typename U, typename V>
                        reject_null_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(U const&, V&)");
        }

                        template <typename U, typename V>
                        reject_null_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(U&, V const&)");
        }

                        template <typename U, typename V>
                        reject_null_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "reject_null(U const&, V const&)");
        }

                       ~reject_null_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~reject_null(void)");
        }

        static
        void            on_default(pointer_param)       { }

        static
        void            on_init(pointer_param p)        { }

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
        void            swap(reject_null_& lhs, reject_null_& rhs)
        {
            base_type::swap(lhs, rhs);
        }
    };
    //------------------------------------------------------------------------
    struct reject_null
    {
        typedef checking_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef reject_null_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_REJECT_NULL_HPP

