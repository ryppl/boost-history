//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_FIELD_HPP)
#define BOOST_SQL_CLI_FIELD_HPP

#include <boost/shared_ptr.hpp>

#include <boost/sql_cli/detail/field_base.hpp>
#include <boost/sql_cli/detail/static_value.hpp>

namespace boost
{
namespace sql_cli
{
class row;

//! @class field field.hpp boost/sql_cli/field.hpp
//! Represents a %field in a %row of a statement's result set.

class field
{
public:
    //! Returns the field's value.
    template <typename ValueT> 
    ValueT get() const
    {
		detail::static_value sv = detail::make_static_value<ValueT>();
		fd->get(sv);
		return sv;
    }

    //! Returns the field's number.
    int number() const { return fd->number(); }

private:
    friend class row;

    field(boost::shared_ptr<detail::field_base> const & f) : fd(f) {}

    boost::shared_ptr<detail::field_base> fd;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_FIELD_HPP)
