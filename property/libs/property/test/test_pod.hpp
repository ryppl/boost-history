#if !defined(TEST_POD_HPP)
#define TEST_POD_HPP

struct test_pod
  {
  
  test_pod() :
    a_char(' '),
    an_int(0),
    a_double(0),
    a_bool(false)
    {
    }
    
  test_pod(char c,int i,double d,bool b) :
    a_char(c),
    an_int(i),
    a_double(d),
    a_bool(b)
    {
    }
  
  char a_char;
  int an_int;
  double a_double;
  bool a_bool;
  };

#endif
