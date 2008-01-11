//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_STATE_HPP)
#define BOOST_SQL_CLI_ODBC_STATE_HPP

#include <algorithm>

#include <boost/array.hpp>
#include <boost/operators.hpp>

#include <boost/sql_cli/odbc/odbc_config.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

class state : 
        public equality_comparable<state, less_than_comparable<state> >
{
    typedef boost::array<SQLCHAR, SQL_SQLSTATE_SIZE> state::* safe_bool_type;

public:
    state() { data[0] = '\0'; }

    explicit state(const char (&d)[SQL_SQLSTATE_SIZE + 1]) 
    { 
        std::copy(d, d + SQL_SQLSTATE_SIZE, data.begin()); 
    }

    operator SQLCHAR * () { return data.c_array(); }

    operator safe_bool_type() { return data[0] == '\0' ? &state::data : 0; }

    bool operator== (state const & rhs) const { return data == rhs.data; }
    bool operator< (state const & rhs) const { return data < rhs.data; }

private:
    boost::array<SQLCHAR, SQL_SQLSTATE_SIZE> data;

};

bool string_truncated(state const & s);
bool fraction_truncated(state const & s);

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_STATE_HPP)
