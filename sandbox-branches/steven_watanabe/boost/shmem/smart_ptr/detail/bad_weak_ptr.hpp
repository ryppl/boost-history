//////////////////////////////////////////////////////////////////////////////
//
// This file is the adaptation for Shmem of boost/detail/bad_weak_ptr.hpp
//
// (C) Copyright Peter Dimov and Multi Media Ltd. 2001, 2002, 2003
// (C) Copyright Ion Gaztañaga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SHMEM_BAD_WEAK_PTR_HPP_INCLUDED
#define BOOST_SHMEM_BAD_WEAK_PTR_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <exception>

namespace boost{
namespace shmem{

class bad_weak_ptr
   :  public std::exception
{
   public:

   virtual char const * what() const throw()
   {  return "boost::shmem::bad_weak_ptr"; }
};

} // namespace shmem 
} // namespace boost

#endif  // #ifndef BOOST_SHMEM_BAD_WEAK_PTR_HPP_INCLUDED
