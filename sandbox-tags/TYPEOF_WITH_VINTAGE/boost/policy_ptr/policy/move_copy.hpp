//----------------------------------------------------------------------------
// move_copy.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_MOVE_COPY_HPP
#define BOOST_MOVE_COPY_HPP
//----------------------------------------------------------------------------
#include <boost/static_assert.hpp>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    class move_copy_ : public StoragePolicy
    {
    public:             // Types
        typedef move_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::stored_type    stored_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;

    protected:          // Protected Interface
                        move_copy_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(void)");
        }

                        move_copy_(move_copy_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(move_copy const&)");
        }

                        template <typename U>
                        move_copy_(move_copy_<U> const& rhs)
                            : base_type(static_cast<
                                typename move_copy_<U>::base_type const&
                            >(rhs))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(move_copy<U> const&)");
        }

                        move_copy_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(stored_param)");
        }

                        template <typename U>
                        move_copy_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(U const&)");
        }

                        template <typename U, typename V>
                        move_copy_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(U&, V&)");
        }

                        template <typename U, typename V>
                        move_copy_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(U const&, V&)");
        }

                        template <typename U, typename V>
                        move_copy_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(U&, V const&)");
        }

                        template <typename U, typename V>
                        move_copy_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "move_copy(U const&, V const&)");
        }

                       ~move_copy_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~move_copy(void)");
        }

        static
        stored_type     clone(stored_type const& p)
        {
            stored_type& ref = const_cast<stored_type&>(p);
            stored_type result(ref);
            ref = storage_policy::default_value();
            return result;
        }

        static
        void            swap(move_copy_& lhs, move_copy_& rhs)
        {
            base_type::swap(lhs, rhs);
        }

        static
        void            on_release(move_copy_ const&)   { }

        static
        void            reset(move_copy_ const&)        { }
    };
    //------------------------------------------------------------------------
    struct move_copy
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef move_copy_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_MOVE_COPY_HPP

