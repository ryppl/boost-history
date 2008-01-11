//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_TRAITS_HPP)
#define BOOST_SQL_CLI_ODBC_TRAITS_HPP

#include <string>

#include <boost/optional.hpp>

#include <boost/sql_cli/null_value.hpp>
#include <boost/sql_cli/odbc/odbc_config.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

template <SQLSMALLINT valueTypeId, SQLSMALLINT paramTypeId, 
        SQLINTEGER strlenOrIndptr> 
struct traits_base
{
    static SQLSMALLINT const value_type_id = valueTypeId;
    static SQLSMALLINT const param_type_id = paramTypeId;
    static SQLINTEGER const strlen_or_indptr = strlenOrIndptr;
};

template <typename ValueT> 
struct traits : traits_base<0, 0, 0>
{
};

template <> 
struct traits<signed char> : traits_base<SQL_C_STINYINT, SQL_TINYINT, SQL_NTS>
{
};

template <> 
struct traits<unsigned char> : traits_base<SQL_C_UTINYINT, SQL_TINYINT, SQL_NTS>
{
};

template <> 
struct traits<short> : traits_base<SQL_C_SSHORT, SQL_SMALLINT, SQL_NTS>
{
};

template <>
struct traits<unsigned short> : traits_base<SQL_C_USHORT, SQL_SMALLINT, SQL_NTS>
{
};

template <> 
struct traits<long> : traits_base<SQL_C_SLONG, SQL_INTEGER, SQL_NTS>
{
};

template <>
struct traits<unsigned long> : traits_base<SQL_C_ULONG, SQL_INTEGER, SQL_NTS>
{
};

template <>
struct traits<long long> : traits_base<SQL_C_SBIGINT, SQL_BIGINT, SQL_NTS>
{
};

template <>
struct traits<unsigned long long> :
        traits_base<SQL_C_UBIGINT, SQL_BIGINT, SQL_NTS>
{
};

template <>
struct traits<float> : traits_base<SQL_C_FLOAT, SQL_REAL, SQL_NTS>
{
};

template <>
struct traits<double> : traits_base<SQL_C_DOUBLE, SQL_DOUBLE, SQL_NTS>
{
};

template <> 
struct traits<std::string> : traits_base<SQL_C_CHAR, SQL_VARCHAR, SQL_NTS>
{
};

template <> 
struct traits<std::wstring> : traits_base<SQL_C_WCHAR, SQL_WVARCHAR, SQL_NTS>
{
};

template <typename ValueT> 
struct traits<sql_cli::null_value<ValueT> > : traits<ValueT>
{
};

template <typename ValueT> 
struct traits<boost::optional<ValueT> > : traits<ValueT>
{
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_TRAITS_HPP)
