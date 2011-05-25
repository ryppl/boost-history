#if !defined(TEST_FUNCTOR_IMPL_HPP)
#define TEST_FUNCTOR_IMPL_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

class p_functor_class_char
  {
  
  public:
  
  p_functor_class_char();
  p_functor_class_char(char c);
  
  char operator()();
  void operator()(char);
  
  private:
  
  char data;
  
  };

class p_functor_class_int
  {
  
  public:
  
  p_functor_class_int();
  p_functor_class_int(int i);
  
  int operator()();
  void operator()(int);
  
  private:
  
  int data;
  
  };

class p_functor_class_double
  {
  
  public:
  
  p_functor_class_double();
  p_functor_class_double(double d);
  
  double operator()();
  void operator()(double);
  
  private:
  
  double data;
  
  };

class p_functor_class_bool
  {
  
  public:
  
  p_functor_class_bool();
  p_functor_class_bool(bool b);
  
  bool operator()();
  void operator()(bool);
  
  private:
  
  bool data;
  
  };

class p_functor_class_pointer
  {
  
  public:
  
  p_functor_class_pointer();
  p_functor_class_pointer(int * p);
  
  int * operator()();
  void operator()(int *);
  
  private:
  
  int * data;
  
  };

class p_functor_class_enum
  {
  
  public:
  
  p_functor_class_enum();
  p_functor_class_enum(test_enum e);
  
  test_enum operator()();
  void operator()(test_enum);
  
  private:
  
  test_enum data;
  
  };

class p_functor_class_pod
  {
  
  public:
  
  p_functor_class_pod();
  p_functor_class_pod(test_pod p);
  
  test_pod operator()();
  void operator()(test_pod);
  
  private:
  
  test_pod data;
  
  };
  
#endif
