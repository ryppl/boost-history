// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DELEGATION_HPP_INCLUDED
#define BOOST_IDL_DELEGATION_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/access.hpp> 
#include <boost/interfaces_fwd.hpp> 
#include <boost/interfaces/fixed_view.hpp> 

namespace boost { namespace interfaces {

template<typename Interface>
class delegating : public fixed_view<Interface> {
protected:
    fixed_view<Interface>* get() { return this; }
    template<typename T>
    void set(T* t) { fixed_view<Interface>::assign_object(*t); }
};

template<typename Interface, typename Delegator>
fixed_view<Interface>* get_delegate(Delegator& d) 
{ return access::get_delegate<Interface>(d); }

template<typename Interface, typename Delegator, typename T>
void set_delegate(Delegator& d, T* t) 
{ access::set_delegate<Interface>(d, t); }

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_DELEGATION_HPP_INCLUDED
