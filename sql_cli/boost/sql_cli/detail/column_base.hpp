//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_COLUMN_BASE_HPP)
#define BOOST_SQL_CLI_DETAIL_COLUMN_BASE_HPP

#include <string>
#include <typeinfo>

namespace boost
{
namespace sql_cli
{
namespace detail
{

class column_base
{
public:
    virtual ~column_base() {}
    virtual std::string name() const = 0;
    virtual const std::type_info & type() const = 0;
    virtual int size() const = 0;
    virtual int precision() const = 0;
    virtual bool nullable() const = 0;
    virtual int number() const = 0;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_COLUMN_BASE_HPP)
