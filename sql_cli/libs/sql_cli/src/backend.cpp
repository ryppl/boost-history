//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#define BOOST_SQL_CLI_SOURCE

#include <boost/sql_cli/detail/backend.hpp>

#include <boost/sql_cli/odbc/odbc_connection.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{

std::map<std::string, backend *> backend::backends;

// Backend How-To: Define a namespace level instance of your backend's
// 'backend_helper' specialization and initialize it with a unique 
// identification string.
// Should the default definition of 'backend_helper' be insufficient for some
// reason, consider providing your own custom specialization here.

backend_helper<odbc_tag> odbc("ODBC");

backend::backend(std::string const & dbms)
{
	assert(backends.find(dbms) == backends.end());
	backends.insert(make_pair(dbms, this));
}

std::auto_ptr<connection_base> backend::create_connection(std::string const & dbms)
{
	std::map<std::string, backend *>::const_iterator i = backends.find(dbms);
	assert(i != backends.end());
	return i->second->create_connection();
}

} // namespace detail
} // namespace sql_cli
} // namespace boost
