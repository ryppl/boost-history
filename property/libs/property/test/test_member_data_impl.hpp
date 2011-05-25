#if !defined(TEST_MEMBER_DATA_IMPL_HPP)
#define TEST_MEMBER_DATA_IMPL_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

class p_member_data_class
  {
  
  public:
  
  p_member_data_class();
  
  char p_data_char;
  int p_data_int;
  double p_data_double;
  bool p_data_bool;
  int * p_data_pointer;
  test_enum p_data_enum;
  test_pod p_data_pod;
  
  char p_char;
  int p_int;
  double p_double;
  bool p_bool;
  int * p_pointer;
  test_enum p_enum;
  test_pod p_pod;
  
  char p_char_2;
  int p_int_2;
  double p_double_2;
  bool p_bool_2;
  int * p_pointer_2;
  test_enum p_enum_2;
  test_pod p_pod_2;
  
  };
  
#endif
