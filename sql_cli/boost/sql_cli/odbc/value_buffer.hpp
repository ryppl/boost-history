//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_VALUE_BUFFER_HPP)
#define BOOST_SQL_CLI_ODBC_VALUE_BUFFER_HPP

#include <string>
#include <vector>

#include <boost/none.hpp>
#include <boost/optional.hpp>

#include <boost/sql_cli/error.hpp>
#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/odbc_column.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

template <typename ValueT> 
class value_buffer_base
{
public:
    operator SQLPOINTER() { return (SQLPOINTER)(&val); }
    SQLUINTEGER size() const { return sizeof(ValueT); }
    
protected:
    value_buffer_base(column const * = 0) {}

    const ValueT & get() const { return val; }

private:
    ValueT val;
};

template <> 
class value_buffer_base<std::string>
{
public:
    operator SQLPOINTER() { return (SQLPOINTER)(&vec[0]); }
    SQLUINTEGER size() const { return static_cast<SQLUINTEGER>(vec.size()); }
    
protected:
    value_buffer_base(column const * c) : vec(c->size()) {}

    std::string get() const { return std::string(&vec[0]); }

private:
    std::vector<char> vec;
};

template <> 
class value_buffer_base<std::wstring>
{
public:
    operator SQLPOINTER() { return (SQLPOINTER)(&vec[0]); }
    SQLUINTEGER size() const { return static_cast<SQLUINTEGER>(vec.size()); }
    
protected:
    value_buffer_base(column const * c) : vec(c->size()) {}

    std::wstring get() const { return std::wstring(&vec[0]); }

private:
    std::vector<wchar_t> vec;
};

template <typename ValueT> 
class null_value_buffer : public value_buffer_base<ValueT>
{
public:
    null_value_buffer(column const * c = 0) : 
            value_buffer_base<ValueT>(c), is_null(false)
    {
    }

    void null() { is_null = true; }

protected:
    bool is_null;
};

template <typename ValueT> 
class value_buffer : public null_value_buffer<ValueT>
{
public:
    value_buffer(column const * c = 0) : 
            null_value_buffer<ValueT>(c)
    {
    }

    operator ValueT() 
    { 
        if ( this->is_null )
            throw null_data();
        return this->get(); 
    }
};

template <typename ValueT> 
class value_buffer<boost::optional<ValueT> > : public null_value_buffer<ValueT>
{
public:
    value_buffer(column const * c = 0) : 
            null_value_buffer<ValueT>(c)
    {
    }

    operator boost::optional<ValueT>() 
    { 
        return this->is_null ? boost::optional<ValueT>() : 
            boost::optional<ValueT>(this->get()); 
    }
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_VALUE_BUFFER_HPP)
