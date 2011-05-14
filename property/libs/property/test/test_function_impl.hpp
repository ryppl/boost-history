#if !defined(TEST_FUNCTION_IMPL_HPP)
#define TEST_FUNCTION_IMPL_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

extern char p_function_char_read();
extern void p_function_char_write(char);
extern int p_function_int_read();
extern void p_function_int_write(int);
extern double p_function_double_read();
extern void p_function_double_write(double);
extern bool p_function_bool_read();
extern void p_function_bool_write(bool);
extern int * p_function_pointer_read();
extern void p_function_pointer_write(int *);
extern test_enum p_function_enum_read();
extern void p_function_enum_write(test_enum);
extern test_pod p_function_class_read();
extern void p_function_class_write(test_pod);

class p_function_class
  {
  
  public:
  
  static char p_char_read();
  static void p_char_write(char);
  static int p_int_read();
  static void p_int_write(int);
  static double p_double_read();
  static void p_double_write(double);
  static bool p_bool_read();
  static void p_bool_write(bool);
  static int * p_pointer_read();
  static void p_pointer_write(int *);
  static test_enum p_enum_read();
  static void p_enum_write(test_enum);
  static test_pod p_class_read();
  static void p_class_write(test_pod);
  
  static char p_char_read_2();
  static void p_char_write_2(char);
  static int p_int_read_2();
  static void p_int_write_2(int);
  static double p_double_read_2();
  static void p_double_write_2(double);
  static bool p_bool_read_2();
  static void p_bool_write_2(bool);
  static int * p_pointer_read_2();
  static void p_pointer_write_2(int *);
  static test_enum p_enum_read_2();
  static void p_enum_write_2(test_enum);
  static test_pod p_class_read_2();
  static void p_class_write_2(test_pod);
  
  static char st_char;
  static int st_int;
  static double st_double;
  static bool st_bool;
  static int * st_pointer;
  static test_enum st_enum;
  static test_pod st_pod;

  static char st_char_2;
  static int st_int_2;
  static double st_double_2;
  static bool st_bool_2;
  static int * st_pointer_2;
  static test_enum st_enum_2;
  static test_pod st_pod_2;

  };
  
#endif
