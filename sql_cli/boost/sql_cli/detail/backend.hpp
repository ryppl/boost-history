//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_BACKEND_HPP)
#define BOOST_SQL_CLI_DETAIL_BACKEND_HPP

#include <cassert>
#include <map>
#include <memory>
#include <string>

#include <boost/sql_cli/detail/config.hpp>
#include <boost/sql_cli/detail/backend_traits.hpp>

// this must occur after all of the includes and before any code appears:
#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost
{
namespace sql_cli
{
namespace detail
{
class connection_base;

class BOOST_SQL_CLI_DECL backend
{
public:
	static std::auto_ptr<connection_base> create_connection(std::string const & dbms);

protected:
	backend(std::string const & dbms);

private:
    virtual std::auto_ptr<connection_base> create_connection() const = 0;

	static std::map<std::string, backend *> backends;
};

template <typename ServerT>
class backend_helper : public backend
{
public:
    typedef ServerT server_type;
    typedef backend_traits<server_type> traits_type;

	backend_helper(std::string const & dbms) : backend(dbms) {}

    std::auto_ptr<connection_base> create_connection() const
    {
        std::auto_ptr<connection_base> c (
                new typename traits_type::connection_type);
        return c;
    }
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

// the suffix header occurs after all of our code:
#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // ! defined(BOOST_SQL_CLI_DETAIL_BACKEND_HPP)
