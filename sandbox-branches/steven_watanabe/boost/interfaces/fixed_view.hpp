// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_FIXED_VIEW_HPP_INCLUDED
#define BOOST_IDL_FIXED_VIEW_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <algorithm>
#include <boost/interfaces/access.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/interfaces/remove_qualifiers.hpp>
#include <boost/mpl/bool.hpp> // mpl::true_.

namespace boost { namespace interfaces {

template<typename Interface>
class fixed_view 
    : public Interface::interface_generator::template 
             apply<
                 fixed_view<Interface>, 
                 mpl::int_<1>,
                 mpl::int_<flags::is_fixed>
             >::type
{
public:
    friend class access;
    typedef typename Interface::interface_generator  interface_generator;
    BOOST_IDL_CTOR_DEFAULT(fixed_view)
    fixed_view(const Interface& i)
        : BOOST_IDL_MEMBER_PTR(access::get_interface_pointer(i)), 
          BOOST_IDL_MEMBER_TABLE(access::get_interface_table(i))
        { } 
    template<typename Subinterface>
    fixed_view(const Subinterface& i)
        : BOOST_IDL_MEMBER_PTR(access::get_interface_pointer(i)), 
          BOOST_IDL_MEMBER_TABLE(
              access::get_interface_table(i) +
              detail::offset_of<Subinterface, Interface>::value
          )
        { } 
    BOOST_IDL_CTOR_FROM_OBJECT(fixed_view, T)
    BOOST_IDL_RESET()
    BOOST_IDL_VOID_POINTER_CONVERSION()
protected:
    template<typename T>
    void assign_object(T& t)
    {
        BOOST_IDL_MEMBER_PTR = &t;
        BOOST_IDL_MEMBER_TABLE = detail::initialize_table<Interface>(t);
    }
    template<typename Subinterface>
    void assign_interface(const Subinterface& i)
    {
        BOOST_IDL_MEMBER_PTR = access::get_interface_pointer(i);
        BOOST_IDL_MEMBER_TABLE = 
            access::get_interface_table(i) +
            detail::offset_of<Subinterface, Interface>::value;
    }
    void swap(fixed_view& fixed)
    {
        std::swap(BOOST_IDL_MEMBER_PTR , access::get_interface_pointer(fixed));
        std::swap(BOOST_IDL_MEMBER_TABLE, access::get_interface_table(fixed));
    }
private:
    fixed_view& operator=(const fixed_view&);
    template<typename T>
    fixed_view& operator=(T&);
    BOOST_IDL_MEMBER_DATA()
};

template<typename Interface>
struct is_interface< fixed_view<Interface> >
    : mpl::true_
    { };

template<typename Interface>
struct remove_qualifiers< fixed_view<Interface> >
    : remove_qualifiers<Interface>
    { };


} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_FIXED_VIEW_HPP_INCLUDED
