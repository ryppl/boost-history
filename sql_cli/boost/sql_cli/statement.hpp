//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_STATEMENT_HPP)
#define BOOST_SQL_CLI_STATEMENT_HPP

#include <string>

#include <boost/scoped_ptr.hpp>

#include <boost/sql_cli/connection.hpp>
#include <boost/sql_cli/detail/result_set_base.hpp>
#include <boost/sql_cli/detail/statement_base.hpp>

namespace boost
{
namespace sql_cli
{
class result_set;

//! @class statement statement.hpp boost/sql_cli/statement.hpp
//! @brief Allows the execution of queries against the database.
//!
//! Each statement performs its queries through the connection instance it is
//! bound to at creation time, which means that use of the same statement
//! instance across threads requires explicit synchronization. In practice
//! statement instances should be considered thread specific, as thread
//! synchronization may already be provided by the underlying technology at the
//! connection level.
//! @nosubgrouping

class statement
{
public:
    //! @name Constructors
    //@{
    //! Binds the new instance with connection 'c'.
    statement(connection & c) : stmt(c.create_statement()) {}
    //@}

    //! @name Statement preparation
    //@{
    //! Prepares the %statement specified in the 's' parameter and binds input
    //! parameter 'v1' to it. Overloads are provided to support binding of up
    //! to ten parameters.
    template <typename Value1T>
    void prepare(std::string const & s, Value1T & v1) 
    { 
        stmt->prepare(s);
        bind_param(1, v1); 
    }

    template <typename Value1T, typename Value2T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2) 
    { 
        prepare(s, v1); 
        bind_param(2, v2); 
    }

    template <typename Value1T, typename Value2T, typename Value3T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, Value3T & v3) 
    { 
        prepare(s, v1, v2); 
        bind_param(3, v3); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4) 
    { 
        prepare(s, v1, v2, v3); 
        bind_param(4, v4); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T, typename Value5T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4, Value5T & v5) 
    { 
        prepare(s, v1, v2, v3, v4); 
        bind_param(5, v5); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T, typename Value5T, typename Value6T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4, Value5T & v5, Value6T & v6) 
    { 
        prepare(s, v1, v2, v3, v4, v5); 
        bind_param(6, v6); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T, typename Value5T, typename Value6T,
            typename Value7T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4, Value5T & v5, Value6T & v6,
            Value7T & v7) 
    { 
        prepare(s, v1, v2, v3, v4, v5, v6); 
        bind_param(7, v7); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T, typename Value5T, typename Value6T,
            typename Value7T, typename Value8T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4, Value5T & v5, Value6T & v6,
            Value7T & v7, Value8T & v8) 
    { 
        prepare(s, v1, v2, v3, v4, v5, v6, v7); 
        bind_param(8, v8); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T, typename Value5T, typename Value6T,
            typename Value7T, typename Value8T, typename Value9T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4, Value5T & v5, Value6T & v6,
            Value7T & v7, Value8T & v8, Value9T & v9) 
    { 
        prepare(s, v1, v2, v3, v4, v5, v6, v7, v8); 
        bind_param(9, v9); 
    }

    template <typename Value1T, typename Value2T, typename Value3T, 
            typename Value4T, typename Value5T, typename Value6T,
            typename Value7T, typename Value8T, typename Value9T,
            typename Value10T>
    void prepare(std::string const & s, Value1T & v1, Value2T & v2, 
            Value3T & v3, Value4T & v4, Value5T & v5, Value6T & v6,
            Value7T & v7, Value8T & v8, Value9T & v9, Value10T & v10) 
    { 
        prepare(s, v1, v2, v3, v4, v5, v6, v7, v8, v9); 
        bind_param(10, v10); 
    }
    //@}

    //! @name Statement execution
    //@{
    //! Executes a previously prepared %statement.
    void exec() { return stmt->exec(); }
    //! Executes the %statement passed as argument 's' directly.
    void exec(std::string const & s) { return stmt->exec(s); }
    //@}

private:
    friend class result_set;

    template <typename ValueT> 
    void bind_param(int n, ValueT & v)
    {
		stmt->bind_param(n, v);
    }

    std::auto_ptr<detail::result_set_base> create_result_set() const
    { 
        return stmt->create_result_set(); 
    }

    boost::scoped_ptr<detail::statement_base> stmt;
};

} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_STATEMENT_HPP)
