//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_RESULT_SET_HPP)
#define BOOST_SQL_CLI_RESULT_SET_HPP

#include <string>

#include <boost/scoped_ptr.hpp>

#include <boost/sql_cli/column.hpp>
#include <boost/sql_cli/error.hpp>
#include <boost/sql_cli/statement.hpp>
#include <boost/sql_cli/detail/result_set_base.hpp>
#include <boost/sql_cli/detail/result_set_iterator.hpp>

namespace boost
{
namespace sql_cli
{

//! @class result_set result_set.hpp boost/sql_cli/result_set.hpp
//! @brief Represents the results of a query that returns data and provides
//! means to iterate over both the data and a description of its
//! structure.

class result_set
{
public:
    typedef detail::result_set_iterator iterator;

    //! @name Constructors
    //@{
    //! Associates a new result_set with statement 's'.
    result_set(statement & s) : res(s.create_result_set()) {}
    //@}

    //! @name Result set description
    //@{
    //! Returns the number of columns returned by the last query executed by
    //! the associated statement.
    int column_count() const { return res->column_count(); }

    //! Returns a description of the 'i'-th %column in the result set.
    column operator[](int i) const
    {
        return res->get_column(i);
    }

    //! Returns a description of the %column named 'n' in the result set.
    column operator[](std::string const & n) const
    {
        return res->get_column(n);
    }
    //@}

    //! @name Iteration over the returned rows
    //@{
    //! \brief Returns an iterator to the first row in the result set.
    //! Dereferencing an iterator yields an instance of the row class.
    iterator begin() const { return iterator(res.get()); }
    //! Returns an iterator to one past the last row in the result set.
    iterator end() const { return iterator(); }
    //@}

private:
    boost::scoped_ptr<detail::result_set_base> res;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_RESULT_SET_HPP)
