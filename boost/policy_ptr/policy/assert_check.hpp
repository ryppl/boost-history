//----------------------------------------------------------------------------
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See
// http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_ASSERT_CHECK_HPP
#define BOOST_ASSERT_CHECK_HPP
//----------------------------------------------------------------------------
// Headers
#include <cstdlib>                                      // assert()
#include "policy/no_check.hpp"
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class OwnershipPolicy>
    class assert_check_ : public OwnershipPolicy
    {
    public:             // Types
        typedef checking_policy_tag                     policy_category;
        typedef OwnershipPolicy                         ownership_policy;
        typedef ownership_policy                        base_type;
        typedef typename ownership_policy::pointer_type pointer_type;

    protected:          // Protected Interface
                        assert_check_(void)             { }

                        assert_check_(assert_check_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
                                                        { }

                        template <typename U>
                        assert_check_(assert_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename assert_check_<U>::base_type const&
                            >(rhs))                     { }

                        template <typename U>
                        assert_check_(no_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_check_<U>::base_type const&
                            >(rhs))                     { }

                        template <typename U>
                        assert_check_(U& p)
                            : base_type(p)              { }

                        template <typename U>
                        assert_check_(U const& p)
                            : base_type(p)              { }

                        template <typename U, typename V>
                        assert_check_(U& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        assert_check_(U const& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        assert_check_(U& p, V const& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        assert_check_(U const& p, V const& d)
                            : base_type(p, d)           { }

        static
        void            on_default(pointer_type const&) { }

        static
        void            on_init(pointer_type const&)    { }

        static
        void            on_dereference(pointer_type const& val)
        {
            assert(val);
        }

        static
        void            on_release(pointer_type const&) { }

        template <class SP>
        static
        void            on_reset(pointer_type const& p, SP const& sp)
        {
            BOOST_ASSERT(p == 0 || p != get_impl(sp));
        }

        static
        void            swap(assert_check_&)            { }
    };
    //------------------------------------------------------------------------
    struct assert_check
    {
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

