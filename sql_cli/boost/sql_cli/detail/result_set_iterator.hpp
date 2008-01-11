//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_DETAIL_RESULT_SET_ITERATOR_HPP)
#define BOOST_SQL_CLI_DETAIL_RESULT_SET_ITERATOR_HPP

#include <cassert>
#include <memory>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/sql_cli/row.hpp>
#include <boost/sql_cli/detail/result_set_base.hpp>
#include <boost/sql_cli/detail/row_base.hpp>

namespace boost
{
namespace sql_cli
{
namespace detail
{

class result_set_iterator : public boost::iterator_facade<result_set_iterator, 
    row const, std::input_iterator_tag>
{
public:
    result_set_iterator() : rsb(0) {}
    explicit result_set_iterator(result_set_base * r) : rsb(r) { increment(); }

private:
    friend class boost::iterator_core_access;

    void increment() 
    { 
        assert(rsb);
        std::auto_ptr<row_base> rb(rsb->create_row());
        curr.reset(rb.get() ? new sql_cli::row(rb) : 0);
    }

    bool equal(result_set_iterator const& other) const
    {
        return rsb == other.rsb && ! rsb || curr == other.curr;
    }

    sql_cli::row & dereference() const 
    { 
        assert(rsb);
        return *curr; 
    }

    result_set_base * rsb;
    boost::shared_ptr<sql_cli::row> curr;
};

} // namespace detail
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_DETAIL_RESULT_SET_ITERATOR_HPP)
