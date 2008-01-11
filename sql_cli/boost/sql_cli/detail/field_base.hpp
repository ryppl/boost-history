//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_FIELD_BASE_HPP)
#define BOOST_SQL_CLI_DETAIL_FIELD_BASE_HPP

#include <boost/sql_cli/detail/static_value.hpp>

namespace boost
{
namespace sql_cli
{
class field;

namespace detail
{

class field_base
{
public:
    virtual ~field_base() {}
    virtual int number() const = 0;

private:
	friend class field;

	virtual void get(static_value & sv) const = 0;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_FIELD_BASE_HPP)
