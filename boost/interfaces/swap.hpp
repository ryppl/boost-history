// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_SWAP_HPP_INCLUDED
#define BOOST_IDL_SWAP_HPP_INCLUDED

#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>

namespace boost { namespace interfaces {   

template<typename Interface>
void swap(Interface& lhs, Interface& rhs)
{
    const void* pv = 
        access::get_interface_pointer(lhs);
    const detail::fn_ptr* table =
        access::get_interface_table(lhs);
    access::set_interface_pointer(
        lhs, access::get_interface_pointer(rhs)
    );
    access::set_interface_table(
        lhs, access::get_interface_table(rhs)
    );
    access::set_interface_pointer(rhs, pv);
    access::set_interface_table(rhs, table);
}

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_SWAP_HPP_INCLUDED
