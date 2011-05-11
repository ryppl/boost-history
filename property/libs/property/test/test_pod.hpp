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
  
  char a_char;
  int an_int;
  double a_double;
  bool a_bool;
  };

#endif
