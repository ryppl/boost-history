//----------------------------------------------------------------------------
// assert_check.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_ASSERT_CHECK_HPP
#define BOOST_ASSERT_CHECK_HPP
//----------------------------------------------------------------------------
// Headers
#include <boost/assert.hpp>
#include "no_check.hpp"
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class OwnershipPolicy>
    class assert_check_ : public OwnershipPolicy
    {
    public:             // Types
        typedef OwnershipPolicy                         ownership_policy;
        typedef ownership_policy                        base_type;
        typedef typename ownership_policy::pointer_type pointer_type;
        typedef typename ownership_policy::stored_param stored_param;
        typedef typename ownership_policy::pointer_param
                                                        pointer_param;

    protected:          // Protected Interface
                        assert_check_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "assert_check(void)");
        }

                        assert_check_(assert_check_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "assert_check(assert_check const&)"
            );
        }

                        template <typename U>
                        assert_check_(assert_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename assert_check_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "assert_check(assert_check<U> const&)"
            );
        }

                        template <typename U>
                        assert_check_(no_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_check_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "assert_check(no_check<U> const&)"
            );
        }

                        assert_check_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "assert_check(stored_param)");
        }

                        template <typename U>
                        assert_check_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "assert_check(U const&)");
        }

                        template <typename U, typename V>
                        assert_check_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "assert_check(U&, V&)");
        }

                        template <typename U, typename V>
                        assert_check_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "assert_check(U const&, V&)");
        }

                        template <typename U, typename V>
                        assert_check_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "assert_check(U&, V const&)");
        }

                        template <typename U, typename V>
                        assert_check_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "assert_check(U const&, V const&)"
            );
        }

                       ~assert_check_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~assert_check(void)");
        }

        static
        void            on_default(pointer_param)       { }

        static
        void            on_init(pointer_param)          { }

        static
        void            on_dereference(pointer_param p)
        {
            BOOST_ASSERT(p);
        }

        static
        void            on_release(pointer_param)       { }

        static
        void            on_reset(pointer_param sp, pointer_param p)
        {
            BOOST_ASSERT(p == 0 || p != sp);
        }

        static
        void            swap(assert_check_& lhs, assert_check_& rhs)
        {
            base_type::swap(lhs, rhs);
        }
    };
    //------------------------------------------------------------------------
    struct assert_check
    {
        typedef checking_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef assert_check_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_ASSERT_CHECK_HPP

