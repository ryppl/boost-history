// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_SMART_PTR_SUPPORT_HPP_INCLUDED
#define BOOST_IDL_DETAIL_SMART_PTR_SUPPORT_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <algorithm> // swap.
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/fixed_view.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>

#define BOOST_IDL_SMART_PTR_CTOR_FROM_PTR(name, interface) \
    template<typename T> \
    explicit name(T* t) \
        : BOOST_IDL_MEMBER_PTR(t), \
          BOOST_IDL_MEMBER_TABLE(detail::initialize_deleter<interface>(*t)) \
        { } \
    /**/

#define BOOST_IDL_SMART_PTR_DEREF(name) \
    element_type* get() const \
    { \
        return BOOST_IDL_MEMBER_PTR ? \
            reinterpret_cast<element_type*>( \
                const_cast<name*>(this) \
            ) : \
            0; \
    } \
    element_type& operator*() const \
    { \
        BOOST_ASSERT(BOOST_IDL_MEMBER_PTR); \
        return *reinterpret_cast<element_type*>( \
            const_cast<name*>(this) \
        ); \
    } \
    element_type* operator->() const { return get(); } \
    /**/

#define BOOST_IDL_SMART_PTR_SWAP(name) \
    void swap(name& p) \
    { \
        std::swap(BOOST_IDL_MEMBER_PTR, p.BOOST_IDL_MEMBER_PTR); \
        std::swap(BOOST_IDL_MEMBER_TABLE, p.BOOST_IDL_MEMBER_TABLE); \
    } \
    /**/

#define BOOST_IDL_CORE_SMART_PTR_SUPPORT(name, interface) \
    BOOST_IDL_PRIVATE: \
        friend class access; \
        BOOST_IDL_BEFRIEND_TEMPLATE_CLASS(name, 1, interface) \
        BOOST_IDL_MEMBER_ACCESS(name) \
        BOOST_IDL_MEMBER_DATA() \
    public: \
        typedef ::boost::interfaces::fixed_view<Interface> element_type; \
        BOOST_IDL_SMART_PTR_DEREF(name) \
        BOOST_IDL_VOID_POINTER_CONVERSION() \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_SMART_PTR_SUPPORT_HPP_INCLUDED
