//----------------------------------------------------------------------------
// no_check.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_NO_CHECK_HPP
#define BOOST_NO_CHECK_HPP
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class OwnershipPolicy>
    class no_check_ : public OwnershipPolicy
    {
    public:             // Types
        typedef OwnershipPolicy                         ownership_policy;
        typedef ownership_policy                        base_type;
        typedef typename ownership_policy::pointer_type pointer_type;
        typedef typename ownership_policy::stored_param stored_param;
        typedef typename ownership_policy::pointer_param
                                                        pointer_param;

    protected:          // Protected Interface
                        no_check_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(void)");
        }

                        no_check_(no_check_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(no_check const&)");
        }

                        template <typename U>
                        no_check_(no_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_check_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(no_check<U> const&)");
        }

                        no_check_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(stored_param)");
        }

                        template <typename U>
                        no_check_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(U const&)");
        }

                        template <typename U, typename V>
                        no_check_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(U&, V&)");
        }

                        template <typename U, typename V>
                        no_check_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(U const&, V&)");
        }

                        template <typename U, typename V>
                        no_check_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(U&, V const&)");
        }

                        template <typename U, typename V>
                        no_check_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_check(U const&, V const&)");
        }

                       ~no_check_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~no_check(void)");
        }

        static
        void            on_default(pointer_param)       { }

        static
        void            on_init(pointer_param)          { }

        static
        void            on_dereference(pointer_param)   { }

        static
        void            on_release(pointer_param)       { }

        static
        void            on_reset(pointer_param, pointer_param)
                                                        { }

        static
        void            swap(no_check_& lhs, no_check_& rhs)
        {
            base_type::swap(lhs, rhs);
        }
    };
    //------------------------------------------------------------------------
    struct no_check
    {
        typedef checking_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef no_check_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_NO_CHECK_HPP

