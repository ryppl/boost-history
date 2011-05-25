#include "test_member_function_impl.hpp"

p_member_function_impl_class::p_member_function_impl_class() :
  st_char(' '),
  st_int(0),
  st_double(0.0),
  st_bool(false),
  st_pointer(0),
  st_enum(e_test_default),
  st_char_2(' '),
  st_int_2(0),
  st_double_2(0.0),
  st_bool_2(false),
  st_pointer_2(0),
  st_enum_2(e_test_default),
  st_char_3(' '),
  st_int_3(0),
  st_double_3(0.0),
  st_bool_3(false),
  st_pointer_3(0),
  st_enum_3(e_test_default)
  {
  }
  
char p_member_function_impl_class::p_function_char_read() 
  {
  return st_char;
  }
  
void p_member_function_impl_class::p_function_char_write(char data)
  {
  st_char = data;
  }
  
int p_member_function_impl_class::p_function_int_read() 
  {
  return st_int;
  }
  
void p_member_function_impl_class::p_function_int_write(int data) 
  {
  st_int = data;
  }
  
double p_member_function_impl_class::p_function_double_read() 
  {
  return st_double;
  }
  
void p_member_function_impl_class::p_function_double_write(double data) 
  {
  st_double = data;
  }
  
bool p_member_function_impl_class::p_function_bool_read() 
  {
  return st_bool;
  }
  
void p_member_function_impl_class::p_function_bool_write(bool data) 
  {
  st_bool = data;
  }
  
int * p_member_function_impl_class::p_function_pointer_read() 
  {
  return st_pointer;
  }
  
void p_member_function_impl_class::p_function_pointer_write(int * data) 
  {
  st_pointer = data;
  }
  
test_enum p_member_function_impl_class::p_function_enum_read() 
  {
  return st_enum;
  }
  
void p_member_function_impl_class::p_function_enum_write(test_enum data) 
  {
  st_enum = data;
  }
  
test_pod p_member_function_impl_class::p_function_pod_read() 
  {
  return st_pod;
  }
  
void p_member_function_impl_class::p_function_pod_write(test_pod data) 
  {
  st_pod = data;
  }
  
char p_member_function_impl_class::p_char_read() 
  {
  return st_char_3;
  }
  
void p_member_function_impl_class::p_char_write(char data) 
  {
  st_char_3 = data;
  }
  
int p_member_function_impl_class::p_int_read() 
  {
  return st_int_3;
  }
  
void p_member_function_impl_class::p_int_write(int data) 
  {
  st_int_3 = data;
  }
  
double p_member_function_impl_class::p_double_read() 
  {
  return st_double_3;
  }
  
void p_member_function_impl_class::p_double_write(double data) 
  {
  st_double_3 = data;
  }
  
bool p_member_function_impl_class::p_bool_read() 
  {
  return st_bool_3;
  }
  
void p_member_function_impl_class::p_bool_write(bool data) 
  {
  st_bool_3 = data;
  }
  
int * p_member_function_impl_class::p_pointer_read() 
  {
  return st_pointer_3;
  }
  
void p_member_function_impl_class::p_pointer_write(int * data) 
  {
  st_pointer_3 = data;
  }
  
test_enum p_member_function_impl_class::p_enum_read() 
  {
  return st_enum_3;
  }
  
void p_member_function_impl_class::p_enum_write(test_enum data) 
  {
  st_enum_3 = data;
  }
  
test_pod p_member_function_impl_class::p_pod_read() 
  {
  return st_pod_3;
  }
  
void p_member_function_impl_class::p_pod_write(test_pod data) 
  {
  st_pod_3 = data;
  }
  
char p_member_function_impl_class::p_char_read_2() 
  {
  return st_char_2;
  }
  
void p_member_function_impl_class::p_char_write_2(char data) 
  {
  st_char_2 = data;
  }
  
int p_member_function_impl_class::p_int_read_2() 
  {
  return st_int_2;
  }
  
void p_member_function_impl_class::p_int_write_2(int data) 
  {
  st_int_2 = data;
  }
  
double p_member_function_impl_class::p_double_read_2() 
  {
  return st_double_2;
  }
  
void p_member_function_impl_class::p_double_write_2(double data) 
  {
  st_double_2 = data;
  }
  
bool p_member_function_impl_class::p_bool_read_2() 
  {
  return st_bool_2;
  }
  
void p_member_function_impl_class::p_bool_write_2(bool data) 
  {
  st_bool_2 = data;
  }
  
int * p_member_function_impl_class::p_pointer_read_2() 
  {
  return st_pointer_2;
  }
  
void p_member_function_impl_class::p_pointer_write_2(int * data) 
  {
  st_pointer_2 = data;
  }
  
test_enum p_member_function_impl_class::p_enum_read_2() 
  {
  return st_enum_2;
  }
  
void p_member_function_impl_class::p_enum_write_2(test_enum data) 
  {
  st_enum_2 = data;
  }
  
test_pod p_member_function_impl_class::p_pod_read_2() 
  {
  return st_pod_2;
  }
  
void p_member_function_impl_class::p_pod_write_2(test_pod data) 
  {
  st_pod_2 = data;
  }
