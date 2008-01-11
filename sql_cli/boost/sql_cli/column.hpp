//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_COLUMN_HPP)
#define BOOST_SQL_CLI_COLUMN_HPP

#include <string>
#include <typeinfo>

#include <boost/shared_ptr.hpp>

#include <boost/sql_cli/detail/column_base.hpp>

namespace boost
{
namespace sql_cli
{
class result_set;

//! @class column column.hpp boost/sql_cli/column.hpp
//! @brief Provides a description for one %column in a result set.

class column
{
public:
    //! Returns the column's name.
    std::string name() const { return col->name(); }
    //! Returns the type_info object associated with the column's C++ type.
    const std::type_info & type() const { return col->type(); }
    //! Returns the column's size.
    int size() const { return col->size(); }
    //! Returns the column's precision.
    int precision() const { return col->precision(); }
    //! Indicates whether the columns accepts null values.
    bool nullable() const { return col->nullable(); }
    //! Returns the column's number.
    int number() const { return col->number(); }

private:
    friend class result_set;

    column(boost::shared_ptr<detail::column_base> const & c) : col(c) {}

    boost::shared_ptr<detail::column_base> col;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_COLUMN_HPP)
