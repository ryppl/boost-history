//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_CONNECTION_HPP)
#define BOOST_SQL_CLI_CONNECTION_HPP

#include <memory>
#include <string>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/sql_cli/detail/backend.hpp>
#include <boost/sql_cli/detail/connection_base.hpp>
#include <boost/sql_cli/detail/statement_base.hpp>

namespace boost
{
namespace sql_cli
{

//! @class connection connection.hpp boost/sql_cli/connection.hpp
//! @brief Represents a %connection to a database of a specific backend type.
//! Different connection instances may be used in different threads without
//! explicit synchronization.
//! @nosubgrouping

class connection
{
public:
    //! @name Constructors
    //@{
    //! Initializes the backend specified in the 'dbms' argument.
    connection(std::string const & dbms) { init(dbms); }
    //! @brief Initializes the backend and opens the 'srv' database, with
    //! user 'user' and password 'auth'.
    connection(std::string const & dbms, std::string const & srv, 
            std::string const & user, std::string const & auth)
    {
        init(dbms);
        connect(srv, user, auth);
    }
    //@}

    //! @name Connection management
    //@{
    //! Opens the 'srv' database, with user 'user' and password 'auth'.
    void connect(std::string const & srv, std::string const & user, 
            std::string const & auth)
    {
        conn->connect(srv, user, auth);
    }

    //! Closes the %connection to the database.
    void disconnect() { conn->disconnect(); }
    //@}

    //! @name Transaction management
    //@{
    //! Commits the current transaction.
    void commit() { conn->commit(); }
    //! Rolls back the current transaction.
    void rollback() { conn->rollback(); }
    //@}

private:
    friend class statement;

    void init(std::string const & dbms) 
    {
		conn.reset(detail::backend::create_connection(dbms).release()); 
    }

    std::auto_ptr<detail::statement_base> create_statement() const
    { 
        return conn->create_statement(); 
    }

    boost::scoped_ptr<detail::connection_base> conn;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_CONNECTION_HPP)
