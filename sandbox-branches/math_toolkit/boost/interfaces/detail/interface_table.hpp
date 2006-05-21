// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_INTERFACE_TABLE_HPP_INCLUDED
#define BOOST_IDL_INTERFACE_TABLE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <typeinfo>
#include <boost/checked_delete.hpp>
#include <boost/interfaces/access.hpp> // BOOST_IDL_PRIVATE.
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/interfaces/detail/preprocessor/befriend_template.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace interfaces { namespace detail {

//------------------Forward declarations--------------------------------------//

template<typename Table, typename X>
struct table_holder;

template<typename Interface>
struct interface_table_impl;

//------------------Definition of deleter and deleter_holder------------------//

typedef void (*deleter)(const void*);

template<typename X, typename D = checked_deleter<X> >
struct deleter_holder {
    static void delete_(const void* pv) 
    { D()(static_cast<X*>(const_cast<void*>(pv))); }
};

//------------------Definition of thrower and thrower_holder------------------//

typedef void (*thrower)(const void*);

template<typename X>
struct thrower_holder {
    static void throw_(const void* pv) 
    { 
        throw static_cast<X*>(const_cast<void*>(pv));
    }
};

//------------------Definition of typer and typer_holder----------------------//

typedef const std::type_info& (*typer)();

template<typename X>
struct typer_holder {
    static const std::type_info& typer() { return typeid(X); }
};

//------------------Definition of interface_table-----------------------------//

// The equivalent of a virtual function table.
template<typename Metadata>
class interface_table {
public:
    template<typename X>
    interface_table(boost::mpl::identity<X>)
    {
        Metadata::template initialize<X>(functions_ + offset::initial::value);
    }
BOOST_IDL_PRIVATE:
    BOOST_IDL_BEFRIEND_TEMPLATE_STRUCT(table_holder, 2, T)
    BOOST_IDL_BEFRIEND_TEMPLATE_STRUCT(interface_table_impl, 1, T)
    fn_ptr functions_[Metadata::size + offset::initial::value];
};

//------------------Definition of table_holder--------------------------------//

// Holds a static instance of an interface table.
template<typename Table, typename X>
struct table_holder {
BOOST_IDL_PRIVATE:
    BOOST_IDL_BEFRIEND_TEMPLATE_STRUCT(interface_table_impl, 1, T)
    static Table table_;
};

template<typename Table, typename X>
Table table_holder<Table, X>::table_ = 
    ::boost::mpl::identity<X>();

//------------------Definition of interface_table_impl------------------------//

template<typename Interface>
struct interface_table_impl {
    template<typename X>
    static const fn_ptr* initialize_table(const X& x)
    {
        typedef typename Interface::interface_metadata  metadata;
        typedef interface_table<metadata>               table;
        table_holder<table, X>::table_.functions_[0] = 0;
        table_holder<table, X>::table_.functions_[1] =
            reinterpret_cast<fn_ptr>(&thrower_holder<X>::throw_);
        table_holder<table, X>::table_.functions_[2] =
            reinterpret_cast<fn_ptr>(&typer_holder<X>::typer);
        return table_holder<table, X>::table_.functions_ + 
               offset::initial::value;
    }
    template<typename X>
    static const fn_ptr* initialize_deleter(const X& x)
    {
        typedef typename Interface::interface_metadata  metadata;
        typedef interface_table<metadata>               table;
        table_holder<table, X>::table_.functions_[0] =
            reinterpret_cast<fn_ptr>(&deleter_holder<X>::delete_);
        table_holder<table, X>::table_.functions_[1] =
            reinterpret_cast<fn_ptr>(&thrower_holder<X>::throw_);
        table_holder<table, X>::table_.functions_[2] =
            reinterpret_cast<fn_ptr>(&typer_holder<X>::typer);
        return table_holder<table, X>::table_.functions_ + 
               offset::initial::value;
    }
    static const deleter& get_deleter(Interface& i)
    { 
        fn_ptr* table = 
            const_cast<fn_ptr*>(access::get_interface_table(i));
        int offset = *reinterpret_cast<int*>(table);
        return *reinterpret_cast<deleter*>(table - offset);
    }
    static const deleter& get_thrower(Interface& i)
    { 
        fn_ptr* table = 
            const_cast<fn_ptr*>(access::get_interface_table(i));
        int offset = *reinterpret_cast<int*>(table);
        return *reinterpret_cast<thrower*>(table - offset + 1);
    }
    static const typer& get_typer(Interface& i)
    { 
        fn_ptr* table = 
            const_cast<fn_ptr*>(access::get_interface_table(i));
        int offset = *reinterpret_cast<int*>(table);
        return *reinterpret_cast<typer*>(table - offset + 2);
    }
};

//------------------Definition of initialize_table----------------------------//

// Returns a pointer to the first function in an interface table.
template<typename Interface, typename X>
const fn_ptr* initialize_table(const X& x)
{ 
    return interface_table_impl<Interface>::initialize_table(x);
}

//------------------Definition of initialize_deleter--------------------------//

// Returns a pointer to the first function in an interface table.
template<typename Interface, typename X>
const fn_ptr* initialize_deleter(const X& x)
{ 
    return interface_table_impl<Interface>::initialize_deleter(x);
}

//------------------Definition of get_deleter---------------------------------//

// Returns a pointer to the first function in an interface table.
template<typename Interface>
const deleter& get_deleter(Interface& i)
{ 
    return interface_table_impl<Interface>::get_deleter(i);
}

//------------------Definition of get_thrower---------------------------------//

// Returns a pointer to the second function in an interface table.
template<typename Interface>
const thrower& get_thrower(Interface& i)
{ 
    return interface_table_impl<Interface>::get_thrower(i);
}

//------------------Definition of get_typer-----------------------------------//

// Returns a pointer to the second function in an interface table.
template<typename Interface>
const typer& get_typer(Interface& i)
{ 
    return interface_table_impl<Interface>::get_typer(i);
}

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_INTERFACE_TABLE_HPP_INCLUDED
