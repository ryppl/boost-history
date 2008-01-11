//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_CONNECTION_HPP)
#define BOOST_SQL_CLI_ODBC_CONNECTION_HPP

#include <memory>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/signals/connection.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/sql_cli/detail/connection_base.hpp>
#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/odbc_handle.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{
class statement_base;
} // namespace detail

namespace odbc
{
class environment;

class connection : public boost::sql_cli::detail::connection_base,
        public handle<SQLHDBC, SQL_HANDLE_DBC>
{
public:
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
    typedef boost::signal0<void> on_disconnect_t;
#else
    typedef boost::signal<void()> on_disconnect_t;
#endif

    connection();
    ~connection();

    void connect(std::string const & srv, std::string const & user, 
            std::string const & auth);
    void disconnect();

    boost::signals::connection on_disconnect (
                on_disconnect_t::slot_type const & s)
    {
        return disc_sig.connect(s);
    }

    void commit();
    void rollback();

private:
    std::auto_ptr<boost::sql_cli::detail::statement_base> create_statement();

    boost::shared_ptr<environment> my_env;
    static boost::weak_ptr<environment> the_env;

    on_disconnect_t disc_sig;
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_CONNECTION_HPP)
