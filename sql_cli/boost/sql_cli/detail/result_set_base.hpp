//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_RESULT_SET_BASE_HPP)
#define BOOST_SQL_CLI_DETAIL_RESULT_SET_BASE_HPP

#include <memory>
#include <string>

#include <boost/shared_ptr.hpp>

namespace boost
{
namespace sql_cli
{
class result_set;

namespace detail
{
class column_base;
class row_base;
class result_set_iterator;

class result_set_base
{
public:
    virtual ~result_set_base() {}
    virtual int column_count() const = 0;

private:
    friend class sql_cli::result_set;
    friend class result_set_iterator;

    virtual boost::shared_ptr<column_base> get_column(int n) = 0;
    virtual boost::shared_ptr<column_base> 
            get_column(std::string const & n) = 0;
    virtual std::auto_ptr<detail::row_base> create_row() = 0;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_RESULT_SET_BASE_HPP)
