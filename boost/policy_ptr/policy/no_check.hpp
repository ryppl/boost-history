//----------------------------------------------------------------------------
// Copyright (C) 2004, David B. Held and Andrei Alexandrescu
//----------------------------------------------------------------------------
#ifndef BOOST_NO_CHECK_HPP
#define BOOST_NO_CHECK_HPP
//----------------------------------------------------------------------------
namespace boost
{
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
    template <class OwnershipPolicy>
    class no_check_ : public OwnershipPolicy
    {
    public:             // Types
        typedef checking_policy_tag                     policy_category;
        typedef OwnershipPolicy                         ownership_policy;
        typedef typename ownership_policy::pointer_type pointer_type;

    protected:          // Protected Interface
                        no_check_(void)             { }

                        template <typename U>
                        no_check_(no_check_<U> const&)
                                                        { }

                        no_check_(no_check_ const&)
                                                        { }

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
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_NO_CHECK_HPP
