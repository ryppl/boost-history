//----------------------------------------------------------------------------
// Copyright (C) 2004, David B. Held and Andrei Alexandrescu
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
    struct assert_check
    {
        template <typename T>
        struct apply
        {
            typedef assert_check_<T> type;
        };
    };
    //------------------------------------------------------------------------
    template <class OwnershipPolicy>
    class assert_check_ : public OwnershipPolicy
    {
    public:             // Types
        typedef checking_policy_tag                     policy_category;
        typedef OwnershipPolicy                         ownership_policy;
        typedef typename ownership_policy::pointer_type pointer_type;

    protected:          // Protected Interface
                        assert_check_(void)             { }

                        template <typename U>
                        assert_check_(assert_check_<U> const&)
                                                        { }

                        assert_check_(assert_check_ const&)
                                                        { }

                        template <typename U>
                        assert_check_(no_check_<U> const&)
                                                        { }

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

        static
        void            swap(assert_check_&)            { }
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_ASSERT_CHECK_HPP
