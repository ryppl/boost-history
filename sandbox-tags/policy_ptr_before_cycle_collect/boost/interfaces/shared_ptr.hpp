// (C) Copyright Jonathan Turkanis 2004.
// (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
// (C) Copyright 2001, 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Based on <boost/shared_ptr.hpp>

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_shared_ptr_HPP_INCLUDED
#define BOOST_IDL_shared_ptr_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <algorithm> // swap.
#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/detail/shared_count.hpp>
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/preprocessor/befriend_template.hpp>
#include <boost/interfaces/detail/preprocessor/smart_ptr_support.hpp>
#include <boost/interfaces/fixed_view.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/swap.hpp>

namespace boost { namespace interfaces {

template<typename Interface>
class shared_ptr {
public:
    BOOST_IDL_CORE_SMART_PTR_SUPPORT(shared_ptr, Interface)

        // Constructors

    shared_ptr() 
        : BOOST_IDL_MEMBER_PTR(0),
          BOOST_IDL_MEMBER_TABLE(0),
          pn_idl_() 
        { }

    //shared_ptr(const shared_ptr& p) 
    //    : BOOST_IDL_MEMBER_PTR(p.get_pointer()),
    //      BOOST_IDL_MEMBER_TABLE(p.get_table()),
    //      pn_idl_(p.pn_idl_)
    //    { }

    template<typename Subinterface>
    shared_ptr(const shared_ptr<Subinterface>& p) 
        : BOOST_IDL_MEMBER_PTR(p.get_pointer()),
          BOOST_IDL_MEMBER_TABLE(
              p.get_table() + 
              detail::offset_of<Subinterface, Interface>::value
          ),
          pn_idl_(p.pn_idl_)
        { }

    template<class T>
    explicit shared_ptr(T* t) 
        : BOOST_IDL_MEMBER_PTR(t),
          BOOST_IDL_MEMBER_TABLE(detail::initialize_table<Interface>(*t)),
          pn_idl_(t, checked_deleter<T>())
        { }

    template<class T, class D> 
    shared_ptr(T* t, D d)
        : BOOST_IDL_MEMBER_PTR(t),
          BOOST_IDL_MEMBER_TABLE(detail::initialize_table<Interface>(*t)),
          pn_idl_(t, d)
          
        { }

        // Assignment

    //shared_ptr& operator=(const shared_ptr& p)
    //{
    //    get_pointer() = p.get_pointer();
    //    get_table() = p.get_table();
    //    pn_idl_ = p.pn_idl_;
    //    return *this;
    //}

    template<class Subinterface>
    shared_ptr& operator=(const shared_ptr<Subinterface>& p)
    {
        get_pointer() = p.get_pointer();
        get_table() = 
            p.get_table()+ 
            detail::offset_of<Subinterface, Interface>::value;
        pn_idl_ = p.pn_idl_;
        return *this;
    }

          // Smart pointer interface

    void reset() { shared_ptr<Interface>().swap(*this); }

    template<class T> 
    void reset(T* t)
    {
        BOOST_ASSERT(t && t != get_pointer());
        shared_ptr<Interface>(t).swap(*this);
    }

    template<class T, class D> 
    void reset(T* t, D d)
    {
        BOOST_ASSERT(t && t != get_pointer());
        shared_ptr<Interface>(t, d).swap(*this);
    }

    void swap(shared_ptr& p)
    {
        std::swap(get_pointer(), p.get_pointer());
        std::swap(get_table(), p.get_table());
        pn_idl_.swap(p.pn_idl_);
    }

    bool unique() const { return pn_idl_.unique(); }

    long use_count() const { return pn_idl_.use_count(); }
BOOST_IDL_PRIVATE:
    boost::detail::shared_count pn_idl_; // reference counter
};

} } // End namespace interfaces, boost.

#endif // #ifndef BOOST_IDL_shared_ptr_HPP_INCLUDED
