//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple facility that mimmic notion of read-only read-write 
//  properties in C++ classes. Original idea by Henrik Ravn.
// ***************************************************************************

#ifndef BOOST_TEST_CLASS_PROPERTIES_HPP
#define BOOST_TEST_CLASS_PROPERTIES_HPP

// BOOST
#include <boost/preprocessor/repetition/repeat.hpp> 
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/utility/addressof.hpp>

// STL
#include <iosfwd>

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                 class_property               ************** //
// ************************************************************************** //

template<class PropertyType>
class class_property
{
protected:
    typedef typename call_traits<PropertyType>::const_reference read_access_t;
    typedef typename call_traits<PropertyType>::param_type      write_param_t;
    typedef typename add_pointer<PropertyType const>::type      address_res_t;

public:
    // Constructor
                    class_property() : value( PropertyType() ) {}
    explicit        class_property( write_param_t init_value ) : value( init_value ) {}

    // Access methods
    operator        read_access_t() const   { return value; }
    read_access_t   get() const             { return value; }
    bool            operator!() const       { return !value; }
    address_res_t   operator&() const       { return &value; }

    // Data members

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x570)
    // Borland can't deal with the using declaration in derived classes
#else
protected:
#endif
    PropertyType        value;
};

//____________________________________________________________________________//

template<typename CharT1, typename Tr,class PropertyType>
inline std::basic_ostream<CharT1,Tr>&
operator<<( std::basic_ostream<CharT1,Tr>& os, class_property<PropertyType> const& p )
{
    return os << p.get();
}

//____________________________________________________________________________//

#define DEFINE_PROPERTY_FREE_BINARY_OPERATOR( op )                              \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( PropertyType const& lhs, class_property<PropertyType> const& rhs ) \
{                                                                               \
    return lhs op rhs.get();                                                    \
}                                                                               \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( class_property<PropertyType> const& lhs, PropertyType const& rhs ) \
{                                                                               \
    return lhs.get() op rhs;                                                    \
}                                                                               \
template<class PropertyType>                                                    \
inline bool                                                                     \
operator op( class_property<PropertyType> const& lhs,                           \
             class_property<PropertyType> const& rhs )                          \
{                                                                               \
    return lhs.get() op rhs.get();                                              \
}                                                                               \
/**/

DEFINE_PROPERTY_FREE_BINARY_OPERATOR( == )
DEFINE_PROPERTY_FREE_BINARY_OPERATOR( != )

#undef DEFINE_PROPERTY_FREE_BINARY_OPERATOR

// ************************************************************************** //
// **************               readonly_property              ************** //
// ************************************************************************** //

template<class PropertyType>
class readonly_property : public class_property<PropertyType>
{
    typedef class_property<PropertyType>    base;
    typedef typename base::address_res_t    arrow_res_t;
protected:
    typedef typename base::write_param_t    write_param_t;
public:
    // Constructor
                    readonly_property() {}
    explicit        readonly_property( write_param_t init_value ) : base( init_value ) {}

    // access methods
    arrow_res_t     operator->() const      { return boost::addressof( base::value ); }
};

//____________________________________________________________________________//

#define DECLARE_FRIEND(r, data, elem) friend class elem;

#define BOOST_READONLY_PROPERTY( property_type, friends )                           \
class BOOST_JOIN( readonly_property, __LINE__ )                                     \
: public boost::unit_test::readonly_property<property_type >                        \
{                                                                                   \
    typedef boost::unit_test::readonly_property<property_type > base;               \
    BOOST_PP_SEQ_FOR_EACH( DECLARE_FRIEND, '', friends )                            \
    typedef base::write_param_t  write_param_t;                                     \
public:                                                                             \
                BOOST_JOIN( readonly_property, __LINE__ )() {}                      \
    explicit    BOOST_JOIN( readonly_property, __LINE__ )( write_param_t init_v  )  \
    : base( init_v ) {}                                                             \
}                                                                                   \
/**/

// ************************************************************************** //
// **************              readwrite_property              ************** //
// ************************************************************************** //

template<class PropertyType>
class readwrite_property : public class_property<PropertyType>
{
    typedef class_property<PropertyType>                base;
    typedef typename add_pointer<PropertyType>::type    arrow_res_t;
    typedef typename base::address_res_t                const_arrow_res_t;
    typedef typename base::write_param_t                write_param_t;
public:
                    readwrite_property() : base() {}
    explicit        readwrite_property( write_param_t init_value ) : base( init_value ) {}

    // access methods
    void            set( write_param_t v )  { value = v; }
    arrow_res_t     operator->()            { return boost::addressof( base::value ); }
    const_arrow_res_t operator->() const    { return boost::addressof( base::value ); }

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x570)
#else
    using           base::value;
#endif
};

//____________________________________________________________________________//

} // unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.15  2004/05/18 13:10:49  dgregor
//  class_properties.hpp: Borland C++ does not handle using declarations for data members properly; fixed the existing Borland workaround.
//
//  Revision 1.14  2004/05/11 11:00:53  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//
//  Revision 1.13  2003/12/01 00:41:56  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

#endif // BOOST_TEST_CLASS_PROPERTIES_HPP
