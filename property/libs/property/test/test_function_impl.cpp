#include "test_function_impl.hpp"
#include "test_global_data.hpp"

char p_function_char_read() 
  {
  return gld_char;
  }
  
void p_function_char_write(char data) 
  {
  gld_char = data;
  }
  
int p_function_int_read() 
  {
  return gld_int;
  }
  
void p_function_int_write(int data) 
  {
  gld_int = data;
  }
  
double p_function_double_read() 
  {
  return gld_double;
  }
  
void p_function_double_write(double data) 
  {
  gld_double = data;
  }
  
bool p_function_bool_read() 
  {
  return gld_bool;
  }
  
void p_function_bool_write(bool data) 
  {
  gld_bool = data;
  }
  
int * p_function_pointer_read() 
  {
  return gld_pointer;
  }
  
void p_function_pointer_write(int * data) 
  {
  gld_pointer = data;
  }
  
test_enum p_function_enum_read() 
  {
  return gld_enum;
  }
  
void p_function_enum_write(test_enum data) 
  {
  gld_enum = data;
  }
  
test_pod p_function_class_read() 
  {
  return gld_pod;
  }
  
void p_function_class_write(test_pod data) 
  {
  gld_pod = data;
  }

char p_function_class::p_char_read() 
  {
  return st_char;
  }
  
void p_function_class::p_char_write(char data) 
  {
  st_char = data;
  }
  
int p_function_class::p_int_read() 
  {
  return st_int;
  }
  
void p_function_class::p_int_write(int data) 
  {
  st_int = data;
  }
  
double p_function_class::p_double_read() 
  {
  return st_double;
  }
  
void p_function_class::p_double_write(double data) 
  {
  data = st_double;
  }
  
bool p_function_class::p_bool_read() 
  {
  return st_bool;
  }
  
void p_function_class::p_bool_write(bool data) 
  {
  st_bool = data;
  }
  
int * p_function_class::p_pointer_read() 
  {
  return st_pointer; 
  }
  
void p_function_class::p_pointer_write(int * data) 
  {
  st_pointer = data;
  }
  
test_enum p_function_class::p_enum_read() 
  {
  return st_enum;
  }
  
void p_function_class::p_enum_write(test_enum data) 
  {
  st_enum = data;
  }
  
test_pod p_function_class::p_class_read() 
  {
  return st_pod;
  }
  
void p_function_class::p_class_write(test_pod data) 
  {
  st_pod = data;
  }
  
char p_function_class::p_char_read_2() 
  {
  return st_char_2; 
  }
  
void p_function_class::p_char_write_2(char data) 
  {
  st_char_2 = data;
  }
  
int p_function_class::p_int_read_2() 
  {
  return st_int_2;
  }
  
void p_function_class::p_int_write_2(int data) 
  {
  st_int_2 = data;
  }
  
double p_function_class::p_double_read_2() 
  {
  return st_double_2;
  }
  
void p_function_class::p_double_write_2(double data) 
  {
  st_double_2 = data;
  }
  
bool p_function_class::p_bool_read_2() 
  {
  return st_bool_2;
  }
  
void p_function_class::p_bool_write_2(bool data) 
  {
  st_bool_2 = data;
  }
  
int * p_function_class::p_pointer_read_2() 
  {
  return st_pointer_2;
  }
  
void p_function_class::p_pointer_write_2(int * data) 
  {
  st_pointer_2 = data;
  }
  
test_enum p_function_class::p_enum_read_2() 
  {
  return st_enum_2;
  }
  
void p_function_class::p_enum_write_2(test_enum data) 
  {
  st_enum_2 = data;
  }
  
test_pod p_function_class::p_class_read_2() 
  {
  return st_pod_2;
  }
  
void p_function_class::p_class_write_2(test_pod data) 
  {
  st_pod_2 = data;
  }

char p_function_class::st_char(' ');
int p_function_class::st_int(0);
double p_function_class::st_double(0.0);
bool p_function_class::st_bool(false);
int * p_function_class::st_pointer(0);
test_enum p_function_class::st_enum(e_test_first);
test_pod p_function_class::st_pod;

char p_function_class::st_char_2(' ');
int p_function_class::st_int_2(0);
double p_function_class::st_double_2(0.0);
bool p_function_class::st_bool_2(false);
int * p_function_class::st_pointer_2(0);
test_enum p_function_class::st_enum_2(e_test_first);
test_pod p_function_class::st_pod_2;

#include "test_global_data.cpp"