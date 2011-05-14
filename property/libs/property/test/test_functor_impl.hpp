#if !defined(TEST_FUNCTOR_IMPL_HPP)
#define TEST_FUNCTOR_IMPL_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

class p_functor_class_char
  {
  
  public:
  
  char operator()();
  void operator()(char);
  };

class p_functor_class_int
  {
  
  public:
  
  int operator()();
  void operator()(int);
  };

class p_functor_class_double
  {
  
  public:
  
  double operator()();
  void operator()(double);
  };

class p_functor_class_bool
  {
  
  public:
  
  bool operator()();
  void operator()(bool);
  };

class p_functor_class_pointer
  {
  
  public:
  
  int * operator()();
  void operator()(int *);
  };

class p_functor_class_enum
  {
  
  public:
  
  test_enum operator()();
  void operator()(test_enum);
  };

class p_functor_class_pod
  {
  
  public:
  
  test_pod operator()();
  void operator()(test_pod);
  };
  
#endif
