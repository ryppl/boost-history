#include <boost/detail/lightweight_test.hpp>
#include <boost/bind.hpp>
#include "test_callable_value.hpp"
#include "test_function_impl.hpp"
#include "test_functor_impl.hpp"
#include "test_member_function_impl.hpp"

p_member_function_impl_class p_member_function_impl_object;

prop_callable<char> p_gl_char
  (
  (boost::bind(&p_member_function_impl_class::p_function_char_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_char_write,&p_member_function_impl_object,_1))
  );
prop_callable<int> p_gl_int(&p_function_int_read,&p_function_int_write);
prop_callable<double const> p_gl_double_const(&p_function_double_read);
prop_callable<bool> p_gl_bool
  (
  (boost::bind(&p_member_function_impl_class::p_function_bool_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_bool_write,&p_member_function_impl_object,_1))
  );
prop_callable<int *> p_gl_pointer(&p_function_pointer_read,&p_function_pointer_write);
prop_callable<test_enum,read_tag> p_gl_enum_const((p_functor_class_enum()));
prop_callable<test_pod const> p_gl_pod_const(&p_function_pod_read);

p_callable_value_class::p_callable_value_class() :
  p_char(&p_function_char_read,&p_function_char_write),
  p_int_const(p_functor_class_int()),
  p_double(p_functor_class_double(),p_functor_class_double()),
  p_bool_const(p_functor_class_bool()),
  p_pointer_const
    (
    boost::bind(&p_member_function_impl_class::p_function_pointer_read,&p_member_function_impl_object)
    ),
  p_enum_const(&p_function_enum_read),
  p_pod(p_functor_class_pod(),p_functor_class_pod())
  {
  }
  
prop_callable<char> p_callable_value_class::p_st_char
  (
  (p_functor_class_char()),
  (p_functor_class_char())
  );
prop_callable<int> p_callable_value_class::p_st_int
  (
  (boost::bind(&p_member_function_impl_class::p_function_int_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_int_write,&p_member_function_impl_object,_1))
  );
prop_callable<double> p_callable_value_class::p_st_double
  (
  (boost::bind(&p_member_function_impl_class::p_function_double_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_double_write,&p_member_function_impl_object,_1))
  );
prop_callable<bool> p_callable_value_class::p_st_bool
  (
  &p_function_bool_read,
  &p_function_bool_write
  );
prop_callable<int * const> p_callable_value_class::p_st_pointer_const
  (
  (p_functor_class_pointer())
  );
prop_callable<test_enum> p_callable_value_class::p_st_enum
  (
  (boost::bind(&p_member_function_impl_class::p_function_enum_read,&p_member_function_impl_object)),
  (boost::bind(&p_member_function_impl_class::p_function_enum_write,&p_member_function_impl_object,_1))
  );
prop_callable<test_pod const> p_callable_value_class::p_st_pod_const
  (
  (boost::bind(&p_member_function_impl_class::p_function_pod_read,&p_member_function_impl_object))
  );
  
void TestCharacter(p_callable_value_class & tcl) 
  {
  }
  
void TestInt(p_callable_value_class & tcl) 
  {
  }
  
void TestDouble(p_callable_value_class & tcl) 
  {
  }
  
void TestBool(p_callable_value_class & tcl) 
  {
  }
  
void TestPointer(p_callable_value_class & tcl) 
  {
  }
  
void TestEnum(p_callable_value_class & tcl) 
  {
  }
  
void TestPod(p_callable_value_class & tcl) 
  {
  }
  
int main()
  {
  
  p_callable_value_class tcl;
  
  TestCharacter(tcl);
  TestInt(tcl);
  TestDouble(tcl);
  TestBool(tcl);
  TestPointer(tcl);
  TestEnum(tcl);
  TestPod(tcl);
  
  return boost::report_errors();
  
  }

#include "test_global_data.cpp"
#include "test_function_impl.cpp"
#include "test_functor_impl.cpp"
#include "test_member_function_impl.cpp"
