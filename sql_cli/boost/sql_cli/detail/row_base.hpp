//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_ROW_BASE_HPP)
#define BOOST_SQL_CLI_DETAIL_ROW_BASE_HPP

#include <boost/shared_ptr.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{
class field_base;

class row_base
{
public:
    virtual ~row_base() {}
    virtual boost::shared_ptr<field_base> get_field(int i) = 0;
    virtual boost::shared_ptr<field_base> get_field(std::string const & n) = 0;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_ROW_BASE_HPP)
