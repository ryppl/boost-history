#if !defined(TEST_PROPERTY_MEMBER_DATA_HPP)
#define TEST_PROPERTY_MEMBER_DATA_HPP

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
  
  private:
  
  static int st_some_int;
  
  };
  
#endif
