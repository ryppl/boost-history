//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_COMMON_THREAD_ALLOCATORS_THREAD_ALLOCATOR_HPP
#define BOOST_ACT_COMMON_THREAD_ALLOCATORS_THREAD_ALLOCATOR_HPP

#include "../detail/thread_manager.hpp"

#include <boost/operators.hpp>

#include <boost/shared_ptr.hpp>

namespace boost
{
namespace act
{

struct thread_allocator
{
  class thread_ref
    : equality_comparable< thread_ref >
  {
  public:
    thread_ref()
      : thread_m( new thread )
    {
    }
  public:
    void join() const
    {
      thread_m->join();
    }
  private:
    friend bool operator ==( thread_ref const& left
                           , thread_ref const& right
                           )
    {
      return *left.thread_m == *right.thread_m;
    }
  private:
    shared_ptr< thread > thread_m;
  };

  template< typename FunctionType >
  thread_ref allocate( FunctionType const& function ) const
  {
  }
};

}
}

#endif
