/*
 * Boost.Extension / shared_library:
 *         functions for shared_library loading
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_LINKED_LIBRARY_HPP
#define BOOST_EXTENSION_LINKED_LIBRARY_HPP
#include <boost/extension/impl/library_impl.hpp>

#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
#include <boost/function.hpp>
#endif

namespace boost{namespace extensions{
template <class ReturnValue, class Param1 = void, class Param2 = void, 
          class Param3 = void, class Param4 = void, class Param5 = void, 
          class Param6 = void>
class functor
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function6<ReturnValue, Param1, Param2, Param3, 
			   Param4, Param5, Param6> FunctionType;
#else
  typedef ReturnValue (*FunctionType)(Param1, Param2, Param3, Param4, 
				      Param5, Param6);
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION
  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)(Param1, Param2, Param3, Param4, 
					 Param5, Param6)) func))
  {}
  ReturnValue operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, 
                         Param5 p5, Param6 p6)
  {
    return func_(p1, p2, p3, p4, p5, p6);
  }
};

template <class ReturnValue, class Param1, class Param2, class Param3, 
          class Param4, class Param5>
class functor<ReturnValue, Param1, Param2, Param3, Param4, Param5>
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function5<ReturnValue, Param1, Param2, Param3, 
			   Param4, Param5> FunctionType;
#else
  typedef ReturnValue (*FunctionType)(Param1, Param2, Param3, Param4, Param5);
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION
  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)(Param1, Param2, Param3, 
					 Param4, Param5)) func))
  {}
  ReturnValue operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
  {
    return func_(p1, p2, p3, p4, p5);
  }
};

template <class ReturnValue, class Param1, class Param2, class Param3, 
          class Param4>
class functor<ReturnValue, Param1, Param2, Param3, Param4>
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function4<ReturnValue, Param1, Param2, Param3, 
			   Param4> FunctionType;
#else
  typedef ReturnValue (*FunctionType)(Param1, Param2, Param3, Param4);
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION
  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)(Param1, Param2, Param3, 
					 Param4)) func))
  {}
  ReturnValue operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
  {
    return func_(p1, p2, p3, p4);
  }
};

template <class ReturnValue, class Param1, class Param2, class Param3>
class functor<ReturnValue, Param1, Param2, Param3>
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function3<ReturnValue, Param1, Param2, Param3> FunctionType;
#else
  typedef ReturnValue (*FunctionType)(Param1, Param2, Param3);
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION
  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)(Param1, Param2, Param3)) func))
  {}
  ReturnValue operator()(Param1 p1, Param2 p2, Param3 p3)
  {
    return func_(p1, p2, p3);
  }
};

template <class ReturnValue, class Param1, class Param2>
class functor<ReturnValue, Param1, Param2>
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function2<ReturnValue, Param1, Param2> FunctionType;
#else
  typedef ReturnValue (*FunctionType)(Param1, Param2);
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION
  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)(Param1, Param2)) func))
  {}
  ReturnValue operator()(Param1 p1, Param2 p2)
  {
    return func_(p1, p2);
  }
};

template <class ReturnValue, class Param1>
class functor<ReturnValue, Param1>
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function1<ReturnValue, Param1> FunctionType;
#else
  typedef ReturnValue (*FunctionType)(Param1);
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION

  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)(Param1)) func))
  {}
  ReturnValue operator()(Param1 p1)
  {
    return func_(p1);
  }
};

template <class ReturnValue>
class functor<ReturnValue>
{
protected:
#ifdef BOOST_EXTENSION_USE_BOOST_FUNCTION
  typedef boost::function0<ReturnValue> FunctionType;
#else
  typedef ReturnValue (*FunctionType)();
#endif // BOOST_EXTENSION_USE_BOOST_FUNCTION
  FunctionType func_;
public:
  bool is_valid(){return func_ != 0;}
  functor(FunctionType func)
    :func_(func)
  {}
  functor(generic_function_ptr func)
    :func_(FunctionType((ReturnValue (*)()) func))
  {}
  ReturnValue operator()()
  {
    return func_();
  }
};

class shared_library
{
protected:
  std::string location_;
  library_handle handle_;
  bool auto_close_;
public:
  bool is_open(){return handle_ != 0;}
  static bool is_linkable_library(const char * file_name) {
    return is_library(file_name);
  }
  bool open(){return (handle_ = load_shared_library(location_.c_str())) != 0;}
  bool close(){return close_shared_library(handle_);}  

  template <class ReturnValue>
  functor<ReturnValue>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue>
        (get_function(handle_, function_name));
  }
  template <class ReturnValue, class Param1>
  functor<ReturnValue, Param1>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue, Param1>
        (get_function(handle_, function_name));
  }
  template <class ReturnValue, class Param1, class Param2>
  functor<ReturnValue, Param1, Param2>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue, Param1, Param2>
        (get_function(handle_, function_name));
  }
  template <class ReturnValue, class Param1, class Param2, class Param3>
  functor<ReturnValue, Param1, Param2, Param3>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue, Param1, Param2, Param3>
        (get_function(handle_, function_name));
  }
  template <class ReturnValue, class Param1, class Param2, class Param3, 
            class Param4>
  functor<ReturnValue, Param1, Param2, Param3, Param4>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue, Param1, Param2, Param3, Param4>
        (get_function(handle_, function_name));
  }
  template <class ReturnValue, class Param1, class Param2, class Param3, 
            class Param4, class Param5>
  functor<ReturnValue, Param1, Param2, Param3, Param4, Param5>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue, Param1, Param2, Param3, Param4, Param5>
        (get_function(handle_, function_name));
  }
  template <class ReturnValue, class Param1, class Param2, class Param3, 
            class Param4, class Param5, class Param6>
  functor<ReturnValue, Param1, Param2, Param3, Param4, Param5, Param6>
    get_functor(const char * function_name)
  {
      return functor<ReturnValue, Param1, Param2, Param3, Param4, Param5, 
      Param6>
        (get_function(handle_, function_name));
  }
shared_library(const char * location, bool auto_close = false)
    :location_(location), handle_(0), auto_close_(auto_close){}
};
}}


#endif
