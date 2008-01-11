//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <boost/bind.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/sql_cli/odbc/odbc_connection.hpp>
#include <boost/sql_cli/odbc/odbc_result_set.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, <= 0x564)
#   pragma hdrstop
#endif

#include <boost/sql_cli/odbc/odbc_statement.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
#   pragma warn -8080
#endif

namespace boost
{
namespace sql_cli
{
namespace odbc
{

void statement::conn_check() const
{
    if ( ! connected )
        throw error("Connection unavailable");
}

statement::statement(connection & c) : connected(false)
{
    SQLRETURN ret = SQLAllocHandle(kind, c.get_handle(), &get_handle());
    if ( ret != SQL_SUCCESS )
        throw error(*this, ret);
    disc_slot = c.on_disconnect(boost::bind(&statement::disconnect, this));
    connected = true;
}

statement::~statement()
{
    if ( connected )
        SQLFreeHandle(kind, get_handle());
}

void statement::prepare(std::string const & s)
{
    conn_check();
    SQLRETURN ret = SQLPrepare(get_handle(), (SQLCHAR *)s.c_str(), SQL_NTS);

    switch(ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        {
            params.clear();
            params.resize(param_count());
        }
        break;
    default:
        throw error(*this, ret);
    }
}


#if BOOST_WORKAROUND(__BORLANDC__, >= 0x582)
#   pragma option push -w-8091
#endif

void statement::exec()
{
    conn_check();
    for ( params_type::iterator i = params.begin(); i != params.end(); ++i )
    {
        if ( (*i)->update() )
            do_bind_param(*i);
    }
    SQLRETURN ret = SQLExecute(get_handle());

/*
    while ( ret == SQL_NEED_DATA )
    {
        SQLPOINTER p;
        ret = SQLParamData(get_handle(), &p);
        SQLSMALLINT n;
        n = reinterpret_cast<SQLSMALLINT>(p) - 1;
        assert(static_cast<std::size_t>(n) <= soi.size());
        std::vector<soi_handler>::iterator i = soi.begin() + n;
        {
            if ( i->get )
            {

                ret = SQLPutData(get_handle(), i->get->ptr(),
                        i->get->soi());
                if ( ret != SQL_SUCCESS && ret != SQL_NEED_DATA )
                    throw error(*this, ret);
            }
        }
    }
*/
    switch(ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        break;
    default:
        throw error(*this, ret);
    }
}

#if BOOST_WORKAROUND(__BORLANDC__, >= 0x582)
#   pragma option pop
#endif


void statement::exec(std::string const & s)
{
    conn_check();
    SQLRETURN ret = SQLExecDirect(get_handle(), (SQLCHAR *)s.c_str(), SQL_NTS);

    switch(ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        break;
    default:
        throw error(*this, ret);
    }
}

int statement::param_count() const
{
    conn_check();
    SQLSMALLINT n;
    SQLRETURN ret = SQLNumParams(get_handle(), &n);

    switch(ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        return n;
    default:
        throw error(*this, ret);
    }
}

void statement::do_bind_param(boost::shared_ptr<param> const & par)
{
    conn_check();
    SQLRETURN ret = SQLBindParameter(get_handle(), 
            static_cast<SQLUSMALLINT>(par->number()), SQL_PARAM_INPUT, 
            par->value_type_id(), par->param_type_id(), 0, 0, 
            par->parameter_value_ptr(), 0, par->strlen_or_indptr());
    if ( ret != SQL_SUCCESS )
        throw error(*this, ret);
}

std::auto_ptr<sql_cli::detail::result_set_base> 
        statement::create_result_set() const
{
    std::auto_ptr<sql_cli::detail::result_set_base> s(new result_set(*this));
    return s;
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost
