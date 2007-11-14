//  Copyright John Maddock 2007.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_POLICY_HPP
#define BOOST_MATH_POLICY_HPP

#include <boost/mpl/list.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/math/tools/config.hpp>
#include <limits>
// Sadly we do need the .h versions of these to be sure of getting
// FLT_MANT_DIG etc.
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

namespace boost{ namespace math{

namespace tools{

template <class T>
int digits(BOOST_EXPLICIT_TEMPLATE_TYPE(T));

}

namespace policies{

//
// Define macros for our default policies, if they're not defined already:
//
#ifndef BOOST_MATH_DOMAIN_ERROR_POLICY
#define BOOST_MATH_DOMAIN_ERROR_POLICY throw_on_error
#endif
#ifndef BOOST_MATH_POLE_ERROR_POLICY
#define BOOST_MATH_POLE_ERROR_POLICY throw_on_error
#endif
#ifndef BOOST_MATH_OVERFLOW_ERROR_POLICY
#define BOOST_MATH_OVERFLOW_ERROR_POLICY throw_on_error
#endif
#ifndef BOOST_MATH_EVALUATION_ERROR_POLICY
#define BOOST_MATH_EVALUATION_ERROR_POLICY throw_on_error
#endif
#ifndef BOOST_MATH_UNDERFLOW_ERROR_POLICY
#define BOOST_MATH_UNDERFLOW_ERROR_POLICY ignore_error
#endif
#ifndef BOOST_MATH_DENORM_ERROR_POLICY
#define BOOST_MATH_DENORM_ERROR_POLICY ignore_error
#endif
#ifndef BOOST_MATH_DIGITS10_POLICY
#define BOOST_MATH_DIGITS10_POLICY 0
#endif
#ifndef BOOST_MATH_PROMOTE_FLOAT_POLICY
#define BOOST_MATH_PROMOTE_FLOAT_POLICY true
#endif
#ifndef BOOST_MATH_PROMOTE_DOUBLE_POLICY
#ifdef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#define BOOST_MATH_PROMOTE_DOUBLE_POLICY false
#else
#define BOOST_MATH_PROMOTE_DOUBLE_POLICY true
#endif
#endif
#ifndef BOOST_MATH_DISCRETE_QUANTILE_POLICY
#define BOOST_MATH_DISCRETE_QUANTILE_POLICY integer_round_outwards
#endif
#ifndef BOOST_MATH_ASSERT_UNDEFINED_POLICY
#define BOOST_MATH_ASSERT_UNDEFINED_POLICY true
#endif
#ifndef BOOST_MATH_MAX_SERIES_ITERATION_POLICY
#define BOOST_MATH_MAX_SERIES_ITERATION_POLICY 1000000
#endif
#ifndef BOOST_MATH_MAX_ROOT_ITERATION_POLICY
#define BOOST_MATH_MAX_ROOT_ITERATION_POLICY 200
#endif

#if !defined(__BORLANDC__)
#define BOOST_MATH_META_INT(type, name, Default)\
   template <type N = Default> struct name : public boost::mpl::int_<N>{};\
   namespace detail{\
   template <type N>\
   char test_is_valid_arg(const name<N>*);\
   char test_is_default_arg(const name<Default>*);\
   template <class T> struct is_##name##_imp\
   {\
      template <type N> static char test(const name<N>*);\
      static double test(...);\
      BOOST_STATIC_CONSTANT(bool, value = sizeof(test(static_cast<T*>(0))) == 1);\
   };\
   }\
   template <class T> struct is_##name : public boost::mpl::bool_<detail::is_##name##_imp<T>::value>{};

#define BOOST_MATH_META_BOOL(name, Default)\
   template <bool N = Default> struct name : public boost::mpl::bool_<N>{};\
   namespace detail{\
   template <bool N>\
   char test_is_valid_arg(const name<N>*);\
   char test_is_default_arg(const name<Default>*);\
   template <class T> struct is_##name##_imp\
   {\
      template <bool N> static char test(const name<N>*);\
      static double test(...);\
      BOOST_STATIC_CONSTANT(bool, value = sizeof(test(static_cast<T*>(0))) == 1);\
   };\
   }\
   template <class T> struct is_##name : public boost::mpl::bool_<detail::is_##name##_imp<T>::value>{};
#else
#define BOOST_MATH_META_INT(Type, name, Default)\
   template <Type N = Default> struct name : public boost::mpl::int_<N>{};\
   namespace detail{\
   template <Type N>\
   char test_is_valid_arg(const name<N>*);\
   char test_is_default_arg(const name<Default>*);\
   template <class T> struct is_##name##_tester\
   {\
      template <Type N> static char test(const name<N>&);\
      static double test(...);\
   };\
   template <class T> struct is_##name##_imp\
   {\
      static T inst;\
      BOOST_STATIC_CONSTANT(bool, value = sizeof(detail::is_##name##_tester<T>::test(inst)) == 1);\
   };\
   }\
   template <class T> struct is_##name : public boost::mpl::bool_<detail::is_##name##_imp<T>::value>\
   {\
      template <class U> struct apply{ typedef is_##name<U> type; };\
   };

#define BOOST_MATH_META_BOOL(name, Default)\
   template <bool N = Default> struct name : public boost::mpl::bool_<N>{};\
   namespace detail{\
   template <bool N>\
   char test_is_valid_arg(const name<N>*);\
   char test_is_default_arg(const name<Default>*);\
   template <class T> struct is_##name##_tester\
   {\
      template <bool N> static char test(const name<N>&);\
      static double test(...);\
   };\
   template <class T> struct is_##name##_imp\
   {\
      static T inst;\
      BOOST_STATIC_CONSTANT(bool, value = sizeof(detail::is_##name##_tester<T>::test(inst)) == 1);\
   };\
   }\
   template <class T> struct is_##name : public boost::mpl::bool_<detail::is_##name##_imp<T>::value>\
   {\
      template <class U> struct apply{ typedef is_##name<U> type;  };\
   };
#endif
//
// Begin by defining policy types for error handling:
//
enum error_policy_type
{
   throw_on_error = 0,
   errno_on_error = 1,
   ignore_error = 2,
   user_error = 3
};

BOOST_MATH_META_INT(error_policy_type, domain_error, BOOST_MATH_DOMAIN_ERROR_POLICY)
BOOST_MATH_META_INT(error_policy_type, pole_error, BOOST_MATH_POLE_ERROR_POLICY)
BOOST_MATH_META_INT(error_policy_type, overflow_error, BOOST_MATH_OVERFLOW_ERROR_POLICY)
BOOST_MATH_META_INT(error_policy_type, underflow_error, BOOST_MATH_UNDERFLOW_ERROR_POLICY)
BOOST_MATH_META_INT(error_policy_type, denorm_error, BOOST_MATH_DENORM_ERROR_POLICY)
BOOST_MATH_META_INT(error_policy_type, evaluation_error, BOOST_MATH_EVALUATION_ERROR_POLICY)

//
// Policy types for internal promotion:
//
BOOST_MATH_META_BOOL(promote_float, BOOST_MATH_PROMOTE_FLOAT_POLICY)
BOOST_MATH_META_BOOL(promote_double, BOOST_MATH_PROMOTE_DOUBLE_POLICY)
BOOST_MATH_META_BOOL(assert_undefined, BOOST_MATH_ASSERT_UNDEFINED_POLICY)
//
// Policy types for discrete quantiles:
//
enum discrete_quantile_policy_type
{
   real,
   integer_round_outwards,
   integer_round_inwards,
   integer_round_down,
   integer_round_up,
   integer_round_nearest
};

BOOST_MATH_META_INT(discrete_quantile_policy_type, discrete_quantile, BOOST_MATH_DISCRETE_QUANTILE_POLICY)
//
// Precision:
//
BOOST_MATH_META_INT(int, digits10, BOOST_MATH_DIGITS10_POLICY)
BOOST_MATH_META_INT(int, digits2, 0)
//
// Iterations:
//
BOOST_MATH_META_INT(unsigned long, max_series_iterations, BOOST_MATH_MAX_SERIES_ITERATION_POLICY)
BOOST_MATH_META_INT(unsigned long, max_root_iterations, BOOST_MATH_MAX_ROOT_ITERATION_POLICY)
//
// Define the names for each possible policy:
//
#define BOOST_MATH_PARAMETER(name)\
   BOOST_PARAMETER_TEMPLATE_KEYWORD(name##_name)\
   BOOST_PARAMETER_NAME(name##_name)

struct default_policy{};

namespace detail{
//
// Trait to work out bits precision from digits10 and digits2:
//
template <class Digits10, class Digits2>
struct precision
{
   //
   // Now work out the precision:
   //
   typedef typename mpl::if_c<
      (Digits10::value == 0),
      digits2<0>,
      digits2<((Digits10::value + 1) * 1000L) / 301L>
   >::type digits2_type;
public:
#ifdef __BORLANDC__
   typedef typename mpl::if_c<
      (Digits2::value > ::boost::math::policies::detail::precision<Digits10,Digits2>::digits2_type::value),
      Digits2, digits2_type>::type type;
#else
   typedef typename mpl::if_c<
      (Digits2::value > digits2_type::value),
      Digits2, digits2_type>::type type;
#endif
};

template <class A, class B, bool b>
struct select_result
{
   typedef A type;
};
template <class A, class B>
struct select_result<A, B, false>
{
   typedef typename mpl::deref<B>::type type;
};

template <class Seq, class Pred, class DefaultType>
struct find_arg
{
private:
   typedef typename mpl::find_if<Seq, Pred>::type iter;
   typedef typename mpl::end<Seq>::type end_type;
public:
   typedef typename select_result<
      DefaultType, iter,
      ::boost::is_same<iter, end_type>::value>::type type;
};

double test_is_valid_arg(...);
double test_is_default_arg(...);
char test_is_valid_arg(const default_policy*);
char test_is_default_arg(const default_policy*);

template <class T>
struct is_valid_policy_imp
{
   BOOST_STATIC_CONSTANT(bool, value = sizeof(test_is_valid_arg(static_cast<T*>(0))) == 1);
};

template <class T>
struct is_default_policy_imp
{
   BOOST_STATIC_CONSTANT(bool, value = sizeof(test_is_default_arg(static_cast<T*>(0))) == 1);
};

template <class T> struct is_valid_policy
: public mpl::bool_<
   ::boost::math::policies::detail::is_valid_policy_imp<T>::value>
{};

template <class T> struct is_default_policy
: public mpl::bool_<
   ::boost::math::policies::detail::is_default_policy_imp<T>::value>
{
   template <class U>
   struct apply
   {
      typedef is_default_policy<U> type;
   };
};

template <class Seq, class T, int N>
struct append_N
{
   typedef typename mpl::push_back<Seq, T>::type new_seq;
   typedef typename append_N<new_seq, T, N-1>::type type;
};

template <class Seq, class T>
struct append_N<Seq, T, 0>
{
   typedef Seq type;
};

//
// Traits class to work out what template parameters our default
// policy<> class will have when modified for forwarding:
//
template <bool f, bool d>
struct default_args
{
   typedef promote_float<false> arg1;
   typedef promote_double<false> arg2;
};

template <>
struct default_args<false, false>
{
   typedef default_policy arg1;
   typedef default_policy arg2;
};

template <>
struct default_args<true, false>
{
   typedef promote_float<false> arg1;
   typedef default_policy arg2;
};

template <>
struct default_args<false, true>
{
   typedef promote_double<false> arg1;
   typedef default_policy arg2;
};

typedef default_args<BOOST_MATH_PROMOTE_FLOAT_POLICY, BOOST_MATH_PROMOTE_DOUBLE_POLICY>::arg1 forwarding_arg1;
typedef default_args<BOOST_MATH_PROMOTE_FLOAT_POLICY, BOOST_MATH_PROMOTE_DOUBLE_POLICY>::arg2 forwarding_arg2;

} // detail
//
// Now define the policy type with enough arguments to handle all
// the policies:
//
template <class A1 = default_policy,
          class A2 = default_policy,
          class A3 = default_policy,
          class A4 = default_policy,
          class A5 = default_policy,
          class A6 = default_policy,
          class A7 = default_policy,
          class A8 = default_policy,
          class A9 = default_policy,
          class A10 = default_policy,
          class A11 = default_policy,
          class A12 = default_policy,
          class A13 = default_policy>
struct policy
{
private:
   //
   // Validate all our arguments:
   //
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A1>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A2>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A3>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A4>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A5>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A6>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A7>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A8>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A9>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A10>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A11>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A12>::value);
   BOOST_STATIC_ASSERT(::boost::math::policies::detail::is_valid_policy<A13>::value);
   //
   // Typelist of the arguments:
   //
   typedef mpl::list<A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13> arg_list;

public:
   typedef typename detail::find_arg<arg_list, is_domain_error<mpl::_1>, domain_error<> >::type domain_error_type;
   typedef typename detail::find_arg<arg_list, is_pole_error<mpl::_1>, pole_error<> >::type pole_error_type;
   typedef typename detail::find_arg<arg_list, is_overflow_error<mpl::_1>, overflow_error<> >::type overflow_error_type;
   typedef typename detail::find_arg<arg_list, is_underflow_error<mpl::_1>, underflow_error<> >::type underflow_error_type;
   typedef typename detail::find_arg<arg_list, is_denorm_error<mpl::_1>, denorm_error<> >::type denorm_error_type;
   typedef typename detail::find_arg<arg_list, is_evaluation_error<mpl::_1>, evaluation_error<> >::type evaluation_error_type;
private:
   //
   // Now work out the precision:
   //
   typedef typename detail::find_arg<arg_list, is_digits10<mpl::_1>, digits10<> >::type digits10_type;
   typedef typename detail::find_arg<arg_list, is_digits2<mpl::_1>, digits2<> >::type bits_precision_type;
public:
   typedef typename detail::precision<digits10_type, bits_precision_type>::type precision_type;
   //
   // Internal promotion:
   //
   typedef typename detail::find_arg<arg_list, is_promote_float<mpl::_1>, promote_float<> >::type promote_float_type;
   typedef typename detail::find_arg<arg_list, is_promote_double<mpl::_1>, promote_double<> >::type promote_double_type;
   //
   // Discrete quantiles:
   //
   typedef typename detail::find_arg<arg_list, is_discrete_quantile<mpl::_1>, discrete_quantile<> >::type discrete_quantile_type;
   //
   // Mathematically undefined properties:
   //
   typedef typename detail::find_arg<arg_list, is_assert_undefined<mpl::_1>, discrete_quantile<> >::type assert_undefined_type;
   //
   // Max iterations:
   //
   typedef typename detail::find_arg<arg_list, is_max_series_iterations<mpl::_1>, max_series_iterations<> >::type max_series_iterations_type;
   typedef typename detail::find_arg<arg_list, is_max_root_iterations<mpl::_1>, max_root_iterations<> >::type max_root_iterations_type;
};
//
// These full specializations are defined to reduce the amount of
// template instantiations that have to take place when using the default
// policies, they have quite a large impact on compile times:
//
template <>
struct policy<default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy>
{
public:
   typedef domain_error<> domain_error_type;
   typedef pole_error<> pole_error_type;
   typedef overflow_error<> overflow_error_type;
   typedef underflow_error<> underflow_error_type;
   typedef denorm_error<> denorm_error_type;
   typedef evaluation_error<> evaluation_error_type;
#if BOOST_MATH_DIGITS10_POLICY == 0
   typedef digits2<> precision_type;
#else
   typedef detail::precision<digits10<>, digits2<> >::type precision_type;
#endif
   typedef promote_float<> promote_float_type;
   typedef promote_double<> promote_double_type;
   typedef discrete_quantile<> discrete_quantile_type;
   typedef assert_undefined<> assert_undefined_type;
   typedef max_series_iterations<> max_series_iterations_type;
   typedef max_root_iterations<> max_root_iterations_type;
};

template <>
struct policy<detail::forwarding_arg1, detail::forwarding_arg2, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy, default_policy>
{
public:
   typedef domain_error<> domain_error_type;
   typedef pole_error<> pole_error_type;
   typedef overflow_error<> overflow_error_type;
   typedef underflow_error<> underflow_error_type;
   typedef denorm_error<> denorm_error_type;
   typedef evaluation_error<> evaluation_error_type;
#if BOOST_MATH_DIGITS10_POLICY == 0
   typedef digits2<> precision_type;
#else
   typedef detail::precision<digits10<>, digits2<> >::type precision_type;
#endif
   typedef promote_float<false> promote_float_type;
   typedef promote_double<false> promote_double_type;
   typedef discrete_quantile<> discrete_quantile_type;
   typedef assert_undefined<> assert_undefined_type;
   typedef max_series_iterations<> max_series_iterations_type;
   typedef max_root_iterations<> max_root_iterations_type;
};

template <class Policy,
          class A1 = default_policy,
          class A2 = default_policy,
          class A3 = default_policy,
          class A4 = default_policy,
          class A5 = default_policy,
          class A6 = default_policy,
          class A7 = default_policy,
          class A8 = default_policy,
          class A9 = default_policy,
          class A10 = default_policy,
          class A11 = default_policy,
          class A12 = default_policy,
          class A13 = default_policy>
struct normalise
{
private:
   typedef mpl::list<A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13> arg_list;
   typedef typename detail::find_arg<arg_list, is_domain_error<mpl::_1>, typename Policy::domain_error_type >::type domain_error_type;
   typedef typename detail::find_arg<arg_list, is_pole_error<mpl::_1>, typename Policy::pole_error_type >::type pole_error_type;
   typedef typename detail::find_arg<arg_list, is_overflow_error<mpl::_1>, typename Policy::overflow_error_type >::type overflow_error_type;
   typedef typename detail::find_arg<arg_list, is_underflow_error<mpl::_1>, typename Policy::underflow_error_type >::type underflow_error_type;
   typedef typename detail::find_arg<arg_list, is_denorm_error<mpl::_1>, typename Policy::denorm_error_type >::type denorm_error_type;
   typedef typename detail::find_arg<arg_list, is_evaluation_error<mpl::_1>, typename Policy::evaluation_error_type >::type evaluation_error_type;
   //
   // Now work out the precision:
   //
   typedef typename detail::find_arg<arg_list, is_digits10<mpl::_1>, digits10<> >::type digits10_type;
   typedef typename detail::find_arg<arg_list, is_digits2<mpl::_1>, typename Policy::precision_type >::type bits_precision_type;
   typedef typename detail::precision<digits10_type, bits_precision_type>::type precision_type;
   //
   // Internal promotion:
   //
   typedef typename detail::find_arg<arg_list, is_promote_float<mpl::_1>, typename Policy::promote_float_type >::type promote_float_type;
   typedef typename detail::find_arg<arg_list, is_promote_double<mpl::_1>, typename Policy::promote_double_type >::type promote_double_type;
   //
   // Discrete quantiles:
   //
   typedef typename detail::find_arg<arg_list, is_discrete_quantile<mpl::_1>, typename Policy::discrete_quantile_type >::type discrete_quantile_type;
   //
   // Mathematically undefined properties:
   //
   typedef typename detail::find_arg<arg_list, is_assert_undefined<mpl::_1>, discrete_quantile<> >::type assert_undefined_type;
   //
   // Max iterations:
   //
   typedef typename detail::find_arg<arg_list, is_max_series_iterations<mpl::_1>, max_series_iterations<> >::type max_series_iterations_type;
   typedef typename detail::find_arg<arg_list, is_max_root_iterations<mpl::_1>, max_root_iterations<> >::type max_root_iterations_type;
   //
   // Define a typelist of the policies:
   //
   typedef mpl::vector<
      domain_error_type,
      pole_error_type,
      overflow_error_type,
      underflow_error_type,
      denorm_error_type,
      evaluation_error_type,
      precision_type,
      promote_float_type,
      promote_double_type,
      discrete_quantile_type,
      assert_undefined_type,
      max_series_iterations_type,
      max_root_iterations_type> result_list;
   //
   // Remove all the policies that are the same as the default:
   //
   typedef typename mpl::remove_if<result_list, detail::is_default_policy<mpl::_> >::type reduced_list;
   //
   // Pad out the list with defaults:
   //
   typedef typename detail::append_N<reduced_list, default_policy, (14 - ::boost::mpl::size<reduced_list>::value)>::type result_type;
public:
   typedef policy<
      typename mpl::at<result_type, mpl::int_<0> >::type,
      typename mpl::at<result_type, mpl::int_<1> >::type,
      typename mpl::at<result_type, mpl::int_<2> >::type,
      typename mpl::at<result_type, mpl::int_<3> >::type,
      typename mpl::at<result_type, mpl::int_<4> >::type,
      typename mpl::at<result_type, mpl::int_<5> >::type,
      typename mpl::at<result_type, mpl::int_<6> >::type,
      typename mpl::at<result_type, mpl::int_<7> >::type,
      typename mpl::at<result_type, mpl::int_<8> >::type,
      typename mpl::at<result_type, mpl::int_<9> >::type,
      typename mpl::at<result_type, mpl::int_<10> >::type,
      typename mpl::at<result_type, mpl::int_<11> >::type,
      typename mpl::at<result_type, mpl::int_<12> >::type > type;
};
//
// Full specialisation to speed up compilation of the common case:
//
template <>
struct normalise<policy<>,
          promote_float<false>,
          promote_double<false>,
          discrete_quantile<>,
          assert_undefined<>,
          default_policy,
          default_policy,
          default_policy,
          default_policy,
          default_policy,
          default_policy,
          default_policy>
{
   typedef policy<detail::forwarding_arg1, detail::forwarding_arg2> type;
};

inline policy<> make_policy()
{ return policy<>(); }

template <class A1>
inline typename normalise<policy<>, A1>::type make_policy(const A1&)
{
   typedef typename normalise<policy<>, A1>::type result_type;
   return result_type();
}

template <class A1, class A2>
inline typename normalise<policy<>, A1, A2>::type make_policy(const A1&, const A2&)
{
   typedef typename normalise<policy<>, A1, A2>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3>
inline typename normalise<policy<>, A1, A2, A3>::type make_policy(const A1&, const A2&, const A3&)
{
   typedef typename normalise<policy<>, A1, A2, A3>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4>
inline typename normalise<policy<>, A1, A2, A3, A4>::type make_policy(const A1&, const A2&, const A3&, const A4&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4, class A5>
inline typename normalise<policy<>, A1, A2, A3, A4, A5>::type make_policy(const A1&, const A2&, const A3&, const A4&, const A5&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4, A5>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4, class A5, class A6>
inline typename normalise<policy<>, A1, A2, A3, A4, A5, A6>::type make_policy(const A1&, const A2&, const A3&, const A4&, const A5&, const A6&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4, A5, A6>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7>::type make_policy(const A1&, const A2&, const A3&, const A4&, const A5&, const A6&, const A7&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7, A8>::type make_policy(const A1&, const A2&, const A3&, const A4&, const A5&, const A6&, const A7&, const A8&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7, A8>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
inline typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type make_policy(const A1&, const A2&, const A3&, const A4&, const A5&, const A6&, const A7&, const A8&, const A9&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7, A8, A9>::type result_type;
   return result_type();
}

template <class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
inline typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10>::type make_policy(const A1&, const A2&, const A3&, const A4&, const A5&, const A6&, const A7&, const A8&, const A9&, const A10&)
{
   typedef typename normalise<policy<>, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10>::type result_type;
   return result_type();
}

//
// Traits class to handle internal promotion:
//
template <class Real, class Policy>
struct evaluation
{
   typedef Real type;
};

template <class Policy>
struct evaluation<float, Policy>
{
   typedef typename mpl::if_<typename Policy::promote_float_type, double, float>::type type;
};

template <class Policy>
struct evaluation<double, Policy>
{
   typedef typename mpl::if_<typename Policy::promote_double_type, long double, double>::type type;
};

#ifdef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS

template <class Real>
struct basic_digits : public mpl::int_<0>{ };
template <>
struct basic_digits<float> : public mpl::int_<FLT_MANT_DIG>{ };
template <>
struct basic_digits<double> : public mpl::int_<DBL_MANT_DIG>{ };
template <>
struct basic_digits<long double> : public mpl::int_<LDBL_MANT_DIG>{ };

template <class Real, class Policy>
struct precision
{
   typedef typename Policy::precision_type precision_type;
   typedef basic_digits<Real> digits_t;
   typedef typename mpl::if_<
      mpl::equal_to<digits_t, mpl::int_<0> >,
      // Possibly unknown precision:
      precision_type,
      typename mpl::if_<
         mpl::or_<mpl::less_equal<digits_t, precision_type>, mpl::less_equal<precision_type, mpl::int_<0> > >,
         // Default case, full precision for RealType:
         digits2< ::std::numeric_limits<Real>::digits>,
         // User customised precision:
         precision_type
      >::type
   >::type type;
};

template <class Policy>
struct precision<float, Policy>
{
   typedef digits2<FLT_MANT_DIG> type;
};
template <class Policy>
struct precision<double, Policy>
{
   typedef digits2<DBL_MANT_DIG> type;
};
template <class Policy>
struct precision<long double, Policy>
{
   typedef digits2<LDBL_MANT_DIG> type;
};

#else

template <class Real, class Policy>
struct precision
{
#ifndef __BORLANDC__
   typedef typename Policy::precision_type precision_type;
   typedef typename mpl::if_c<
      ((::std::numeric_limits<Real>::is_specialized == 0) || (::std::numeric_limits<Real>::digits == 0)),
      // Possibly unknown precision:
      precision_type,
      typename mpl::if_c<
         ((::std::numeric_limits<Real>::digits <= precision_type::value)
         || (Policy::precision_type::value <= 0)),
         // Default case, full precision for RealType:
         digits2< ::std::numeric_limits<Real>::digits>,
         // User customised precision:
         precision_type
      >::type
   >::type type;
#else
   typedef typename Policy::precision_type precision_type;
   typedef mpl::int_< ::std::numeric_limits<Real>::digits> digits_t;
   typedef mpl::bool_< ::std::numeric_limits<Real>::is_specialized> spec_t;
   typedef typename mpl::if_<
      mpl::or_<mpl::equal_to<spec_t, mpl::false_>, mpl::equal_to<digits_t, mpl::int_<0> > >,
      // Possibly unknown precision:
      precision_type,
      typename mpl::if_<
         mpl::or_<mpl::less_equal<digits_t, precision_type>, mpl::less_equal<precision_type, mpl::int_<0> > >,
         // Default case, full precision for RealType:
         digits2< ::std::numeric_limits<Real>::digits>,
         // User customised precision:
         precision_type
      >::type
   >::type type;
#endif
};

#endif

namespace detail{

template <class T, class Policy>
inline int digits_imp(mpl::true_ const&)
{
#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
   BOOST_STATIC_ASSERT( ::std::numeric_limits<T>::is_specialized);
#else
   BOOST_ASSERT(::std::numeric_limits<T>::is_specialized);
#endif
   typedef typename boost::math::policies::precision<T, Policy>::type p_t;
   return p_t::value;
}

template <class T, class Policy>
inline int digits_imp(mpl::false_ const&)
{
   return tools::digits<T>();
}

} // namespace detail

template <class T, class Policy>
inline int digits()
{
   typedef mpl::bool_< std::numeric_limits<T>::is_specialized > tag_type;
   return detail::digits_imp<T, Policy>(tag_type());
}

template <class Policy>
inline unsigned long get_max_series_iterations()
{
   typedef typename Policy::max_series_iterations_type iter_type;
   return iter_type::value;
}

template <class Policy>
inline unsigned long get_max_root_iterations()
{
   typedef typename Policy::max_root_iterations_type iter_type;
   return iter_type::value;
}

namespace detail{

template <class A1,
          class A2,
          class A3,
          class A4,
          class A5,
          class A6,
          class A7,
          class A8,
          class A9,
          class A10,
          class A11>
char test_is_policy(const policy<A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11>*);
double test_is_policy(...);

template <class P>
struct is_policy_imp
{
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(test_is_policy(static_cast<P*>(0))) == 1));
};

}

template <class P>
struct is_policy : public mpl::bool_< ::boost::math::policies::detail::is_policy_imp<P>::value> {};

}}} // namespaces

#endif // BOOST_MATH_POLICY_HPP



