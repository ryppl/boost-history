#ifndef BOOST_INTERVAL_DETAIL_BUGS
#define BOOST_INTERVAL_DETAIL_BUGS

#if defined(__GLIBC__) && !defined(__GLIBCPP__) && (defined(__USE_MISC) || defined(__USE_XOPEN_EXTENDED) || defined(__USE_ISOC99))
#define BOOST_HAVE_INV_HYPERBOLIC
#endif

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std {
  using ::min;
  using ::max;
  using ::sqrt;
  using ::exp;
  using ::log;
  using ::cos;
  using ::tan;
  using ::asin;
  using ::acos;
  using ::atan;
  using ::ceil;
  using ::floor;
  using ::sinh;
  using ::cosh;
  using ::tanh;
#ifdef BOOST_HAVE_INV_HYPERBOLIC
  using ::asinh;
  using ::acosh;
  using ::atanh;
#endif
} // namespace std
#endif

#if __GNUC__ <= 2
// cf PR c++/1981 for a description of the bug
namespace boost {
  using std::min;
  using std::max;
  using std::sqrt;
  using std::exp;
  using std::log;
  using std::cos;
  using std::tan;
  using std::asin;
  using std::acos;
  using std::atan;
  using std::ceil;
  using std::floor;
  using std::sinh;
  using std::cosh;
  using std::tanh;
#if defined(BOOST_HAVE_INV_HYPERBOLIC)
  using std::asinh;
  using std::acosh;
  using std::atanh;
#endif
} // namespace boost
#endif

#endif // BOOST_INTERVAL_DETAIL_BUGS
