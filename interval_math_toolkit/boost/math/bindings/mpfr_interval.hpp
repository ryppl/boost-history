//  Copyright John Maddock 2007-8.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_MPFR_INTERVAL_BINDINGS_HPP
#define BOOST_MATH_MPFR_INTERVAL_BINDINGS_HPP

#include <boost/math/bindings/interval.hpp>
#include <boost/math/bindings/mpfr.hpp>

namespace boost {

namespace math{

namespace detail{

struct mpfr_rounding_base
{
  typedef mp_rnd_t rounding_mode;
  void set_rounding_mode(rounding_mode m){ mpfr_set_default_rounding_mode(m); }
  rounding_mode get_rounding_mode(rounding_mode&){ return mpfr_get_default_rounding_mode(); }
  void downward (){ mpfr_set_default_rounding_mode(GMP_RNDD); }
  void upward   (){ mpfr_set_default_rounding_mode(GMP_RNDU); }
  void to_nearest(){ mpfr_set_default_rounding_mode(GMP_RNDN); }
  mpfr_class to_int(mpfr_class const& v)
  {
     mpfr_get_si(v.__get_mp(), mpfr_get_default_rounding_mode());
  }
  mpfr_class const& force_rounding(mpfr_class const& v)
  {
     return v;
  }
};

struct mpfr_checking
{
  static mpfr_class pos_inf()
  {
     mpfr_class v;
     mpfr_set_inf(v.__get_mp(), 1);
     return v;
  }
  static mpfr_class neg_inf()
  {
     mpfr_class v;
     mpfr_set_inf(v.__get_mp(), -1);
     return v;
  }
  static mpfr_class nan()
  {
     mpfr_class v;
     mpfr_set_nan(v.__get_mp());
     return v;
  }
  static bool is_nan(const mpfr_class& r)
  {
     return mpfr_nan_p(r.__get_mp());
  }
  static mpfr_class empty_lower()
  {
     return mpfr_checking::nan();
  }
  static mpfr_class empty_upper()
  {
     return mpfr_checking::nan();
  }
  static bool is_empty(const mpfr_class& a, const mpfr_class& b)
  {
     return mpfr_checking::is_nan(a) && mpfr_checking::is_nan(a);
  }
};

struct mpfr_rounding_control 
{
  // defaut constructor, destructor
  // mpfr_rounding_control();
  // ~mpfr_rounding_control();
  // mathematical operations
  mpfr_class add_down(mpfr_class const& a, mpfr_class const& b)
  {
     mpfr_class r;
     mpfr_add(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class add_up  (mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_add(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class sub_down(mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_sub(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class sub_up  (mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_sub(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class mul_down(mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_mul(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class mul_up  (mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_mul(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class div_down(mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_div(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class div_up  (mpfr_class const& a, mpfr_class const& b) 
  {
     mpfr_class r;
     mpfr_div(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class sqrt_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_sqrt(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class sqrt_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_sqrt(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class exp_down(mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_exp(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class exp_up  (mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_exp(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class pow_down(mpfr_class const& a, mpfr_class const& b)    
  {
     mpfr_class r;
     mpfr_pow(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class pow_up  (mpfr_class const& a, mpfr_class const& b)    
  {
     mpfr_class r;
     mpfr_pow(r.__get_mp(), a.__get_mp(), b.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class log_down(mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_log(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class log_up  (mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_log(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class cos_down(mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_cos(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class cos_up  (mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_cos(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class tan_down(mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_tan(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class tan_up  (mpfr_class const& a)    
  {
     mpfr_class r;
     mpfr_tan(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class asin_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_asin(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class asin_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_asin(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class acos_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_acos(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class acos_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_acos(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class atan_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_atan(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class atan_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_atan(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class sinh_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_sinh(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class sinh_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_sinh(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class cosh_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_cosh(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class cosh_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_cosh(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class tanh_down(mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_tanh(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class tanh_up  (mpfr_class const& a)   
  {
     mpfr_class r;
     mpfr_tanh(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class asinh_down(mpfr_class const& a)  
  {
     mpfr_class r;
     mpfr_asinh(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class asinh_up  (mpfr_class const& a)  
  {
     mpfr_class r;
     mpfr_asinh(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class acosh_down(mpfr_class const& a)  
  {
     mpfr_class r;
     mpfr_acosh(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class acosh_up  (mpfr_class const& a)  
  {
     mpfr_class r;
     mpfr_acosh(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class atanh_down(mpfr_class const& a)  
  {
     mpfr_class r;
     mpfr_atanh(r.__get_mp(), a.__get_mp(), GMP_RNDD);
     return r;
  }
  mpfr_class atanh_up  (mpfr_class const& a)  
  {
     mpfr_class r;
     mpfr_atanh(r.__get_mp(), a.__get_mp(), GMP_RNDU);
     return r;
  }
  mpfr_class median(mpfr_class const& a, mpfr_class const& b)
  {
     return (a + b) / 2;
  }
  mpfr_class int_down(mpfr_class const& a)
  {
     return floor(a);
  }
  mpfr_class int_up  (mpfr_class const& a)
  {
     return ceil(a);
  }
  // conversion functions
  mpfr_class conv_down(long i)
  {
     mpfr_class result;
     mpfr_set_si(result.__get_mp(), i, GMP_RNDD);
     return result;
  }
  mpfr_class conv_down(int i)
  { return conv_down((long)i); }
  mpfr_class conv_down(short i)
  { return conv_down((long)i); }
  mpfr_class conv_down(signed char i)
  { return conv_down((long)i); }
  mpfr_class conv_down(char i)
  { return conv_down((long)i); }
  mpfr_class conv_down(unsigned long i)
  {
     mpfr_class result;
     mpfr_set_ui(result.__get_mp(), i, GMP_RNDD);
     return result;
  }
  mpfr_class conv_down(unsigned int i)
  { return conv_down((unsigned long)i); }
  mpfr_class conv_down(unsigned short i)
  { return conv_down((unsigned long)i); }
  mpfr_class conv_down(unsigned char i)
  { return conv_down((unsigned long)i); }
  mpfr_class conv_down(double i)
  {
     mpfr_class result;
     mpfr_set_d(result.__get_mp(), i, GMP_RNDD);
     return result;
  }
  mpfr_class conv_down(long double i)
  {
     mpfr_class result;
     mpfr_set_ld(result.__get_mp(), i, GMP_RNDD);
     return result;
  }
  mpfr_class conv_down(float i)
  { return conv_down((double)i); }
  /*
  mpfr_class conv_down(long long i)
  {
     mpfr_class result;
     mpfr_set_sj(result.__get_mp(), i, GMP_RNDD);
     return result;
  }
  mpfr_class conv_down(unsigned long long i)
  {
     mpfr_class result;
     mpfr_set_uj(result.__get_mp(), i, GMP_RNDD);
     return result;
  }
*/
  mpfr_class conv_up(long i)
  {
     mpfr_class result;
     mpfr_set_si(result.__get_mp(), i, GMP_RNDU);
     return result;
  }
  mpfr_class conv_up(int i)
  { return conv_up((long)i); }
  mpfr_class conv_up(short i)
  { return conv_up((long)i); }
  mpfr_class conv_up(signed char i)
  { return conv_up((long)i); }
  mpfr_class conv_up(char i)
  { return conv_up((long)i); }
  mpfr_class conv_up(unsigned long i)
  {
     mpfr_class result;
     mpfr_set_ui(result.__get_mp(), i, GMP_RNDU);
     return result;
  }
  mpfr_class conv_up(unsigned int i)
  { return conv_up((unsigned long)i); }
  mpfr_class conv_up(unsigned short i)
  { return conv_up((unsigned long)i); }
  mpfr_class conv_up(unsigned char i)
  { return conv_up((unsigned long)i); }
  mpfr_class conv_up(double i)
  {
     mpfr_class result;
     mpfr_set_d(result.__get_mp(), i, GMP_RNDU);
     return result;
  }
  mpfr_class conv_up(long double i)
  {
     mpfr_class result;
     mpfr_set_ld(result.__get_mp(), i, GMP_RNDU);
     return result;
  }
  mpfr_class conv_up(float i)
  { return conv_up((double)i); }
  /*
  mpfr_class conv_up(long long i)
  {
     mpfr_class result;
     mpfr_set_sj(result.__get_mp(), i, GMP_RNDU);
     return result;
  }
  mpfr_class conv_up(unsigned long long i)
  {
     mpfr_class result;
     mpfr_set_uj(result.__get_mp(), i, GMP_RNDU);
     return result;
  }
  */
  template <class T1, class T2>
  mpfr_class conv_up(__gmp_expr<T1,T2> express)
  {
     return express;
  }
  template <class T1, class T2>
  mpfr_class conv_down(__gmp_expr<T1,T2> express)
  {
     return express;
  }
  // unprotected mpfr_rounding_control class
  typedef mpfr_rounding_control unprotected_rounding;
  typedef mpfr_checking checking;
  typedef mpfr_rounding_control rounding;
};

} // namespace detail

typedef boost::numeric::interval<mpfr_class, detail::mpfr_rounding_control> mpfr_interval;

namespace tools{

template <>
inline int digits<mpfr_interval>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(mpfr_interval))
{
   return digits<mpfr_class>();
}

template <>
inline unsigned real_cast<unsigned, mpfr_interval>(mpfr_interval t)
{
   return real_cast<unsigned>(median(t));
}
template <>
inline int real_cast<int, mpfr_interval>(mpfr_interval t)
{
   return real_cast<int>(median(t));
}

template <>
inline mpfr_interval max_value<mpfr_interval>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(mpfr_interval))
{
   return max_value<mpfr_class>();
}

template <>
inline mpfr_interval min_value<mpfr_interval>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(mpfr_interval))
{
   return min_value<mpfr_class>();
}

template <>
inline mpfr_interval log_max_value<mpfr_interval>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(mpfr_interval))
{
   return log_max_value<mpfr_class>();
}

template <>
inline mpfr_interval log_min_value<mpfr_interval>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(mpfr_interval))
{
   return log_max_value<mpfr_class>();
}

template <>
inline mpfr_interval epsilon<mpfr_interval>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(mpfr_interval))
{
   return epsilon<mpfr_class>();
}

}

namespace lanczos{

struct lanczos13UDT_mpfr_interval
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      static const mpfr_class num[13] = {
         boost::lexical_cast<mpfr_class>("44012138428004.60895436261759919070125699"),
         boost::lexical_cast<mpfr_class>("41590453358593.20051581730723108131357995"),
         boost::lexical_cast<mpfr_class>("18013842787117.99677796276038389462742949"),
         boost::lexical_cast<mpfr_class>("4728736263475.388896889723995205703970787"),
         boost::lexical_cast<mpfr_class>("837910083628.4046470415724300225777912264"),
         boost::lexical_cast<mpfr_class>("105583707273.4299344907359855510105321192"),
         boost::lexical_cast<mpfr_class>("9701363618.494999493386608345339104922694"),
         boost::lexical_cast<mpfr_class>("654914397.5482052641016767125048538245644"),
         boost::lexical_cast<mpfr_class>("32238322.94213356530668889463945849409184"),
         boost::lexical_cast<mpfr_class>("1128514.219497091438040721811544858643121"),
         boost::lexical_cast<mpfr_class>("26665.79378459858944762533958798805525125"),
         boost::lexical_cast<mpfr_class>("381.8801248632926870394389468349331394196"),
         boost::lexical_cast<mpfr_class>("2.506628274631000502415763426076722427007"),
      };
      static const boost::uint32_t denom[13] = {
         static_cast<boost::uint32_t>(0u),
         static_cast<boost::uint32_t>(39916800u),
         static_cast<boost::uint32_t>(120543840u),
         static_cast<boost::uint32_t>(150917976u),
         static_cast<boost::uint32_t>(105258076u),
         static_cast<boost::uint32_t>(45995730u),
         static_cast<boost::uint32_t>(13339535u),
         static_cast<boost::uint32_t>(2637558u),
         static_cast<boost::uint32_t>(357423u),
         static_cast<boost::uint32_t>(32670u),
         static_cast<boost::uint32_t>(1925u),
         static_cast<boost::uint32_t>(66u),
         static_cast<boost::uint32_t>(1u),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 13);
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      static const mpfr_class num[13] = {
         boost::lexical_cast<mpfr_class>("86091529.53418537217994842267760536134841"),
         boost::lexical_cast<mpfr_class>("81354505.17858011242874285785316135398567"),
         boost::lexical_cast<mpfr_class>("35236626.38815461910817650960734605416521"),
         boost::lexical_cast<mpfr_class>("9249814.988024471294683815872977672237195"),
         boost::lexical_cast<mpfr_class>("1639024.216687146960253839656643518985826"),
         boost::lexical_cast<mpfr_class>("206530.8157641225032631778026076868855623"),
         boost::lexical_cast<mpfr_class>("18976.70193530288915698282139308582105936"),
         boost::lexical_cast<mpfr_class>("1281.068909912559479885759622791374106059"),
         boost::lexical_cast<mpfr_class>("63.06093343420234536146194868906771599354"),
         boost::lexical_cast<mpfr_class>("2.207470909792527638222674678171050209691"),
         boost::lexical_cast<mpfr_class>("0.05216058694613505427476207805814960742102"),
         boost::lexical_cast<mpfr_class>("0.0007469903808915448316510079585999893674101"),
         boost::lexical_cast<mpfr_class>("0.4903180573459871862552197089738373164184e-5"),
      };
      static const boost::uint32_t denom[13] = {
         static_cast<boost::uint32_t>(0u),
         static_cast<boost::uint32_t>(39916800u),
         static_cast<boost::uint32_t>(120543840u),
         static_cast<boost::uint32_t>(150917976u),
         static_cast<boost::uint32_t>(105258076u),
         static_cast<boost::uint32_t>(45995730u),
         static_cast<boost::uint32_t>(13339535u),
         static_cast<boost::uint32_t>(2637558u),
         static_cast<boost::uint32_t>(357423u),
         static_cast<boost::uint32_t>(32670u),
         static_cast<boost::uint32_t>(1925u),
         static_cast<boost::uint32_t>(66u),
         static_cast<boost::uint32_t>(1u),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 13);
   }


   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      static const mpfr_class d[12] = {
         boost::lexical_cast<mpfr_class>("4.832115561461656947793029596285626840312"),
         boost::lexical_cast<mpfr_class>("-19.86441536140337740383120735104359034688"),
         boost::lexical_cast<mpfr_class>("33.9927422807443239927197864963170585331"),
         boost::lexical_cast<mpfr_class>("-31.41520692249765980987427413991250886138"),
         boost::lexical_cast<mpfr_class>("17.0270866009599345679868972409543597821"),
         boost::lexical_cast<mpfr_class>("-5.5077216950865501362506920516723682167"),
         boost::lexical_cast<mpfr_class>("1.037811741948214855286817963800439373362"),
         boost::lexical_cast<mpfr_class>("-0.106640468537356182313660880481398642811"),
         boost::lexical_cast<mpfr_class>("0.005276450526660653288757565778182586742831"),
         boost::lexical_cast<mpfr_class>("-0.0001000935625597121545867453746252064770029"),
         boost::lexical_cast<mpfr_class>("0.462590910138598083940803704521211569234e-6"),
         boost::lexical_cast<mpfr_class>("-0.1735307814426389420248044907765671743012e-9"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      static const mpfr_class d[12] = {
         boost::lexical_cast<mpfr_class>("26.96979819614830698367887026728396466395"),
         boost::lexical_cast<mpfr_class>("-110.8705424709385114023884328797900204863"),
         boost::lexical_cast<mpfr_class>("189.7258846119231466417015694690434770085"),
         boost::lexical_cast<mpfr_class>("-175.3397202971107486383321670769397356553"),
         boost::lexical_cast<mpfr_class>("95.03437648691551457087250340903980824948"),
         boost::lexical_cast<mpfr_class>("-30.7406022781665264273675797983497141978"),
         boost::lexical_cast<mpfr_class>("5.792405601630517993355102578874590410552"),
         boost::lexical_cast<mpfr_class>("-0.5951993240669148697377539518639997795831"),
         boost::lexical_cast<mpfr_class>("0.02944979359164017509944724739946255067671"),
         boost::lexical_cast<mpfr_class>("-0.0005586586555377030921194246330399163602684"),
         boost::lexical_cast<mpfr_class>("0.2581888478270733025288922038673392636029e-5"),
         boost::lexical_cast<mpfr_class>("-0.9685385411006641478305219367315965391289e-9"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 13.1445650000000000545696821063756942749; }
};


//
// Lanczos Coefficients for N=22 G=22.61891
// Max experimental error (with arbitary precision arithmetic) 2.9524e-38
// Generated with compiler: Microsoft Visual C++ version 8.0 on Win32 at Mar 23 2006
//
struct lanczos22UDT_mpfr_interval
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      static const mpfr_class num[22] = {
         boost::lexical_cast<mpfr_class>("46198410803245094237463011094.12173081986"),
         boost::lexical_cast<mpfr_class>("43735859291852324413622037436.321513777"),
         boost::lexical_cast<mpfr_class>("19716607234435171720534556386.97481377748"),
         boost::lexical_cast<mpfr_class>("5629401471315018442177955161.245623932129"),
         boost::lexical_cast<mpfr_class>("1142024910634417138386281569.245580222392"),
         boost::lexical_cast<mpfr_class>("175048529315951173131586747.695329230778"),
         boost::lexical_cast<mpfr_class>("21044290245653709191654675.41581372963167"),
         boost::lexical_cast<mpfr_class>("2033001410561031998451380.335553678782601"),
         boost::lexical_cast<mpfr_class>("160394318862140953773928.8736211601848891"),
         boost::lexical_cast<mpfr_class>("10444944438396359705707.48957290388740896"),
         boost::lexical_cast<mpfr_class>("565075825801617290121.1466393747967538948"),
         boost::lexical_cast<mpfr_class>("25475874292116227538.99448534450411942597"),
         boost::lexical_cast<mpfr_class>("957135055846602154.6720835535232270205725"),
         boost::lexical_cast<mpfr_class>("29874506304047462.23662392445173880821515"),
         boost::lexical_cast<mpfr_class>("769651310384737.2749087590725764959689181"),
         boost::lexical_cast<mpfr_class>("16193289100889.15989633624378404096011797"),
         boost::lexical_cast<mpfr_class>("273781151680.6807433264462376754578933261"),
         boost::lexical_cast<mpfr_class>("3630485900.32917021712188739762161583295"),
         boost::lexical_cast<mpfr_class>("36374352.05577334277856865691538582936484"),
         boost::lexical_cast<mpfr_class>("258945.7742115532455441786924971194951043"),
         boost::lexical_cast<mpfr_class>("1167.501919472435718934219997431551246996"),
         boost::lexical_cast<mpfr_class>("2.50662827463100050241576528481104525333"),
      };
      static const mpfr_class denom[22] = {
         boost::lexical_cast<mpfr_class>("0"),
         boost::lexical_cast<mpfr_class>("2432902008176640000"),
         boost::lexical_cast<mpfr_class>("8752948036761600000"),
         boost::lexical_cast<mpfr_class>("13803759753640704000"),
         boost::lexical_cast<mpfr_class>("12870931245150988800"),
         boost::lexical_cast<mpfr_class>("8037811822645051776"),
         boost::lexical_cast<mpfr_class>("3599979517947607200"),
         boost::lexical_cast<mpfr_class>("1206647803780373360"),
         boost::lexical_cast<mpfr_class>("311333643161390640"),
         boost::lexical_cast<mpfr_class>("63030812099294896"),
         boost::lexical_cast<mpfr_class>("10142299865511450"),
         boost::lexical_cast<mpfr_class>("1307535010540395"),
         boost::lexical_cast<mpfr_class>("135585182899530"),
         boost::lexical_cast<mpfr_class>("11310276995381"),
         boost::lexical_cast<mpfr_class>("756111184500"),
         boost::lexical_cast<mpfr_class>("40171771630"),
         boost::lexical_cast<mpfr_class>("1672280820"),
         boost::lexical_cast<mpfr_class>("53327946"),
         boost::lexical_cast<mpfr_class>("1256850"),
         boost::lexical_cast<mpfr_class>("20615"),
         boost::lexical_cast<mpfr_class>("210"),
         boost::lexical_cast<mpfr_class>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 22);
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      static const mpfr_class num[22] = {
         boost::lexical_cast<mpfr_class>("6939996264376682180.277485395074954356211"),
         boost::lexical_cast<mpfr_class>("6570067992110214451.87201438870245659384"),
         boost::lexical_cast<mpfr_class>("2961859037444440551.986724631496417064121"),
         boost::lexical_cast<mpfr_class>("845657339772791245.3541226499766163431651"),
         boost::lexical_cast<mpfr_class>("171556737035449095.2475716923888737881837"),
         boost::lexical_cast<mpfr_class>("26296059072490867.7822441885603400926007"),
         boost::lexical_cast<mpfr_class>("3161305619652108.433798300149816829198706"),
         boost::lexical_cast<mpfr_class>("305400596026022.4774396904484542582526472"),
         boost::lexical_cast<mpfr_class>("24094681058862.55120507202622377623528108"),
         boost::lexical_cast<mpfr_class>("1569055604375.919477574824168939428328839"),
         boost::lexical_cast<mpfr_class>("84886558909.02047889339710230696942513159"),
         boost::lexical_cast<mpfr_class>("3827024985.166751989686050643579753162298"),
         boost::lexical_cast<mpfr_class>("143782298.9273215199098728674282885500522"),
         boost::lexical_cast<mpfr_class>("4487794.24541641841336786238909171265944"),
         boost::lexical_cast<mpfr_class>("115618.2025760830513505888216285273541959"),
         boost::lexical_cast<mpfr_class>("2432.580773108508276957461757328744780439"),
         boost::lexical_cast<mpfr_class>("41.12782532742893597168530008461874360191"),
         boost::lexical_cast<mpfr_class>("0.5453771709477689805460179187388702295792"),
         boost::lexical_cast<mpfr_class>("0.005464211062612080347167337964166505282809"),
         boost::lexical_cast<mpfr_class>("0.388992321263586767037090706042788910953e-4"),
         boost::lexical_cast<mpfr_class>("0.1753839324538447655939518484052327068859e-6"),
         boost::lexical_cast<mpfr_class>("0.3765495513732730583386223384116545391759e-9"),
      };
      static const mpfr_class denom[22] = {
         boost::lexical_cast<mpfr_class>("0"),
         boost::lexical_cast<mpfr_class>("2432902008176640000"),
         boost::lexical_cast<mpfr_class>("8752948036761600000"),
         boost::lexical_cast<mpfr_class>("13803759753640704000"),
         boost::lexical_cast<mpfr_class>("12870931245150988800"),
         boost::lexical_cast<mpfr_class>("8037811822645051776"),
         boost::lexical_cast<mpfr_class>("3599979517947607200"),
         boost::lexical_cast<mpfr_class>("1206647803780373360"),
         boost::lexical_cast<mpfr_class>("311333643161390640"),
         boost::lexical_cast<mpfr_class>("63030812099294896"),
         boost::lexical_cast<mpfr_class>("10142299865511450"),
         boost::lexical_cast<mpfr_class>("1307535010540395"),
         boost::lexical_cast<mpfr_class>("135585182899530"),
         boost::lexical_cast<mpfr_class>("11310276995381"),
         boost::lexical_cast<mpfr_class>("756111184500"),
         boost::lexical_cast<mpfr_class>("40171771630"),
         boost::lexical_cast<mpfr_class>("1672280820"),
         boost::lexical_cast<mpfr_class>("53327946"),
         boost::lexical_cast<mpfr_class>("1256850"),
         boost::lexical_cast<mpfr_class>("20615"),
         boost::lexical_cast<mpfr_class>("210"),
         boost::lexical_cast<mpfr_class>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 22);
   }


   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      static const mpfr_class d[21] = {
         boost::lexical_cast<mpfr_class>("8.318998691953337183034781139546384476554"),
         boost::lexical_cast<mpfr_class>("-63.15415991415959158214140353299240638675"),
         boost::lexical_cast<mpfr_class>("217.3108224383632868591462242669081540163"),
         boost::lexical_cast<mpfr_class>("-448.5134281386108366899784093610397354889"),
         boost::lexical_cast<mpfr_class>("619.2903759363285456927248474593012711346"),
         boost::lexical_cast<mpfr_class>("-604.1630177420625418522025080080444177046"),
         boost::lexical_cast<mpfr_class>("428.8166750424646119935047118287362193314"),
         boost::lexical_cast<mpfr_class>("-224.6988753721310913866347429589434550302"),
         boost::lexical_cast<mpfr_class>("87.32181627555510833499451817622786940961"),
         boost::lexical_cast<mpfr_class>("-25.07866854821128965662498003029199058098"),
         boost::lexical_cast<mpfr_class>("5.264398125689025351448861011657789005392"),
         boost::lexical_cast<mpfr_class>("-0.792518936256495243383586076579921559914"),
         boost::lexical_cast<mpfr_class>("0.08317448364744713773350272460937904691566"),
         boost::lexical_cast<mpfr_class>("-0.005845345166274053157781068150827567998882"),
         boost::lexical_cast<mpfr_class>("0.0002599412126352082483326238522490030412391"),
         boost::lexical_cast<mpfr_class>("-0.6748102079670763884917431338234783496303e-5"),
         boost::lexical_cast<mpfr_class>("0.908824383434109002762325095643458603605e-7"),
         boost::lexical_cast<mpfr_class>("-0.5299325929309389890892469299969669579725e-9"),
         boost::lexical_cast<mpfr_class>("0.994306085859549890267983602248532869362e-12"),
         boost::lexical_cast<mpfr_class>("-0.3499893692975262747371544905820891835298e-15"),
         boost::lexical_cast<mpfr_class>("0.7260746353663365145454867069182884694961e-20"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      static const mpfr_class d[21] = {
         boost::lexical_cast<mpfr_class>("75.39272007105208086018421070699575462226"),
         boost::lexical_cast<mpfr_class>("-572.3481967049935412452681346759966390319"),
         boost::lexical_cast<mpfr_class>("1969.426202741555335078065370698955484358"),
         boost::lexical_cast<mpfr_class>("-4064.74968778032030891520063865996757519"),
         boost::lexical_cast<mpfr_class>("5612.452614138013929794736248384309574814"),
         boost::lexical_cast<mpfr_class>("-5475.357667500026172903620177988213902339"),
         boost::lexical_cast<mpfr_class>("3886.243614216111328329547926490398103492"),
         boost::lexical_cast<mpfr_class>("-2036.382026072125407192448069428134470564"),
         boost::lexical_cast<mpfr_class>("791.3727954936062108045551843636692287652"),
         boost::lexical_cast<mpfr_class>("-227.2808432388436552794021219198885223122"),
         boost::lexical_cast<mpfr_class>("47.70974355562144229897637024320739257284"),
         boost::lexical_cast<mpfr_class>("-7.182373807798293545187073539819697141572"),
         boost::lexical_cast<mpfr_class>("0.7537866989631514559601547530490976100468"),
         boost::lexical_cast<mpfr_class>("-0.05297470142240154822658739758236594717787"),
         boost::lexical_cast<mpfr_class>("0.00235577330936380542539812701472320434133"),
         boost::lexical_cast<mpfr_class>("-0.6115613067659273118098229498679502138802e-4"),
         boost::lexical_cast<mpfr_class>("0.8236417010170941915758315020695551724181e-6"),
         boost::lexical_cast<mpfr_class>("-0.4802628430993048190311242611330072198089e-8"),
         boost::lexical_cast<mpfr_class>("0.9011113376981524418952720279739624707342e-11"),
         boost::lexical_cast<mpfr_class>("-0.3171854152689711198382455703658589996796e-14"),
         boost::lexical_cast<mpfr_class>("0.6580207998808093935798753964580596673177e-19"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 22.61890999999999962710717227309942245483; }
};

//
// Lanczos Coefficients for N=31 G=32.08067
// Max experimental error (with arbitary precision arithmetic) 0.162e-52
// Generated with compiler: Microsoft Visual C++ version 8.0 on Win32 at May 9 2006
//
struct lanczos31UDT_mpfr_interval
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      static const mpfr_class num[31] = {
         boost::lexical_cast<mpfr_class>("0.2579646553333513328235723061836959833277e46"),
         boost::lexical_cast<mpfr_class>("0.2444796504337453845497419271639377138264e46"),
         boost::lexical_cast<mpfr_class>("0.1119885499016017172212179730662673475329e46"),
         boost::lexical_cast<mpfr_class>("0.3301983829072723658949204487793889113715e45"),
         boost::lexical_cast<mpfr_class>("0.7041171040503851585152895336505379417066e44"),
         boost::lexical_cast<mpfr_class>("0.1156687509001223855125097826246939403504e44"),
         boost::lexical_cast<mpfr_class>("1522559363393940883866575697565974893306000"),
         boost::lexical_cast<mpfr_class>("164914363507650839510801418717701057005700"),
         boost::lexical_cast<mpfr_class>("14978522943127593263654178827041568394060"),
         boost::lexical_cast<mpfr_class>("1156707153701375383907746879648168666774"),
         boost::lexical_cast<mpfr_class>("76739431129980851159755403434593664173.2"),
         boost::lexical_cast<mpfr_class>("4407916278928188620282281495575981079.306"),
         boost::lexical_cast<mpfr_class>("220487883931812802092792125175269667.3004"),
         boost::lexical_cast<mpfr_class>("9644828280794966468052381443992828.433924"),
         boost::lexical_cast<mpfr_class>("369996467042247229310044531282837.6549068"),
         boost::lexical_cast<mpfr_class>("12468380890717344610932904378961.13494291"),
         boost::lexical_cast<mpfr_class>("369289245210898235894444657859.0529720075"),
         boost::lexical_cast<mpfr_class>("9607992460262594951559461829.34885209022"),
         boost::lexical_cast<mpfr_class>("219225935074853412540086410.981421315799"),
         boost::lexical_cast<mpfr_class>("4374309943598658046326340.720767382079549"),
         boost::lexical_cast<mpfr_class>("76008779092264509404014.10530947173485581"),
         boost::lexical_cast<mpfr_class>("1143503533822162444712.335663112617754987"),
         boost::lexical_cast<mpfr_class>("14779233719977576920.37884890049671578409"),
         boost::lexical_cast<mpfr_class>("162409028440678302.9992838032166348069916"),
         boost::lexical_cast<mpfr_class>("1496561553388385.733407609544964535634135"),
         boost::lexical_cast<mpfr_class>("11347624460661.81008311053190661436107043"),
         boost::lexical_cast<mpfr_class>("68944915931.32004991941950530448472223832"),
         boost::lexical_cast<mpfr_class>("322701221.6391432296123937035480931903651"),
         boost::lexical_cast<mpfr_class>("1092364.213992634267819050120261755371294"),
         boost::lexical_cast<mpfr_class>("2380.151399852411512711176940867823024864"),
         boost::lexical_cast<mpfr_class>("2.506628274631000502415765284811045253007"),
      };
      static const mpfr_class denom[31] = {
         boost::lexical_cast<mpfr_class>("0"),
         boost::lexical_cast<mpfr_class>("0.8841761993739701954543616e31"),
         boost::lexical_cast<mpfr_class>("0.3502799997985980526649278464e32"),
         boost::lexical_cast<mpfr_class>("0.622621928420356134910574592e32"),
         boost::lexical_cast<mpfr_class>("66951000306085302338993639424000"),
         boost::lexical_cast<mpfr_class>("49361465831621147825759587123200"),
         boost::lexical_cast<mpfr_class>("26751280755793398822580822142976"),
         boost::lexical_cast<mpfr_class>("11139316913434780466101123891200"),
         boost::lexical_cast<mpfr_class>("3674201658710345201899117607040"),
         boost::lexical_cast<mpfr_class>("981347603630155088295475765440"),
         boost::lexical_cast<mpfr_class>("215760462268683520394805979744"),
         boost::lexical_cast<mpfr_class>("39539238727270799376544542000"),
         boost::lexical_cast<mpfr_class>("6097272817323042122728617800"),
         boost::lexical_cast<mpfr_class>("796974693974455191377937300"),
         boost::lexical_cast<mpfr_class>("88776380550648116217781890"),
         boost::lexical_cast<mpfr_class>("8459574446076318147830625"),
         boost::lexical_cast<mpfr_class>("691254538651580660999025"),
         boost::lexical_cast<mpfr_class>("48487623689430693038025"),
         boost::lexical_cast<mpfr_class>("2918939500751087661105"),
         boost::lexical_cast<mpfr_class>("150566737512021319125"),
         boost::lexical_cast<mpfr_class>("6634460278534540725"),
         boost::lexical_cast<mpfr_class>("248526574856284725"),
         boost::lexical_cast<mpfr_class>("7860403394108265"),
         boost::lexical_cast<mpfr_class>("207912996295875"),
         boost::lexical_cast<mpfr_class>("4539323721075"),
         boost::lexical_cast<mpfr_class>("80328850875"),
         boost::lexical_cast<mpfr_class>("1122686019"),
         boost::lexical_cast<mpfr_class>("11921175"),
         boost::lexical_cast<mpfr_class>("90335"),
         boost::lexical_cast<mpfr_class>("435"),
         boost::lexical_cast<mpfr_class>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 31);
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      static const mpfr_class num[31] = {
         boost::lexical_cast<mpfr_class>("30137154810677525966583148469478.52374216"),
         boost::lexical_cast<mpfr_class>("28561746428637727032849890123131.36314653"),
         boost::lexical_cast<mpfr_class>("13083250730789213354063781611435.74046294"),
         boost::lexical_cast<mpfr_class>("3857598154697777600846539129354.783647"),
         boost::lexical_cast<mpfr_class>("822596651552555685068015316144.0952185852"),
         boost::lexical_cast<mpfr_class>("135131964033213842052904200372.039133532"),
         boost::lexical_cast<mpfr_class>("17787555889683709693655685146.19771358863"),
         boost::lexical_cast<mpfr_class>("1926639793777927562221423874.149673297196"),
         boost::lexical_cast<mpfr_class>("174989113988888477076973808.6991839697774"),
         boost::lexical_cast<mpfr_class>("13513425905835560387095425.01158383184045"),
         boost::lexical_cast<mpfr_class>("896521313378762433091075.1446749283094845"),
         boost::lexical_cast<mpfr_class>("51496223433749515758124.71524415105430686"),
         boost::lexical_cast<mpfr_class>("2575886794780078381228.37205955912263407"),
         boost::lexical_cast<mpfr_class>("112677328855422964200.4155776009524490958"),
         boost::lexical_cast<mpfr_class>("4322545967487943330.625233358130724324796"),
         boost::lexical_cast<mpfr_class>("145663957202380774.0362027607207590519723"),
         boost::lexical_cast<mpfr_class>("4314283729473470.686566233465428332496534"),
         boost::lexical_cast<mpfr_class>("112246988185485.8877916434026906290603878"),
         boost::lexical_cast<mpfr_class>("2561143864972.040563435178307062626388193"),
         boost::lexical_cast<mpfr_class>("51103611767.9626550674442537989885239605"),
         boost::lexical_cast<mpfr_class>("887985348.0369447209508500133077232094491"),
         boost::lexical_cast<mpfr_class>("13359172.3954672607019822025834072685839"),
         boost::lexical_cast<mpfr_class>("172660.8841147568768783928167105965064459"),
         boost::lexical_cast<mpfr_class>("1897.370795407433013556725714874693719617"),
         boost::lexical_cast<mpfr_class>("17.48383210090980598861217644749573257178"),
         boost::lexical_cast<mpfr_class>("0.1325705316732132940835251054350153028901"),
         boost::lexical_cast<mpfr_class>("0.0008054605783673449641889260501816356090452"),
         boost::lexical_cast<mpfr_class>("0.377001130700104515644336869896819162464e-5"),
         boost::lexical_cast<mpfr_class>("0.1276172868883867038813825443204454996531e-7"),
         boost::lexical_cast<mpfr_class>("0.2780651912081116274907381023821492811093e-10"),
         boost::lexical_cast<mpfr_class>("0.2928410648650955854121639682890739211234e-13"),
      };
      static const mpfr_class denom[31] = {
         boost::lexical_cast<mpfr_class>("0"),
         boost::lexical_cast<mpfr_class>("0.8841761993739701954543616e31"),
         boost::lexical_cast<mpfr_class>("0.3502799997985980526649278464e32"),
         boost::lexical_cast<mpfr_class>("0.622621928420356134910574592e32"),
         boost::lexical_cast<mpfr_class>("66951000306085302338993639424000"),
         boost::lexical_cast<mpfr_class>("49361465831621147825759587123200"),
         boost::lexical_cast<mpfr_class>("26751280755793398822580822142976"),
         boost::lexical_cast<mpfr_class>("11139316913434780466101123891200"),
         boost::lexical_cast<mpfr_class>("3674201658710345201899117607040"),
         boost::lexical_cast<mpfr_class>("981347603630155088295475765440"),
         boost::lexical_cast<mpfr_class>("215760462268683520394805979744"),
         boost::lexical_cast<mpfr_class>("39539238727270799376544542000"),
         boost::lexical_cast<mpfr_class>("6097272817323042122728617800"),
         boost::lexical_cast<mpfr_class>("796974693974455191377937300"),
         boost::lexical_cast<mpfr_class>("88776380550648116217781890"),
         boost::lexical_cast<mpfr_class>("8459574446076318147830625"),
         boost::lexical_cast<mpfr_class>("691254538651580660999025"),
         boost::lexical_cast<mpfr_class>("48487623689430693038025"),
         boost::lexical_cast<mpfr_class>("2918939500751087661105"),
         boost::lexical_cast<mpfr_class>("150566737512021319125"),
         boost::lexical_cast<mpfr_class>("6634460278534540725"),
         boost::lexical_cast<mpfr_class>("248526574856284725"),
         boost::lexical_cast<mpfr_class>("7860403394108265"),
         boost::lexical_cast<mpfr_class>("207912996295875"),
         boost::lexical_cast<mpfr_class>("4539323721075"),
         boost::lexical_cast<mpfr_class>("80328850875"),
         boost::lexical_cast<mpfr_class>("1122686019"),
         boost::lexical_cast<mpfr_class>("11921175"),
         boost::lexical_cast<mpfr_class>("90335"),
         boost::lexical_cast<mpfr_class>("435"),
         boost::lexical_cast<mpfr_class>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 31);
   }


   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      static const mpfr_class d[30] = {
         boost::lexical_cast<mpfr_class>("11.80038544942943603508206880307972596807"),
         boost::lexical_cast<mpfr_class>("-130.6355975335626214564236363322099481079"),
         boost::lexical_cast<mpfr_class>("676.2177719145993049893392276809256538927"),
         boost::lexical_cast<mpfr_class>("-2174.724497783850503069990936574060452057"),
         boost::lexical_cast<mpfr_class>("4869.877180638131076410069103742986502022"),
         boost::lexical_cast<mpfr_class>("-8065.744271864238179992762265472478229172"),
         boost::lexical_cast<mpfr_class>("10245.03825618572106228191509520638651539"),
         boost::lexical_cast<mpfr_class>("-10212.83902362683215459850403668669647192"),
         boost::lexical_cast<mpfr_class>("8110.289185383288952562767679576754140336"),
         boost::lexical_cast<mpfr_class>("-5179.310892558291062401828964000776095156"),
         boost::lexical_cast<mpfr_class>("2673.987492589052370230989109591011091273"),
         boost::lexical_cast<mpfr_class>("-1118.342574651205183051884250033505609141"),
         boost::lexical_cast<mpfr_class>("378.5812742511620662650096436471920295596"),
         boost::lexical_cast<mpfr_class>("-103.3725999812126067084828735543906768961"),
         boost::lexical_cast<mpfr_class>("22.62913974335996321848099677797888917792"),
         boost::lexical_cast<mpfr_class>("-3.936414819950859548507275533569588041446"),
         boost::lexical_cast<mpfr_class>("0.5376818198843817355682124535902641644854"),
         boost::lexical_cast<mpfr_class>("-0.0567827903603478957483409124122554243201"),
         boost::lexical_cast<mpfr_class>("0.004545544993648879420352693271088478106482"),
         boost::lexical_cast<mpfr_class>("-0.0002689795568951033950042375135970897959935"),
         boost::lexical_cast<mpfr_class>("0.1139493459006846530734617710847103572122e-4"),
         boost::lexical_cast<mpfr_class>("-0.3316581197839213921885210451302820192794e-6"),
         boost::lexical_cast<mpfr_class>("0.6285613334898374028443777562554713906213e-8"),
         boost::lexical_cast<mpfr_class>("-0.7222145115734409070310317999856424167091e-10"),
         boost::lexical_cast<mpfr_class>("0.4562976983547274766890241815002584238219e-12"),
         boost::lexical_cast<mpfr_class>("-0.1380593023819058919640038942493212141072e-14"),
         boost::lexical_cast<mpfr_class>("0.1629663871586410129307496385264268190679e-17"),
         boost::lexical_cast<mpfr_class>("-0.5429994291916548849493889660077076739993e-21"),
         boost::lexical_cast<mpfr_class>("0.2922682842441892106795386303084661338957e-25"),
         boost::lexical_cast<mpfr_class>("-0.8456967065309046044689041041336866118459e-31"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      static const mpfr_class d[30] = {
         boost::lexical_cast<mpfr_class>("147.9979641587472136175636384176549713358"),
         boost::lexical_cast<mpfr_class>("-1638.404318611773924210055619836375434296"),
         boost::lexical_cast<mpfr_class>("8480.981744216135641122944743711911653273"),
         boost::lexical_cast<mpfr_class>("-27274.93942104458448200467097634494071176"),
         boost::lexical_cast<mpfr_class>("61076.98019918759324489193232276937262854"),
         boost::lexical_cast<mpfr_class>("-101158.8762737154296509560513952101409264"),
         boost::lexical_cast<mpfr_class>("128491.1252383947174824913796141607174379"),
         boost::lexical_cast<mpfr_class>("-128087.2892038336581928787480535905496026"),
         boost::lexical_cast<mpfr_class>("101717.5492545853663296795562084430123258"),
         boost::lexical_cast<mpfr_class>("-64957.8330410311808907869707511362206858"),
         boost::lexical_cast<mpfr_class>("33536.59139229792478811870738772305570317"),
         boost::lexical_cast<mpfr_class>("-14026.01847115365926835980820243003785821"),
         boost::lexical_cast<mpfr_class>("4748.087094096186515212209389240715050212"),
         boost::lexical_cast<mpfr_class>("-1296.477510211815971152205100242259733245"),
         boost::lexical_cast<mpfr_class>("283.8099337545793198947620951499958085157"),
         boost::lexical_cast<mpfr_class>("-49.36969067101255103452092297769364837753"),
         boost::lexical_cast<mpfr_class>("6.743492833270653628580811118017061581404"),
         boost::lexical_cast<mpfr_class>("-0.7121578704864048548351804794951487823626"),
         boost::lexical_cast<mpfr_class>("0.0570092738016915476694118877057948681298"),
         boost::lexical_cast<mpfr_class>("-0.003373485297696102660302960722607722438643"),
         boost::lexical_cast<mpfr_class>("0.0001429128843527532859999752593761934089751"),
         boost::lexical_cast<mpfr_class>("-0.41595867130858508233493767243236888636e-5"),
         boost::lexical_cast<mpfr_class>("0.7883284669307241040059778207492255409785e-7"),
         boost::lexical_cast<mpfr_class>("-0.905786322462384670803148223703187214379e-9"),
         boost::lexical_cast<mpfr_class>("0.5722790216999820323272452464661250331451e-11"),
         boost::lexical_cast<mpfr_class>("-0.1731510870832349779315841757234562309727e-13"),
         boost::lexical_cast<mpfr_class>("0.2043890314358438601429048378015983874378e-16"),
         boost::lexical_cast<mpfr_class>("-0.6810185176079344204740000170500311171065e-20"),
         boost::lexical_cast<mpfr_class>("0.3665567641131713928114853776588342403919e-24"),
         boost::lexical_cast<mpfr_class>("-0.1060655106553177007425710511436497259484e-29"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 32.08066999999999779902282170951366424561; }
};

//
// Lanczos Coefficients for N=61 G=63.192152
// Max experimental error (with 1000-bit precision arithmetic) 3.740e-113
// Generated with compiler: Microsoft Visual C++ version 8.0 on Win32 at Mar 12 2006
//
struct lanczos61UDT_mpfr_interval
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      using namespace boost;
      static const mpfr_class d[61] = {
         boost::lexical_cast<mpfr_class>("2.50662827463100050241576528481104525300698674060993831662992357634229365460784197494659584"),
         boost::lexical_cast<mpfr_class>("13349415823254323512107320481.3495396037261649201426994438803767191136434970492309775123879"),
         boost::lexical_cast<mpfr_class>("-300542621510568204264185787475.230003734889859348050696493467253861933279360152095861484548"),
         boost::lexical_cast<mpfr_class>("3273919938390136737194044982676.40271056035622723775417608127544182097346526115858803376474"),
         boost::lexical_cast<mpfr_class>("-22989594065095806099337396006399.5874206181563663855129141706748733174902067950115092492439"),
         boost::lexical_cast<mpfr_class>("116970582893952893160414263796102.542775878583510989850142808618916073286745084692189044738"),
         boost::lexical_cast<mpfr_class>("-459561969036479455224850813196807.283291532483532558959779434457349912822256480548436066098"),
         boost::lexical_cast<mpfr_class>("1450959909778264914956547227964788.89797179379520834974601372820249784303794436366366810477"),
         boost::lexical_cast<mpfr_class>("-3782846865486775046285288437885921.41537699732805465141128848354901016102326190612528503251"),
         boost::lexical_cast<mpfr_class>("8305043213936355459145388670886540.09976337905520168067329932809302445437208115570138102767"),
         boost::lexical_cast<mpfr_class>("-15580988484396722546934484726970745.4927787160273626078250810989811865283255762028143642311"),
         boost::lexical_cast<mpfr_class>("25262722284076250779006793435537600.0822901485517345545978818780090308947301031347345640449"),
         boost::lexical_cast<mpfr_class>("-35714428027687018805443603728757116.5304655170478705341887572982734901197345415291580897698"),
         boost::lexical_cast<mpfr_class>("44334726194692443174715432419157343.2294160783772787096321009453791271387235388689346602833"),
         boost::lexical_cast<mpfr_class>("-48599573547617297831555162417695106.187829304963846482633791012658974681648157963911491985"),
         boost::lexical_cast<mpfr_class>("47258466493366798944386359199482189.0753349196625125615316002614813737880755896979754845101"),
         boost::lexical_cast<mpfr_class>("-40913448215392412059728312039233342.142914753896559359297977982314043378636755884088383226"),
         boost::lexical_cast<mpfr_class>("31626312914486892948769164616982902.7262756989418188077611392594232674722318027323102462687"),
         boost::lexical_cast<mpfr_class>("-21878079174441332123064991795834438.4699982361692990285700077902601657354101259411789722708"),
         boost::lexical_cast<mpfr_class>("13567268503974326527361474986354265.3136632133935430378937191911532112778452274286122946396"),
         boost::lexical_cast<mpfr_class>("-7551494211746723529747611556474669.62996644923557605747803028485900789337467673523741066527"),
         boost::lexical_cast<mpfr_class>("3775516572689476384052312341432597.70584966904950490541958869730702790312581801585742038997"),
         boost::lexical_cast<mpfr_class>("-1696271471453637244930364711513292.79902955514107737992185368006225264329876265486853482449"),
         boost::lexical_cast<mpfr_class>("684857608019352767999083000986166.20765273693720041519286231015176745354062413008561259139"),
         boost::lexical_cast<mpfr_class>("-248397566275708464679881624417990.410438107634139924805871051723444048539177890346227250473"),
         boost::lexical_cast<mpfr_class>("80880368999557992138783568858556.1512378233079327986518410244522800950609595592170022878937"),
         boost::lexical_cast<mpfr_class>("-23618197945394013802495450485616.9025005749893350650829964098117490779655546610665927669729"),
         boost::lexical_cast<mpfr_class>("6176884636893816103087134481332.06708966653493024119556843727320635285468825056891248447124"),
         boost::lexical_cast<mpfr_class>("-1444348683723439589948246285262.64080678953468490544615312565485170860503207005915261691108"),
         boost::lexical_cast<mpfr_class>("301342031656979076702313946827.961658905182634508217626783081241074250132289461989777865387"),
         boost::lexical_cast<mpfr_class>("-55959656587719766738301589651.3940625826610668990368881615587469329021742236397809951765678"),
         boost::lexical_cast<mpfr_class>("9223339169004064297247180402.36227016155682738556103138196079389248843082157924368301293963"),
         boost::lexical_cast<mpfr_class>("-1344882881571942601385730283.42710150182526891377514071881534880944872423492272147871101373"),
         boost::lexical_cast<mpfr_class>("172841913316760599352601139.54409257740173055624405575900164401527761357324625574736896079"),
         boost::lexical_cast<mpfr_class>("-19496120443876233531343952.3802212016691702737346568192063937387825469602063310488794471653"),
         boost::lexical_cast<mpfr_class>("1920907372583710284097959.44121420322495784420169085871802458519363819782779653621724219067"),
         boost::lexical_cast<mpfr_class>("-164429314798240461613359.399597503536657962383155875723527581699785846599051112719962464604"),
         boost::lexical_cast<mpfr_class>("12154026644351189572525.1452249886865981747374191977801688548318519692423556934568426042152"),
         boost::lexical_cast<mpfr_class>("-770443988366210815096.519382051977221101156336663806705367929328924137169970381042234329058"),
         boost::lexical_cast<mpfr_class>("41558909851418707920.4696085656889424895313728719601503526476333404973280596225722152966128"),
         boost::lexical_cast<mpfr_class>("-1890879946549708819.24562220042687554209318172044783707920086716716717574156283898330017796"),
         boost::lexical_cast<mpfr_class>("71844996557297623.9583461685535340440524052492427928388171299145330229958643439878608673403"),
         boost::lexical_cast<mpfr_class>("-2253785109518255.55600197759875781765803818232939130127735487613049577235879610065545755637"),
         boost::lexical_cast<mpfr_class>("57616883849355.997562563968344493719962252675875692642406455612671495250543228005045106721"),
         boost::lexical_cast<mpfr_class>("-1182495730353.08218118278997948852215670614084013289033222774171548915344541249351599628436"),
         boost::lexical_cast<mpfr_class>("19148649358.6196967288062261380599423925174178776792840639099120170800869284300966978300613"),
         boost::lexical_cast<mpfr_class>("-239779605.891370259668403359614360511661030470269478602533200704639655585967442891496784613"),
         boost::lexical_cast<mpfr_class>("2267583.00284368310957842936892685032434505866445291643236133553754152047677944820353796872"),
         boost::lexical_cast<mpfr_class>("-15749.490806784673108773558070497383604733010677027764233749920147549999361110299643477893"),
         boost::lexical_cast<mpfr_class>("77.7059495149052727171505425584459982871343274332635726864135949842508025564999785370162956"),
         boost::lexical_cast<mpfr_class>("-0.261619987273930331397625130282851629108569607193781378836014468617185550622160348688297247"),
         boost::lexical_cast<mpfr_class>("0.000572252321659691600529444769356185993188551770817110673186068921175991328434642504616377475"),
         boost::lexical_cast<mpfr_class>("-0.765167220661540041663007112207436426423746402583423562585653954743978584117929356523307954e-6"),
         boost::lexical_cast<mpfr_class>("0.579179571056209077507916813937971472839851499147582627425979879366849876944438724610663401e-9"),
         boost::lexical_cast<mpfr_class>("-0.224804733043915149719206760378355636826808754704148660354494460792713189958510735070096991e-12"),
         boost::lexical_cast<mpfr_class>("0.392711975389579343321746945135488409914483448652884894759297584020979857734289645857714768e-16"),
         boost::lexical_cast<mpfr_class>("-0.258603588346412049542768766878162221817684639789901440429511261589010049357907537684380983e-20"),
         boost::lexical_cast<mpfr_class>("0.499992460848751668441190360024540741752242879565548017176883304716370989218399797418478685e-25"),
         boost::lexical_cast<mpfr_class>("-0.196211614533318174187346267877390498735734213905206562766348625767919122511096089367364025e-30"),
         boost::lexical_cast<mpfr_class>("0.874722648949676363732094858062907290148733370978226751462386623191111439121706262759209573e-37"),
         boost::lexical_cast<mpfr_class>("-0.163907874717737848669759890242660846846105433791283903651924563157080252845636658802930428e-44"),
      };
      T result = d[0];
      for(int k = 1; k < sizeof(d)/sizeof(d[0]); ++k)
      {
         result += d[k]/(z+(k-1));
      }
      return result;
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      using namespace boost;
      static const mpfr_class d[61] = {
         boost::lexical_cast<mpfr_class>("0.901751806425638853077358552989167785490911341809902155556127108480303870921448984935411583e-27"),
         boost::lexical_cast<mpfr_class>("4.80241125306810017699523302110401965428995345115391817406006361151407344955277298373661032"),
         boost::lexical_cast<mpfr_class>("-108.119283021710869401330097315436214587270846871451487282117128515476478251641970487922552"),
         boost::lexical_cast<mpfr_class>("1177.78262074811362219818923738088833932279000985161077740440010901595132448469513438139561"),
         boost::lexical_cast<mpfr_class>("-8270.43570321334374279057432173814835581983913167617217749736484999327758232081395983082867"),
         boost::lexical_cast<mpfr_class>("42079.807161997077661752306902088979258826568702655699995911391774839958572703348502730394"),
         boost::lexical_cast<mpfr_class>("-165326.003834443330215001219988296482004968548294447320869281647211603153902436231468280089"),
         boost::lexical_cast<mpfr_class>("521978.361504895300685499370463597042533432134369277742485307843747923127933979566742421213"),
         boost::lexical_cast<mpfr_class>("-1360867.51629992863544553419296636395576666570468519805449755596254912681418267100657262281"),
         boost::lexical_cast<mpfr_class>("2987713.73338656161102517003716335104823650191612448011720936412226357385029800040631754755"),
         boost::lexical_cast<mpfr_class>("-5605212.64915921452169919008770165304171481697939254152852673009005154549681704553438450709"),
         boost::lexical_cast<mpfr_class>("9088186.58332916818449459635132673652700922052988327069535513580836143146727832380184335474"),
         boost::lexical_cast<mpfr_class>("-12848155.5543636394746355365819800465364996596310467415907815393346205151090486190421959769"),
         boost::lexical_cast<mpfr_class>("15949281.2867656960575878805158849857756293807220033618942867694361569866468996967761600898"),
         boost::lexical_cast<mpfr_class>("-17483546.9948295433308250581770557182576171673272450149400973735206019559576269174369907171"),
         boost::lexical_cast<mpfr_class>("17001087.8599749419660906448951424280111036786456594738278573653160553115681287326064596964"),
         boost::lexical_cast<mpfr_class>("-14718487.0643665950346574802384331125115747311674609017568623694516187494204567579595827859"),
         boost::lexical_cast<mpfr_class>("11377468.7255609717716845971105161298889777425898291183866813269222281486121330837791392732"),
         boost::lexical_cast<mpfr_class>("-7870571.64253038587947746661946939286858490057774448573157856145556080330153403858747655263"),
         boost::lexical_cast<mpfr_class>("4880783.08440908743641723492059912671377140680710345996273343885045364205895751515063844239"),
         boost::lexical_cast<mpfr_class>("-2716626.7992639625103140035635916455652302249897918893040695025407382316653674141983775542"),
         boost::lexical_cast<mpfr_class>("1358230.46602865696544327299659410214201327791319846880787515156343361311278133805428800255"),
         boost::lexical_cast<mpfr_class>("-610228.440751458395860905749312275043435828322076830117123636938979942213530882048883969802"),
         boost::lexical_cast<mpfr_class>("246375.416501158654327780901087115642493055617468601787093268312234390446439555559050129729"),
         boost::lexical_cast<mpfr_class>("-89360.2599028475206119333931211015869139511677735549267100272095432140508089207221096740632"),
         boost::lexical_cast<mpfr_class>("29096.4637987498328341260960356772198979319790332957125131055960448759586930781530063775634"),
         boost::lexical_cast<mpfr_class>("-8496.57401431514433694413130585404918350686834939156759654375188338156288564260152505382438"),
         boost::lexical_cast<mpfr_class>("2222.11523574301594407443285016240908726891841242444092960094015874546135316534057865883047"),
         boost::lexical_cast<mpfr_class>("-519.599993280949289705514822058693289933461756514489674453254304308040888101533569480646682"),
         boost::lexical_cast<mpfr_class>("108.406868361306987817730701109400305482972790224573776407166683184990131682003417239181112"),
         boost::lexical_cast<mpfr_class>("-20.1313142142558596796857948064047373605439974799116521459977609253211918146595346493447238"),
         boost::lexical_cast<mpfr_class>("3.31806787671783168020012913552384112429614503798293169239082032849759155847394955909684383"),
         boost::lexical_cast<mpfr_class>("-0.483817477111537877685595212919784447924875428848331771524426361483392903320495411973587861"),
         boost::lexical_cast<mpfr_class>("0.0621793463102927384924303843912913542297042029136293808338022462765755471002366206711862637"),
         boost::lexical_cast<mpfr_class>("-0.00701366932085103924241526535768453911099671087892444015581511551813219720807206445462785293"),
         boost::lexical_cast<mpfr_class>("0.000691040514756294308758606917671220770856269030526647010461217455799229645004351524024364997"),
         boost::lexical_cast<mpfr_class>("-0.591529398871361458428147660822525365922497109038495896497692806150033516658042357799869656e-4"),
         boost::lexical_cast<mpfr_class>("0.437237367535177689875119370170434437030440227275583289093139147244747901678407875809020739e-5"),
         boost::lexical_cast<mpfr_class>("-0.277164853397051135996651958345647824709602266382721185838782221179129726200661453504250697e-6"),
         boost::lexical_cast<mpfr_class>("0.149506899012035980148891401548317536032574502641368034781671941165064546410613201579653674e-7"),
         boost::lexical_cast<mpfr_class>("-0.68023824066463262779882895193964639544061678698791279217407325790147925675797085217462974e-9"),
         boost::lexical_cast<mpfr_class>("0.258460163734186329938721529982859244969655253624066115559707985878606277800329299821882688e-10"),
         boost::lexical_cast<mpfr_class>("-0.810792256024669306744649981276512583535251727474303382740940985102669076169168931092026491e-12"),
         boost::lexical_cast<mpfr_class>("0.207274966207031327521921078048021807442500113231320959236850963529304158700096495799022922e-13"),
         boost::lexical_cast<mpfr_class>("-0.425399199286327802950259994834798737777721414442095221716122926637623478450472871269742436e-15"),
         boost::lexical_cast<mpfr_class>("0.688866766744198529064607574117697940084548375790020728788313274612845280173338912495478431e-17"),
         boost::lexical_cast<mpfr_class>("-0.862599751805643281578607291655858333628582704771553874199104377131082877406179933909898885e-19"),
         boost::lexical_cast<mpfr_class>("0.815756005678735657200275584442908437977926312650210429668619446123450972547018343768177988e-21"),
         boost::lexical_cast<mpfr_class>("-0.566583084099007858124915716926967268295318152203932871370429534546567151650626184750291695e-23"),
         boost::lexical_cast<mpfr_class>("0.279544761599725082805446124351997692260093135930731230328454667675190245860598623539891708e-25"),
         boost::lexical_cast<mpfr_class>("-0.941169851584987983984201821679114408126982142904386301937192011680047611188837432096199601e-28"),
         boost::lexical_cast<mpfr_class>("0.205866011331040736302780507155525142187875191518455173304638008169488993406425201915370746e-30"),
         boost::lexical_cast<mpfr_class>("-0.27526655245712584371295491216289353976964567057707464008951584303679019796521332324352501e-33"),
         boost::lexical_cast<mpfr_class>("0.208358067979444305082929004102609366169534624348056112144990933897581971394396210379638792e-36"),
         boost::lexical_cast<mpfr_class>("-0.808728107661779323263133007119729988596844663194254976820030366188579170595441991680169012e-40"),
         boost::lexical_cast<mpfr_class>("0.141276924383478964519776436955079978012672985961918248012931336621229652792338950573694356e-43"),
         boost::lexical_cast<mpfr_class>("-0.930318449287651389310440021745842417218125582685428432576258687100661462527604238849332053e-48"),
         boost::lexical_cast<mpfr_class>("0.179870748819321661641184169834635133045197146966203370650788171790610563029431722308057539e-52"),
         boost::lexical_cast<mpfr_class>("-0.705865243912790337263229413370018392321238639017433365017168104310561824133229343750737083e-58"),
         boost::lexical_cast<mpfr_class>("0.3146787805734405996448268100558028857930560442377698646099945108125281507396722995748398e-64"),
         boost::lexical_cast<mpfr_class>("-0.589653534231618730406843260601322236697428143603814281282790370329151249078338470962782338e-72"),
      };
      T result = d[0];
      for(int k = 1; k < sizeof(d)/sizeof(d[0]); ++k)
      {
         result += d[k]/(z+(k-1));
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      using namespace boost;
      static const mpfr_class d[60] = {
         boost::lexical_cast<mpfr_class>("23.2463658527729692390378860713647146932236940604550445351214987229819352880524561852919518"),
         boost::lexical_cast<mpfr_class>("-523.358012551815715084547614110229469295755088686612838322817729744722233637819564673967396"),
         boost::lexical_cast<mpfr_class>("5701.12892340421080714956066268650092612647620400476183901625272640935853188559347587495571"),
         boost::lexical_cast<mpfr_class>("-40033.5506451901904954336453419007623117537868026994808919201793803506999271787018654246699"),
         boost::lexical_cast<mpfr_class>("203689.884259074923009439144410340256983393397995558814367995938668111650624499963153485034"),
         boost::lexical_cast<mpfr_class>("-800270.648969745331278757692597096167418585957703057412758177038340791380011708874081291202"),
         boost::lexical_cast<mpfr_class>("2526668.23380061659863999395867315313385499515711742092815402701950519696944982260718031476"),
         boost::lexical_cast<mpfr_class>("-6587362.57559198722630391278043503867973853468105110382293763174847657538179665571836023631"),
         boost::lexical_cast<mpfr_class>("14462211.3454541602975917764900442754186801975533106565506542322063393991552960595701762805"),
         boost::lexical_cast<mpfr_class>("-27132375.1879227404375395522940895789625516798992585980380939378508607160857820002128106898"),
         boost::lexical_cast<mpfr_class>("43991923.8735251977856804364757478459275087361742168436524951824945035673768875988985478116"),
         boost::lexical_cast<mpfr_class>("-62192284.0030124679010201921841372967696262036115679150017649233887633598058364494608060812"),
         boost::lexical_cast<mpfr_class>("77203473.0770033513405070667417251568915937590689150831268228886281254637715669678358204991"),
         boost::lexical_cast<mpfr_class>("-84630180.2217173903516348977915150565994784278120192219937728967986198118628659866582594874"),
         boost::lexical_cast<mpfr_class>("82294807.2253549409847505891112074804416229757832871133388349982640444405470371147991706317"),
         boost::lexical_cast<mpfr_class>("-71245738.2484649177928765605893043553453557808557887270209768163561363857395639001251515788"),
         boost::lexical_cast<mpfr_class>("55073334.3180266913441333534260714059077572215147571872597651029894142803987981342430068594"),
         boost::lexical_cast<mpfr_class>("-38097984.1648990787690036742690550656061009857688125101191167768314179751258568264424911474"),
         boost::lexical_cast<mpfr_class>("23625729.5032184580395130592017474282828236643586203914515183078852982915252442161768527976"),
         boost::lexical_cast<mpfr_class>("-13149998.4348054726172055622442157883429575511528431835657668083088902165366619827169829685"),
         boost::lexical_cast<mpfr_class>("6574597.77221556423683199818131482663205682902023554728024972161230111356285973623550338976"),
         boost::lexical_cast<mpfr_class>("-2953848.1483469149918109110050192571921018042012905892107136410603990336401921230407043408"),
         boost::lexical_cast<mpfr_class>("1192595.29584357246380113611351829515963605337523874715861849584306265512819543347806085356"),
         boost::lexical_cast<mpfr_class>("-432553.812019608638388918135375154289816441900572658692369491476137741687213006403648722272"),
         boost::lexical_cast<mpfr_class>("140843.215385933866391177743292449477205328233960902455943995092958295858485718885800427129"),
         boost::lexical_cast<mpfr_class>("-41128.186992679630058614841985110676526199977321524879849001760603476646382839182691529968"),
         boost::lexical_cast<mpfr_class>("10756.2849191854701631989789887757784944313743544315113894758328432005767448056040879337769"),
         boost::lexical_cast<mpfr_class>("-2515.15559672041299884426826962296210458052543246529646213159169885444118227871246315458787"),
         boost::lexical_cast<mpfr_class>("524.750087004805200600237632074908875763734578390662349666321453103782638818305404274166951"),
         boost::lexical_cast<mpfr_class>("-97.4468596421732493988298219295878130651986131393383646674144877163795143930682205035917965"),
         boost::lexical_cast<mpfr_class>("16.0613108128210806736384551896802799172445298357754547684100294231532127326987175444453058"),
         boost::lexical_cast<mpfr_class>("-2.34194813526540240672426202485306862230641838409943369059203455578340880900483887447559874"),
         boost::lexical_cast<mpfr_class>("0.300982934748016059399829007219431333744032924923669397318820178988611410275964499475465815"),
         boost::lexical_cast<mpfr_class>("-0.033950095985367909789000959795708551814461844488183964432565731809399824963680858993718525"),
         boost::lexical_cast<mpfr_class>("0.00334502394288921146242772614150438404658527112198421937945605441697314216921393987758378122"),
         boost::lexical_cast<mpfr_class>("-0.000286333429067523984607730553301991502191011265745476190940771685897687956262049750683013485"),
         boost::lexical_cast<mpfr_class>("0.211647426149364947402896718485536530479491688838087899435991994237067890628274492042231115e-4"),
         boost::lexical_cast<mpfr_class>("-0.134163345121302758109675190598169832775248626443483098532368562186356128620805552609175683e-5"),
         boost::lexical_cast<mpfr_class>("0.723697303042715085329782938306424498336642078597508935450663080894255773653328980495047891e-7"),
         boost::lexical_cast<mpfr_class>("-0.329273487343139063533251321553223583999676337945788660475231347828772272134656322947906888e-8"),
         boost::lexical_cast<mpfr_class>("0.12510922551028971731767784013117088894558604838820475961392154031378891971216135267744134e-9"),
         boost::lexical_cast<mpfr_class>("-0.392468958215589939603666430583400537413757786057185505426804034745840192914621891690369903e-11"),
         boost::lexical_cast<mpfr_class>("0.100332717101049934370760667782927946803279422001380028508200697081188326364078428184546051e-12"),
         boost::lexical_cast<mpfr_class>("-0.205917088291197705194762747639836655808855850989058813560983717575008725393428497910009756e-14"),
         boost::lexical_cast<mpfr_class>("0.333450178247893143608439314203175490705783992567107481617660357577257627854979230819461489e-16"),
         boost::lexical_cast<mpfr_class>("-0.417546693906616047110563550428133589051498362676394888715581845170969319500638944065594319e-18"),
         boost::lexical_cast<mpfr_class>("0.394871691642184410859178529844325390739857256666676534513661579365702353214518478078730801e-20"),
         boost::lexical_cast<mpfr_class>("-0.274258012587811199757875927548699264063511843669070634471054184977334027224611843434000922e-22"),
         boost::lexical_cast<mpfr_class>("0.135315354265459854889496635967343027244391821142592599244505313738163473730636430399785048e-24"),
         boost::lexical_cast<mpfr_class>("-0.455579032003288910408487905303223613382276173706542364543918076752861628464036586507967767e-27"),
         boost::lexical_cast<mpfr_class>("0.99650703862462739161520123768147312466695159780582506041370833824093136783202694548427718e-30"),
         boost::lexical_cast<mpfr_class>("-0.1332444609228706921659395801935919548447859029572115502899861345555006827214220195650058e-32"),
         boost::lexical_cast<mpfr_class>("0.100856999148765307000182397631280249632761913433008379786888200467467364474581430670889392e-35"),
         boost::lexical_cast<mpfr_class>("-0.39146979455613683472384690509165395074425354524713697428673406058157887065953366609738731e-39"),
         boost::lexical_cast<mpfr_class>("0.683859606707931248105140296850112494069265272540298100341919970496564103098283709868586478e-43"),
         boost::lexical_cast<mpfr_class>("-0.450326344248604222735147147805963966503893913752040066400766411031387063854141246972061792e-47"),
         boost::lexical_cast<mpfr_class>("0.870675378039492904184581895322153006461319724931909799151743284769901586333730037761678891e-52"),
         boost::lexical_cast<mpfr_class>("-0.341678395249272265744518787745356400350877656459401143889000625280131819505857966769964401e-57"),
         boost::lexical_cast<mpfr_class>("0.152322191370871666358069530949353871960316638394428595988162174042653299702098929238880862e-63"),
         boost::lexical_cast<mpfr_class>("-0.285425405297633795767452984791738825078111150078605004958179057245980222485147999495352632e-71"),
      };
      T result = 0;
      for(int k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      using namespace boost;
      static const T d[60] = {
         boost::lexical_cast<mpfr_class>("557.56438192770795764344217888434355281097193198928944200046501607026919782564033547346298"),
         boost::lexical_cast<mpfr_class>("-12552.748616427645475141433405567201788681683808077269330800392600825597799119572762385222"),
         boost::lexical_cast<mpfr_class>("136741.650054039199076788077149441364242294724343897779563222338447737802381279007988884806"),
         boost::lexical_cast<mpfr_class>("-960205.223613240309942047656967301131022760634321049075674684679438471862998829007639437133"),
         boost::lexical_cast<mpfr_class>("4885504.47588736223774859617054275229642041717942140469884121916073195308537421162982679289"),
         boost::lexical_cast<mpfr_class>("-19194501.738192166918904824982935279260356575935661514109550613809352009246483412530545583"),
         boost::lexical_cast<mpfr_class>("60602169.8633537742937457094837494059849674261357199218329545854990149896822944801504450843"),
         boost::lexical_cast<mpfr_class>("-157997975.522506767297528502540724511908584668874487506510120462561270100749019845014382885"),
         boost::lexical_cast<mpfr_class>("346876323.86092543685419723290495817330608574729543216092477261152247521712190505658568876"),
         boost::lexical_cast<mpfr_class>("-650770365.471136883718747607976242475416651908858429752332176373467422603953536408709972919"),
         boost::lexical_cast<mpfr_class>("1055146856.05909309330903130910708372830487315684258450293308627289334336871273080305128138"),
         boost::lexical_cast<mpfr_class>("-1491682726.25614447429071368736790697283307005456720192465860871846879804207692411263924608"),
         boost::lexical_cast<mpfr_class>("1851726287.94866167094858600116562210167031458934987154557042242638980748286188183300900268"),
         boost::lexical_cast<mpfr_class>("-2029855953.68334371445800569238095379629407314338521720558391277508374519526853827142679839"),
         boost::lexical_cast<mpfr_class>("1973842002.53354868177824629525448788555435194808657489238517523691040148611221295436287925"),
         boost::lexical_cast<mpfr_class>("-1708829941.98209573247426625323314413060108441455314880934710595647408841619484540679859098"),
         boost::lexical_cast<mpfr_class>("1320934627.12433688699625456833930317624783222321555050330381730035733198249283009357314954"),
         boost::lexical_cast<mpfr_class>("-913780636.858542526294419197161614811332299249415125108737474024007693329922089123296358727"),
         boost::lexical_cast<mpfr_class>("566663423.929632170286007468016419798879660054391183200464733820209439185545886930103546787"),
         boost::lexical_cast<mpfr_class>("-315402880.436816230388857961460509181823167373029384218959199936902955049832392362044305869"),
         boost::lexical_cast<mpfr_class>("157691811.550465734461741500275930418786875005567018699867961482249002625886064187146134966"),
         boost::lexical_cast<mpfr_class>("-70848085.5705405970640618473551954585013808128304384354476488268600720054598122945113512731"),
         boost::lexical_cast<mpfr_class>("28604413.4050137708444142264980840059788755325900041515286382001704951527733220637586013815"),
         boost::lexical_cast<mpfr_class>("-10374808.7067303054787164054055989420809074792801592763124972441820101840292558840131568633"),
         boost::lexical_cast<mpfr_class>("3378126.32016207486657791623723515804931231041318964254116390764473281291389374196880720069"),
         boost::lexical_cast<mpfr_class>("-986460.090390653140964189383080344920103075349535669020623874668558777188889544398718979744"),
         boost::lexical_cast<mpfr_class>("257989.631187387317948158483575125380011593209850756066176921901006833159795100137743395985"),
         boost::lexical_cast<mpfr_class>("-60326.0391159227288325790327830741260824763549807922845004854215660451399733578621565837087"),
         boost::lexical_cast<mpfr_class>("12586.1375399649496159880821645216260841794563919652590583420570326276086323953958907053394"),
         boost::lexical_cast<mpfr_class>("-2337.26417330316922535871922886167444795452055677161063205953141105726549966801856628447293"),
         boost::lexical_cast<mpfr_class>("385.230745012608736644117458716226876976056390433401632749144285378123105481506733917763829"),
         boost::lexical_cast<mpfr_class>("-56.1716559403731491675970177460841997333796694857076749852739159067307309470690838101179615"),
         boost::lexical_cast<mpfr_class>("7.21907953468550196348585224042498727840087634483369357697580053424523903859773769748599575"),
         boost::lexical_cast<mpfr_class>("-0.814293485887386870805786409956942772883474224091975496298369877683530509729332902182019049"),
         boost::lexical_cast<mpfr_class>("0.0802304419995150047616460464220884371214157889148846405799324851793571580868840034085001373"),
         boost::lexical_cast<mpfr_class>("-0.00686771095380619535195996193943858680694970000948742557733102777115482017857981277171196115"),
         boost::lexical_cast<mpfr_class>("0.000507636621977556438232617777542864427109623356049335590894564220687567763620803789858345916"),
         boost::lexical_cast<mpfr_class>("-0.32179095465362720747836116655088181481893063531138957363431280817392443948706754917605911e-4"),
         boost::lexical_cast<mpfr_class>("0.173578890579848508947329833426585354230744194615295570820295052665075101971588563893718407e-5"),
         boost::lexical_cast<mpfr_class>("-0.789762880006288893888161070734302768702358633525134582027140158619195373770299678322596835e-7"),
         boost::lexical_cast<mpfr_class>("0.300074637200885066788470310738617992259402710843493097610337134266720909870967550606601658e-8"),
         boost::lexical_cast<mpfr_class>("-0.941337297619721713151110244242536308266701344868601679868536153775533330272973088246835359e-10"),
         boost::lexical_cast<mpfr_class>("0.24064815013182536657310186836079333949814111498828401548170442715552017773994482539471456e-11"),
         boost::lexical_cast<mpfr_class>("-0.493892399304811910466345686492277504628763169549384435563232052965821874553923373100791477e-13"),
         boost::lexical_cast<mpfr_class>("0.799780678476644196901221989475355609743387528732994566453160178199295104357319723742820593e-15"),
         boost::lexical_cast<mpfr_class>("-0.100148627870893347527249092785257443532967736956154251497569191947184705954310833302770086e-16"),
         boost::lexical_cast<mpfr_class>("0.947100256812658897084619699699028861352615460106539259289295071616221848196411749449858071e-19"),
         boost::lexical_cast<mpfr_class>("-0.657808193528898116367845405906343884364280888644748907819280236995018351085371701094007759e-21"),
         boost::lexical_cast<mpfr_class>("0.324554050057463845012469010247790763753999056976705084126950591088538742509983426730851614e-23"),
         boost::lexical_cast<mpfr_class>("-0.10927068902162908990029309141242256163212535730975970310918370355165185052827948996110107e-25"),
         boost::lexical_cast<mpfr_class>("0.239012340507870646690121104637467232366271566488184795459093215303237974655782634371712486e-28"),
         boost::lexical_cast<mpfr_class>("-0.31958700972990573259359660326375143524864710953063781494908314884519046349402409667329667e-31"),
         boost::lexical_cast<mpfr_class>("0.241905641292988284384362036555782113275737930713192053073501265726048089991747342247551645e-34"),
         boost::lexical_cast<mpfr_class>("-0.93894080230619233745797029179332447129464915420290457418429337322820997038069119047864035e-38"),
         boost::lexical_cast<mpfr_class>("0.164023814025085488413251990798690797467351995518990067783355251949198292596815470576539877e-41"),
         boost::lexical_cast<mpfr_class>("-0.108010831192689925518484618970761942019888832176355541674171850211917230280206410356465451e-45"),
         boost::lexical_cast<mpfr_class>("0.208831600642796805563854019033577205240227465154130766898180386564934443551840379116390645e-50"),
         boost::lexical_cast<mpfr_class>("-0.819516067465171848863933747691434138146789031214932473898084756489529673230665363014007306e-56"),
         boost::lexical_cast<mpfr_class>("0.365344970579318347488211604761724311582675708113250505307342682118101409913523622073678179e-62"),
         boost::lexical_cast<mpfr_class>("-0.684593199208628857931267904308244537968349564351534581234005234847904343404822808648361291e-70"),
      };
      T result = 0;
      T z = dz + 2;
      for(int k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 63.19215200000000010049916454590857028961181640625; }
};


struct mpfr_interval_lanczos
{
   static mpfr_interval lanczos_sum(const mpfr_interval& z)
   {
      unsigned long p = mpfr_class::get_dprec();
      if(p <= 72)
         return lanczos13UDT_mpfr_interval::lanczos_sum(z);
      else if(p <= 120)
         return lanczos22UDT_mpfr_interval::lanczos_sum(z);
      else if(p <= 170)
         return lanczos31UDT_mpfr_interval::lanczos_sum(z);
      else //if(p <= 370) approx 100 digit precision:
         return lanczos61UDT_mpfr_interval::lanczos_sum(z);
   }
   static mpfr_interval lanczos_sum_expG_scaled(const mpfr_interval& z)
   {
      unsigned long p = mpfr_class::get_dprec();
      if(p <= 72)
         return lanczos13UDT_mpfr_interval::lanczos_sum_expG_scaled(z);
      else if(p <= 120)
         return lanczos22UDT_mpfr_interval::lanczos_sum_expG_scaled(z);
      else if(p <= 170)
         return lanczos31UDT_mpfr_interval::lanczos_sum_expG_scaled(z);
      else //if(p <= 370) approx 100 digit precision:
         return lanczos61UDT_mpfr_interval::lanczos_sum_expG_scaled(z);
   }
   static mpfr_interval lanczos_sum_near_1(const mpfr_interval& z)
   {
      unsigned long p = mpfr_class::get_dprec();
      if(p <= 72)
         return lanczos13UDT_mpfr_interval::lanczos_sum_near_1(z);
      else if(p <= 120)
         return lanczos22UDT_mpfr_interval::lanczos_sum_near_1(z);
      else if(p <= 170)
         return lanczos31UDT_mpfr_interval::lanczos_sum_near_1(z);
      else //if(p <= 370) approx 100 digit precision:
         return lanczos61UDT_mpfr_interval::lanczos_sum_near_1(z);
   }
   static mpfr_interval lanczos_sum_near_2(const mpfr_interval& z)
   {
      unsigned long p = mpfr_class::get_dprec();
      if(p <= 72)
         return lanczos13UDT_mpfr_interval::lanczos_sum_near_2(z);
      else if(p <= 120)
         return lanczos22UDT_mpfr_interval::lanczos_sum_near_2(z);
      else if(p <= 170)
         return lanczos31UDT_mpfr_interval::lanczos_sum_near_2(z);
      else //if(p <= 370) approx 100 digit precision:
         return lanczos61UDT_mpfr_interval::lanczos_sum_near_2(z);
   }
   static mpfr_interval g()
   { 
      unsigned long p = mpfr_class::get_dprec();
      if(p <= 72)
         return lanczos13UDT_mpfr_interval::g();
      else if(p <= 120)
         return lanczos22UDT_mpfr_interval::g();
      else if(p <= 170)
         return lanczos31UDT_mpfr_interval::g();
      else //if(p <= 370) approx 100 digit precision:
         return lanczos61UDT_mpfr_interval::g();
   }
};

template<class Policy>
struct lanczos<mpfr_interval, Policy>
{
   typedef mpfr_interval_lanczos type;
};

} // namespace lanczos

template <class Policy>
inline mpfr_interval skewness(const extreme_value_distribution<mpfr_interval, Policy>& /*dist*/)
{
   return boost::lexical_cast<mpfr_class>("1.1395470994046486574927930193898461120875997958366");
}

template <class Policy>
inline mpfr_interval skewness(const rayleigh_distribution<mpfr_interval, Policy>& /*dist*/)
{
  // using namespace boost::math::constants;
  return boost::lexical_cast<mpfr_class>("0.63111065781893713819189935154422777984404221106391");
  // Computed using NTL at 150 bit, about 50 decimal digits.
  // return 2 * root_pi<RealType>() * pi_minus_three<RealType>() / pow23_four_minus_pi<RealType>();
}

template <class Policy>
inline mpfr_interval kurtosis(const rayleigh_distribution<mpfr_interval, Policy>& /*dist*/)
{
  // using namespace boost::math::constants;
  return boost::lexical_cast<mpfr_class>("3.2450893006876380628486604106197544154170667057995");
  // Computed using NTL at 150 bit, about 50 decimal digits.
  // return 3 - (6 * pi<RealType>() * pi<RealType>() - 24 * pi<RealType>() + 16) /
  // (four_minus_pi<RealType>() * four_minus_pi<RealType>());
}

template <class Policy>
inline mpfr_interval kurtosis_excess(const rayleigh_distribution<mpfr_interval, Policy>& /*dist*/)
{
  //using namespace boost::math::constants;
  // Computed using NTL at 150 bit, about 50 decimal digits.
  return boost::lexical_cast<mpfr_class>("0.2450893006876380628486604106197544154170667057995");
  // return -(6 * pi<RealType>() * pi<RealType>() - 24 * pi<RealType>() + 16) /
  //   (four_minus_pi<RealType>() * four_minus_pi<RealType>());
} // kurtosis

namespace detail{

template <class Policy>
mpfr_interval digamma_imp(mpfr_interval x, const mpl::int_<0>* t, const Policy& pol)
{
   //
   // This handles reflection of negative arguments, and all our
   // error handling, then forwards to the T-specific approximation.
   //
   BOOST_MATH_STD_USING // ADL of std functions.

   mpfr_interval result = 0;
   //
   // Check for negative arguments and use reflection:
   //
   if(x < 0)
   {
      // Reflect:
      x = 1 - x;
      // Argument reduction for tan:
      mpfr_interval remainder = x - floor(x);
      // Shift to negative if > 0.5:
      if(remainder > 0.5)
      {
         remainder -= 1;
      }
      //
      // check for evaluation at a negative pole:
      //
      if(remainder == 0)
      {
         return policies::raise_pole_error<mpfr_interval>("boost::math::digamma<%1%>(%1%)", 0, (1-x), pol);
      }
      result = constants::pi<mpfr_class>() / tan(constants::pi<mpfr_class>() * remainder);
   }
   result += big_digamma(x);
   return result;
}

template <class Policy>
mpfr_interval erf_inv_imp(const mpfr_interval& p, const mpfr_interval& q, const Policy&, const boost::mpl::int_<64>*)
{
   BOOST_MATH_STD_USING // for ADL of std names.

   mpfr_interval result = 0;
   
   if(p <= 0.5)
   {
      //
      // Evaluate inverse erf using the rational approximation:
      //
      // x = p(p+10)(Y+R(p))
      //
      // Where Y is a constant, and R(p) is optimised for a low
      // absolute error compared to |Y|.
      //
      // double: Max error found: 2.001849e-18
      // long double: Max error found: 1.017064e-20
      // Maximum Deviation Found (actual error term at infinite precision) 8.030e-21
      //
      static const float Y = 0.0891314744949340820313f;
      static const mpfr_class P[] = {    
         -0.000508781949658280665617,
         -0.00836874819741736770379,
         0.0334806625409744615033,
         -0.0126926147662974029034,
         -0.0365637971411762664006,
         0.0219878681111168899165,
         0.00822687874676915743155,
         -0.00538772965071242932965
      };
      static const mpfr_class Q[] = {    
         1,
         -0.970005043303290640362,
         -1.56574558234175846809,
         1.56221558398423026363,
         0.662328840472002992063,
         -0.71228902341542847553,
         -0.0527396382340099713954,
         0.0795283687341571680018,
         -0.00233393759374190016776,
         0.000886216390456424707504
      };
      mpfr_interval g = p * (p + 10);
      mpfr_interval r = tools::evaluate_polynomial(P, p) / tools::evaluate_polynomial(Q, p);
      result = g * Y + g * r;
   }
   else if(q >= 0.25)
   {
      //
      // Rational approximation for 0.5 > q >= 0.25
      //
      // x = sqrt(-2*log(q)) / (Y + R(q))
      //
      // Where Y is a constant, and R(q) is optimised for a low
      // absolute error compared to Y.
      //
      // double : Max error found: 7.403372e-17
      // long double : Max error found: 6.084616e-20
      // Maximum Deviation Found (error term) 4.811e-20
      //
      static const float Y = 2.249481201171875f;
      static const mpfr_class P[] = {    
         -0.202433508355938759655,
         0.105264680699391713268,
         8.37050328343119927838,
         17.6447298408374015486,
         -18.8510648058714251895,
         -44.6382324441786960818,
         17.445385985570866523,
         21.1294655448340526258,
         -3.67192254707729348546
      };
      static const mpfr_class Q[] = {    
         1,
         6.24264124854247537712,
         3.9713437953343869095,
         -28.6608180499800029974,
         -20.1432634680485188801,
         48.5609213108739935468,
         10.8268667355460159008,
         -22.6436933413139721736,
         1.72114765761200282724
      };
      mpfr_interval g = sqrt(-2 * log(q));
      mpfr_interval xs = q - 0.25;
      mpfr_interval r = tools::evaluate_polynomial(P, xs) / tools::evaluate_polynomial(Q, xs);
      result = g / (Y + r);
   }
   else
   {
      //
      // For q < 0.25 we have a series of rational approximations all
      // of the general form:
      //
      // let: x = sqrt(-log(q))
      //
      // Then the result is given by:
      //
      // x(Y+R(x-B))
      //
      // where Y is a constant, B is the lowest value of x for which 
      // the approximation is valid, and R(x-B) is optimised for a low
      // absolute error compared to Y.
      //
      // Note that almost all code will really go through the first
      // or maybe second approximation.  After than we're dealing with very
      // small input values indeed: 80 and 128 bit long double's go all the
      // way down to ~ 1e-5000 so the "tail" is rather long...
      //
      mpfr_interval x = sqrt(-log(q));
      if(x < 3)
      {
         // Max error found: 1.089051e-20
         static const float Y = 0.807220458984375f;
         static const mpfr_class P[] = {    
            -0.131102781679951906451,
            -0.163794047193317060787,
            0.117030156341995252019,
            0.387079738972604337464,
            0.337785538912035898924,
            0.142869534408157156766,
            0.0290157910005329060432,
            0.00214558995388805277169,
            -0.679465575181126350155e-6,
            0.285225331782217055858e-7,
            -0.681149956853776992068e-9
         };
         static const mpfr_class Q[] = {    
            1,
            3.46625407242567245975,
            5.38168345707006855425,
            4.77846592945843778382,
            2.59301921623620271374,
            0.848854343457902036425,
            0.152264338295331783612,
            0.01105924229346489121
         };
         mpfr_interval xs = x - 1.125;
         mpfr_interval R = tools::evaluate_polynomial(P, xs) / tools::evaluate_polynomial(Q, xs);
         result = Y * x + R * x;
      }
      else if(x < 6)
      {
         // Max error found: 8.389174e-21
         static const float Y = 0.93995571136474609375f;
         static const mpfr_class P[] = {    
            -0.0350353787183177984712,
            -0.00222426529213447927281,
            0.0185573306514231072324,
            0.00950804701325919603619,
            0.00187123492819559223345,
            0.000157544617424960554631,
            0.460469890584317994083e-5,
            -0.230404776911882601748e-9,
            0.266339227425782031962e-11
         };
         static const mpfr_class Q[] = {    
            1,
            1.3653349817554063097,
            0.762059164553623404043,
            0.220091105764131249824,
            0.0341589143670947727934,
            0.00263861676657015992959,
            0.764675292302794483503e-4
         };
         mpfr_interval xs = x - 3;
         mpfr_interval R = tools::evaluate_polynomial(P, xs) / tools::evaluate_polynomial(Q, xs);
         result = Y * x + R * x;
      }
      else if(x < 18)
      {
         // Max error found: 1.481312e-19
         static const float Y = 0.98362827301025390625f;
         static const mpfr_class P[] = {    
            -0.0167431005076633737133,
            -0.00112951438745580278863,
            0.00105628862152492910091,
            0.000209386317487588078668,
            0.149624783758342370182e-4,
            0.449696789927706453732e-6,
            0.462596163522878599135e-8,
            -0.281128735628831791805e-13,
            0.99055709973310326855e-16
         };
         static const mpfr_class Q[] = {    
            1,
            0.591429344886417493481,
            0.138151865749083321638,
            0.0160746087093676504695,
            0.000964011807005165528527,
            0.275335474764726041141e-4,
            0.282243172016108031869e-6
         };
         mpfr_interval xs = x - 6;
         mpfr_interval R = tools::evaluate_polynomial(P, xs) / tools::evaluate_polynomial(Q, xs);
         result = Y * x + R * x;
      }
      else if(x < 44)
      {
         // Max error found: 5.697761e-20
         static const float Y = 0.99714565277099609375f;
         static const mpfr_class P[] = {    
            -0.0024978212791898131227,
            -0.779190719229053954292e-5,
            0.254723037413027451751e-4,
            0.162397777342510920873e-5,
            0.396341011304801168516e-7,
            0.411632831190944208473e-9,
            0.145596286718675035587e-11,
            -0.116765012397184275695e-17
         };
         static const mpfr_class Q[] = {    
            1,
            0.207123112214422517181,
            0.0169410838120975906478,
            0.000690538265622684595676,
            0.145007359818232637924e-4,
            0.144437756628144157666e-6,
            0.509761276599778486139e-9
         };
         mpfr_interval xs = x - 18;
         mpfr_interval R = tools::evaluate_polynomial(P, xs) / tools::evaluate_polynomial(Q, xs);
         result = Y * x + R * x;
      }
      else
      {
         // Max error found: 1.279746e-20
         static const float Y = 0.99941349029541015625f;
         static const mpfr_class P[] = {    
            -0.000539042911019078575891,
            -0.28398759004727721098e-6,
            0.899465114892291446442e-6,
            0.229345859265920864296e-7,
            0.225561444863500149219e-9,
            0.947846627503022684216e-12,
            0.135880130108924861008e-14,
            -0.348890393399948882918e-21
         };
         static const mpfr_class Q[] = {    
            1,
            0.0845746234001899436914,
            0.00282092984726264681981,
            0.468292921940894236786e-4,
            0.399968812193862100054e-6,
            0.161809290887904476097e-8,
            0.231558608310259605225e-11
         };
         mpfr_interval xs = x - 44;
         mpfr_interval R = tools::evaluate_polynomial(P, xs) / tools::evaluate_polynomial(Q, xs);
         result = Y * x + R * x;
      }
   }
   return result;
}

mpfr_interval bessel_i0(mpfr_interval x)
{
    static const mpfr_class P1[] = {
        boost::lexical_cast<mpfr_class>("-2.2335582639474375249e+15"),
        boost::lexical_cast<mpfr_class>("-5.5050369673018427753e+14"),
        boost::lexical_cast<mpfr_class>("-3.2940087627407749166e+13"),
        boost::lexical_cast<mpfr_class>("-8.4925101247114157499e+11"),
        boost::lexical_cast<mpfr_class>("-1.1912746104985237192e+10"),
        boost::lexical_cast<mpfr_class>("-1.0313066708737980747e+08"),
        boost::lexical_cast<mpfr_class>("-5.9545626019847898221e+05"),
        boost::lexical_cast<mpfr_class>("-2.4125195876041896775e+03"),
        boost::lexical_cast<mpfr_class>("-7.0935347449210549190e+00"),
        boost::lexical_cast<mpfr_class>("-1.5453977791786851041e-02"),
        boost::lexical_cast<mpfr_class>("-2.5172644670688975051e-05"),
        boost::lexical_cast<mpfr_class>("-3.0517226450451067446e-08"),
        boost::lexical_cast<mpfr_class>("-2.6843448573468483278e-11"),
        boost::lexical_cast<mpfr_class>("-1.5982226675653184646e-14"),
        boost::lexical_cast<mpfr_class>("-5.2487866627945699800e-18"),
    };
    static const mpfr_class Q1[] = {
        boost::lexical_cast<mpfr_class>("-2.2335582639474375245e+15"),
        boost::lexical_cast<mpfr_class>("7.8858692566751002988e+12"),
        boost::lexical_cast<mpfr_class>("-1.2207067397808979846e+10"),
        boost::lexical_cast<mpfr_class>("1.0377081058062166144e+07"),
        boost::lexical_cast<mpfr_class>("-4.8527560179962773045e+03"),
        boost::lexical_cast<mpfr_class>("1.0"),
    };
    static const mpfr_class P2[] = {
        boost::lexical_cast<mpfr_class>("-2.2210262233306573296e-04"),
        boost::lexical_cast<mpfr_class>("1.3067392038106924055e-02"),
        boost::lexical_cast<mpfr_class>("-4.4700805721174453923e-01"),
        boost::lexical_cast<mpfr_class>("5.5674518371240761397e+00"),
        boost::lexical_cast<mpfr_class>("-2.3517945679239481621e+01"),
        boost::lexical_cast<mpfr_class>("3.1611322818701131207e+01"),
        boost::lexical_cast<mpfr_class>("-9.6090021968656180000e+00"),
    };
    static const mpfr_class Q2[] = {
        boost::lexical_cast<mpfr_class>("-5.5194330231005480228e-04"),
        boost::lexical_cast<mpfr_class>("3.2547697594819615062e-02"),
        boost::lexical_cast<mpfr_class>("-1.1151759188741312645e+00"),
        boost::lexical_cast<mpfr_class>("1.3982595353892851542e+01"),
        boost::lexical_cast<mpfr_class>("-6.0228002066743340583e+01"),
        boost::lexical_cast<mpfr_class>("8.5539563258012929600e+01"),
        boost::lexical_cast<mpfr_class>("-3.1446690275135491500e+01"),
        boost::lexical_cast<mpfr_class>("1.0"),
    };
    mpfr_interval value, factor, r;

    BOOST_MATH_STD_USING
    using namespace boost::math::tools;

    if (x < 0)
    {
        x = -x;                         // even function
    }
    if (x == 0)
    {
        return static_cast<mpfr_interval>(1);
    }
    if (x <= 15)                        // x in (0, 15]
    {
        mpfr_interval y = x * x;
        value = evaluate_polynomial(P1, y) / evaluate_polynomial(Q1, y);
    }
    else                                // x in (15, \infty)
    {
        mpfr_interval y = 1 / x - 1 / 15;
        r = evaluate_polynomial(P2, y) / evaluate_polynomial(Q2, y);
        factor = exp(x) / sqrt(x);
        value = factor * r;
    }

    return value;
}

mpfr_interval bessel_i1(mpfr_interval x)
{
    static const mpfr_class P1[] = {
        static_cast<mpfr_class>("-1.4577180278143463643e+15"),
        static_cast<mpfr_class>("-1.7732037840791591320e+14"),
        static_cast<mpfr_class>("-6.9876779648010090070e+12"),
        static_cast<mpfr_class>("-1.3357437682275493024e+11"),
        static_cast<mpfr_class>("-1.4828267606612366099e+09"),
        static_cast<mpfr_class>("-1.0588550724769347106e+07"),
        static_cast<mpfr_class>("-5.1894091982308017540e+04"),
        static_cast<mpfr_class>("-1.8225946631657315931e+02"),
        static_cast<mpfr_class>("-4.7207090827310162436e-01"),
        static_cast<mpfr_class>("-9.1746443287817501309e-04"),
        static_cast<mpfr_class>("-1.3466829827635152875e-06"),
        static_cast<mpfr_class>("-1.4831904935994647675e-09"),
        static_cast<mpfr_class>("-1.1928788903603238754e-12"),
        static_cast<mpfr_class>("-6.5245515583151902910e-16"),
        static_cast<mpfr_class>("-1.9705291802535139930e-19"),
    };
    static const mpfr_class Q1[] = {
        static_cast<mpfr_class>("-2.9154360556286927285e+15"),
        static_cast<mpfr_class>("9.7887501377547640438e+12"),
        static_cast<mpfr_class>("-1.4386907088588283434e+10"),
        static_cast<mpfr_class>("1.1594225856856884006e+07"),
        static_cast<mpfr_class>("-5.1326864679904189920e+03"),
        static_cast<mpfr_class>("1.0"),
    };
    static const mpfr_class P2[] = {
        static_cast<mpfr_class>("1.4582087408985668208e-05"),
        static_cast<mpfr_class>("-8.9359825138577646443e-04"),
        static_cast<mpfr_class>("2.9204895411257790122e-02"),
        static_cast<mpfr_class>("-3.4198728018058047439e-01"),
        static_cast<mpfr_class>("1.3960118277609544334e+00"),
        static_cast<mpfr_class>("-1.9746376087200685843e+00"),
        static_cast<mpfr_class>("8.5591872901933459000e-01"),
        static_cast<mpfr_class>("-6.0437159056137599999e-02"),
    };
    static const mpfr_class Q2[] = {
        static_cast<mpfr_class>("3.7510433111922824643e-05"),
        static_cast<mpfr_class>("-2.2835624489492512649e-03"),
        static_cast<mpfr_class>("7.4212010813186530069e-02"),
        static_cast<mpfr_class>("-8.5017476463217924408e-01"),
        static_cast<mpfr_class>("3.2593714889036996297e+00"),
        static_cast<mpfr_class>("-3.8806586721556593450e+00"),
        static_cast<mpfr_class>("1.0"),
    };
    mpfr_interval value, factor, r, w;

    BOOST_MATH_STD_USING
    using namespace boost::math::tools;

    w = abs(x);
    if (x == 0)
    {
        return static_cast<mpfr_interval>(0);
    }
    if (w <= 15)                        // w in (0, 15]
    {
        mpfr_interval y = x * x;
        r = evaluate_polynomial(P1, y) / evaluate_polynomial(Q1, y);
        factor = w;
        value = factor * r;
    }
    else                                // w in (15, \infty)
    {
        mpfr_interval y = 1 / w - mpfr_interval(1) / 15;
        r = evaluate_polynomial(P2, y) / evaluate_polynomial(Q2, y);
        factor = exp(w) / sqrt(w);
        value = factor * r;
    }

    if (x < 0)
    {
        value *= -value;                 // odd function
    }
    return value;
}

template <>
mpfr_interval big_digamma_helper(mpfr_interval x)
{
      typedef mpfr_class T;
      static const T P[61] = {
         boost::lexical_cast<T>("0.6660133691143982067148122682345055274952e81"),
         boost::lexical_cast<T>("0.6365271516829242456324234577164675383137e81"),
         boost::lexical_cast<T>("0.2991038873096202943405966144203628966976e81"),
         boost::lexical_cast<T>("0.9211116495503170498076013367421231351115e80"),
         boost::lexical_cast<T>("0.2090792764676090716286400360584443891749e80"),
         boost::lexical_cast<T>("0.3730037777359591428226035156377978092809e79"),
         boost::lexical_cast<T>("0.5446396536956682043376492370432031543834e78"),
         boost::lexical_cast<T>("0.6692523966335177847425047827449069256345e77"),
         boost::lexical_cast<T>("0.7062543624100864681625612653756619116848e76"),
         boost::lexical_cast<T>("0.6499914905966283735005256964443226879158e75"),
         boost::lexical_cast<T>("0.5280364564853225211197557708655426736091e74"),
         boost::lexical_cast<T>("0.3823205608981176913075543599005095206953e73"),
         boost::lexical_cast<T>("0.2486733714214237704739129972671154532415e72"),
         boost::lexical_cast<T>("0.1462562139602039577983434547171318011675e71"),
         boost::lexical_cast<T>("0.7821169065036815012381267259559910324285e69"),
         boost::lexical_cast<T>("0.3820552182348155468636157988764435365078e68"),
         boost::lexical_cast<T>("0.1711618296983598244658239925535632505062e67"),
         boost::lexical_cast<T>("0.7056661618357643731419080738521475204245e65"),
         boost::lexical_cast<T>("0.2685246896473614017356264531791459936036e64"),
         boost::lexical_cast<T>("0.9455168125599643085283071944864977592391e62"),
         boost::lexical_cast<T>("0.3087541626972538362237309145177486236219e61"),
         boost::lexical_cast<T>("0.9367928873352980208052601301625005737407e59"),
         boost::lexical_cast<T>("0.2645306130689794942883818547314327466007e58"),
         boost::lexical_cast<T>("0.6961815141171454309161007351079576190079e56"),
         boost::lexical_cast<T>("0.1709637824471794552313802669803885946843e55"),
         boost::lexical_cast<T>("0.3921553258481531526663112728778759311158e53"),
         boost::lexical_cast<T>("0.8409006354449988687714450897575728228696e51"),
         boost::lexical_cast<T>("0.1686755204461325935742097669030363344927e50"),
         boost::lexical_cast<T>("0.3166653542877070999007425197729038754254e48"),
         boost::lexical_cast<T>("0.5566029092358215049069560272835654229637e46"),
         boost::lexical_cast<T>("0.9161766287916328133080586672953875116242e44"),
         boost::lexical_cast<T>("1412317772330871298317974693514430627922000"),
         boost::lexical_cast<T>("20387991986727877473732570146112459874790"),
         boost::lexical_cast<T>("275557928713904105182512535678580359839.3"),
         boost::lexical_cast<T>("3485719851040516559072031256589598330.723"),
         boost::lexical_cast<T>("41247046743564028399938106707656877.40859"),
         boost::lexical_cast<T>("456274078125709314602601667471879.0147312"),
         boost::lexical_cast<T>("4714450683242899367025707077155.310613012"),
         boost::lexical_cast<T>("45453933537925041680009544258.75073849996"),
         boost::lexical_cast<T>("408437900487067278846361972.302331241052"),
         boost::lexical_cast<T>("3415719344386166273085838.705771571751035"),
         boost::lexical_cast<T>("26541502879185876562320.93134691487351145"),
         boost::lexical_cast<T>("191261415065918713661.1571433274648417668"),
         boost::lexical_cast<T>("1275349770108718421.645275944284937551702"),
         boost::lexical_cast<T>("7849171120971773.318910987434906905704272"),
         boost::lexical_cast<T>("44455946386549.80866460312682983576538056"),
         boost::lexical_cast<T>("230920362395.3198137186361608905136598046"),
         boost::lexical_cast<T>("1095700096.240863858624279930600654130254"),
         boost::lexical_cast<T>("4727085.467506050153744334085516289728134"),
         boost::lexical_cast<T>("18440.75118859447173303252421991479005424"),
         boost::lexical_cast<T>("64.62515887799460295677071749181651317052"),
         boost::lexical_cast<T>("0.201851568864688406206528472883512147547"),
         boost::lexical_cast<T>("0.0005565091674187978029138500039504078098143"),
         boost::lexical_cast<T>("0.1338097668312907986354698683493366559613e-5"),
         boost::lexical_cast<T>("0.276308225077464312820179030238305271638e-8"),
         boost::lexical_cast<T>("0.4801582970473168520375942100071070575043e-11"),
         boost::lexical_cast<T>("0.6829184144212920949740376186058541800175e-14"),
         boost::lexical_cast<T>("0.7634080076358511276617829524639455399182e-17"),
         boost::lexical_cast<T>("0.6290035083727140966418512608156646142409e-20"),
         boost::lexical_cast<T>("0.339652245667538733044036638506893821352e-23"),
         boost::lexical_cast<T>("0.9017518064256388530773585529891677854909e-27")
      };
      static const T Q[61] = {
         boost::lexical_cast<T>("0"),
         boost::lexical_cast<T>("0.1386831185456898357379390197203894063459e81"),
         boost::lexical_cast<T>("0.6467076379487574703291056110838151259438e81"),
         boost::lexical_cast<T>("0.1394967823848615838336194279565285465161e82"),
         boost::lexical_cast<T>("0.1872927317344192945218570366455046340458e82"),
         boost::lexical_cast<T>("0.1772461045338946243584650759986310355937e82"),
         boost::lexical_cast<T>("0.1267294892200258648315971144069595555118e82"),
         boost::lexical_cast<T>("0.7157764838362416821508872117623058626589e81"),
         boost::lexical_cast<T>("0.329447266909948668265277828268378274513e81"),
         boost::lexical_cast<T>("0.1264376077317689779509250183194342571207e81"),
         boost::lexical_cast<T>("0.4118230304191980787640446056583623228873e80"),
         boost::lexical_cast<T>("0.1154393529762694616405952270558316515261e80"),
         boost::lexical_cast<T>("0.281655612889423906125295485693696744275e79"),
         boost::lexical_cast<T>("0.6037483524928743102724159846414025482077e78"),
         boost::lexical_cast<T>("0.1145927995397835468123576831800276999614e78"),
         boost::lexical_cast<T>("0.1938624296151985600348534009382865995154e77"),
         boost::lexical_cast<T>("0.293980925856227626211879961219188406675e76"),
         boost::lexical_cast<T>("0.4015574518216966910319562902099567437832e75"),
         boost::lexical_cast<T>("0.4961475457509727343545565970423431880907e74"),
         boost::lexical_cast<T>("0.5565482348278933960215521991000378896338e73"),
         boost::lexical_cast<T>("0.5686112924615820754631098622770303094938e72"),
         boost::lexical_cast<T>("0.5305988545844796293285410303747469932856e71"),
         boost::lexical_cast<T>("0.4533363413802585060568537458067343491358e70"),
         boost::lexical_cast<T>("0.3553932059473516064068322757331575565718e69"),
         boost::lexical_cast<T>("0.2561198565218704414618802902533972354203e68"),
         boost::lexical_cast<T>("0.1699519313292900324098102065697454295572e67"),
         boost::lexical_cast<T>("0.1039830160862334505389615281373574959236e66"),
         boost::lexical_cast<T>("0.5873082967977428281000961954715372504986e64"),
         boost::lexical_cast<T>("0.3065255179030575882202133042549783442446e63"),
         boost::lexical_cast<T>("0.1479494813481364701208655943688307245459e62"),
         boost::lexical_cast<T>("0.6608150467921598615495180659808895663164e60"),
         boost::lexical_cast<T>("0.2732535313770902021791888953487787496976e59"),
         boost::lexical_cast<T>("0.1046402297662493314531194338414508049069e58"),
         boost::lexical_cast<T>("0.3711375077192882936085049147920021549622e56"),
         boost::lexical_cast<T>("0.1219154482883895482637944309702972234576e55"),
         boost::lexical_cast<T>("0.3708359374149458741391374452286837880162e53"),
         boost::lexical_cast<T>("0.1044095509971707189716913168889769471468e52"),
         boost::lexical_cast<T>("0.271951506225063286130946773813524945052e50"),
         boost::lexical_cast<T>("0.6548016291215163843464133978454065823866e48"),
         boost::lexical_cast<T>("0.1456062447610542135403751730809295219344e47"),
         boost::lexical_cast<T>("0.2986690175077969760978388356833006028929e45"),
         boost::lexical_cast<T>("5643149706574013350061247429006443326844000"),
         boost::lexical_cast<T>("98047545414467090421964387960743688053480"),
         boost::lexical_cast<T>("1563378767746846395507385099301468978550"),
         boost::lexical_cast<T>("22823360528584500077862274918382796495"),
         boost::lexical_cast<T>("304215527004115213046601295970388750"),
         boost::lexical_cast<T>("3690289075895685793844344966820325"),
         boost::lexical_cast<T>("40584512015702371433911456606050"),
         boost::lexical_cast<T>("402834190897282802772754873905"),
         boost::lexical_cast<T>("3589522158493606918146495750"),
         boost::lexical_cast<T>("28530557707503483723634725"),
         boost::lexical_cast<T>("200714561335055753000730"),
         boost::lexical_cast<T>("1237953783437761888641"),
         boost::lexical_cast<T>("6614698701445762950"),
         boost::lexical_cast<T>("30155495647727505"),
         boost::lexical_cast<T>("114953256021450"),
         boost::lexical_cast<T>("356398020013"),
         boost::lexical_cast<T>("863113950"),
         boost::lexical_cast<T>("1531345"),
         boost::lexical_cast<T>("1770"),
         boost::lexical_cast<T>("1")
      };
      static const T PD[60] = {
         boost::lexical_cast<T>("0.6365271516829242456324234577164675383137e81"),
         2*boost::lexical_cast<T>("0.2991038873096202943405966144203628966976e81"),
         3*boost::lexical_cast<T>("0.9211116495503170498076013367421231351115e80"),
         4*boost::lexical_cast<T>("0.2090792764676090716286400360584443891749e80"),
         5*boost::lexical_cast<T>("0.3730037777359591428226035156377978092809e79"),
         6*boost::lexical_cast<T>("0.5446396536956682043376492370432031543834e78"),
         7*boost::lexical_cast<T>("0.6692523966335177847425047827449069256345e77"),
         8*boost::lexical_cast<T>("0.7062543624100864681625612653756619116848e76"),
         9*boost::lexical_cast<T>("0.6499914905966283735005256964443226879158e75"),
         10*boost::lexical_cast<T>("0.5280364564853225211197557708655426736091e74"),
         11*boost::lexical_cast<T>("0.3823205608981176913075543599005095206953e73"),
         12*boost::lexical_cast<T>("0.2486733714214237704739129972671154532415e72"),
         13*boost::lexical_cast<T>("0.1462562139602039577983434547171318011675e71"),
         14*boost::lexical_cast<T>("0.7821169065036815012381267259559910324285e69"),
         15*boost::lexical_cast<T>("0.3820552182348155468636157988764435365078e68"),
         16*boost::lexical_cast<T>("0.1711618296983598244658239925535632505062e67"),
         17*boost::lexical_cast<T>("0.7056661618357643731419080738521475204245e65"),
         18*boost::lexical_cast<T>("0.2685246896473614017356264531791459936036e64"),
         19*boost::lexical_cast<T>("0.9455168125599643085283071944864977592391e62"),
         20*boost::lexical_cast<T>("0.3087541626972538362237309145177486236219e61"),
         21*boost::lexical_cast<T>("0.9367928873352980208052601301625005737407e59"),
         22*boost::lexical_cast<T>("0.2645306130689794942883818547314327466007e58"),
         23*boost::lexical_cast<T>("0.6961815141171454309161007351079576190079e56"),
         24*boost::lexical_cast<T>("0.1709637824471794552313802669803885946843e55"),
         25*boost::lexical_cast<T>("0.3921553258481531526663112728778759311158e53"),
         26*boost::lexical_cast<T>("0.8409006354449988687714450897575728228696e51"),
         27*boost::lexical_cast<T>("0.1686755204461325935742097669030363344927e50"),
         28*boost::lexical_cast<T>("0.3166653542877070999007425197729038754254e48"),
         29*boost::lexical_cast<T>("0.5566029092358215049069560272835654229637e46"),
         30*boost::lexical_cast<T>("0.9161766287916328133080586672953875116242e44"),
         31*boost::lexical_cast<T>("1412317772330871298317974693514430627922000"),
         32*boost::lexical_cast<T>("20387991986727877473732570146112459874790"),
         33*boost::lexical_cast<T>("275557928713904105182512535678580359839.3"),
         34*boost::lexical_cast<T>("3485719851040516559072031256589598330.723"),
         35*boost::lexical_cast<T>("41247046743564028399938106707656877.40859"),
         36*boost::lexical_cast<T>("456274078125709314602601667471879.0147312"),
         37*boost::lexical_cast<T>("4714450683242899367025707077155.310613012"),
         38*boost::lexical_cast<T>("45453933537925041680009544258.75073849996"),
         39*boost::lexical_cast<T>("408437900487067278846361972.302331241052"),
         40*boost::lexical_cast<T>("3415719344386166273085838.705771571751035"),
         41*boost::lexical_cast<T>("26541502879185876562320.93134691487351145"),
         42*boost::lexical_cast<T>("191261415065918713661.1571433274648417668"),
         43*boost::lexical_cast<T>("1275349770108718421.645275944284937551702"),
         44*boost::lexical_cast<T>("7849171120971773.318910987434906905704272"),
         45*boost::lexical_cast<T>("44455946386549.80866460312682983576538056"),
         46*boost::lexical_cast<T>("230920362395.3198137186361608905136598046"),
         47*boost::lexical_cast<T>("1095700096.240863858624279930600654130254"),
         48*boost::lexical_cast<T>("4727085.467506050153744334085516289728134"),
         49*boost::lexical_cast<T>("18440.75118859447173303252421991479005424"),
         50*boost::lexical_cast<T>("64.62515887799460295677071749181651317052"),
         51*boost::lexical_cast<T>("0.201851568864688406206528472883512147547"),
         52*boost::lexical_cast<T>("0.0005565091674187978029138500039504078098143"),
         53*boost::lexical_cast<T>("0.1338097668312907986354698683493366559613e-5"),
         54*boost::lexical_cast<T>("0.276308225077464312820179030238305271638e-8"),
         55*boost::lexical_cast<T>("0.4801582970473168520375942100071070575043e-11"),
         56*boost::lexical_cast<T>("0.6829184144212920949740376186058541800175e-14"),
         57*boost::lexical_cast<T>("0.7634080076358511276617829524639455399182e-17"),
         58*boost::lexical_cast<T>("0.6290035083727140966418512608156646142409e-20"),
         59*boost::lexical_cast<T>("0.339652245667538733044036638506893821352e-23"),
         60*boost::lexical_cast<T>("0.9017518064256388530773585529891677854909e-27")
      };
      static const T QD[60] = {
         boost::lexical_cast<T>("0.1386831185456898357379390197203894063459e81"),
         2*boost::lexical_cast<T>("0.6467076379487574703291056110838151259438e81"),
         3*boost::lexical_cast<T>("0.1394967823848615838336194279565285465161e82"),
         4*boost::lexical_cast<T>("0.1872927317344192945218570366455046340458e82"),
         5*boost::lexical_cast<T>("0.1772461045338946243584650759986310355937e82"),
         6*boost::lexical_cast<T>("0.1267294892200258648315971144069595555118e82"),
         7*boost::lexical_cast<T>("0.7157764838362416821508872117623058626589e81"),
         8*boost::lexical_cast<T>("0.329447266909948668265277828268378274513e81"),
         9*boost::lexical_cast<T>("0.1264376077317689779509250183194342571207e81"),
         10*boost::lexical_cast<T>("0.4118230304191980787640446056583623228873e80"),
         11*boost::lexical_cast<T>("0.1154393529762694616405952270558316515261e80"),
         12*boost::lexical_cast<T>("0.281655612889423906125295485693696744275e79"),
         13*boost::lexical_cast<T>("0.6037483524928743102724159846414025482077e78"),
         14*boost::lexical_cast<T>("0.1145927995397835468123576831800276999614e78"),
         15*boost::lexical_cast<T>("0.1938624296151985600348534009382865995154e77"),
         16*boost::lexical_cast<T>("0.293980925856227626211879961219188406675e76"),
         17*boost::lexical_cast<T>("0.4015574518216966910319562902099567437832e75"),
         18*boost::lexical_cast<T>("0.4961475457509727343545565970423431880907e74"),
         19*boost::lexical_cast<T>("0.5565482348278933960215521991000378896338e73"),
         20*boost::lexical_cast<T>("0.5686112924615820754631098622770303094938e72"),
         21*boost::lexical_cast<T>("0.5305988545844796293285410303747469932856e71"),
         22*boost::lexical_cast<T>("0.4533363413802585060568537458067343491358e70"),
         23*boost::lexical_cast<T>("0.3553932059473516064068322757331575565718e69"),
         24*boost::lexical_cast<T>("0.2561198565218704414618802902533972354203e68"),
         25*boost::lexical_cast<T>("0.1699519313292900324098102065697454295572e67"),
         26*boost::lexical_cast<T>("0.1039830160862334505389615281373574959236e66"),
         27*boost::lexical_cast<T>("0.5873082967977428281000961954715372504986e64"),
         28*boost::lexical_cast<T>("0.3065255179030575882202133042549783442446e63"),
         29*boost::lexical_cast<T>("0.1479494813481364701208655943688307245459e62"),
         30*boost::lexical_cast<T>("0.6608150467921598615495180659808895663164e60"),
         31*boost::lexical_cast<T>("0.2732535313770902021791888953487787496976e59"),
         32*boost::lexical_cast<T>("0.1046402297662493314531194338414508049069e58"),
         33*boost::lexical_cast<T>("0.3711375077192882936085049147920021549622e56"),
         34*boost::lexical_cast<T>("0.1219154482883895482637944309702972234576e55"),
         35*boost::lexical_cast<T>("0.3708359374149458741391374452286837880162e53"),
         36*boost::lexical_cast<T>("0.1044095509971707189716913168889769471468e52"),
         37*boost::lexical_cast<T>("0.271951506225063286130946773813524945052e50"),
         38*boost::lexical_cast<T>("0.6548016291215163843464133978454065823866e48"),
         39*boost::lexical_cast<T>("0.1456062447610542135403751730809295219344e47"),
         40*boost::lexical_cast<T>("0.2986690175077969760978388356833006028929e45"),
         41*boost::lexical_cast<T>("5643149706574013350061247429006443326844000"),
         42*boost::lexical_cast<T>("98047545414467090421964387960743688053480"),
         43*boost::lexical_cast<T>("1563378767746846395507385099301468978550"),
         44*boost::lexical_cast<T>("22823360528584500077862274918382796495"),
         45*boost::lexical_cast<T>("304215527004115213046601295970388750"),
         46*boost::lexical_cast<T>("3690289075895685793844344966820325"),
         47*boost::lexical_cast<T>("40584512015702371433911456606050"),
         48*boost::lexical_cast<T>("402834190897282802772754873905"),
         49*boost::lexical_cast<T>("3589522158493606918146495750"),
         50*boost::lexical_cast<T>("28530557707503483723634725"),
         51*boost::lexical_cast<T>("200714561335055753000730"),
         52*boost::lexical_cast<T>("1237953783437761888641"),
         53*boost::lexical_cast<T>("6614698701445762950"),
         54*boost::lexical_cast<T>("30155495647727505"),
         55*boost::lexical_cast<T>("114953256021450"),
         56*boost::lexical_cast<T>("356398020013"),
         57*boost::lexical_cast<T>("863113950"),
         58*boost::lexical_cast<T>("1531345"),
         59*boost::lexical_cast<T>("1770"),
         60*boost::lexical_cast<T>("1")
      };
   static const double g = 63.192152;

   mpfr_interval zgh = x + g - 0.5;

   mpfr_interval result = (x - 0.5) / zgh;
   result += log(zgh);
   result += tools::evaluate_polynomial(PD, x) / tools::evaluate_polynomial(P, x);
   result -= tools::evaluate_polynomial(QD, x) / tools::evaluate_polynomial(Q, x);
   result -= 1;

   return result;
}


} // namespace detail


}} // namespaces

#endif // BOOST_MATH_MPFR_INTERVAL_BINDINGS_HPP
