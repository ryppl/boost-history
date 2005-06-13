// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_manual_ptr_HPP_INCLUDED
#define BOOST_IDL_manual_ptr_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assert.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/smart_ptr_support.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/interfaces_fwd.hpp>

namespace boost { namespace interfaces {

template<typename Interface>
class manual_ptr {
public:
    BOOST_IDL_CORE_SMART_PTR_SUPPORT(manual_ptr, Interface)

        // Constructors

    BOOST_IDL_CTOR_DEFAULT(manual_ptr)
    BOOST_IDL_SMART_PTR_CTOR_FROM_PTR(manual_ptr, Interface)

    template<typename Subinterface>
    manual_ptr(const manual_ptr<Subinterface>& p) 
        : BOOST_IDL_MEMBER_PTR(access::get_interface_pointer(p)), 
          BOOST_IDL_MEMBER_TABLE(
              p.get_table() +
              detail::offset_of<Subinterface, Interface>::value
          )
        { } 

        // Assignment

    manual_ptr& operator=(manual_ptr p)
    {
        get_pointer() = p.get_pointer();
        get_table() = p.get_table();
        return *this;
    }

    template<typename Subinterface>    
    manual_ptr& operator=(manual_ptr<Subinterface> p)
    {
        get_pointer() = p.get_pointer();
        get_table() = 
            p.get_table() + 
            detail::offset_of<Subinterface, Interface>::value;
        return *this;
    }

        // Smart pointer interface

    BOOST_IDL_SMART_PTR_SWAP(manual_ptr)

    void reset()
    {
        get_pointer() = 0;
        get_table() = 0;
    }

    template<typename T>
    void reset(T* t) 
    { 
        get_pointer() = t;
        get_table() = detail::initialize_deleter<Interface>(*t);
    }

    void destroy()
    {
        if (get_pointer()) {
            detail::get_deleter(*this)(get_pointer());
            reset();
        }
    }
};

} } // End namespace interfaces, boost.

#endif // #ifndef BOOST_IDL_manual_ptr_HPP_INCLUDED
