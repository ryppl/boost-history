//  boost/rank_list/detail/exception.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/exception.hpp
  --------------------

  Exceptions thrown by rank_list:

    allocator_returned_null  (constructors, insert, resize ...)
    index_out_of_bounds      (op.[], op.(), move ...)
    invalid_op_with_end      (move, erase, dereference ...)
    lesser_and_greater       (sort, insert_sorted ...)
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_EXCEPTION_HPP_
#define _BOOST_RANK_LIST_EXCEPTION_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {
    namespace detail
    {

//////////////////////////////////////////////////////////////////

class allocator_returned_null : public virtual std::exception
{
  public:
    virtual ~allocator_returned_null() throw() { }
    virtual const char* what() const throw()
    { return "Memory allocator returned NULL"; }
};

//////////////////////////////////////////////////////////////////

class index_out_of_bounds : public virtual std::exception
{
  public:
    virtual ~index_out_of_bounds() throw() { }
    virtual const char* what() const throw()
    { return "Index out of bounds"; }
};

//////////////////////////////////////////////////////////////////

class invalid_op_with_end : public virtual std::exception
{
  public:
    virtual ~invalid_op_with_end() throw() { }
    virtual const char* what() const throw()
    { return "Invalid operation with end element"; }
};

//////////////////////////////////////////////////////////////////

class lesser_and_greater : public virtual std::exception
{
  public:
    virtual ~lesser_and_greater() throw() { }
    virtual const char* what() const throw()
    { return "Lesser and greater at the same time"; }
};

//////////////////////////////////////////////////////////////////

    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif

