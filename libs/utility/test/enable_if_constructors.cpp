//
//    Copyright 2003 � The Trustees of Indiana University.  All rights
//    reserved.
//
//    See the file enable_if_LICENSE for licensing conditions.
//
//    Authors: Jaakko J�rvi (jajarvi@osl.iu.edu)
//             Jeremiah Willcock (jewillco@osl.iu.edu)
//             Andrew Lumsdaine (lums@osl.iu.edu)
//

#include <boost/test/minimal.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

using boost::enable_if;
using boost::disable_if;
using boost::is_arithmetic;

struct container {
  bool my_value;

  template <class T>
  container(const T&, const typename enable_if<is_arithmetic<T>, T>::type * = 0):
  my_value(true) {}

  template <class T>
  container(const T&, const typename disable_if<is_arithmetic<T>, T>::type * = 0):
  my_value(false) {}
};

// example from Howard Hinnant (tests enable_if template members of a templated class)
template <class charT>
struct xstring
{
  template <class It>
  xstring(It begin, It end, typename 
	 disable_if<is_arithmetic<It> >::type* = 0)
    : data(end-begin) {}
  
  int data;
};


int test_main(int, char*[])
{
 
  BOOST_TEST(container(1).my_value);
  BOOST_TEST(container(1.0).my_value);

  BOOST_TEST(!container("1").my_value);  
  BOOST_TEST(!container(static_cast<void*>(0)).my_value);  

  char sa[] = "123456";
  BOOST_TEST(xstring<char>(sa, sa+6).data == 6);


  return 0;
}

