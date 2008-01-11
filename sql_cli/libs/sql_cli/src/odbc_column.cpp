//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#include <sstream>

#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>

#include <boost/sql_cli/odbc/odbc_column.hpp>
#include <boost/sql_cli/odbc/traits.hpp>
#include <boost/sql_cli/odbc/odbc_error.hpp>
#include <boost/sql_cli/detail/value_types.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{
template <int n>
const std::type_info & code_to_typeinfo(int t)
{
    using namespace boost::mpl;
    typedef typename at<detail::value_types, int_<n> >::type
            type;
    if ( traits<type>::param_type_id == t )
        return typeid(type);
    else
        return code_to_typeinfo<n+1>(t);
}

template <>
const std::type_info & code_to_typeinfo<boost::mpl::size <
        detail::value_types>::type::value>(int t)
{
    std::ostringstream s;
    s << t << ": Invalid type";
    throw error(s.str());
}

const std::type_info & column::type() const
{
    return code_to_typeinfo<0>(typ);
}

} // namespace odbc
} // namespace sql_cli
} // namespace boost
