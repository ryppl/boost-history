// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

// reinterpret_cast is eliminable here; it is used to simplify the
// implementation.

#ifndef BOOST_IDL_UNIQUE_OBJ_HPP_INCLUDED
#define BOOST_IDL_UNIQUE_OBJ_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <algorithm>                          // swap.
#include <boost/assert.hpp>
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/befriend_template.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/preprocessor/move_support.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/interfaces/fixed_view.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/manual_ptr.hpp>
#include <boost/interfaces/swap.hpp>
#include <boost/interfaces/move.hpp>     

#include <boost/config.hpp>     // BOOST_MSVC.
#if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4521) // Multiple copy constuctors.
#endif

namespace boost { namespace interfaces {

namespace detail {

template<typename Interface>
class unique_obj_impl {
public:
    static fixed_view<Interface>* get(const unique_obj<Interface>& obj)
    { 
        return access::get_interface_pointer(obj) ? 
            const_cast<fixed_view<Interface>*>(
                static_cast<const fixed_view<Interface>*>(&obj)
            ) :
            0;
    } 

    static void reset(unique_obj<Interface>& obj)
    {
        if (const void* pv = access::get_interface_pointer(obj)) {
            get_deleter(obj)(pv);
            access::set_interface_pointer(obj, 0);
            access::set_interface_table(obj, 0);
        }
    }

    template<typename T>
    static void reset(unique_obj<Interface>& obj, T* t) 
    { 
        BOOST_ASSERT(t && t != access::get_interface_pointer(obj));
        unique_obj<Interface> temp(t);
        interfaces::swap(obj, temp);
    }

    static manual_ptr<Interface> release(unique_obj<Interface>& obj) 
    { 
        manual_ptr<Interface> result =
            *reinterpret_cast<manual_ptr<Interface>*>(&obj);
        access::set_interface_pointer(obj, 0);
        access::set_interface_table(obj, 0);
        return result;
    }
};

} // End namespace detail.

template<typename Interface>
class unique_obj : public fixed_view<Interface> {
public:
    BOOST_IDL_BEFRIEND_TEMPLATE_CLASS(unique_obj, 1, T)
    friend class access;

        // Constructors

    unique_obj() { }

    unique_obj(const unique_obj& obj) 
        : fixed_view<Interface>(
              reinterpret_cast<const Interface&>(obj)
          )
    { 
        access::set_interface_pointer(obj, 0);
        access::set_interface_table(obj, 0);
    }

    template<typename Subinterface>
    unique_obj(const unique_obj<Subinterface>& obj) 
        : fixed_view<Interface>(
              reinterpret_cast<const Subinterface&>(obj)
          )
    { 
        access::set_interface_pointer(obj, 0);
        access::set_interface_table(obj, 0);
    }

    template<typename Subinterface>
    unique_obj(move_source< unique_obj<Subinterface> > src)
        : fixed_view<Interface>(
              reinterpret_cast<const Subinterface&>(src.ptr())
          )
    { 
        access::set_interface_pointer(src.ptr(), 0);
        access::set_interface_table(src.ptr(), 0);
    }

    template<typename T>
    explicit unique_obj(T* t) 
    {
        access::set_interface_pointer(*this, t);
        access::set_interface_table(
            *this, detail::initialize_deleter<Interface>(*t)
        );
    }

        // Destructor

    ~unique_obj() 
    { 
        if (const void* pv = access::get_interface_pointer(*this))
            detail::get_deleter(*this)(pv);
    }

        // Assignment

    unique_obj& operator=(unique_obj rhs)
    {
        boost::interfaces::swap(*this, rhs);
        return *this;
    }

    template<typename Subinterface>    
    unique_obj& operator=(unique_obj<Subinterface> p)
    { // Slightly more efficient version is cumbersome to express.
        unique_obj<Interface> temp(move(p));
        boost::interfaces::swap(*this, temp);
        return *this;
    }
BOOST_IDL_PRIVATE:
    BOOST_IDL_MOVE_SUPPORT(unique_obj)
};

template<typename Interface>
fixed_view<Interface>* get(const unique_obj<Interface>& obj)
{ 
    return detail::unique_obj_impl<Interface>::get(obj);
}

template<typename Interface>
void reset(unique_obj<Interface>& obj) 
{ 
    detail::unique_obj_impl<Interface>::reset(obj);
}

template<typename Interface, class T> 
void reset(unique_obj<Interface>& obj, T* t)
{ 
    detail::unique_obj_impl<Interface>::reset(obj, t);
}

template<typename Interface> 
manual_ptr<Interface> release(unique_obj<Interface>& obj)
{ 
    return detail::unique_obj_impl<Interface>::release(obj);
}

} } // End namespace interfaces, boost.

#if defined(BOOST_MSVC)
#pragma warning(pop) // #pragma warning(disable:4251)
#endif

#endif // #ifndef BOOST_IDL_UNIQUE_OBJ_HPP_INCLUDED
