// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

// reinterpret_cast is eliminable here; it is used to simplify the
// implementation.

#ifndef BOOST_IDL_UNIQUE_INTERFACE_PTR_HPP_INCLUDED
#define BOOST_IDL_UNIQUE_INTERFACE_PTR_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <algorithm> // swap.
#include <boost/assert.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/smart_ptr_support.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/preprocessor/move_support.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/manual_ptr.hpp>
#include <boost/interfaces/move.hpp>     

#include <boost/config.hpp>     // BOOST_MSVC.
#if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable:4521)  // Multiple copy constuctors.
#endif       

namespace boost { namespace interfaces {

template<typename Interface>
class unique_ptr {
public:
    BOOST_IDL_CORE_SMART_PTR_SUPPORT(unique_ptr, Interface)

        // Constructors

    BOOST_IDL_CTOR_DEFAULT(unique_ptr)
    BOOST_IDL_SMART_PTR_CTOR_FROM_PTR(unique_ptr, Interface)

    unique_ptr(const unique_ptr& p) 
        : BOOST_IDL_MEMBER_PTR(p.get_pointer()),
          BOOST_IDL_MEMBER_TABLE(p.get_table())
    { 
        p.get_pointer() = 0;
        p.get_table() = 0;
    }

    template<typename Subinterface>
    unique_ptr(const unique_ptr<Subinterface>& p) 
        : BOOST_IDL_MEMBER_PTR(p.get_pointer()),
          BOOST_IDL_MEMBER_TABLE(
              p.get_table() + 
              detail::offset_of<Subinterface, Interface>::value
          )
    { 
        p.get_pointer() = 0;
        p.get_table() = 0;
    }

    template<typename Subinterface>
    unique_ptr(move_source< unique_ptr<Subinterface> > src)
        : BOOST_IDL_MEMBER_PTR(src.ptr().get_pointer()),
          BOOST_IDL_MEMBER_TABLE(
              src.ptr().get_table() + 
              detail::offset_of<Subinterface, Interface>::value
          )
    {   
        src.ptr().get_pointer() = 0;
        src.ptr().get_table() = 0;
    }

        // Destructor

    ~unique_ptr() 
    { 
        if (get_pointer())
            detail::get_deleter(*this)(get_pointer());
    }

          // Assignment

    unique_ptr& operator=(unique_ptr p)
    {
        p.swap(*this);
        return *this;
    }

    template<typename Subinterface>    
    unique_ptr& operator=(unique_ptr<Subinterface> p)
    {
        std::swap(get_pointer(), p.get_pointer());
        get_table() = 
            p.get_table() + 
            detail::offset_of<Subinterface, Interface>::value;
        p.get_table() = 0;
        return *this;
    }

        // Smart pointer interface

    BOOST_IDL_SMART_PTR_SWAP(unique_ptr)

    void reset()
    {
        if (get_pointer()) {
            detail::get_deleter(*this)(get_pointer());
            get_pointer() = 0;
            get_table() = 0;
        }
    }

    template<typename T>
    void reset(T* t) 
    { 
        BOOST_ASSERT(t && t != get_pointer());
        unique_ptr<Interface>(t).swap(*this); 
    }

    manual_ptr<Interface> release()
    {
        manual_ptr<Interface> result =
            *reinterpret_cast<manual_ptr<Interface>*>(this);
        get_pointer() = 0;
        get_table() = 0;
        return result;
    }

    // VC7.1 requires this to be at end.
    BOOST_IDL_MOVE_SUPPORT(unique_ptr)
};

} } // End namespace interfaces, boost.

#if defined(BOOST_MSVC)
#pragma warning(pop) // #pragma warning(disable:4251)
#endif

#endif // #ifndef BOOST_IDL_UNIQUE_INTERFACE_PTR_HPP_INCLUDED
