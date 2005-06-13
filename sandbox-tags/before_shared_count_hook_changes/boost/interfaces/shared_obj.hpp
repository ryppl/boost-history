// (C) Copyright Jonathan Turkanis 2004.
// (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
// (C) Copyright 2001, 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Based on <boost/shared_ptr.hpp>

// Disclaimer: Not a Boost library.

// reinterpret_cast is eliminable here; it is used to simplify the
// implementation.

#ifndef BOOST_IDL_SHARED_OBJ_HPP_INCLUDED
#define BOOST_IDL_SHARED_OBJ_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <algorithm>                      // swap.
#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/detail/shared_count.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/interfaces/access.hpp>    // BOOST_IDL_PRIVATE.
#include <boost/interfaces/detail/preprocessor/befriend_template.hpp>
#include <boost/interfaces/fixed_view.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/swap.hpp>

namespace boost { namespace interfaces {

namespace detail {

template<typename Interface>
class shared_obj_impl {
public:
    static fixed_view<Interface>* get(const shared_obj<Interface>& obj)
    { 
        return access::get_interface_pointer(obj) ? 
            const_cast<fixed_view<Interface>*>(
                static_cast<const fixed_view<Interface>*>(&obj)
            ) :
            0;
    } 

    static void reset(shared_obj<Interface>& obj) 
    { 
        shared_obj<Interface> temp;
        shared_obj_impl<Interface>::swap(temp, obj); 
    }

    template<class T> 
    static void reset(shared_obj<Interface>& obj, T* t)
    {
        BOOST_ASSERT(t && t != access::get_interface_pointer(obj));
        shared_obj<Interface> temp(t);
        shared_obj_impl<Interface>::swap(temp, obj); 
    }

    template<class T, class D> 
    static void reset(shared_obj<Interface>& obj, T* t, D d)
    {
        BOOST_ASSERT(t && t != access::get_interface_pointer(obj));
        shared_obj<Interface> temp(t, d);
        shared_obj_impl<Interface>::swap(temp, obj); 
    }

    static void swap(shared_obj<Interface>& lhs, shared_obj<Interface>& rhs)
    {
        boost::interfaces::swap(
            implicit_cast< fixed_view<Interface>& >(lhs),
            implicit_cast< fixed_view<Interface>& >(rhs)
        );
        lhs.pn_idl_.swap(rhs.pn_idl_);
    }

    static bool unique(const shared_obj<Interface>& obj) 
    { 
        return obj.pn_idl_.unique(); 
    }

    static long use_count(const shared_obj<Interface>& obj) 
    { 
        return obj.pn_idl_.use_count(); 
    }
};

} // End namespace detail.

template<typename Interface>
class shared_obj : public fixed_view<Interface> {
public:
    BOOST_IDL_BEFRIEND_TEMPLATE_CLASS(shared_obj, 1, T)
    BOOST_IDL_BEFRIEND_TEMPLATE_CLASS(detail::shared_obj_impl, 1, T)
    friend class access;

        // Constructors

    shared_obj() : pn_idl_() { }

    shared_obj(const shared_obj& obj) 
        : fixed_view<Interface>(
              reinterpret_cast<const Interface&>(obj)
          ),
          pn_idl_(obj.pn_idl_)
        { }

    template<typename Subinterface>
    shared_obj(const shared_obj<Subinterface>& obj) 
        : fixed_view<Interface>(
              reinterpret_cast<const Subinterface&>(obj)
          ),
          pn_idl_(obj.pn_idl_)
        { }

    template<class T>
    explicit shared_obj(T* t) 
        : fixed_view<Interface>(*t), pn_idl_(t, checked_deleter<T>())
        { }

    template<class T, class D> 
    shared_obj(T* t, D d)
        : fixed_view<Interface>(*t), pn_idl_(t, d)
        { }

        // Assignment

    shared_obj& operator=(const shared_obj& obj)
    {
        fixed_view<Interface>::assign_interface(
            reinterpret_cast<const Interface&>(obj)
        );
        pn_idl_ = obj.pn_idl_;
        return *this;
    }

    template<class Subinterface>
    shared_obj& operator=(const shared_obj<Subinterface>& obj)
    {
        fixed_view<Interface>::assign_interface(
            reinterpret_cast<const Subinterface&>(obj)
        );
        pn_idl_ = obj.pn_idl_;
        return *this;
    }
BOOST_IDL_PRIVATE:
    boost::detail::shared_count pn_idl_; // reference counter
};

template<typename Interface>
fixed_view<Interface>* get(const shared_obj<Interface>& obj)
{ 
    return detail::shared_obj_impl<Interface>::get(obj);
}


template<typename Interface>
void reset(shared_obj<Interface>& obj) 
{ 
    detail::shared_obj_impl<Interface>::reset(obj);
}

template<typename Interface, class T> 
void reset(shared_obj<Interface>& obj, T* t)
{ 
    detail::shared_obj_impl<Interface>::reset(obj, t);
}

template<typename Interface, class T, class D> 
void reset(shared_obj<Interface>& obj, T* t, D d)
{ 
    detail::shared_obj_impl<Interface>::reset(obj, t, d);
}

template<typename Interface>
void swap(shared_obj<Interface>& lhs, shared_obj<Interface>& rhs)
{ 
    detail::shared_obj_impl<Interface>::swap(lhs, rhs);
}

template<typename Interface>
bool unique(const shared_obj<Interface>& obj) 
{ 
    return detail::shared_obj_impl<Interface>::unique(obj);
}

template<typename Interface>
long use_count(const shared_obj<Interface>& obj)
{ 
    return detail::shared_obj_impl<Interface>::use_count(obj);
}

} } // End namespace interfaces, boost.

#endif // #ifndef BOOST_IDL_SHARED_OBJ_HPP_INCLUDED
