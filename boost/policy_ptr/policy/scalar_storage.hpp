//----------------------------------------------------------------------------
// Copyright (C) 2004, David B. Held and Andrei Alexandrescu
//----------------------------------------------------------------------------
#ifndef BOOST_SCALAR_STORAGE_HPP
#define BOOST_SCALAR_STORAGE_HPP
//----------------------------------------------------------------------------
// Headers
#include <algorithm>                                    // std::swap()
#include <boost/checked_delete.hpp>
#include <boost/type_traits/add_reference.hpp>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    struct scalar_storage
    {
        template <typename T>
        struct apply
        {
            typedef scalar_storage_<T> type;
        };
    };
    //------------------------------------------------------------------------
    // The storage policy doesn't initialize the stored pointer
    //     which will be initialized by OwnershipPolicy::clone()
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

    protected:          // Protected Interface
                        scalar_storage_(void)
                            : p_(default_value())       { }

                        scalar_storage_(scalar_storage_ const&)
                            : p_(default_value())       { }

                        template <typename U>
                        scalar_storage_(scalar_storage_<U> const&)
                            : p_(default_value())       { }

                        scalar_storage_(stored_type const& p)
                            : p_(p)                     { }

                       ~scalar_storage_(void)
        {
            checked_delete(p_);
        }

        void            swap(scalar_storage_& rhs)
        {
            std::swap(p_, rhs.p_);
        }

        pointer_type    get_pointer(void) const
        {
            return p_;
        }

        reference_type  get_reference(void) const
        {
            return *p_;
        }

        bool            is_valid(void) const
        {
            return p_ != default_value();
        }

		void            release(void)
		{
            p_ = 0;
        }

        static
        stored_type     default_value(void)
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
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_SCALAR_STORAGE_HPP
