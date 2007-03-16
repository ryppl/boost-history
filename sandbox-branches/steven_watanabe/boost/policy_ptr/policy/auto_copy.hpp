//----------------------------------------------------------------------------
// move_copy.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_AUTO_COPY_HPP
#define BOOST_AUTO_COPY_HPP
//----------------------------------------------------------------------------
#include <boost/policy_ptr/policy/move_copy.hpp>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    class auto_copy_ : public move_copy_<StoragePolicy>
    {
    public:             // Types
        typedef auto_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef move_copy_<StoragePolicy>               base_type;
        typedef typename storage_policy::stored_type    stored_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;

    protected:          // Protected Interface
                        auto_copy_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(void)");
        }

                        auto_copy_(auto_copy_ const& rhs)
                            : base_type(rhs)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(move_copy const&)");
        }

                        template <typename U>
                        auto_copy_(auto_copy_<U> const& rhs)
                            : base_type(rhs)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(move_copy<U> const&)");
        }

                        auto_copy_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(stored_param)");
        }

                        template <typename U>
                        auto_copy_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(U const&)");
        }

                        template <typename U, typename V>
                        auto_copy_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(U&, V&)");
        }

                        template <typename U, typename V>
                        auto_copy_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(U const&, V&)");
        }

                        template <typename U, typename V>
                        auto_copy_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(U&, V const&)");
        }

                        template <typename U, typename V>
                        auto_copy_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "auto_copy(U const&, V const&)");
        }

                       ~auto_copy_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~auto_copy(void)");
        }
    };
    //------------------------------------------------------------------------
    struct auto_copy
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef auto_copy_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_AUTO_COPY_HPP

