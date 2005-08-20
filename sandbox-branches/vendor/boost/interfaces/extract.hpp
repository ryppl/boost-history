// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_EXTRACT_HPP_INCLUDED
#define BOOST_IDL_EXTRACT_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <typeinfo> // bad_cast.
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces_fwd.hpp>
//#include <boost/static_assert.hpp>
//#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace interfaces {

class bad_extract : public std::bad_cast {
public:
    const char* what() const throw()
    {
        return "boost::interfaces::bad_extract: failed extraction";
    }
};

template<typename X, typename Interface>
X& extract(const Interface& i)
{
    typedef typename remove_const<X>::type base_type;
    // BOOST_STATIC_ASSERT( is_const<X>::value || 
    //                      !(Interface::interface_flags & flags::is_const) );
    try {
        if (!i) throw bad_extract();
        detail::thrower throw_ = detail::get_thrower(i);
        throw_(access::get_interface_pointer(i));
        throw 0; // Unreachable.
    } catch (base_type* x) {
        return *x;
    } catch (void*) {
        throw bad_extract();
    }
}

template<typename Interface>
const std::type_info& target_type(const Interface& i)
{
    if (!i) throw bad_extract();
    detail::typer typer = detail::get_typer(i);
    return typer();
}

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_EXTRACT_HPP_INCLUDED
