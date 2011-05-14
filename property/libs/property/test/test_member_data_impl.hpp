#if !defined(TEST_MEMBER_DATA_HPP)
#define TEST_MEMBER_DATA_HPP

#include "test_enum.hpp"
#include "test_pod.hpp"

class p_member_data_class
  {
  
  public:
  
  p_member_data_class();
  
  char p_char;
  int p_int;
  double p_double;
  bool p_bool;
  int * p_pointer;
  test_enum p_enum;
  test_pod p_class;
  
  };
  
#endif
