//----------------------------------------------------------------------------
// deep_copy.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_DEEP_COPY_HPP
#define BOOST_DEEP_COPY_HPP
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    class deep_copy_ : public StoragePolicy
    {
    public:             // Types
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;

    protected:          // Protected Interface
                        deep_copy_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(void)");
        }

                        deep_copy_(deep_copy_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(deep_copy const&)");
        }

                        template <typename U>
                        deep_copy_(deep_copy_<U> const& rhs)
                            : base_type(static_cast<
                                typename deep_copy_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(deep_copy<U> const&)");
        }

                        deep_copy_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(stored_param)");
        }

                        template <typename U>
                        deep_copy_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(U const&)");
        }

                        template <typename U, typename V>
                        deep_copy_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(U&, V&)");
        }

                        template <typename U, typename V>
                        deep_copy_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(U const&, V&)");
        }

                        template <typename U, typename V>
                        deep_copy_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(U&, V const&)");
        }

                        template <typename U, typename V>
                        deep_copy_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "deep_copy(U const&, V const&)");
        }

                       ~deep_copy_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~deep_copy(void)");
        }

        static
        stored_param    clone(stored_param p)
        {
            return p->clone();
        }

        static
        void            swap(deep_copy_& lhs, deep_copy_& rhs)
        {
            base_type::swap(lhs, rhs);
        }

        static
        void            on_release(deep_copy_ const&)   { }

        static
        void            reset(deep_copy_ const&)        { }
    };
    //------------------------------------------------------------------------
    struct deep_copy
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef deep_copy_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_DEEP_COPY_HPP
