//----------------------------------------------------------------------------
// deep_copy.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_NO_COPY_HPP
#define BOOST_NO_COPY_HPP
//----------------------------------------------------------------------------
#include <boost/static_assert.hpp>
#include "../always_false.hpp"
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    class no_copy_ : public StoragePolicy
    {
    public:             // Types
        typedef no_copy_semantics_tag                   ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;

    protected:          // Protected Interface
                        no_copy_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(void)");
        }

                        no_copy_(no_copy_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(no_copy const&)");
        }

                        template <typename U>
                        no_copy_(no_copy_<U> const& rhs)
                            : base_type(static_cast<
                                typename no_copy_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(no_copy<U> const&)");
        }

                        no_copy_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(stored_param)");
        }

                        template <typename U>
                        no_copy_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(U const&)");
        }

                        template <typename U, typename V>
                        no_copy_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(U&, V&)");
        }

                        template <typename U, typename V>
                        no_copy_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(U const&, V&)");
        }

                        template <typename U, typename V>
                        no_copy_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(U&, V const&)");
        }

                        template <typename U, typename V>
                        no_copy_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "no_copy(U const&, V const&)");
        }

                       ~no_copy_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~no_copy(void)");
        }

        static
        stored_param    clone(stored_param)
        {
            // This policy disallows value copying
            BOOST_STATIC_ASSERT(always_false<storage_policy>::value);
        }

        static
        void            swap(no_copy_& lhs, no_copy_& rhs)
        {
            base_type::swap(lhs, rhs);
        }

        static
        void            on_release(no_copy_ const&)     { }

        static
        void            reset(no_copy_ const&)          { }
    };
    //------------------------------------------------------------------------
    struct no_copy
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef no_copy_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_NO_COPY_HPP
