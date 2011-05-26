#include <boost/detail/lightweight_test.hpp>
#include "test_functor_value.hpp"
#include "test_global_data.hpp"

prop_functor<char,p_functor_class_char> p_gl_char(pf_gl_char);
prop_functor<int,p_functor_class_int> p_gl_int(pf_gl_int);
prop_functor<double,p_functor_class_double> p_gl_double(pf_gl_double);
prop_functor<bool,p_functor_class_bool,write_tag> p_gl_bool_write(pf_gl_bool);
prop_functor<int *,p_functor_class_pointer> p_gl_pointer(pf_gl_pointer);
prop_functor<test_enum,p_functor_class_enum,read_tag> p_gl_enum_const(pf_gl_enum);
prop_functor<test_pod const,p_functor_class_pod> p_gl_pod_const(pf_gl_pod);

p_functor_value_class::p_functor_value_class() :
  p_char(pf_char),
  p_int(pf_int),
  p_double_const(pf_double),
  p_bool_const(pf_bool),
  p_pointer_const(pf_pointer),
  p_enum(pf_enum),
  p_pod_const(pf_pod),
  pf_double(672.56),
  pf_bool(true),
  pf_pointer(&pvalue),
  pf_pod(test_pod('&',9872,98.763,true))
  {
  }

prop_functor<char,p_functor_class_char> p_functor_value_class::p_st_char(pf_st_char);
prop_functor<int,p_functor_class_int,read_tag> p_functor_value_class::p_st_int_const(pf_st_int);
prop_functor<double,p_functor_class_double> p_functor_value_class::p_st_double(pf_st_double);
prop_functor<bool,p_functor_class_bool> p_functor_value_class::p_st_bool(pf_st_bool);
prop_functor<int *,p_functor_class_pointer> p_functor_value_class::p_st_pointer(pf_st_pointer);
prop_functor<test_enum,p_functor_class_enum,read_tag> p_functor_value_class::p_st_enum_const(pf_st_enum);
prop_functor<test_pod,p_functor_class_pod,write_tag> p_functor_value_class::p_st_pod_write(pf_st_pod);
int p_functor_value_class::pvalue(801);

void TestCharacter(p_functor_value_class & tcl) 
  {
  }
  
void TestInt(p_functor_value_class & tcl) 
  {
  }
  
void TestDouble(p_functor_value_class & tcl) 
  {
  }
  
void TestBool(p_functor_value_class & tcl) 
  {
  }
  
void TestPointer(p_functor_value_class & tcl) 
  {
  }
  
void TestEnum(p_functor_value_class & tcl) 
  {
  }
  
void TestPod(p_functor_value_class & tcl) 
  {
  }

int main()
  {
  
  p_functor_value_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestBool(tcl);
  TestPointer(tcl);
  TestEnum(tcl);
  TestPod(tcl);
  
  return boost::report_errors();
  
  }

p_functor_class_char pf_gl_char;
p_functor_class_int pf_gl_int;
p_functor_class_double pf_gl_double;
p_functor_class_bool pf_gl_bool;
p_functor_class_pointer pf_gl_pointer;
p_functor_class_enum pf_gl_enum(e_test_second);
p_functor_class_pod pf_gl_pod((test_pod('Z',455,2.34,true)));

p_functor_class_char p_functor_value_class::pf_st_char;
p_functor_class_int p_functor_value_class::pf_st_int(89);
p_functor_class_double p_functor_value_class::pf_st_double;
p_functor_class_bool p_functor_value_class::pf_st_bool;
p_functor_class_pointer p_functor_value_class::pf_st_pointer;
p_functor_class_enum p_functor_value_class::pf_st_enum(e_test_third);
p_functor_class_pod p_functor_value_class::pf_st_pod;
  
#include "test_global_data.cpp"
#include "test_functor_impl.cpp"
