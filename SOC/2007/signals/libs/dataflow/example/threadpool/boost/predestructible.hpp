// A simple framework for creating objects with predestructors.
// The objects must inherit from boost::predestructible, and
// have their lifetimes managed by
// boost::shared_ptr created with the boost::deconstruct_ptr()
// function.
//
// Author: Frank Mori Hess 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PREDESTRUCTIBLE_HEADER
#define BOOST_PREDESTRUCTIBLE_HEADER

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost
{
  class predestructible
  {
  public:
    virtual void predestruct() {}
  protected:
    predestructible() {}
    virtual ~predestructible() {}
  };
}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
