//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ERROR_HPP)
#define BOOST_SQL_CLI_ERROR_HPP

#include <string>
#include <stdexcept>

namespace boost
{
namespace sql_cli
{

//! @class error error.hpp boost/sql_cli/error.hpp
//! Exception class that represents a generic SqlCli error.

class error : public std::runtime_error
{
public:
    //! @name Constructors
    //@{
    //! @brief The 'msg' string will be returned by the what() member
    //! function, possibly augmented with internal information.
    error(std::string const & msg) : 
            std::runtime_error("Boost.SqlCli error: " + msg) {}
    //@}
};

//! @class null_data error.hpp boost/sql_cli/error.hpp
//! Exception class that represents a generic SqlCli error.
//! Represents null values in context where they should not be ignored.
//! This may be thrown when retrieving a null value in an object that is not
//! suitable to represent it (i.e. it is neither a boost::optional<> nor a
//! sql_cli::null_value<>).

class null_data : public error
{
public:
    null_data() : error("Null data") {}
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ERROR_HPP)
