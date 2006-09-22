//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_NTL_HPP
#define BOOST_MATH_TOOLS_NTL_HPP

#include <NTL/RR.h>
#include <boost/math/tools/real_cast.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/math/constants/constants.hpp>

namespace NTL
{

   inline quad_float pow(quad_float a, quad_float b)
   {
      return to_quad_float(pow(RR(a), RR(b)));
   }

   inline quad_float pow(quad_float a, long b)
   {
      return to_quad_float(pow(RR(a), RR(b)));
   }

   inline RR pow(const RR& r, long l)
   {
      return power(r, l);
   }
   inline RR tan(const RR& a)
   {
      return sin(a)/cos(a);
   }
   inline RR frexp(RR r, int* exp)
   {
      *exp = r.e;
      r.e = 0;
      while(r >= 1)
      {
         *exp += 1;
         r.e -= 1;
      }
      while(r < 0.5)
      {
         *exp -= 1;
         r.e += 1;
      }
      BOOST_ASSERT(r < 1);
      BOOST_ASSERT(r >= 0.5);
      return r;
   }
   inline RR ldexp(RR r, int exp)
   {
      r.e += exp;
      return r;
   }

} // namespace NTL

namespace boost{ namespace math{ namespace tools{

template <>
inline float real_cast<float, NTL::RR>(NTL::RR t)
{
   double r;
   conv(r, t);
   return static_cast<float>(r);
}
template <>
inline double real_cast<double, NTL::RR>(NTL::RR t)
{
   double r;
   conv(r, t);
   return r;
}
template <>
inline long double real_cast<long double, NTL::RR>(NTL::RR t)
{
   long double result(0), last_result(0);
   double term;
   do
   {
      term = real_cast<double>(t);
      last_result = result;
      result += term;
      t -= term;
   }while(result != last_result);
   return result;
}
template <>
inline NTL::RR real_cast<NTL::RR, NTL::RR>(NTL::RR t)
{
   return t;
}

template<>
inline int digits<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
{
   return NTL::RR::precision();
}

template <>
inline NTL::RR max_value<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = NTL_OVFBND-20;
      has_init = true;
   }
   return val;
}

template <>
inline NTL::RR min_value<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = -NTL_OVFBND+20;
      has_init = true;
   }
   return val;
}

template <>
inline NTL::RR log_max_value<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = NTL_OVFBND-20;
      val = log(val);
      has_init = true;
   }
   return val;
}

template <>
inline NTL::RR log_min_value<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = -NTL_OVFBND+20;
      val = log(val);
      has_init = true;
   }
   return val;
}

template <>
inline NTL::RR epsilon<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
{
   return ldexp(NTL::RR(1), 1-digits<NTL::RR>());
}

inline void setprecision(std::ostream& /*os*/, NTL::RR, int p)
{
   NTL::RR::SetOutputPrecision(p);
}

} // namespace tools

//
// The number of digits precision in RR can vary with each call
// so we need to recalculate these with each call:
//
namespace constants{

	template<> inline NTL::RR pi<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
	{
		 NTL::RR result;
		 ComputePi(result);
		 return result;
	}
	template<> inline NTL::RR e<NTL::RR>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::RR))
	{
		 NTL::RR result = 1;
		 return exp(result);
	}

} // namespace constants

namespace tools
{

template <>
inline float real_cast<float, NTL::quad_float>(NTL::quad_float t)
{
   return to_float(t);
}
template <>
inline double real_cast<double, NTL::quad_float>(NTL::quad_float t)
{
   return to_double(t);
}
template <>
inline long double real_cast<long double, NTL::quad_float>(NTL::quad_float x)
{
   long double result = x.hi;
   result += x.lo;
   return result;
}
template <>
inline NTL::quad_float real_cast<NTL::quad_float, NTL::quad_float>(NTL::quad_float t)
{
   return t;
}

template <>
inline NTL::quad_float real_cast<NTL::quad_float, NTL::RR>(NTL::RR t)
{
   return to_quad_float(t);
}

template<>
inline int digits<NTL::quad_float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::quad_float))
{
   return 106;
}

template <>
inline NTL::quad_float max_value<NTL::quad_float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::quad_float))
{
   return max_value<double>();
}

template <>
inline NTL::quad_float min_value<NTL::quad_float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::quad_float))
{
   return min_value<double>();
}

template <>
inline NTL::quad_float log_max_value<NTL::quad_float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::quad_float))
{
   return log_max_value<double>();
}

template <>
inline NTL::quad_float log_min_value<NTL::quad_float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::quad_float))
{
   return log_min_value<double>();
}

template <>
inline NTL::quad_float epsilon<NTL::quad_float>(BOOST_EXPLICIT_TEMPLATE_TYPE_SPEC(NTL::quad_float))
{
   static const NTL::quad_float val = pow(NTL::quad_float(2.0), NTL::quad_float(1-2*digits<double>()));
   return val;
}

inline void setprecision(std::ostream& /*os*/, NTL::quad_float, int p)
{
   NTL::quad_float::SetOutputPrecision(p);
}

} // namespace tools
} // namespace math
} // namespaceboost

//
// The following *must* all occur after the definitions above
// since we rely on them being already defined:
//
#include <boost/math/tools/roots.hpp>

namespace NTL{

   //
   // Inverse trig functions:
   //
   struct asin_root
   {
      asin_root(NTL::RR const& target) : t(target){}

      std::tr1::tuple<NTL::RR, NTL::RR> operator()(NTL::RR const& p)
      {
         NTL::RR f0 = sin(p) - t;
         NTL::RR f1 = cos(p);
         return std::tr1::make_tuple(f0, f1);
      }
   private:
      NTL::RR t;
   };

   inline NTL::RR asin(NTL::RR z)
   {
      return boost::math::tools::newton_raphson_iterate(
         asin_root(z), 
         NTL::RR(0), 
         NTL::RR(-boost::math::constants::pi<NTL::RR>()/2),
         NTL::RR(boost::math::constants::pi<NTL::RR>()/2),
         boost::math::tools::digits<NTL::RR>());
   }

} // namespace NTL


#endif // BOOST_MATH_TOOLS_NTL_HPP



