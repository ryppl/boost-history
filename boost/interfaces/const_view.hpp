// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_CONST_VIEW_PP_INCLUDED
#define BOOST_IDL_CONST_VIEW_PP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/access.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/befriend_template.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/interfaces/remove_qualifiers.hpp>
#include <boost/mpl/bool.hpp> // mpl::true_.

namespace boost { namespace interfaces {

template<typename Interface>
class const_view 
    : public Interface::interface_generator::template 
             apply<
                 const_view<Interface>, 
                 mpl::int_<1>,
                 mpl::int_<flags::is_const>
             >::type
{
public:
    friend class access;
    typedef typename Interface::interface_generator  interface_generator;
    BOOST_IDL_CTOR_DEFAULT(const_view)
    const_view(const Interface& i)
        : BOOST_IDL_MEMBER_PTR(access::get_interface_pointer(i)), 
          BOOST_IDL_MEMBER_TABLE(access::get_interface_table(i))
        { } 
    template<typename Subinterface>
    const_view( const Subinterface& i,
                typename 
                enable_if<
                    is_base_and_derived<Subinterface, Interface>
                >::type* = 0 )
      : BOOST_IDL_MEMBER_PTR(access::get_interface_pointer(i)),
        BOOST_IDL_MEMBER_TABLE(
            access::get_interface_table(i) +
            detail::offset_of<Subinterface, Interface>::value
        )
        { } 
    template<typename T>
    const_view( const T& t, 
                typename 
                disable_if< 
                    is_interface<T> 
                >::type* = 0)
        : BOOST_IDL_MEMBER_PTR(&t), 
          BOOST_IDL_MEMBER_TABLE(detail::initialize_table<const_view>(t)) 
        { }
    BOOST_IDL_CTOR_FROM_OBJECT(const_view, T)
    BOOST_IDL_CORE_INTERFACE_SUPPORT(const_view, 1)
};

template<typename Interface>
struct is_interface< const_view<Interface> >
    : mpl::true_
    { };

template<typename Interface>
struct remove_qualifiers< const_view<Interface> >
    : remove_qualifiers<Interface>
    { };

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_CONST_VIEW_PP_INCLUDED
