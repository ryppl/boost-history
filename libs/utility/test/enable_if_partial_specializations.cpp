//
//    Copyright 2003 � The Trustees of Indiana University.  All rights
//    reserved.
//
//    See the file enable_if_LICENSE for licensing conditions.
//
//    Authors: Jaakko J�rvi (jajarvi at osl.iu.edu)
//             Jeremiah Willcock (jewillco at osl.iu.edu)
//             Andrew Lumsdaine (lums at osl.iu.edu)
//

#include <boost/test/minimal.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

using boost::enable_if_c;
using boost::disable_if_c;
using boost::enable_if;
using boost::disable_if;
using boost::is_arithmetic;

template <class T, class Enable = void>
struct tester;

template <class T>
struct tester<T, typename enable_if_c<is_arithmetic<T>::value>::type> {
  BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class T>
struct tester<T, typename disable_if_c<is_arithmetic<T>::value>::type> {
  BOOST_STATIC_CONSTANT(bool, value = false);
};

template <class T, class Enable = void>
struct tester2;

template <class T>
struct tester2<T, typename enable_if<is_arithmetic<T> >::type> {
  BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class T>
struct tester2<T, typename disable_if<is_arithmetic<T> >::type> {
  BOOST_STATIC_CONSTANT(bool, value = false);
};

int test_main(int, char*[])
{
 
  BOOST_TEST(tester<int>::value);
  BOOST_TEST(tester<double>::value);

  BOOST_TEST(!tester<char*>::value);
  BOOST_TEST(!tester<void*>::value);

  BOOST_TEST(tester2<int>::value);
  BOOST_TEST(tester2<double>::value);

  BOOST_TEST(!tester2<char*>::value);
  BOOST_TEST(!tester2<void*>::value);

  return 0;
}

