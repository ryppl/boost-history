// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_INTERFACE_ACCESS_HPP_INCLUDED
#define BOOST_IDL_INTERFACE_ACCESS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp> // BOOST_NO_MEMBER_TEMPLATE_FRIENDS.
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces_fwd.hpp> 
#include <boost/preprocessor/enum_params.hpp>

#ifndef BOOST_IDL_NO_ACCESS_CONTROL
# if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
#  define BOOST_IDL_NO_ACCESS_CONTROL
# endif
#endif

#ifndef BOOST_IDL_NO_ACCESS_CONTROL
# define BOOST_IDL_PRIVATE private
#else
# define BOOST_IDL_PRIVATE public
#endif

namespace boost { namespace interfaces { 

class access {
#ifndef BOOST_IDL_NO_ACCESS_CONTROL
    template<typename Subinterface, typename Superinterfaces>
    friend class subinterface;

    template<typename Interface>
    friend class const_view;

    template<typename Interface>
    friend class fixed_view;

    template<typename Interface>
    friend class unique_ptr;

    template<typename Interface>
    friend class shared_ptr;

    template<typename Interface>
    friend class manual_ptr;

    template<typename Interface>
    friend class unique_obj;

    template<typename Interface>
    friend class shared_obj;

    template<typename T, typename Interface>
    friend struct implements;

    template<typename InterfaceImpl, int Lines>
    friend struct interface_base;

    template<typename Interface, typename AccessTag, typename Signature, int Arity>
    friend struct detail::null_invoker;

    template< typename Interface, typename AccessTag, typename Signature, 
            typename Names, typename Advice, int Arity >
    friend struct detail::basic_invoker;

    template< typename Interface, typename AccessTag, typename Signature, 
            typename Names, typename Advice, int Arity >
    friend struct detail::transforming_invoker;

    template<typename Interface>
    friend struct detail::interface_table_impl;

    template<typename T>
    friend class detail::unique_obj_impl;

    template<typename T>
    friend class detail::shared_obj_impl;

    template<typename Interface>
    friend void swap(Interface&, Interface&);

    template<typename Interface, typename Delegator>
    friend fixed_view<Interface>* get_delegate(Delegator& d);

    template<typename Interface, typename Delegator, typename T>
    friend void set_delegate(Delegator& d, T* t);

    template<typename X, typename Interface>
    friend X& extract(const Interface& i);

    template<typename Interface>
    friend const std::type_info& target_type(const Interface& i);
#else
    public:
#endif
    template<typename T>
    static const void* get_interface_pointer(const T& t)
    { return t.BOOST_IDL_MEMBER_PTR; }

    template<typename T>
    static const detail::fn_ptr* get_interface_table(const T& t)
    { return t.BOOST_IDL_MEMBER_TABLE; }

    template<typename T>
    static void set_interface_pointer(T& t, const void* pv)
    { 
        const_cast<void*&>(t.BOOST_IDL_MEMBER_PTR) = 
            const_cast<void*>(pv);
    }

    template<typename T>
    static void set_interface_table(T& t, const detail::fn_ptr* table)
    { 
        const_cast<detail::fn_ptr*&>(t.BOOST_IDL_MEMBER_TABLE) = 
            const_cast<detail::fn_ptr*>(table);
    }

    template<typename Interface, typename Delegator>
    static fixed_view<Interface>* 
    get_delegate(Delegator& d) 
    { return d.delegating<Interface>::get(); }

    template<typename Interface, typename Delegator, typename T>
    static void set_delegate(Delegator& d, T* t) 
    { d.delegating<Interface>::set(t); }
};

} } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_INTERFACE_ACCESS_HPP_INCLUDED
