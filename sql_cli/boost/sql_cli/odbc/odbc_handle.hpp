//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_HANDLE_HPP)
#define BOOST_SQL_CLI_ODBC_HANDLE_HPP

#include <string>
#include <sstream>
#include <vector>

#include <boost/noncopyable.hpp>

#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>
#include <boost/sql_cli/odbc/odbc_state.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

// TODO: Consider providing single diagnostic values by using SQLGetDiagField
// instead of SQLGetDiagRec
template <typename TypeT, SQLSMALLINT KindV>
class handle : private boost::noncopyable
{
public:
    typedef TypeT type;

    static SQLSMALLINT const kind = KindV;

    type get_handle() const { return hn; }
    type & get_handle() { return hn; }

    std::string error_msg(SQLRETURN r) const;
    state error_code() const;
    state check_return(SQLRETURN ret, std::string * msg = 0) const;

private:
    struct diag_rec
    {
        diag_rec() : msg(81), len(0) {}

        state code;
        SQLINTEGER	err;
        std::vector<SQLCHAR> msg;
        SQLSMALLINT len;
    };

    SQLRETURN update_diag() const;

    type hn;
    mutable diag_rec current_diag;
};

// Does not throw because it may be used in order to set up an exception object

template <typename TypeT, SQLSMALLINT KindV>
SQLRETURN handle<TypeT, KindV>::update_diag() const
{
    // TODO: Loop over the available diagnostic records
    SQLRETURN ret = SQLGetDiagRec(kind, hn, static_cast<SQLSMALLINT>(1), 
            current_diag.code, &current_diag.err, &current_diag.msg[0], 
            static_cast<SQLSMALLINT>(current_diag.msg.size()), 
            &current_diag.len);
    if ( ret == SQL_SUCCESS_WITH_INFO &&
            static_cast<SQLSMALLINT>(current_diag.msg.size()) < 
                    current_diag.len + 1 )
    {
        current_diag.msg.resize(current_diag.len + 1);
        ret = SQLGetDiagRec(kind, hn, static_cast<SQLSMALLINT>(1),
                current_diag.code, &current_diag.err, &current_diag.msg[0],
                static_cast<SQLSMALLINT>(current_diag.msg.size()), 
                &current_diag.len);
    }
    return ret;
}

template <typename TypeT, SQLSMALLINT KindV>
std::string handle<TypeT, KindV>::error_msg(SQLRETURN r) const
{
    if ( r == SQL_INVALID_HANDLE )
        return "Invalid handle";
 
    SQLRETURN ret = update_diag();
    if ( ret == SQL_SUCCESS )
    {
        std::ostringstream s;
        // TODO: Correct the static_cast
        s << '[' << static_cast<SQLCHAR *>(current_diag.code) << ']' << " " << &current_diag.msg[0] << 
                " (" << current_diag.err << ')';
        return s.str();
    }
    else
        return "Unknown problem";
}

template <typename TypeT, SQLSMALLINT KindV>
state handle<TypeT, KindV>::error_code() const
{
    if ( update_diag() != SQL_SUCCESS )
        throw error("Invalid diagnostic record");
    return current_diag.code;
}

template <typename TypeT, SQLSMALLINT KindV>
state handle<TypeT, KindV>::check_return(SQLRETURN ret, std::string * msg) const
{
    if ( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
    {
        if ( msg != 0 )
            throw error(*msg);
        else
            throw error(*this, ret);
    }
    state st;
    if ( ret == SQL_SUCCESS_WITH_INFO )
    {
        st = error_code();
        // TODO: Handle warnings
    }
    return st;
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_HANDLE_HPP)
