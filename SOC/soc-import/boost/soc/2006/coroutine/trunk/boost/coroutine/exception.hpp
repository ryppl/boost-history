//  (C) Copyright Giovanni P. Deretta 200b. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_EXCEPTION_HPP_20060601
#define BOOST_COROUTINE_EXCEPTION_HPP_20060601
#include <exception>
#include <typeinfo>
namespace boost { namespace coroutines {

  // All coroutine exceptions are derived from this base.
  class exception_base : public std::exception {};

  // This exception is thrown when a coroutine is requested
  // to exit.
  class exit_exception:  public exception_base {};

  // This exception is thrown on a coroutine invocation
  // if a coroutine exits without
  // returning a result. Note that calling invoke()
  // on an already exited coroutine is undefined behaviour.
  class coroutine_exited: public exception_base {};  
  
  // This exception is thrown on a coroutine invocation
  // if a coroutine enter the wait state without
  // returning a result. Note that calling invoke()
  // on a waiting coroutine is undefined behaviour.
  class waiting : public exception_base {};

  class unknown_exception_tag {};

  // This exception is thrown on a coroutine invocation
  // if the coroutine is exited by an uncatched exception
  // (not derived from exit_exception). abnormal_exit::type()
  // returns the typeid of that exception if it is derived
  // from std::exception, else returns typeid(unknonw_exception_tag)
  class abnormal_exit : public std::exception {
  public:
    abnormal_exit(std::type_info const& e) :
      m_e (e) {};

    const char* what() const throw() {
      return m_e == typeid(unknown_exception_tag)? 
	"unknown exception" :
	m_e.name();
    }

    std::type_info const& type() const throw() {
      return m_e;
    }
  private:
    std::type_info const& m_e;
  };
} } 

#endif
