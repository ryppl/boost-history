//  chrono.hpp  --------------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*

This code was derived by Beman Dawes from Howard Hinnant's time2_demo prototype.
Many thanks to Howard for making his code available under the Boost license.
The original code was modified to conform to Boost conventions and to section
20.9 Time utilities [time] of the C++ committee's working paper N2798.
See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2798.pdf.

time2_demo contained this comment:

    Much thanks to Andrei Alexandrescu,
                   Walter Brown,
                   Peter Dimov,
                   Jeff Garland,
                   Terry Golubiewski,
                   Daniel Kr�gler,
                   Anthony Williams.
*/

/*

TODO:

  * Fully implement error handling, with test cases.
  * Use boost::throw_exception. (Currently not used because of an issue with Intel 11.0.)
  * Consider issues raised by Michael Marcin:

    > In the past I've seen QueryPerformanceCounter give incorrect results,
    > especially with SpeedStep processors on laptops. This was many years ago and
    > might have been fixed by service packs and drivers.
    >
    > Typically you check the results of QPC against GetTickCount to see if the
    > results are reasonable.
    > http://support.microsoft.com/kb/274323
    >
    > I've also heard of problems with QueryPerformanceCounter in multi-processor
    > systems.
    >
    > I know some people SetThreadAffinityMask to 1 for the current thread call
    > their QueryPerformance* functions then restore SetThreadAffinityMask. This
    > seems horrible to me because it forces your program to jump to another
    > physical processor if it isn't already on cpu0 but they claim it worked well
    > in practice because they called the timing functions infrequently.
    >
    > In the past I have chosen to use timeGetTime with timeBeginPeriod(1) for
    > high resolution timers to avoid these issues.

*/

#ifndef BOOST_CHRONO_CHRONO_HPP
#define BOOST_CHRONO_CHRONO_HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/detail/static_assert.hpp>

#include <iostream>

#include <ctime>
#include <climits>
#include <limits>


#include <boost/ratio.hpp>
#include <boost/type_traits/common_type.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/detail/workaround.hpp>

#ifndef BOOST_CHRONO_USES_MPL_ASSERT
#define BOOST_CHRONO_A_DURATION_REPRESENTATION_CAN_NOT_BE_A_DURATION        "A duration representation can not be a duration"
#define BOOST_CHRONO_SECOND_TEMPLATE_PARAMETER_OF_DURATION_MUST_BE_A_STD_RATIO "Second template parameter of duration must be a std::ratio"
#define BOOST_CHRONO_DURATION_PERIOD_MUST_BE_POSITIVE "duration period must be positive"
#define BOOST_CHRONO_SECOND_TEMPLATE_PARAMETER_OF_TIME_POINT_MUST_BE_A_BOOST_CHRONO_DURATION "Second template parameter of time_point must be a boost::chrono::duration"
#endif


#ifdef BOOST_CHRONO_WINDOWS_API
// The system_clock tick is 100 nanoseconds
# define BOOST_SYSTEM_CLOCK_DURATION boost::chrono::duration<boost::int_least64_t, ratio<BOOST_INTMAX_C(1), BOOST_INTMAX_C(10000000)> >
#else
# define BOOST_SYSTEM_CLOCK_DURATION boost::chrono::nanoseconds
#endif

#include <boost/config/abi_prefix.hpp> // must be the last #include

//----------------------------------------------------------------------------//
//                                                                            //
//                        20.9 Time utilities [time]                          //
//                                 synopsis                                   //
//                                                                            //
//----------------------------------------------------------------------------//

namespace boost {
namespace chrono {

  template <class Rep, class Period = ratio<1> >
    class duration;
  template <class Clock, class Duration = typename Clock::duration>
    class time_point;

    namespace detail
    {
    template <class T>
      struct is_duration
        : boost::false_type {};

    template <class Rep, class Period>
      struct is_duration<duration<Rep, Period> >
        : boost::true_type  {};
    //template <class T>
    //  struct is_duration
    //    : is_duration<typename boost::remove_cv<T>::type> {};

    template <class Duration, class Rep, bool = is_duration<Rep>::value>
    struct duration_divide_result
    {
    };

    template <class Duration, class Rep2,
        bool = (
                    (boost::is_convertible<typename Duration::rep,
                        typename common_type<typename Duration::rep, Rep2>::type>::value)
                &&  (boost::is_convertible<Rep2,
                        typename common_type<typename Duration::rep, Rep2>::type>::value)
                )
        >
    struct duration_divide_imp
    {
    };

    template <class Rep1, class Period, class Rep2>
    struct duration_divide_imp<duration<Rep1, Period>, Rep2, true>
    {
        typedef duration<typename common_type<Rep1, Rep2>::type, Period> type;
    };

    template <class Rep1, class Period, class Rep2>
    struct duration_divide_result<duration<Rep1, Period>, Rep2, false>
        : duration_divide_imp<duration<Rep1, Period>, Rep2>
    {
    };

///
    template <class Rep, class Duration, bool = is_duration<Rep>::value>
    struct duration_divide_result2
    {
    };

    template <class Rep, class Duration,
        bool = (
                    (boost::is_convertible<typename Duration::rep,
                        typename common_type<typename Duration::rep, Rep>::type>::value)
                &&  (boost::is_convertible<Rep,
                        typename common_type<typename Duration::rep, Rep>::type>::value)
                )
        >
    struct duration_divide_imp2
    {
    };

    template <class Rep1, class Rep2, class Period >
    struct duration_divide_imp2<Rep1, duration<Rep2, Period>, true>
    {
        //typedef typename common_type<Rep1, Rep2>::type type;
        typedef double type;
    };

    template <class Rep1, class Rep2, class Period >
    struct duration_divide_result2<Rep1, duration<Rep2, Period>, false>
        : duration_divide_imp2<Rep1, duration<Rep2, Period> >
    {
    };

///
    template <class Duration, class Rep, bool = is_duration<Rep>::value>
    struct duration_modulo_result
    {
    };

    template <class Duration, class Rep2,
        bool = (
                    //boost::is_convertible<typename Duration::rep,
                        //typename common_type<typename Duration::rep, Rep2>::type>::value
                //&&
    boost::is_convertible<Rep2,
                        typename common_type<typename Duration::rep, Rep2>::type>::value
                )
        >
    struct duration_modulo_imp
    {
    };

    template <class Rep1, class Period, class Rep2>
    struct duration_modulo_imp<duration<Rep1, Period>, Rep2, true>
    {
        typedef duration<typename common_type<Rep1, Rep2>::type, Period> type;
    };

    template <class Rep1, class Period, class Rep2>
    struct duration_modulo_result<duration<Rep1, Period>, Rep2, false>
        : duration_modulo_imp<duration<Rep1, Period>, Rep2>
    {
    };

  } // namespace detail
} // namespace chrono


// common_type trait specializations

template <class Rep1, class Period1, class Rep2, class Period2>
  struct common_type<chrono::duration<Rep1, Period1>,
                     chrono::duration<Rep2, Period2> >;

template <class Clock, class Duration1, class Duration2>
  struct common_type<chrono::time_point<Clock, Duration1>,
                     chrono::time_point<Clock, Duration2> >;

namespace chrono {

  // customization traits
  template <class Rep> struct treat_as_floating_point;
  template <class Rep> struct duration_values;

  // duration arithmetic
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2> >::type
//    operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2> >::type
//    operator-(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period, class Rep2>
//    typename boost::enable_if_c
//    <
//      boost::is_convertible<Rep1, typename common_type<Rep1, Rep2>::type>::value
//        && boost::is_convertible<Rep2, typename common_type<Rep1, Rep2>::type>::value,
//      duration<typename common_type<Rep1, Rep2>::type, Period>
//    >::type
//   operator*(const duration<Rep1, Period>& d, const Rep2& s);
//  template <class Rep1, class Period, class Rep2>
//    typename boost::enable_if_c
//    <
//      boost::is_convertible<Rep1, typename common_type<Rep1, Rep2>::type>::value
//        && boost::is_convertible<Rep2, typename common_type<Rep1, Rep2>::type>::value,
//      duration<typename common_type<Rep1, Rep2>::type, Period>
//    >::type
//    operator*(const Rep1& s, const duration<Rep2, Period>& d);

//  template <class Rep1, class Period, class Rep2>
//    typename boost::disable_if <detail::is_duration<Rep2>,
//      typename detail::duration_divide_result<duration<Rep1, Period>, Rep2>::type
//    >::type
//    operator/(const duration<Rep1, Period>& d, const Rep2& s);

//  template <class Rep1, class Period1, class Rep2, class Period2>
//    typename common_type<Rep1, Rep2>::type
//    operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

  // duration comparisons
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    bool operator==(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    bool operator!=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    bool operator< (const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    bool operator<=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    bool operator> (const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);
//  template <class Rep1, class Period1, class Rep2, class Period2>
//    bool operator>=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs);

  // duration_cast

  //template <class ToDuration, class Rep, class Period>
  //  ToDuration duration_cast(const duration<Rep, Period>& d);

  // convenience typedefs
  typedef duration<boost::int_least64_t, nano> nanoseconds;    // at least 64 bits needed
  typedef duration<boost::int_least64_t, micro> microseconds;  // at least 55 bits needed
  typedef duration<boost::int_least64_t, milli> milliseconds;  // at least 45 bits needed
  typedef duration<boost::int_least64_t> seconds;              // at least 35 bits needed
  typedef duration<boost::int_least32_t, ratio< 60> > minutes; // at least 29 bits needed
  typedef duration<boost::int_least32_t, ratio<3600> > hours;  // at least 23 bits needed

  // time_point arithmetic
  template <class Clock, class Duration1, class Rep2, class Period2>
    time_point<Clock, typename common_type<Duration1, duration<Rep2, Period2> >::type>
    operator+(const time_point<Clock, Duration1>& lhs, const duration<Rep2, Period2>& rhs);
  template <class Rep1, class Period1, class Clock, class Duration2>
    time_point<Clock, typename common_type<duration<Rep1, Period1>, Duration2>::type>
    operator+(const duration<Rep1, Period1>& lhs, const time_point<Clock, Duration2>& rhs);
  template <class Clock, class Duration1, class Rep2, class Period2>
    time_point<Clock, typename common_type<Duration1, duration<Rep2, Period2> >::type>
    operator-(const time_point<Clock, Duration1>& lhs, const duration<Rep2, Period2>& rhs);
  template <class Clock, class Duration1, class Duration2>
    typename common_type<Duration1, Duration2>::type
    operator-(const time_point<Clock, Duration1>& lhs, const time_point<Clock,
            Duration2>& rhs);

  // time_point comparisons
  template <class Clock, class Duration1, class Duration2>
  bool operator==(const time_point<Clock, Duration1>& lhs, const time_point<Clock,
                  Duration2>& rhs);
  template <class Clock, class Duration1, class Duration2>
  bool operator!=(const time_point<Clock, Duration1>& lhs, const time_point<Clock,
                  Duration2>& rhs);
  template <class Clock, class Duration1, class Duration2>
  bool operator< (const time_point<Clock, Duration1>& lhs, const time_point<Clock,
                  Duration2>& rhs);
  template <class Clock, class Duration1, class Duration2>
  bool operator<=(const time_point<Clock, Duration1>& lhs, const time_point<Clock,
                  Duration2>& rhs);
  template <class Clock, class Duration1, class Duration2>
  bool operator> (const time_point<Clock, Duration1>& lhs, const time_point<Clock,
                  Duration2>& rhs);
  template <class Clock, class Duration1, class Duration2>
  bool operator>=(const time_point<Clock, Duration1>& lhs, const time_point<Clock,
                  Duration2>& rhs);

  // time_point_cast
  template <class ToDuration, class Clock, class Duration>
  time_point<Clock, ToDuration> time_point_cast(const time_point<Clock, Duration>& t);

  // Clocks
  class BOOST_CHRONO_DECL system_clock;
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
  class BOOST_CHRONO_DECL monotonic_clock; // as permitted by [time.clock.monotonic]
#endif

#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
  typedef monotonic_clock high_resolution_clock;  // as permitted by [time.clock.hires]
#else
  typedef system_clock high_resolution_clock;  // as permitted by [time.clock.hires]
#endif
//----------------------------------------------------------------------------//
//                          duration helpers                                  //
//----------------------------------------------------------------------------//

  namespace detail
  {

    // duration_cast

    // duration_cast is the heart of this whole prototype.  It can convert any
    //   duration to any other.  It is also (implicitly) used in converting
    //   time_points.  The conversion is always exact if possible.  And it is
    //   always as efficient as hand written code.  If different representations
    //   are involved, care is taken to never require implicit conversions.
    //   Instead static_cast is used explicitly for every required conversion.
    //   If there are a mixture of integral and floating point representations,
    //   the use of common_type ensures that the most logical "intermediate"
    //   representation is used.
    template <class FromDuration, class ToDuration,
              class Period = typename ratio_divide<typename FromDuration::period,
              typename ToDuration::period>::type,
              bool = Period::num == 1,
              bool = Period::den == 1>
    struct duration_cast;

    // When the two periods are the same, all that is left to do is static_cast from
    //   the source representation to the target representation (which may be a no-op).
    //   This conversion is always exact as long as the static_cast from the source
    //   representation to the destination representation is exact.
    template <class FromDuration, class ToDuration, class Period>
    struct duration_cast<FromDuration, ToDuration, Period, true, true>
    {
        ToDuration operator()(const FromDuration& fd) const
        {
            return ToDuration(static_cast<typename ToDuration::rep>(fd.count()));
        }
    };

    // When the numerator of FromPeriod / ToPeriod is 1, then all we need to do is
    //   divide by the denominator of FromPeriod / ToPeriod.  The common_type of
    //   the two representations is used for the intermediate computation before
    //   static_cast'ing to the destination.
    //   This conversion is generally not exact because of the division (but could be
    //   if you get lucky on the run time value of fd.count()).
    template <class FromDuration, class ToDuration, class Period>
    struct duration_cast<FromDuration, ToDuration, Period, true, false>
    {
        ToDuration operator()(const FromDuration& fd) const
        {
            typedef typename common_type<
                typename ToDuration::rep,
                typename FromDuration::rep,
                intmax_t>::type C;
            return ToDuration(static_cast<typename ToDuration::rep>(
                              static_cast<C>(fd.count()) / static_cast<C>(Period::den)));
        }
    };

    // When the denomenator of FromPeriod / ToPeriod is 1, then all we need to do is
    //   multiply by the numerator of FromPeriod / ToPeriod.  The common_type of
    //   the two representations is used for the intermediate computation before
    //   static_cast'ing to the destination.
    //   This conversion is always exact as long as the static_cast's involved are exact.
    template <class FromDuration, class ToDuration, class Period>
    struct duration_cast<FromDuration, ToDuration, Period, false, true>
    {
        ToDuration operator()(const FromDuration& fd) const
        {
            typedef typename common_type<
              typename ToDuration::rep,
              typename FromDuration::rep,
              intmax_t>::type C;
            return ToDuration(static_cast<typename ToDuration::rep>(
                              static_cast<C>(fd.count()) * static_cast<C>(Period::num)));
        }
    };

    // When neither the numerator or denominator of FromPeriod / ToPeriod is 1, then we need to
    //   multiply by the numerator and divide by the denominator of FromPeriod / ToPeriod.  The
    //   common_type of the two representations is used for the intermediate computation before
    //   static_cast'ing to the destination.
    //   This conversion is generally not exact because of the division (but could be
    //   if you get lucky on the run time value of fd.count()).
    template <class FromDuration, class ToDuration, class Period>
    struct duration_cast<FromDuration, ToDuration, Period, false, false>
    {
        ToDuration operator()(const FromDuration& fd) const
        {
            typedef typename common_type<
              typename ToDuration::rep,
              typename FromDuration::rep,
              intmax_t>::type C;
            return ToDuration(static_cast<typename ToDuration::rep>(
               static_cast<C>(fd.count()) * static_cast<C>(Period::num)
                 / static_cast<C>(Period::den)));
        }
    };

  } // namespace detail

//----------------------------------------------------------------------------//
//                                                                            //
//      20.9.2 Time-related traits [time.traits]                              //
//                                                                            //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//      20.9.2.1 is_floating_point [time.traits.is_fp]                        //
//      Probably should have been treat_as_floating_point. Editor notifed.    //
//----------------------------------------------------------------------------//

  // Support bidirectional (non-exact) conversions for floating point rep types
  //   (or user defined rep types which specialize treat_as_floating_point).
  template <class Rep>
    struct treat_as_floating_point : boost::is_floating_point<Rep> {};

//----------------------------------------------------------------------------//
//      20.9.2.2 duration_values [time.traits.duration_values]                //
//----------------------------------------------------------------------------//

  template <class Rep>
  struct duration_values
  {
      static BOOST_CHRONO_CONSTEXPR Rep m_min_imp(boost::false_type) {return -max();}
      static BOOST_CHRONO_CONSTEXPR Rep m_min_imp(boost::true_type)  {return zero();}
  public:
      static BOOST_CHRONO_CONSTEXPR Rep zero() {return Rep(0);}
      static BOOST_CHRONO_CONSTEXPR Rep max BOOST_PREVENT_MACRO_SUBSTITUTION ()  {return (std::numeric_limits<Rep>::max)();}
      static BOOST_CHRONO_CONSTEXPR Rep min BOOST_PREVENT_MACRO_SUBSTITUTION ()  {return m_min_imp(boost::is_unsigned<Rep>());}
  };

}  // namespace chrono

//----------------------------------------------------------------------------//
//      20.9.2.3 Specializations of common_type [time.traits.specializations] //
//----------------------------------------------------------------------------//

template <class Rep1, class Period1, class Rep2, class Period2>
struct common_type<chrono::duration<Rep1, Period1>,
                   chrono::duration<Rep2, Period2> >
{
  typedef chrono::duration<typename common_type<Rep1, Rep2>::type,
                      typename boost::detail::ratio_gcd<Period1, Period2>::type> type;
};

template <class Clock, class Duration1, class Duration2>
struct common_type<chrono::time_point<Clock, Duration1>,
                   chrono::time_point<Clock, Duration2> >
{
  typedef chrono::time_point<Clock,
    typename common_type<Duration1, Duration2>::type> type;
};

//----------------------------------------------------------------------------//
//                                                                            //
//         20.9.3 Class template duration [time.duration]                     //
//                                                                            //
//----------------------------------------------------------------------------//


namespace chrono {

    template <class Rep, class Period>
    class duration
    {
    BOOST_CHRONO_STATIC_ASSERT(!boost::chrono::detail::is_duration<Rep>::value, BOOST_CHRONO_A_DURATION_REPRESENTATION_CAN_NOT_BE_A_DURATION, ());
    BOOST_CHRONO_STATIC_ASSERT(boost::detail::is_ratio<Period>::value, BOOST_CHRONO_SECOND_TEMPLATE_PARAMETER_OF_DURATION_MUST_BE_A_STD_RATIO, ());
    BOOST_CHRONO_STATIC_ASSERT(Period::num>0, BOOST_CHRONO_DURATION_PERIOD_MUST_BE_POSITIVE, ());
    public:
        typedef Rep rep;
        typedef Period period;
    private:
        rep rep_;
    public:

        BOOST_CHRONO_CONSTEXPR duration() { } // = default;
        template <class Rep2>
        BOOST_CHRONO_CONSTEXPR explicit duration(const Rep2& r,
            typename boost::enable_if_c
                <
                (   (boost::is_convertible<Rep2, rep>::value)
                &&  ((treat_as_floating_point<rep>::value)
                    || (    !treat_as_floating_point<rep>::value
                        &&  !treat_as_floating_point<Rep2>::value)
                    )
                )
                >::type* = 0)
                  : rep_(r) { }
        ~duration() {} //= default;
        duration(const duration& rhs) : rep_(rhs.rep_) {} // = default;
        duration& operator=(const duration& rhs) // = default;
        {
            if (&rhs != this) rep_= rhs.rep_;
            return *this;
        }

        // conversions
        template <class Rep2, class Period2>
        BOOST_CHRONO_CONSTEXPR duration(const duration<Rep2, Period2>& d,
            typename boost::enable_if_c
                <
                (   (treat_as_floating_point<rep>::value)
                || (    (ratio_divide<Period2, period>::type::den == 1)
                    &&  (!treat_as_floating_point<Rep2>::value))
                )
                >::type* = 0)
#ifdef        __GNUC__
            // GCC 4.2.4 refused to accept a definition at this point,
            // yet both VC++ 9.0 SP1 and Intel ia32 11.0 accepted the definition
            // without complaint. VC++ 9.0 SP1 refused to accept a later definition,
            // although that was fine with GCC 4.2.4 and Intel ia32 11.0. Thus we
            // have to support both approaches.
            ;
#else
            : rep_(duration_cast<duration>(d).count()) {}
#endif

        // observer

        BOOST_CHRONO_CONSTEXPR rep count() const {return rep_;}

        // arithmetic

        duration  operator+() const {return *this;}
        duration  operator-() const {return duration(-rep_);}
        duration& operator++()      {++rep_; return *this;}
        duration  operator++(int)   {return duration(rep_++);}
        duration& operator--()      {--rep_; return *this;}
        duration  operator--(int)   {return duration(rep_--);}

        duration& operator+=(const duration& d) {rep_ += d.count(); return *this;}
        duration& operator-=(const duration& d) {rep_ -= d.count(); return *this;}

        duration& operator*=(const rep& rhs) {rep_ *= rhs; return *this;}
        duration& operator/=(const rep& rhs) {rep_ /= rhs; return *this;}
        duration& operator%=(const rep& rhs) {rep_ %= rhs; return *this;}
        duration& operator%=(const duration& rhs) {rep_ %= rhs.count(); return *this;};
        // 20.9.3.4 duration special values [time.duration.special]

        static BOOST_CHRONO_CONSTEXPR duration zero() {return duration(duration_values<rep>::zero());}
        static BOOST_CHRONO_CONSTEXPR duration min BOOST_PREVENT_MACRO_SUBSTITUTION ()  {return duration((duration_values<rep>::min)());}
        static BOOST_CHRONO_CONSTEXPR duration max BOOST_PREVENT_MACRO_SUBSTITUTION ()  {return duration((duration_values<rep>::max)());}
    };

//----------------------------------------------------------------------------//
//      20.9.3.5 duration non-member arithmetic [time.duration.nonmember]     //
//----------------------------------------------------------------------------//

  // Duration +

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2> >::type
  operator+(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      typename common_type<duration<Rep1, Period1>,
        duration<Rep2, Period2> >::type result = lhs;
      result += rhs;
      return result;
  }

  // Duration -

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2> >::type
  operator-(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      typename common_type<duration<Rep1, Period1>,
        duration<Rep2, Period2> >::type result = lhs;
      result -= rhs;
      return result;
  }

  // Duration *

  template <class Rep1, class Period, class Rep2>
  inline
  typename boost::enable_if_c
    <
        (   (boost::is_convertible<Rep1, typename common_type<Rep1, Rep2>::type>::value)
        &&  (boost::is_convertible<Rep2, typename common_type<Rep1, Rep2>::type>::value)
        ),
        duration<typename common_type<Rep1, Rep2>::type, Period>
  >::type
  operator*(const duration<Rep1, Period>& d, const Rep2& s)
  {
      typedef typename common_type<Rep1, Rep2>::type CR;
      duration<CR, Period> r = d;
      r *= static_cast<CR>(s);
      return r;
  }

  template <class Rep1, class Period, class Rep2>
  inline
  typename boost::enable_if_c
    <
        (   (boost::is_convertible<Rep1, typename common_type<Rep1, Rep2>::type>::value)
        &&  (boost::is_convertible<Rep2, typename common_type<Rep1, Rep2>::type>::value)
        ),
        duration<typename common_type<Rep1, Rep2>::type, Period>
  >::type
  operator*(const Rep1& s, const duration<Rep2, Period>& d)
  {
      return d * s;
  }

  // Duration /

  template <class Rep1, class Period, class Rep2>
  inline
  typename boost::disable_if <boost::chrono::detail::is_duration<Rep2>,
    typename boost::chrono::detail::duration_divide_result<duration<Rep1, Period>, Rep2>::type
  >::type
  operator/(const duration<Rep1, Period>& d, const Rep2& s)
  {
      typedef typename common_type<Rep1, Rep2>::type CR;
      duration<CR, Period> r = d;
      r /= static_cast<CR>(s);
      return r;
  }

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  typename common_type<Rep1, Rep2>::type
  operator/(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      typedef typename common_type<duration<Rep1, Period1>,
                                   duration<Rep2, Period2> >::type CD;
      return CD(lhs).count() / CD(rhs).count();
  }

  template <class Rep1, class Rep2, class Period>
  inline
  typename boost::disable_if <boost::chrono::detail::is_duration<Rep1>,
    typename boost::chrono::detail::duration_divide_result2<Rep1, duration<Rep2, Period> >::type
  >::type
  operator/(const Rep1& s, const duration<Rep2, Period>& d)
  {
      typedef typename common_type<Rep1, Rep2>::type CR;
      duration<CR, Period> r = d;
      //return static_cast<CR>(r.count()) / static_cast<CR>(s);
      return  static_cast<CR>(s)/r.count();
  }

  // Duration %

  template <class Rep1, class Period, class Rep2>
  typename boost::disable_if <boost::chrono::detail::is_duration<Rep2>,
    typename boost::chrono::detail::duration_modulo_result<duration<Rep1, Period>, Rep2>::type
  >::type
  operator%(const duration<Rep1, Period>& d, const Rep2& s) {
    typedef typename common_type<Rep1, Rep2>::type CR;
    duration<CR, Period> r = d;
    r %= static_cast<CR>(s);
    return r;
  }

  template <class Rep1, class Period1, class Rep2, class Period2>
  typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2> >::type
  operator%(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs) {
    typedef typename common_type<duration<Rep1, Period1>,
                                 duration<Rep2, Period2> >::type CD;
    CD r(lhs);
    r%=CD(rhs);
    return r;
  }


//----------------------------------------------------------------------------//
//      20.9.3.6 duration comparisons [time.duration.comparisons]             //
//----------------------------------------------------------------------------//

  namespace detail
  {
    template <class LhsDuration, class RhsDuration>
    struct duration_eq
    {
        bool operator()(const LhsDuration& lhs, const RhsDuration& rhs)
            {
                typedef typename common_type<LhsDuration, RhsDuration>::type CD;
                return CD(lhs).count() == CD(rhs).count();
            }
    };

    template <class LhsDuration>
    struct duration_eq<LhsDuration, LhsDuration>
    {
        bool operator()(const LhsDuration& lhs, const LhsDuration& rhs)
            {return lhs.count() == rhs.count();}
    };

    template <class LhsDuration, class RhsDuration>
    struct duration_lt
    {
        bool operator()(const LhsDuration& lhs, const RhsDuration& rhs)
            {
                typedef typename common_type<LhsDuration, RhsDuration>::type CD;
                return CD(lhs).count() < CD(rhs).count();
            }
    };

    template <class LhsDuration>
    struct duration_lt<LhsDuration, LhsDuration>
    {
        bool operator()(const LhsDuration& lhs, const LhsDuration& rhs)
            {return lhs.count() < rhs.count();}
    };

  } // namespace detail

  // Duration ==

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  bool
  operator==(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      return boost::chrono::detail::duration_eq<duration<Rep1, Period1>, duration<Rep2, Period2> >()(lhs, rhs);
  }

  // Duration !=

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  bool
  operator!=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      return !(lhs == rhs);
  }

  // Duration <

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  bool
  operator< (const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      return boost::chrono::detail::duration_lt<duration<Rep1, Period1>, duration<Rep2, Period2> >()(lhs, rhs);
  }

  // Duration >

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  bool
  operator> (const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      return rhs < lhs;
  }

  // Duration <=

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  bool
  operator<=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      return !(rhs < lhs);
  }

  // Duration >=

  template <class Rep1, class Period1, class Rep2, class Period2>
  inline
  bool
  operator>=(const duration<Rep1, Period1>& lhs, const duration<Rep2, Period2>& rhs)
  {
      return !(lhs < rhs);
  }

//----------------------------------------------------------------------------//
//      20.9.3.7 duration_cast [time.duration.cast]                           //
//----------------------------------------------------------------------------//

  // Compile-time select the most efficient algorithm for the conversion...
  template <class ToDuration, class Rep, class Period>
  inline
  typename boost::enable_if <boost::chrono::detail::is_duration<ToDuration>, ToDuration>::type
  duration_cast(const duration<Rep, Period>& fd)
  {
      return boost::chrono::detail::duration_cast<duration<Rep, Period>, ToDuration>()(fd);
  }

//----------------------------------------------------------------------------//
//                                                                            //
//      20.9.4 Class template time_point [time.point]                         //
//                                                                            //
//----------------------------------------------------------------------------//

template <class Clock, class Duration>
  class time_point
  {
  BOOST_CHRONO_STATIC_ASSERT(boost::chrono::detail::is_duration<Duration>::value,
      BOOST_CHRONO_SECOND_TEMPLATE_PARAMETER_OF_TIME_POINT_MUST_BE_A_BOOST_CHRONO_DURATION, (Duration));
  public:
      typedef Clock                     clock;
      typedef Duration                  duration;
      typedef typename duration::rep    rep;
      typedef typename duration::period period;
  private:
      duration d_;

  public:
      time_point() : d_(duration::zero()) {}
      explicit time_point(const duration& d) : d_(d) {}

      // conversions
      template <class Duration2>
      time_point(const time_point<clock, Duration2>& t,
          typename boost::enable_if
          <
              boost::is_convertible<Duration2, duration>
          >::type* = 0)
              : d_(t.time_since_epoch()) {}

      // observer

      duration time_since_epoch() const {return d_;}

      // arithmetic

      time_point& operator+=(const duration& d) {d_ += d; return *this;}
      time_point& operator-=(const duration& d) {d_ -= d; return *this;}

      // special values

      static BOOST_CHRONO_CONSTEXPR time_point min BOOST_PREVENT_MACRO_SUBSTITUTION () {return time_point((duration::min)());}
      static BOOST_CHRONO_CONSTEXPR time_point max BOOST_PREVENT_MACRO_SUBSTITUTION () {return time_point((duration::max)());}
  };

//----------------------------------------------------------------------------//
//      20.9.4.5 time_point non-member arithmetic [time.point.nonmember]      //
//----------------------------------------------------------------------------//

  // time_point operator+(time_point x, duration y);

  template <class Clock, class Duration1, class Rep2, class Period2>
  inline
  time_point<Clock, typename common_type<Duration1, duration<Rep2, Period2> >::type>
  operator+(const time_point<Clock, Duration1>& lhs,
            const duration<Rep2, Period2>& rhs)
  {
      typedef time_point<Clock,
        typename common_type<Duration1, duration<Rep2, Period2> >::type> TimeResult;
      TimeResult r(lhs);
      r += rhs;
      return r;
  }

  // time_point operator+(duration x, time_point y);

  template <class Rep1, class Period1, class Clock, class Duration2>
  inline
  time_point<Clock, typename common_type<duration<Rep1, Period1>, Duration2>::type>
  operator+(const duration<Rep1, Period1>& lhs,
            const time_point<Clock, Duration2>& rhs)
  {
      return rhs + lhs;
  }

  // time_point operator-(time_point x, duration y);

  template <class Clock, class Duration1, class Rep2, class Period2>
  inline
  time_point<Clock, typename common_type<Duration1, duration<Rep2, Period2> >::type>
  operator-(const time_point<Clock, Duration1>& lhs,
            const duration<Rep2, Period2>& rhs)
  {
      return lhs + (-rhs);
  }

  // duration operator-(time_point x, time_point y);

  template <class Clock, class Duration1, class Duration2>
  inline
  typename common_type<Duration1, Duration2>::type
  operator-(const time_point<Clock, Duration1>& lhs,
            const time_point<Clock, Duration2>& rhs)
  {
      return lhs.time_since_epoch() - rhs.time_since_epoch();
  }

//----------------------------------------------------------------------------//
//      20.9.4.6 time_point comparisons [time.point.comparisons]              //
//----------------------------------------------------------------------------//

  // time_point ==

  template <class Clock, class Duration1, class Duration2>
  inline
  bool
  operator==(const time_point<Clock, Duration1>& lhs,
             const time_point<Clock, Duration2>& rhs)
  {
      return lhs.time_since_epoch() == rhs.time_since_epoch();
  }

  // time_point !=

  template <class Clock, class Duration1, class Duration2>
  inline
  bool
  operator!=(const time_point<Clock, Duration1>& lhs,
             const time_point<Clock, Duration2>& rhs)
  {
      return !(lhs == rhs);
  }

  // time_point <

  template <class Clock, class Duration1, class Duration2>
  inline
  bool
  operator<(const time_point<Clock, Duration1>& lhs,
            const time_point<Clock, Duration2>& rhs)
  {
      return lhs.time_since_epoch() < rhs.time_since_epoch();
  }

  // time_point >

  template <class Clock, class Duration1, class Duration2>
  inline
  bool
  operator>(const time_point<Clock, Duration1>& lhs,
            const time_point<Clock, Duration2>& rhs)
  {
      return rhs < lhs;
  }

  // time_point <=

  template <class Clock, class Duration1, class Duration2>
  inline
  bool
  operator<=(const time_point<Clock, Duration1>& lhs,
             const time_point<Clock, Duration2>& rhs)
  {
      return !(rhs < lhs);
  }

  // time_point >=

  template <class Clock, class Duration1, class Duration2>
  inline
  bool
  operator>=(const time_point<Clock, Duration1>& lhs,
             const time_point<Clock, Duration2>& rhs)
  {
      return !(lhs < rhs);
  }

//----------------------------------------------------------------------------//
//      20.9.4.7 time_point_cast [time.point.cast]                            //
//----------------------------------------------------------------------------//

  template <class ToDuration, class Clock, class Duration>
  inline
  time_point<Clock, ToDuration>
  time_point_cast(const time_point<Clock, Duration>& t)
  {
      return time_point<Clock, ToDuration>(
        duration_cast<ToDuration>(t.time_since_epoch()));
  }

//----------------------------------------------------------------------------//
//                                                                            //
//      20.9.5 Clocks [time.clock]                                            //
//                                                                            //
//----------------------------------------------------------------------------//

// If you're porting, clocks are the system-specific (non-portable) part.
// You'll need to know how to get the current time and implement that under now().
// You'll need to know what units (tick period) and representation makes the most
// sense for your clock and set those accordingly.
// If you know how to map this clock to time_t (perhaps your clock is std::time, which
// makes that trivial), then you can fill out system_clock's to_time_t() and from_time_t().

//----------------------------------------------------------------------------//
//      20.9.5.1 Class system_clock [time.clock.system]                       //
//----------------------------------------------------------------------------//

  class BOOST_CHRONO_DECL system_clock
  {
  public:
      typedef BOOST_SYSTEM_CLOCK_DURATION          duration;
      typedef duration::rep                        rep;
      typedef duration::period                     period;
      typedef chrono::time_point<system_clock>     time_point;
      static const bool is_monotonic =             false;

      static time_point  now();                         // throws on error
      static time_point  now(system::error_code & ec);  // never throws

      static std::time_t to_time_t(const time_point& t);
      static time_point  from_time_t(std::time_t t);
  };

//----------------------------------------------------------------------------//
//      20.9.5.2 Class monotonic_clock [time.clock.monotonic]                 //
//----------------------------------------------------------------------------//

// As permitted  by [time.clock.monotonic]
// The class monotonic_clock is conditionally supported.

#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
  class BOOST_CHRONO_DECL monotonic_clock
  {
  public:
      typedef nanoseconds                          duration;
      typedef duration::rep                        rep;
      typedef duration::period                     period;
      typedef chrono::time_point<monotonic_clock>  time_point;
      static const bool is_monotonic =             true;

      static time_point  now();                         // throws on error
      static time_point  now(system::error_code & ec);  // never throws
  };
#endif
//----------------------------------------------------------------------------//
//      20.9.5.3 Class high_resolution_clock [time.clock.hires]               //
//----------------------------------------------------------------------------//

//  As permitted, monotonic_clock or system_clock is a typedef for high_resolution_clock.
//  See synopsis.


//----------------------------------------------------------------------------//
//                 duration constructor implementation                        //
//              See comment in the class duration synopsis                    //
//----------------------------------------------------------------------------//

#ifdef __GNUC__
    // see comment above in section 20.9.3 Class template duration [time.duration]
    template <class Rep, class Period>
    template <class Rep2, class Period2>
    BOOST_CHRONO_CONSTEXPR duration<Rep, Period>::duration(const duration<Rep2, Period2>& d,
        typename boost::enable_if_c
            <
            (   (treat_as_floating_point<rep>::value)
            || (    (ratio_divide<Period2, period>::type::den == 1)
                &&  (!treat_as_floating_point<Rep2>::value))
            )
            >::type*)
          : rep_(duration_cast<duration>(d).count()) {}
#endif

} // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_CHRONO_HPP
