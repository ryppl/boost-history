//----------------------------------------------------------------------------
// array_storage.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_ARRAY_STORAGE_HPP
#define BOOST_ARRAY_STORAGE_HPP
//----------------------------------------------------------------------------
// Headers
#include <algorithm>                                    // std::swap()
#include <boost/checked_delete.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/add_reference.hpp>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <typename T>
    class array_storage_
    {
    public:             // Types
        typedef T*                                      stored_type;
        typedef T const*                                const_stored_type;
        typedef T*                                      pointer_type;
        typedef T const*                                const_pointer_type;
        typedef typename add_reference<T>::type         reference_type;
        typedef typename add_reference<T const>::type   const_reference_type;
        typedef typename call_traits<stored_type>::param_type
                                                        stored_param;
        typedef typename call_traits<pointer_type>::param_type
                                                        pointer_param;

    protected:          // Protected Interface
                        array_storage_(void)
                            : p_(default_value())
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "array_storage(void)");
        }

                        array_storage_(array_storage_ const&)
                            : p_(default_value())
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "array_storage(array_storage const&)"
            );
        }

                        template <typename U>
                        array_storage_(array_storage_<U> const&)
                            : p_(default_value())
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "array_storage(array_storage<U> const&)"
            );
        }

                        array_storage_(stored_param p)
                            : p_(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "array_storage(stored_param)"
            );
        }

                       ~array_storage_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~array_storage(void)");
            checked_array_delete(p_);
        }

        static
        void            swap(array_storage_& lhs, array_storage_& rhs)
        {
            std::swap(lhs.p_, rhs.p_);
        }

		void            release(void)
		{
            p_ = 0;
        }

        bool            is_valid(void) const
        {
            return p_ != default_value();
        }

        static
        stored_param    default_value(void)
        {
            return 0;
        }

    public:             // Public Interface
        friend inline
        pointer_type    get_impl(array_storage_ const& sp)
        {
            return sp.p_;
        }

        friend inline
        stored_type const&
                        get_impl_ref(array_storage_ const& sp)
        {
            return sp.p_;
        }

        friend inline
        stored_type&    get_impl_ref(array_storage_& sp)
        {
            return sp.p_;
        }

        const_reference_type
                        operator[](int i) const
        {
            BOOST_SP_INVARIANT(i >= 0);
            return p_[i];
        }

        reference_type  operator[](int i)
        {
            BOOST_SP_INVARIANT(i >= 0);
            return p_[i];
        }

    private:            // Implementation
        stored_type     p_;
    };
    //------------------------------------------------------------------------
    struct array_storage
    {
        typedef storage_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef array_storage_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_ARRAY_STORAGE_HPP

