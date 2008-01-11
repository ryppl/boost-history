//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_STATEMENT_BASE_HPP)
#define BOOST_SQL_CLI_DETAIL_STATEMENT_BASE_HPP

#include <memory>
#include <string>

#include <boost/sql_cli/detail/static_param.hpp>

namespace boost
{
namespace sql_cli
{
class statement;

namespace detail
{
class result_set_base;

class statement_base
{
public:
    virtual ~statement_base() {}
    virtual void prepare(std::string const & s) = 0;
    virtual void exec() = 0;
    virtual void exec(std::string const & s) = 0;

private:
    friend class sql_cli::statement;

    virtual std::auto_ptr<result_set_base> create_result_set() const = 0;

	virtual void bind_param(int n, static_param sp) = 0;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_STATEMENT_BASE_HPP)
