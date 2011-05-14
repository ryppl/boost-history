#if !defined(TEST_MEMBER_FUNCTION_IMPL_HPP)
#define TEST_MEMBER_FUNCTION_IMPL_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

class p_member_function_impl_class
  {
  
  public:
  
  p_member_function_impl_class();
  
  char p_function_char_read();
  void p_function_char_write(char);
  int p_function_int_read();
  void p_function_int_write(int);
  double p_function_double_read();
  void p_function_double_write(double);
  bool p_function_bool_read();
  void p_function_bool_write(bool);
  int * p_function_pointer_read();
  void p_function_pointer_write(int *);
  test_enum p_function_enum_read();
  void p_function_enum_write(test_enum);
  test_pod p_function_class_read();
  void p_function_class_write(test_pod);

  char p_char_read();
  void p_char_write(char);
  int p_int_read();
  void p_int_write(int);
  double p_double_read();
  void p_double_write(double);
  bool p_bool_read();
  void p_bool_write(bool);
  int * p_pointer_read();
  void p_pointer_write(int *);
  test_enum p_enum_read();
  void p_enum_write(test_enum);
  test_pod p_class_read();
  void p_class_write(test_pod);
  
  char p_char_read_2();
  void p_char_write_2(char);
  int p_int_read_2();
  void p_int_write_2(int);
  double p_double_read_2();
  void p_double_write_2(double);
  bool p_bool_read_2();
  void p_bool_write_2(bool);
  int * p_pointer_read_2();
  void p_pointer_write_2(int *);
  test_enum p_enum_read_2();
  void p_enum_write_2(test_enum);
  test_pod p_class_read_2();
  void p_class_write_2(test_pod);
  
  char st_char;
  int st_int;
  double st_double;
  bool st_bool;
  int * st_pointer;
  test_enum st_enum;
  test_pod st_pod;

  char st_char_2;
  int st_int_2;
  double st_double_2;
  bool st_bool_2;
  int * st_pointer_2;
  test_enum st_enum_2;
  test_pod st_pod_2;

  char st_char_3;
  int st_int_3;
  double st_double_3;
  bool st_bool_3;
  int * st_pointer_3;
  test_enum st_enum_3;
  test_pod st_pod_3;

  };
  
#endif
