//----------------------------------------------------------------------------
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See
// http://www.boost.org/LICENSE_1_0.txt)
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
        typedef checking_policy_tag                     policy_category;
        typedef OwnershipPolicy                         ownership_policy;
        typedef ownership_policy                        base_type;
        typedef typename ownership_policy::pointer_type pointer_type;

    protected:          // Protected Interface
                        no_check_(void)                 { }

                        no_check_(no_check_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
                                                        { }

                        template <typename U>
                        no_check_(no_check_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_check<U>::base_type const&
                            >(rhs))                     { }

                        template <typename U>
                        no_check_(U& p)
                            : base_type(p)              { }

                        template <typename U>
                        no_check_(U const& p)
                            : base_type(p)              { }

                        template <typename U, typename V>
                        no_check_(U& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        no_check_(U const& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        no_check_(U& p, V const& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        no_check_(U const& p, V const& d)
                            : base_type(p, d)           { }

        static
        void            on_default(pointer_type const&) { }

        static
        void            on_init(pointer_type const&)    { }

        static
        void            on_dereference(pointer_type const& val)
                                                        { }

        static
        void            on_release(pointer_type const&) { }

        static
        void            swap(no_check_&)            { }
    };
    //------------------------------------------------------------------------
    struct no_check
    {
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
