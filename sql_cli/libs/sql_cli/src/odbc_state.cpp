//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <boost/sql_cli/odbc/odbc_state.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{
state src_trunc("01004");
state frac_trunc("01S07");

bool string_truncated(state const & s)
{
    return s == src_trunc;
}

bool fraction_truncated(state const & s)
{
    return s == frac_trunc;
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost
