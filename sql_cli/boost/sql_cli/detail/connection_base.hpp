//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_CONNECTION_BASE_HPP)
#define BOOST_SQL_CLI_DETAIL_CONNECTION_BASE_HPP

#include <string>
#include <memory>

namespace boost
{
namespace sql_cli
{
class connection;

namespace detail
{
class statement_base;

class connection_base
{
public:
    virtual ~connection_base() {}

    virtual void connect(std::string const & srv, std::string const & user, 
            std::string const & auth) = 0;
    virtual void disconnect() = 0;

    virtual void commit() = 0;
    virtual void rollback() = 0;

private:
    friend class sql_cli::connection;

    virtual std::auto_ptr<statement_base> create_statement() = 0;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_CONNECTION_BASE_HPP)
