//
//  generic_ptr/null_deleter.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_NULL_DELETER_HPP_INCLUDED
#define BOOST_GENERIC_PTR_NULL_DELETER_INCLUDED

namespace boost
{
  namespace generic_ptr
  {
    class null_deleter
    {
    public:
      template<typename T>
      void operator()(const T&)
      {}
    };
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_NULL_DELETER_HPP_INCLUDED
