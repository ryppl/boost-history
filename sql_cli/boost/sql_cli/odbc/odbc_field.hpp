//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_FIELD_HPP)
#define BOOST_SQL_CLI_ODBC_FIELD_HPP

#include <memory>

#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/traits.hpp>
#include <boost/sql_cli/odbc/value_buffer.hpp>
#include <boost/sql_cli/odbc/odbc_column.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>
#include <boost/sql_cli/odbc/odbc_statement.hpp>
#include <boost/sql_cli/detail/field_base.hpp>
#include <boost/sql_cli/detail/static_value.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

class field : public sql_cli::detail::field_base
{
public:
    field(statement const & s, boost::shared_ptr<column> const & c) : 
            stmt(&s), col(c), trunc(false)
    {
    }

    int number() const { return col->number(); }
    std::string name() const { return col->name(); }

    template <typename ValueT> 
    ValueT get() const;

    bool truncated() const { return trunc; }

private:
	void get(detail::static_value & sv) const
	{
		sv.set_value(*this);
	}

	statement const * stmt;
    boost::shared_ptr<column> col;
    mutable bool trunc;
};


// Null values are either retrieved by means of a Boost.Optional or result in a 
// sql_cli::null_value exception being thrown.
// Currently data should never be truncated because strings are retrieved as
// properly sized std::vector<char>, although it may happen due to platform
// limits.
// TODO: Cache the value; a subsequent call to SQLGetData will return 
// SQL_NO_DATA

template <typename ValueT> 
ValueT field::get() const
{
    typedef traits<ValueT> traits_type;

    stmt->conn_check();
    value_buffer<ValueT> v(col.get());
    SQLINTEGER soi;

    SQLRETURN ret = SQLGetData(stmt->get_handle(), 
            static_cast<SQLUSMALLINT>(col->number()), 
            traits_type::value_type_id, v, v.size(), &soi);  
    
    state code = stmt->check_return(ret);
    if ( ret == SQL_SUCCESS_WITH_INFO )
    {
        trunc = sql_cli::odbc::string_truncated(code) || 
                sql_cli::odbc::fraction_truncated(code);
    }
    if ( soi == SQL_NULL_DATA )
        v.null();
    return v;
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_FIELD_HPP)
