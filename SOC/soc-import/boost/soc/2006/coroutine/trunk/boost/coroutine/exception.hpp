//  (C) Copyright Giovanni P. Deretta 200b. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_EXCEPTION_HPP_20060601
#define BOOST_COROUTINE_EXCEPTION_HPP_20060601
#include <exception>
#include <typeinfo>
namespace boost { namespace coroutines {
  class swap_error: public std::exception {};
  class exit_exception:  public std::exception {};
  class coroutine_exited: public std::exception {};
  class coroutine_not_ready : public std::exception {};
  class unknown_exception_tag {};
  class abnormal_exit : public std::exception {
  public:
    abnormal_exit(std::type_info const& e) :
      m_e (e) {};

    const char* what() const throw() {
      return m_e.name();
    }
  private:
    std::type_info const& m_e;
  };
} } 

#endif
