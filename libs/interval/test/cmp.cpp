#include <boost/interval/interval.hpp>
#include <boost/interval/checking.hpp>
#include <boost/interval/compare.hpp>
#include <boost/test/test_tools.hpp>

struct empty_class {};

typedef boost::interval_policies<empty_class,
				 boost::interval_lib::checking_base<int> >
  policies;

typedef boost::interval<int, policies> I;

#define BOOST_C_EXN(e) \
  BOOST_CHECK_THROW(e, boost::interval_lib::comparison_error)

// comparisons between [1,2] and [3,4]

static void test_i_12_34() {
  const I a(1,2), b(3,4);

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(b > a);
  BOOST_CHECK(b >= a);
  BOOST_CHECK(!(b < a));
  BOOST_CHECK(!(b <= a));

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

static void test_e_12_34() {
  using namespace boost::interval_lib;
  const I a(1,2), b(3,4);

  BOOST_CHECK(cerlt(a, b));
  BOOST_CHECK(cerle(a, b));
  BOOST_CHECK(!cergt(a, b));
  BOOST_CHECK(!cerge(a, b));

  BOOST_CHECK(!cerlt(b, a));
  BOOST_CHECK(!cerle(b, a));
  BOOST_CHECK(cergt(b, a));
  BOOST_CHECK(cerge(b, a));

  BOOST_CHECK(poslt(a, b));
  BOOST_CHECK(posle(a, b));
  BOOST_CHECK(!posgt(a, b));
  BOOST_CHECK(!posge(a, b));

  BOOST_CHECK(!poslt(b, a));
  BOOST_CHECK(!posle(b, a));
  BOOST_CHECK(posgt(b, a));
  BOOST_CHECK(posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(cerne(a, b));
  BOOST_CHECK(!poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(cerne(b, a));
  BOOST_CHECK(!poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_12_34() {
  test_i_12_34();
  test_e_12_34();
}

// comparisons between [1,3] and [2,4]

static void test_i_13_24() {
  const I a(1,3), b(2,4);

  BOOST_C_EXN(a < b);
  BOOST_C_EXN(a <= b);
  BOOST_C_EXN(a > b);
  BOOST_C_EXN(a >= b);

  BOOST_C_EXN(b < a);
  BOOST_C_EXN(b <= a);
  BOOST_C_EXN(b > a);
  BOOST_C_EXN(b >= a);

  BOOST_C_EXN(a == b);
  BOOST_C_EXN(a != b);
}

static void test_e_13_24() {
  using namespace boost::interval_lib;
  const I a(1,3), b(2,4);

  BOOST_CHECK(!cerlt(a, b));
  BOOST_CHECK(!cerle(a, b));
  BOOST_CHECK(!cergt(a, b));
  BOOST_CHECK(!cerge(a, b));

  BOOST_CHECK(!cerlt(b, a));
  BOOST_CHECK(!cerle(b, a));
  BOOST_CHECK(!cergt(b, a));
  BOOST_CHECK(!cerge(b, a));

  BOOST_CHECK(poslt(a, b));
  BOOST_CHECK(posle(a, b));
  BOOST_CHECK(posgt(a, b));
  BOOST_CHECK(posge(a, b));

  BOOST_CHECK(poslt(b, a));
  BOOST_CHECK(posle(b, a));
  BOOST_CHECK(posgt(b, a));
  BOOST_CHECK(posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_13_24() {
  test_i_13_24();
  test_e_13_24();
}

// comparisons between [1,2] and [2,3]

static void test_i_12_23() {
  const I a(1,2), b(2,3);

  BOOST_C_EXN(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_C_EXN(a >= b);

  BOOST_CHECK(!(b < a));
  BOOST_C_EXN(b <= a);
  BOOST_C_EXN(b > a);
  BOOST_CHECK(b >= a);

  BOOST_C_EXN(a == b);
  BOOST_C_EXN(a != b);
}

static void test_e_12_23() {
  using namespace boost::interval_lib;
  const I a(1,2), b(2,3);

  BOOST_CHECK(!cerlt(a, b));
  BOOST_CHECK(cerle(a, b));
  BOOST_CHECK(!cergt(a, b));
  BOOST_CHECK(!cerge(a, b));

  BOOST_CHECK(!cerlt(b, a));
  BOOST_CHECK(!cerle(b, a));
  BOOST_CHECK(!cergt(b, a));
  BOOST_CHECK(cerge(b, a));

  BOOST_CHECK(poslt(a, b));
  BOOST_CHECK(posle(a, b));
  BOOST_CHECK(!posgt(a, b));
  BOOST_CHECK(posge(a, b));

  BOOST_CHECK(!poslt(b, a));
  BOOST_CHECK(posle(b, a));
  BOOST_CHECK(posgt(b, a));
  BOOST_CHECK(posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_12_23() {
  test_i_12_23();
  test_e_12_23();
}

static void test_12_E() {
  I a(1, 2), b(I::empty());
  
  BOOST_C_EXN(a < b);
  BOOST_C_EXN(a <= b);
  BOOST_C_EXN(a > b);
  BOOST_C_EXN(a >= b);

  BOOST_C_EXN(b < a);
  BOOST_C_EXN(b <= a);
  BOOST_C_EXN(b > a);
  BOOST_C_EXN(b >= a);

  BOOST_C_EXN(a == b);
  BOOST_C_EXN(a != b);
}

// comparisons between [1,2] and 0

static void test_i_12_0() {
  const I a(1,2);
  const int b = 0;

  BOOST_CHECK(!(a < b));
  BOOST_CHECK(!(a <= b));
  BOOST_CHECK(a > b);
  BOOST_CHECK(a >= b);

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

static void test_e_12_0() {
  using namespace boost::interval_lib;
  const I a(1,2);
  const int b = 0;

  BOOST_CHECK(!cerlt(a, b));
  BOOST_CHECK(!cerle(a, b));
  BOOST_CHECK(cergt(a, b));
  BOOST_CHECK(cerge(a, b));

  BOOST_CHECK(cerlt(b, a));
  BOOST_CHECK(cerle(b, a));
  BOOST_CHECK(!cergt(b, a));
  BOOST_CHECK(!cerge(b, a));

  BOOST_CHECK(!poslt(a, b));
  BOOST_CHECK(!posle(a, b));
  BOOST_CHECK(posgt(a, b));
  BOOST_CHECK(posge(a, b));

  BOOST_CHECK(poslt(b, a));
  BOOST_CHECK(posle(b, a));
  BOOST_CHECK(!posgt(b, a));
  BOOST_CHECK(!posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(cerne(a, b));
  BOOST_CHECK(!poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(cerne(b, a));
  BOOST_CHECK(!poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_12_0() {
  test_i_12_0();
  test_e_12_0();
}

// comparisons between [1,2] and 1

static void test_i_12_1() {
  const I a(1,2);
  const int b = 1;

  BOOST_CHECK(!(a < b));
  BOOST_C_EXN(a <= b);
  BOOST_C_EXN(!(a > b));
  BOOST_CHECK(a >= b);

  BOOST_C_EXN(a == b);
  BOOST_C_EXN(a != b);
}

static void test_e_12_1() {
  using namespace boost::interval_lib;
  const I a(1,2);
  const int b = 1;

  BOOST_CHECK(!cerlt(a, b));
  BOOST_CHECK(!cerle(a, b));
  BOOST_CHECK(!cergt(a, b));
  BOOST_CHECK(cerge(a, b));

  BOOST_CHECK(!cerlt(b, a));
  BOOST_CHECK(cerle(b, a));
  BOOST_CHECK(!cergt(b, a));
  BOOST_CHECK(!cerge(b, a));

  BOOST_CHECK(!poslt(a, b));
  BOOST_CHECK(posle(a, b));
  BOOST_CHECK(posgt(a, b));
  BOOST_CHECK(posge(a, b));

  BOOST_CHECK(poslt(b, a));
  BOOST_CHECK(posle(b, a));
  BOOST_CHECK(!posgt(b, a));
  BOOST_CHECK(posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_12_1() {
  test_i_12_1();
  test_e_12_1();
}

// comparisons between [1,2] and 2

static void test_i_12_2() {
  const I a(1,2);
  const int b = 2;

  BOOST_C_EXN(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_C_EXN(a >= b);

  BOOST_C_EXN(a == b);
  BOOST_C_EXN(a != b);
}

static void test_e_12_2() {
  using namespace boost::interval_lib;
  const I a(1,2);
  const int b = 2;

  BOOST_CHECK(!cerlt(a, b));
  BOOST_CHECK(cerle(a, b));
  BOOST_CHECK(!cergt(a, b));
  BOOST_CHECK(!cerge(a, b));

  BOOST_CHECK(!cerlt(b, a));
  BOOST_CHECK(!cerle(b, a));
  BOOST_CHECK(!cergt(b, a));
  BOOST_CHECK(cerge(b, a));

  BOOST_CHECK(poslt(a, b));
  BOOST_CHECK(posle(a, b));
  BOOST_CHECK(!posgt(a, b));
  BOOST_CHECK(posge(a, b));

  BOOST_CHECK(!poslt(b, a));
  BOOST_CHECK(posle(b, a));
  BOOST_CHECK(posgt(b, a));
  BOOST_CHECK(posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_12_2() {
  test_i_12_2();
  test_e_12_2();
}

// comparisons between [1,2] and 3

static void test_i_12_3() {
  const I a(1,2);
  const int b = 3;

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

static void test_e_12_3() {
  using namespace boost::interval_lib;
  const I a(1,2);
  const int b = 3;

  BOOST_CHECK(cerlt(a, b));
  BOOST_CHECK(cerle(a, b));
  BOOST_CHECK(!cergt(a, b));
  BOOST_CHECK(!cerge(a, b));

  BOOST_CHECK(!cerlt(b, a));
  BOOST_CHECK(!cerle(b, a));
  BOOST_CHECK(cergt(b, a));
  BOOST_CHECK(cerge(b, a));

  BOOST_CHECK(poslt(a, b));
  BOOST_CHECK(posle(a, b));
  BOOST_CHECK(!posgt(a, b));
  BOOST_CHECK(!posge(a, b));

  BOOST_CHECK(!poslt(b, a));
  BOOST_CHECK(!posle(b, a));
  BOOST_CHECK(posgt(b, a));
  BOOST_CHECK(posge(b, a));

  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(cerne(a, b));
  BOOST_CHECK(!poseq(a, b));
  BOOST_CHECK(posne(a, b));

  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(cerne(b, a));
  BOOST_CHECK(!poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_12_3() {
  test_i_12_3();
  test_e_12_3();
}

static void test_12_12() {
  const I a(1,2), b(1,2);
  BOOST_C_EXN(a == b);
  BOOST_C_EXN(a != b);
  BOOST_CHECK(!cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(posne(a, b));
  BOOST_CHECK(!cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(posne(b, a));
}

static void test_11_11() {
  const I a(1,1), b(1,1);
  BOOST_CHECK(a == b);
  BOOST_CHECK(!(a != b));
  BOOST_CHECK(cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(!posne(a, b));
  BOOST_CHECK(cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(!posne(b, a));
}

static void test_11_1() {
  const I a(1,1);
  const int b = 1;
  BOOST_CHECK(a == b);
  BOOST_CHECK(!(a != b));
  BOOST_CHECK(cereq(a, b));
  BOOST_CHECK(!cerne(a, b));
  BOOST_CHECK(poseq(a, b));
  BOOST_CHECK(!posne(a, b));
  BOOST_CHECK(cereq(b, a));
  BOOST_CHECK(!cerne(b, a));
  BOOST_CHECK(poseq(b, a));
  BOOST_CHECK(!posne(b, a));
}

int test_main(int, char *[]) {
  test_12_34();
  test_13_24();
  test_12_23();
  test_12_E();
  test_12_0();
  test_12_1();
  test_12_2();
  test_12_3();
  test_12_12();
  test_11_11();
  test_11_1();

  return 0;
}
