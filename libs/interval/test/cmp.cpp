#include <boost/interval.hpp>
#include <boost/test/test_tools.hpp>

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

// comparisons between [1,2] and [3,4]

static void test_c_12_34() {
  const I_c a(1,2), b(3,4);

  BOOST_TEST(a < b);
  BOOST_TEST(a <= b);
  BOOST_TEST(!(a > b));
  BOOST_TEST(!(a >= b));

  BOOST_TEST(b > a);
  BOOST_TEST(b >= a);
  BOOST_TEST(!(b < a));
  BOOST_TEST(!(b <= a));

  BOOST_TEST(!(a == b));
  BOOST_TEST(a != b);
}

static void test_p_12_34() {
  const I_p a(1,2), b(3,4);

  BOOST_TEST(a < b);
  BOOST_TEST(a <= b);
  BOOST_TEST(!(a > b));
  BOOST_TEST(!(a >= b));

  BOOST_TEST(b > a);
  BOOST_TEST(b >= a);
  BOOST_TEST(!(b < a));
  BOOST_TEST(!(b <= a));

  BOOST_TEST(!(a == b));
  BOOST_TEST(a != b);
}

static void test_f_12_34() {
  const I_f a(1,2), b(3,4);

  TEST_EXN_N(a < b) ;
  TEST_EXN_N(a <= b);
  TEST_EXN_N(!(a > b));
  TEST_EXN_N(!(a >= b));

  TEST_EXN_N(b > a);
  TEST_EXN_N(b >= a);
  TEST_EXN_N(!(b < a));
  TEST_EXN_N(!(b <= a));

  TEST_EXN_N(!(a == b));
  TEST_EXN_N(a != b);
}

static void test_12_34() {
  test_c_12_34();
  test_p_12_34();
  test_f_12_34();

  const I a(1,2), b(3,4);

  BOOST_TEST(cerlt(a, b));
  BOOST_TEST(cerle(a, b));
  BOOST_TEST(!cergt(a, b));
  BOOST_TEST(!cerge(a, b));

  BOOST_TEST(!cerlt(b, a));
  BOOST_TEST(!cerle(b, a));
  BOOST_TEST(cergt(b, a));
  BOOST_TEST(cerge(b, a));

  BOOST_TEST(poslt(a, b));
  BOOST_TEST(posle(a, b));
  BOOST_TEST(!posgt(a, b));
  BOOST_TEST(!posge(a, b));

  BOOST_TEST(!poslt(b, a));
  BOOST_TEST(!posle(b, a));
  BOOST_TEST(posgt(b, a));
  BOOST_TEST(posge(b, a));

  BOOST_TEST(!cereq(a, b));
  BOOST_TEST(cerne(a, b));
  BOOST_TEST(!poseq(a, b));
  BOOST_TEST(posne(a, b));
}

// comparisons between [1,3] and [2,4]

static void test_c_13_24() {
  const I_c a(1,3), b(2,4);

  BOOST_TEST(!(a < b));
  BOOST_TEST(a <= b);
  BOOST_TEST(!(a > b));
  BOOST_TEST(a >= b);

  BOOST_TEST(!(b < a));
  BOOST_TEST(b <= a);
  BOOST_TEST(!(b > a));
  BOOST_TEST(b >= a);

  BOOST_TEST(!(a == b));
  BOOST_TEST(a != b);
}

static void test_p_13_24() {
  const I_p a(1,3), b(2,4);

  BOOST_TEST(a < b);
  BOOST_TEST(!(a <= b));
  BOOST_TEST(a > b);
  BOOST_TEST(!(a >= b));

  BOOST_TEST(b < a);
  BOOST_TEST(!(b <= a));
  BOOST_TEST(b > a);
  BOOST_TEST(!(b >= a));

  BOOST_TEST(a == b);
  BOOST_TEST(!(a != b));
}

static void test_f_13_24() {
  const I_f a(1,3), b(2,4);

  TEST_EXN_Y(a < b);
  TEST_EXN_Y(a <= b);
  TEST_EXN_Y(a > b);
  TEST_EXN_Y(a >= b);

  TEST_EXN_Y(b < a);
  TEST_EXN_Y(b <= a);
  TEST_EXN_Y(b > a);
  TEST_EXN_Y(b >= a);

  TEST_EXN_Y(a == b);
  TEST_EXN_Y(a != b);
}

static void test_13_24() {
  test_c_13_24();
  test_p_13_24();
  test_f_13_24();

  const I a(1,3), b(2,4);

  BOOST_TEST(!cerlt(a, b));
  BOOST_TEST(!cerle(a, b));
  BOOST_TEST(!cergt(a, b));
  BOOST_TEST(!cerge(a, b));

  BOOST_TEST(!cerlt(b, a));
  BOOST_TEST(!cerle(b, a));
  BOOST_TEST(!cergt(b, a));
  BOOST_TEST(!cerge(b, a));

  BOOST_TEST(poslt(a, b));
  BOOST_TEST(posle(a, b));
  BOOST_TEST(posgt(a, b));
  BOOST_TEST(posge(a, b));

  BOOST_TEST(poslt(b, a));
  BOOST_TEST(posle(b, a));
  BOOST_TEST(posgt(b, a));
  BOOST_TEST(posge(b, a));

  BOOST_TEST(!cereq(a, b));
  BOOST_TEST(!cerne(a, b));
  BOOST_TEST(poseq(a, b));
  BOOST_TEST(posne(a, b));
}

// comparisons between [1,2] and [2,3]

static void test_c_12_23() {
  const I_c a(1,2), b(2,3);

  BOOST_TEST(!(a < b));
  BOOST_TEST(a <= b);
  BOOST_TEST(!(a > b));
  BOOST_TEST(a >= b);

  BOOST_TEST(!(b < a));
  BOOST_TEST(b <= a);
  BOOST_TEST(!(b > a));
  BOOST_TEST(b >= a);

  BOOST_TEST(!(a == b));
  BOOST_TEST(a != b);
}

static void test_p_12_23() {
  const I_p a(1,2), b(2,3);

  BOOST_TEST(a < b);
  BOOST_TEST(a <= b);
  BOOST_TEST(!(a > b));
  BOOST_TEST(!(a >= b));

  BOOST_TEST(!(b < a));
  BOOST_TEST(!(b <= a));
  BOOST_TEST(b > a);
  BOOST_TEST(b >= a);

  BOOST_TEST(a == b);
  BOOST_TEST(!(a != b));
}

static void test_f_12_23() {
  const I_f a(1,2), b(2,3);

  TEST_EXN_Y(a < b);
  TEST_EXN_N(a <= b);
  TEST_EXN_N(!(a > b));
  TEST_EXN_Y(a >= b);

  TEST_EXN_N(!(b < a));
  TEST_EXN_Y(b <= a);
  TEST_EXN_Y(b > a);
  TEST_EXN_N(b >= a);

  TEST_EXN_Y(a == b);
  TEST_EXN_Y(a != b);
}

static void test_12_23() {
  test_c_12_23();
  test_p_12_23();
  test_f_12_23();

  const I a(1,2), b(2,3);

  BOOST_TEST(!cerlt(a, b));
  BOOST_TEST(cerle(a, b));
  BOOST_TEST(!cergt(a, b));
  BOOST_TEST(!cerge(a, b));

  BOOST_TEST(!cerlt(b, a));
  BOOST_TEST(!cerle(b, a));
  BOOST_TEST(!cergt(b, a));
  BOOST_TEST(cerge(b, a));

  BOOST_TEST(poslt(a, b));
  BOOST_TEST(posle(a, b));
  BOOST_TEST(!posgt(a, b));
  BOOST_TEST(posge(a, b));

  BOOST_TEST(!poslt(b, a));
  BOOST_TEST(posle(b, a));
  BOOST_TEST(posgt(b, a));
  BOOST_TEST(posge(b, a));

  BOOST_TEST(!cereq(a, b));
  BOOST_TEST(!cerne(a, b));
  BOOST_TEST(poseq(a, b));
  BOOST_TEST(posne(a, b));
}


int test_main(int, char *[])
{
  test_12_34();
  test_13_24();
  test_12_23();

  { I_c a(1,2), b(1,2);
    BOOST_TEST(!(a == b));
    BOOST_TEST(a != b); }

  { I_p a(1,2), b(1,2);
    BOOST_TEST(a == b);
    BOOST_TEST(!(a != b)); }

  { I_f a(1,2), b(1,2);
    TEST_EXN_Y(a == b);
    TEST_EXN_Y(a != b); }

  { I a(1,2), b(1,2);
    BOOST_TEST(!cereq(a, b));
    BOOST_TEST(!cerne(a, b));
    BOOST_TEST(poseq(a, b));
    BOOST_TEST(posne(a, b)); }

  { I_c a(1,1), b(1,1);
    BOOST_TEST(a == b);
    BOOST_TEST(!(a != b)); }

  { I_p a(1,1), b(1,1);
    BOOST_TEST(a == b);
    BOOST_TEST(!(a != b)); }

  { I_f a(1,1), b(1,1);
    TEST_EXN_N(a == b);
    TEST_EXN_N(!(a != b)); }

  { I a(1,1), b(1,1);
    BOOST_TEST(cereq(a, b));
    BOOST_TEST(!cerne(a, b));
    BOOST_TEST(poseq(a, b));
    BOOST_TEST(!posne(a, b)); }

  return 0;
}
