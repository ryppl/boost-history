// A function for creating a shared_ptr that enhances the plain
// shared_ptr constructors by adding support for postconstructors
// and predestructors through the boost::postconstructible and
// boost::predestructible base classes.
//
// Author: Frank Mori Hess 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DECONSTRUCT_PTR_HEADER
#define BOOST_DECONSTRUCT_PTR_HEADER

#include <boost/postconstructible.hpp>
#include <boost/predestructible.hpp>
#include <boost/shared_ptr.hpp>

namespace boost
{
  namespace deconstruct_detail
  {
    void do_postconstruct(const boost::postconstructible *ptr)
    {
      boost::postconstructible *nonconst_ptr = const_cast<boost::postconstructible*>(ptr);
      nonconst_ptr->postconstruct();
    }
    void do_postconstruct(...)
    {
    }
  }
  template<typename T> class predestructing_deleter
  {
  public:
    void operator()(T *ptr)
    {
      m_predestruct(ptr);
      delete ptr;
    }
  private:
    void m_predestruct(...)
    {
    }
    void m_predestruct(const boost::predestructible *ptr)
    {
      boost::predestructible *nonconst_ptr = const_cast<boost::predestructible*>(ptr);
      nonconst_ptr->predestruct();
    }
  };

  template<typename T>
  shared_ptr<T> deconstruct_ptr(T *ptr)
  {
    shared_ptr<T> shared(ptr, boost::predestructing_deleter<T>());
    deconstruct_detail::do_postconstruct(ptr);
    return shared;
  }
  template<typename T, typename D>
  shared_ptr<T> deconstruct_ptr(T *ptr, D deleter)
  {
    shared_ptr<T> shared(ptr, deleter);
    deconstruct_detail::do_postconstruct(ptr);
    return shared;
  }
}
#endif
