#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include <boost/interval.hpp>

template<class T, class Compare>
struct my_interval {
  typedef boost::interval<T, boost::interval_traits<T, Compare> > type;
};

struct my_function {
  bool operator()() { throw "Uncertain comparison..."; }
};

typedef boost::interval<double> I;
typedef my_interval<double, boost::interval_lib::compare_certainly<double> >::type I_c;
typedef my_interval<double, boost::interval_lib::compare_possibly<double> >::type I_p;
typedef my_interval<double, boost::interval_lib::compare_full<double, my_function> >::type I_f;

#define TEST_EXN_Y(e) \
  try { (void)(e); BOOST_TEST(false); } catch (...) { }

#define TEST_EXN_N(e) \
  try { BOOST_TEST(e); } catch (...) { BOOST_TEST(false); }

int test_main(int, char *[])
{
  // comparisons between [1,2] and [3,4]
  BOOST_TEST(I_c(1,2) < I_c(3,4));
  BOOST_TEST(I_c(1,2) <= I_c(3,4));
  BOOST_TEST(!(I_c(1,2) > I_c(3,4)));
  BOOST_TEST(!(I_c(1,2) >= I_c(3,4)));

  BOOST_TEST(I_c(3,4) > I_c(1,2));
  BOOST_TEST(I_c(3,4) >= I_c(1,2));
  BOOST_TEST(!(I_c(3,4) < I_c(1,2)));
  BOOST_TEST(!(I_c(3,4) <= I_c(1,2)));

  BOOST_TEST(I_p(1,2) < I_p(3,4));
  BOOST_TEST(I_p(1,2) <= I_p(3,4));
  BOOST_TEST(!(I_p(1,2) > I_p(3,4)));
  BOOST_TEST(!(I_p(1,2) >= I_p(3,4)));

  BOOST_TEST(I_p(3,4) > I_p(1,2));
  BOOST_TEST(I_p(3,4) >= I_p(1,2));
  BOOST_TEST(!(I_p(3,4) < I_p(1,2)));
  BOOST_TEST(!(I_p(3,4) <= I_p(1,2)));

  TEST_EXN_N(I_f(1,2) < I_f(3,4)) ;
  TEST_EXN_N(I_f(1,2) <= I_f(3,4));
  TEST_EXN_N(!(I_f(1,2) > I_f(3,4)));
  TEST_EXN_N(!(I_f(1,2) >= I_f(3,4)));

  TEST_EXN_N(I_f(3,4) > I_f(1,2));
  TEST_EXN_N(I_f(3,4) >= I_f(1,2));
  TEST_EXN_N(!(I_f(3,4) < I_f(1,2)));
  TEST_EXN_N(!(I_f(3,4) <= I_f(1,2)));

  BOOST_TEST(cerlt(I(1,2), I(3,4)));
  BOOST_TEST(cerle(I(1,2), I(3,4)));
  BOOST_TEST(!cergt(I(1,2), I(3,4)));
  BOOST_TEST(!cerge(I(1,2), I(3,4)));

  BOOST_TEST(!cerlt(I(3,4), I(1,2)));
  BOOST_TEST(!cerle(I(3,4), I(1,2)));
  BOOST_TEST(cergt(I(3,4), I(1,2)));
  BOOST_TEST(cerge(I(3,4), I(1,2)));

  BOOST_TEST(poslt(I(1,2), I(3,4)));
  BOOST_TEST(posle(I(1,2), I(3,4)));
  BOOST_TEST(!posgt(I(1,2), I(3,4)));
  BOOST_TEST(!posge(I(1,2), I(3,4)));

  BOOST_TEST(!poslt(I(3,4), I(1,2)));
  BOOST_TEST(!posle(I(3,4), I(1,2)));
  BOOST_TEST(posgt(I(3,4), I(1,2)));
  BOOST_TEST(posge(I(3,4), I(1,2)));

  // comparisons between [1,3] and [2,4]
  BOOST_TEST(!(I_c(1,3) < I_c(2,4)));
  BOOST_TEST(I_c(1,3) <= I_c(2,4));
  BOOST_TEST(!(I_c(1,3) > I_c(2,4)));
  BOOST_TEST(I_c(1,3) >= I_c(2,4));

  BOOST_TEST(!(I_c(2,4) < I_c(1,3)));
  BOOST_TEST(I_c(2,4) <= I_c(1,3));
  BOOST_TEST(!(I_c(2,4) > I_c(1,3)));
  BOOST_TEST(I_c(2,4) >= I_c(1,3));

  BOOST_TEST(I_p(1,3) < I_p(2,4));
  BOOST_TEST(!(I_p(1,3) <= I_p(2,4)));
  BOOST_TEST(I_p(1,3) > I_p(2,4));
  BOOST_TEST(!(I_p(1,3) >= I_p(2,4)));

  BOOST_TEST(I_p(2,4) < I_p(1,3));
  BOOST_TEST(!(I_p(2,4) <= I_p(1,3)));
  BOOST_TEST(I_p(2,4) > I_p(1,3));
  BOOST_TEST(!(I_p(2,4) >= I_p(1,3)));

  TEST_EXN_Y(I_f(1,3) < I_f(2,4));
  TEST_EXN_Y(I_f(1,3) <= I_f(2,4));
  TEST_EXN_Y(I_f(1,3) > I_f(2,4));
  TEST_EXN_Y(I_f(1,3) >= I_f(2,4));

  TEST_EXN_Y(I_f(2,4) < I_f(1,3));
  TEST_EXN_Y(I_f(2,4) <= I_f(1,3));
  TEST_EXN_Y(I_f(2,4) > I_f(1,3));
  TEST_EXN_Y(I_f(2,4) >= I_f(1,3));

  BOOST_TEST(!cerlt(I(1,3), I(2,4)));
  BOOST_TEST(!cerle(I(1,3), I(2,4)));
  BOOST_TEST(!cergt(I(1,3), I(2,4)));
  BOOST_TEST(!cerge(I(1,3), I(2,4)));

  BOOST_TEST(!cerlt(I(2,4), I(1,3)));
  BOOST_TEST(!cerle(I(2,4), I(1,3)));
  BOOST_TEST(!cergt(I(2,4), I(1,3)));
  BOOST_TEST(!cerge(I(2,4), I(1,3)));

  BOOST_TEST(poslt(I(1,3), I(2,4)));
  BOOST_TEST(posle(I(1,3), I(2,4)));
  BOOST_TEST(posgt(I(1,3), I(2,4)));
  BOOST_TEST(posge(I(1,3), I(2,4)));

  BOOST_TEST(poslt(I(2,4), I(1,3)));
  BOOST_TEST(posle(I(2,4), I(1,3)));
  BOOST_TEST(posgt(I(2,4), I(1,3)));
  BOOST_TEST(posge(I(2,4), I(1,3)));

  // comparisons between [1,2] and [2,3]
  BOOST_TEST(!(I_c(1,2) < I_c(2,3)));
  BOOST_TEST(I_c(1,2) <= I_c(2,3));
  BOOST_TEST(!(I_c(1,2) > I_c(2,3)));
  BOOST_TEST(I_c(1,2) >= I_c(2,3));

  BOOST_TEST(!(I_c(2,3) < I_c(1,2)));
  BOOST_TEST(I_c(2,3) <= I_c(1,2));
  BOOST_TEST(!(I_c(2,3) > I_c(1,2)));
  BOOST_TEST(I_c(2,3) >= I_c(1,2));

  BOOST_TEST(I_p(1,2) < I_p(2,3));
  BOOST_TEST(I_p(1,2) <= I_p(2,3));
  BOOST_TEST(!(I_p(1,2) > I_p(2,3)));
  BOOST_TEST(!(I_p(1,2) >= I_p(2,3)));

  BOOST_TEST(!(I_p(2,3) < I_p(1,2)));
  BOOST_TEST(!(I_p(2,3) <= I_p(1,2)));
  BOOST_TEST(I_p(2,3) > I_p(1,2));
  BOOST_TEST(I_p(2,3) >= I_p(1,2));

  TEST_EXN_Y(I_f(1,2) < I_f(2,3));
  TEST_EXN_N(I_f(1,2) <= I_f(2,3));
  TEST_EXN_N(!(I_f(1,2) > I_f(2,3)));
  TEST_EXN_Y(I_f(1,2) >= I_f(2,3));

  TEST_EXN_N(!(I_f(2,3) < I_f(1,2)));
  TEST_EXN_Y(I_f(2,3) <= I_f(1,2));
  TEST_EXN_Y(I_f(2,3) > I_f(1,2));
  TEST_EXN_N(I_f(2,3) >= I_f(1,2));

  BOOST_TEST(!cerlt(I(1,2), I(2,3)));
  BOOST_TEST(cerle(I(1,2), I(2,3)));
  BOOST_TEST(!cergt(I(1,2), I(2,3)));
  BOOST_TEST(!cerge(I(1,2), I(2,3)));

  BOOST_TEST(!cerlt(I(2,3), I(1,2)));
  BOOST_TEST(!cerle(I(2,3), I(1,2)));
  BOOST_TEST(!cergt(I(2,3), I(1,2)));
  BOOST_TEST(cerge(I(2,3), I(1,2)));

  BOOST_TEST(poslt(I(1,2), I(2,3)));
  BOOST_TEST(posle(I(1,2), I(2,3)));
  BOOST_TEST(!posgt(I(1,2), I(2,3)));
  BOOST_TEST(posge(I(1,2), I(2,3)));

  BOOST_TEST(!poslt(I(2,3), I(1,2)));
  BOOST_TEST(posle(I(2,3), I(1,2)));
  BOOST_TEST(posgt(I(2,3), I(1,2)));
  BOOST_TEST(posge(I(2,3), I(1,2)));

  // equality between [1,2] and [3,4]
  BOOST_TEST(!(I_c(1,2) == I_c(3,4)));
  BOOST_TEST(I_c(1,2) != I_c(3,4));

  BOOST_TEST(!(I_p(1,2) == I_p(3,4)));
  BOOST_TEST(I_p(1,2) != I_p(3,4));

  TEST_EXN_N(!(I_f(1,2) == I_f(3,4)));
  TEST_EXN_N(I_f(1,2) != I_f(3,4));

  BOOST_TEST(!cereq(I_c(1,2), I_c(3,4)));
  BOOST_TEST(cerne(I_c(1,2), I_c(3,4)));

  BOOST_TEST(!poseq(I_c(1,2), I_c(3,4)));
  BOOST_TEST(posne(I_c(1,2), I_c(3,4)));

  // equality between [1,3] and [2,4]
  BOOST_TEST(!(I_c(1,3) == I_c(2,4)));
  BOOST_TEST(I_c(1,3) != I_c(2,4));

  BOOST_TEST(I_p(1,3) == I_p(2,4));
  BOOST_TEST(!(I_p(1,3) != I_p(2,4)));

  TEST_EXN_Y(I_f(1,3) == I_f(2,4));
  TEST_EXN_Y(I_f(1,3) != I_f(2,4));

  BOOST_TEST(!cereq(I_c(1,3), I_c(2,4)));
  BOOST_TEST(!cerne(I_c(1,3), I_c(2,4)));

  BOOST_TEST(poseq(I_c(1,3), I_c(2,4)));
  BOOST_TEST(posne(I_c(1,3), I_c(2,4)));

  // equality between [1,2] and [1,2]
  BOOST_TEST(!(I_c(1,2) == I_c(1,2)));
  BOOST_TEST(I_c(1,2) != I_c(1,2));

  BOOST_TEST(I_p(1,2) == I_p(1,2));
  BOOST_TEST(!(I_p(1,2) != I_p(1,2)));

  TEST_EXN_Y(I_f(1,2) == I_f(1,2));
  TEST_EXN_Y(I_f(1,2) != I_f(1,2));

  BOOST_TEST(!cereq(I_c(1,2), I_c(1,2)));
  BOOST_TEST(!cerne(I_c(1,2), I_c(1,2)));

  BOOST_TEST(poseq(I_c(1,2), I_c(1,2)));
  BOOST_TEST(posne(I_c(1,2), I_c(1,2)));

  // equality between [1,1] and [1,1]
  BOOST_TEST(I_c(1,1) == I_c(1,1));
  BOOST_TEST(!(I_c(1,1) != I_c(1,1)));

  BOOST_TEST(I_p(1,1) == I_p(1,1));
  BOOST_TEST(!(I_p(1,1) != I_p(1,1)));

  TEST_EXN_N(I_f(1,1) == I_f(1,1));
  TEST_EXN_N(!(I_f(1,1) != I_f(1,1)));

  BOOST_TEST(cereq(I_c(1,1), I_c(1,1)));
  BOOST_TEST(!cerne(I_c(1,1), I_c(1,1)));

  BOOST_TEST(poseq(I_c(1,1), I_c(1,1)));
  BOOST_TEST(!posne(I_c(1,1), I_c(1,1)));

  return 0;
}
