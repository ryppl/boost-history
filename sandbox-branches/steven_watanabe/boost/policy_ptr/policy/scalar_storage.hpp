//----------------------------------------------------------------------------
// scalar_storage.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
// ChangeLog(latest at top):
//   2005-06-01 Larry Evans
//     WHAT:
//       1) Added friend get_less_comparator.
//     WHY:
//       1) It is now used in smart_ptr.hpp in comparison ops; hence,
//          it needs to be defined here.
//----------------------------------------------------------------------------
#ifndef BOOST_SCALAR_STORAGE_HPP
#define BOOST_SCALAR_STORAGE_HPP
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
    class scalar_storage_
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
                        scalar_storage_(void)
                            : p_(default_value())
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "scalar_storage(void)");
        }

                        scalar_storage_(scalar_storage_ const&)
                            : p_(default_value())
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "scalar_storage(scalar_storage const&)"
            );
        }

                        template <typename U>
                        scalar_storage_(scalar_storage_<U> const&)
                            : p_(default_value())
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "scalar_storage_(scalar_storage<U> const&)"
            );
        }

                        scalar_storage_(stored_param p)
                            : p_(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "scalar_storage(stored_param)");
        }

                       ~scalar_storage_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~scalar_storage(void)");
            checked_delete(p_);
        }

        static
        void            swap(scalar_storage_& lhs, scalar_storage_& rhs)
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
        pointer_type    get_impl(scalar_storage_ const& sp)
        {
            return sp.p_;
        }

        friend inline 
        pointer_type    get_less_comparator(scalar_storage_ const& sp)
        { 
            return get_impl(sp); 
        }

        friend inline
        stored_type const&
                        get_impl_ref(scalar_storage_ const& sp)
        {
            return sp.p_;
        }

        friend inline
        stored_type&    get_impl_ref(scalar_storage_& sp)
        {
            return sp.p_;
        }

    private:            // Implementation
        stored_type     p_;
    };
    //------------------------------------------------------------------------
    struct scalar_storage
    {
        typedef storage_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef scalar_storage_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_SCALAR_STORAGE_HPP

