//  Copyright John Maddock 2010.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_CALCULATE_CONSTANTS_CONSTANTS_INCLUDED
#define BOOST_MATH_CALCULATE_CONSTANTS_CONSTANTS_INCLUDED

namespace boost{ namespace math{ namespace constants{

template <class T, int N> 
inline T calculate_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return 2 * acos(static_cast<T>(0));
}

template <class T, int N> 
inline T calculate_two_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return 2 * pi<T, policies::policy<policies::digits2<N> > >();
}

template <class T, int N> 
inline T calculate_one_div_two_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return 1 / two_pi<T, policies::policy<policies::digits2<N> > >();
}

template <class T, int N> 
inline T calculate_root_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return sqrt(pi<T, policies::policy<policies::digits2<N> > >());
}

template <class T, int N> 
inline T calculate_root_half_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return sqrt(pi<T, policies::policy<policies::digits2<N> > >() / 2);
}

template <class T, int N> 
inline T calculate_root_two_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return sqrt(two_pi<T, policies::policy<policies::digits2<N> > >());
}

template <class T, int N> 
inline T calculate_root_ln_four(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return sqrt(log(static_cast<T>(4)));
}

template <class T, int N> 
inline T calculate_e(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return exp(static_cast<T>(1));
}

template <class T, int N> 
inline T calculate_half(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return static_cast<T>(1) / static_cast<T>(2);
}

template <class T, int N> 
inline T calculate_euler(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return 0;
}

template <class T, int N> 
inline T calculate_root_two(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return sqrt(static_cast<T>(2));
}

template <class T, int N> 
inline T calculate_half_root_two(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return sqrt(static_cast<T>(2)) / 2;
}

template <class T, int N> 
inline T calculate_ln_two(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return log(static_cast<T>(2));
}

template <class T, int N> 
inline T calculate_ln_ln_two(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return log(log(static_cast<T>(2)));
}

template <class T, int N> 
inline T calculate_third(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return static_cast<T>(1) / static_cast<T>(3);
}

template <class T, int N> 
inline T calculate_twothirds(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return static_cast<T>(2) / static_cast<T>(3);
}

template <class T, int N> 
inline T calculate_pi_minus_three(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return pi<T, policies::policy<policies::digits2<N> > >() - static_cast<T>(3);
}

template <class T, int N> 
inline T calculate_four_minus_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return static_cast<T>(4) - pi<T, policies::policy<policies::digits2<N> > >();
}

template <class T, int N> 
inline T calculate_pow23_four_minus_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return pow(four_minus_pi<T, policies::policy<policies::digits2<N> > >(), static_cast<T>(1.5));
}

template <class T, int N> 
inline T calculate_exp_minus_half(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   BOOST_MATH_STD_USING
   return exp(static_cast<T>(-0.5));
}

template <class T, int N> 
inline T calculate_one_div_root_two(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return static_cast<T>(1) / root_two<T, policies::policy<policies::digits2<N> > >();
}

template <class T, int N> 
inline T calculate_one_div_root_two_pi(const mpl::int_<N>&BOOST_MATH_APPEND_EXPLICIT_TEMPLATE_TYPE_SPEC(T))
{
   return static_cast<T>(1) / root_two_pi<T, policies::policy<policies::digits2<N> > >();
}

}}} // namespaces

#endif // BOOST_MATH_CALCULATE_CONSTANTS_CONSTANTS_INCLUDED
