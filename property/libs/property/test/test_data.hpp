#if !defined(TEST_DATA_HPP)
#define TEST_DATA_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

extern char p_data_char;
extern int p_data_int;
extern double p_data_double;
extern bool p_data_bool;
extern int * p_data_pointer;
extern test_enum p_data_enum;
extern test_pod p_data_class;

class p_data_class
  {
  
  public:
  
  static char p_char;
  static int p_int;
  static double p_double;
  static bool p_bool;
  static int * p_pointer;
  static test_enum p_enum;
  static test_pod p_class;
  
  static char p_char_2;
  static int p_int_2;
  static double p_double_2;
  static bool p_bool_2;
  static int * p_pointer_2;
  static test_enum p_enum_2;
  static test_pod p_class_2;
  
  };
  
#endif
