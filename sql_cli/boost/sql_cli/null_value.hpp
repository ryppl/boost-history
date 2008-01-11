//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_NULL_VALUE_HPP)
#define BOOST_SQL_CLI_NULL_VALUE_HPP

namespace boost
{
namespace sql_cli
{

//! @class null_value null_value.hpp boost/sql_cli/null_value.hpp
//! Represents a typed null value.
//! It is typically used for input parameters that are always null.

template <typename ValueT>
struct null_value
{
    typedef ValueT type;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_NULL_VALUE_HPP)
