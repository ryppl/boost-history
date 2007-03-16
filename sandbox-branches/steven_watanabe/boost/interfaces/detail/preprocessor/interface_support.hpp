// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_INTERFACE_SUPPORT_HPP_INCLUDED
#define BOOST_IDL_DETAIL_INTERFACE_SUPPORT_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/access.hpp> // BOOST_IDL_PRIVATE.
#include <boost/interfaces/detail/always_false.hpp>
#include <boost/interfaces/detail/fat_pointer.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/interfaces/detail/interface_table.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/interfaces/is_base_and_derived.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/remove_qualifiers.hpp>
#include <boost/mpl/or.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/utility/enable_if.hpp>

//------------------Definition of null_pointer_constant-----------------------//

namespace boost { namespace interfaces { namespace detail {

struct null_pointer_constant { 
    null_pointer_constant() { }
    null_pointer_constant(void*) { }
    template<typename T>
    null_pointer_constant(T*) 
    { BOOST_STATIC_ASSERT(always_false<T>::value); }
};

} } } // End namespace detail, interfaces, boost.

//------------------Definition of interface support macros--------------------//

#define BOOST_IDL_CTOR_DEFAULT(name) \
    name() : BOOST_IDL_MEMBER_PTR(0), BOOST_IDL_MEMBER_TABLE(0) { } \
    /**/

#define BOOST_IDL_CTOR_FROM_OBJECT(name, param) \
    template<typename param> \
    name( param& object_, \
          typename \
          ::boost::disable_if< \
              ::boost::interfaces::is_interface<param> \
          >::type* = 0 ) \
      : BOOST_IDL_MEMBER_PTR(&object_), \
        BOOST_IDL_MEMBER_TABLE( \
            ::boost::interfaces::detail::initialize_table<name>(object_) \
        ) \
        { \
        BOOST_STATIC_ASSERT(!::boost::is_const<param>::value); \
        } \
    /**/

#define BOOST_IDL_CTOR_FROM_INTERFACE(name, param, arity) \
    template<typename param> \
    name( const param& interface_, \
          typename \
          ::boost::enable_if< \
              ::boost::interfaces::is_base_and_derived< \
                   param, \
                   BOOST_PP_EXPR_IF(arity, typename) \
                   ::boost::interfaces::remove_qualifiers<name>::type \
              > \
          >::type* = 0 ) \
    { \
        ::boost::interfaces::detail::fat_pointer ptr(interface_); \
        BOOST_IDL_MEMBER_PTR = ptr.pv; \
        BOOST_IDL_MEMBER_TABLE = \
              ptr.table + \
              ::boost::interfaces::detail::offset_of< \
                  param, \
                  BOOST_PP_EXPR_IF(arity, typename) \
                  ::boost::interfaces::remove_qualifiers<name>::type \
              >::value; \
    } \
    /**/

#define BOOST_IDL_CTORS(name, param, arity) \
    BOOST_IDL_CTOR_DEFAULT(name) \
    BOOST_IDL_CTOR_FROM_OBJECT(name, param) \
    BOOST_IDL_CTOR_FROM_INTERFACE(name, param, arity) \
    /**/

#define BOOST_IDL_NULL_POINTER_SUPPORT(name, arity) \
    name& operator=(::boost::interfaces::detail::null_pointer_constant) \
    { this->reset(); return *this; } \
  private: \
    template<typename XXX_> \
    name& operator=(const XXX_* ptr); \
  public: \
    template<typename XXX_> \
    typename \
    ::boost::enable_if< \
        ::boost::interfaces::is_base_and_derived< \
            XXX_, \
            BOOST_PP_EXPR_IF(arity, typename) \
            ::boost::interfaces::remove_qualifiers<name>::type \
        >, \
        name& \
    >::type \
    operator=(const XXX_& interface_) \
    { \
        ::boost::interfaces::detail::fat_pointer ptr(interface_); \
        BOOST_IDL_MEMBER_PTR = ptr.pv; \
        BOOST_IDL_MEMBER_TABLE = \
              ptr.table + \
              ::boost::interfaces::detail::offset_of< \
                  XXX_, \
                  BOOST_PP_EXPR_IF(arity, typename) \
                  ::boost::interfaces::remove_qualifiers<name>::type \
              >::value; \
        return *this; \
    } \
    /**/

#define BOOST_IDL_RESET() void reset() \
{ \
    BOOST_IDL_MEMBER_PTR = 0; \
    BOOST_IDL_MEMBER_TABLE = 0; \
} \
/**/

#define BOOST_IDL_VOID_POINTER_CONVERSION() \
    operator const void*() const { return BOOST_IDL_MEMBER_TABLE; } \
    /**/

#define BOOST_IDL_MEMBER_ACCESS(name) \
    void*& get_pointer() const \
    { return const_cast<void*&>(BOOST_IDL_MEMBER_PTR); } \
    ::boost::interfaces::detail::fn_ptr*& get_table() const \
    { \
        using namespace ::boost::interfaces::detail; \
        return const_cast<fn_ptr*&>(BOOST_IDL_MEMBER_TABLE); \
    } \
    /**/

#define BOOST_IDL_MEMBER_DATA() \
    BOOST_IDL_PRIVATE: \
        const void*                                 BOOST_IDL_MEMBER_PTR; \
        const ::boost::interfaces::detail::fn_ptr*  BOOST_IDL_MEMBER_TABLE; \
    /**/

#define BOOST_IDL_CORE_INTERFACE_SUPPORT(name, arity) \
    BOOST_IDL_NULL_POINTER_SUPPORT(name, arity) \
    BOOST_IDL_RESET() \
    BOOST_IDL_VOID_POINTER_CONVERSION() \
    BOOST_IDL_MEMBER_DATA() \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_INTERFACE_SUPPORT_HPP_INCLUDED
