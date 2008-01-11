//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_STATEMENT_HPP)
#define BOOST_SQL_CLI_ODBC_STATEMENT_HPP

#include <memory>
#include <string>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/odbc_handle.hpp>
#include <boost/sql_cli/odbc/odbc_param.hpp>
#include <boost/sql_cli/odbc/traits.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>
#include <boost/sql_cli/detail/statement_base.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{
class result_set_base;
} // namespace detail

namespace odbc
{
class connection;

class statement : public sql_cli::detail::statement_base,
        public handle<SQLHSTMT, SQL_HANDLE_STMT>
{
public:
    statement(connection & c);
    ~statement();

    void conn_check() const;

    void prepare(std::string const & s);
    void exec();
    void exec(std::string const & s);

    int param_count() const;
    template <typename ValueT> 
    void bind_param(int n, ValueT & v);

private:
    typedef std::vector<boost::shared_ptr<param> > params_type;

    void disconnect() { connected = false; }

    std::auto_ptr<sql_cli::detail::result_set_base> create_result_set() const;

	void bind_param(int n, detail::static_param sp) { sp.bind_param(n, *this); }

	void do_bind_param(boost::shared_ptr<param> const & par);

    boost::signals::scoped_connection disc_slot;
    bool connected;

    params_type params;
};

template <typename ValueT> 
void statement::bind_param(int n, ValueT & v)
{
    unsigned short usn = boost::numeric_cast<unsigned short>(n);
    boost::shared_ptr<param> p(new typed_param<ValueT>(v, usn));
    params[n-1] = p;
    do_bind_param(p);
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_STATEMENT_HPP)
