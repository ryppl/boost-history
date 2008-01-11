//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ROW_HPP)
#define BOOST_SQL_CLI_ROW_HPP

#include <memory>

#include <boost/scoped_ptr.hpp>

#include <boost/sql_cli/field.hpp>
#include <boost/sql_cli/detail/row_base.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{
class result_set_iterator;
} // namespace detail

//! @class row row.hpp boost/sql_cli/row.hpp
//! Allows iteration over the fields in a result set's %row.

class row
{
public:
    //! @name Field iteration
    //@{
    //! Returns the 'i'-th field.
    field operator[] (int i) const { return field(rw->get_field(i)); }
    //! Returns the field named 'n'.
    field operator[] (std::string const & n) const 
    { 
        return field(rw->get_field(n)); 
    }
    //@}

private:
    friend class detail::result_set_iterator;

    row(std::auto_ptr<detail::row_base> rb) : rw(rb) {}

    boost::scoped_ptr<detail::row_base> rw;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ROW_HPP)
