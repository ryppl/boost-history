#if !defined(TEST_DATA_IMPL_HPP)
#define TEST_DATA_IMPL_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

class p_data_class
  {
  
  public:
  
  static char p_char;
  static int p_int;
  static double p_double;
  static bool p_bool;
  static int * p_pointer;
  static test_enum p_enum;
  static test_pod p_pod;
  
  static char p_char_2;
  static int p_int_2;
  static double p_double_2;
  static bool p_bool_2;
  static int * p_pointer_2;
  static test_enum p_enum_2;
  static test_pod p_pod_2;
  
  };
  
#endif
