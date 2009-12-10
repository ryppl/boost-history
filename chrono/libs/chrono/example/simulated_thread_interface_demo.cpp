//  simulated_thread_interface_demo.cpp  ----------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*
This code was extracted by Vicente J. Botet Escriba from Beman Dawes time2_demo.cpp which
was derived by Beman Dawes from Howard Hinnant's time2_demo prototype.
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
                   Daniel Krügler,
                   Anthony Williams.
*/

#define _CRT_SECURE_NO_WARNINGS  // disable VC++ foolishness

#include <boost/chrono/chrono.hpp>
#include <boost/type_traits.hpp>

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <climits>

#undef min
#undef max

#if defined(BOOST_CHRONO_WINDOWS_API)
#include <windows.h>  

namespace
{
  //struct timeval {
  //        long    tv_sec;         /* seconds */
  //        long    tv_usec;        /* and microseconds */
  //};

  int gettimeofday(struct timeval * tp, void *)
  {
    FILETIME ft;
    ::GetSystemTimeAsFileTime( &ft );  // never fails
    long long t = (static_cast<long long>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
  # if !defined( BOOST_MSVC ) || BOOST_MSVC > 1300 // > VC++ 7.0
    t -= 116444736000000000LL;
  # else
    t -= 116444736000000000;
  # endif
    t /= 10;  // microseconds
    tp->tv_sec = static_cast<long>( t / 1000000UL);
    tp->tv_usec = static_cast<long>( t % 1000000UL);
    return 0;
  }
}  // unnamed namespace

#endif

#undef min
#undef max

//////////////////////////////////////////////////////////
///////////// simulated thread interface /////////////////
//////////////////////////////////////////////////////////

namespace std {

void __print_time(boost::chrono::system_clock::time_point t)
{
    using namespace boost::chrono;
    time_t c_time = system_clock::to_time_t(t);
    std::tm* tmptr = std::localtime(&c_time);
    system_clock::duration d = t.time_since_epoch();
    std::cout << tmptr->tm_hour << ':' << tmptr->tm_min << ':' << tmptr->tm_sec
              << '.' << (d - duration_cast<seconds>(d)).count();
}

namespace this_thread {

template <class Rep, class Period>
void sleep_for(const boost::chrono::duration<Rep, Period>& d)
{
    boost::chrono::microseconds t = boost::chrono::duration_cast<boost::chrono::microseconds>(d);
    if (t < d)
        ++t;
    if (t > boost::chrono::microseconds(0))
        std::cout << "sleep_for " << t.count() << " microseconds\n";
}

template <class Clock, class Duration>
void sleep_until(const boost::chrono::time_point<Clock, Duration>& t)
{
    using namespace boost::chrono;
    typedef time_point<Clock, Duration> Time;
    typedef system_clock::time_point SysTime;
    if (t > Clock::now())
    {
        typedef typename boost::common_type<typename Time::duration,
                                     typename SysTime::duration>::type D;
        /* auto */ D d = t - Clock::now();
        microseconds us = duration_cast<microseconds>(d);
        if (us < d)
            ++us;
        SysTime st = system_clock::now() + us;
        std::cout << "sleep_until    ";
        __print_time(st);
        std::cout << " which is " << (st - system_clock::now()).count() << " microseconds away\n";
    }
}

}  // this_thread

struct mutex {};

struct timed_mutex
{
    bool try_lock() {std::cout << "timed_mutex::try_lock()\n"; return true;}

    template <class Rep, class Period>
        bool try_lock_for(const boost::chrono::duration<Rep, Period>& d)
        {
            boost::chrono::microseconds t = boost::chrono::duration_cast<boost::chrono::microseconds>(d);
            if (t <= boost::chrono::microseconds(0))
                return try_lock();
            std::cout << "try_lock_for " << t.count() << " microseconds\n";
            return true;
        }

    template <class Clock, class Duration>
    bool try_lock_until(const boost::chrono::time_point<Clock, Duration>& t)
    {
        using namespace boost::chrono;
        typedef time_point<Clock, Duration> Time;
        typedef system_clock::time_point SysTime;
        if (t <= Clock::now())
            return try_lock();
        typedef typename boost::common_type<typename Time::duration,
          typename Clock::duration>::type D;
        /* auto */ D d = t - Clock::now();
        microseconds us = duration_cast<microseconds>(d);
        SysTime st = system_clock::now() + us;
        std::cout << "try_lock_until ";
        __print_time(st);
        std::cout << " which is " << (st - system_clock::now()).count()
          << " microseconds away\n";
        return true;
    }
};

struct condition_variable
{
    template <class Rep, class Period>
        bool wait_for(mutex&, const boost::chrono::duration<Rep, Period>& d)
        {
            boost::chrono::microseconds t = boost::chrono::duration_cast<boost::chrono::microseconds>(d);
            std::cout << "wait_for " << t.count() << " microseconds\n";
            return true;
        }

    template <class Clock, class Duration>
    bool wait_until(mutex&, const boost::chrono::time_point<Clock, Duration>& t)
    {
        using namespace boost::chrono;
        typedef time_point<Clock, Duration> Time;
        typedef system_clock::time_point SysTime;
        if (t <= Clock::now())
            return false;
        typedef typename boost::common_type<typename Time::duration,
          typename Clock::duration>::type D;
        /* auto */ D d = t - Clock::now();
        microseconds us = duration_cast<microseconds>(d);
        SysTime st = system_clock::now() + us;
         std::cout << "wait_until     ";
        __print_time(st);
        std::cout << " which is " << (st - system_clock::now()).count()
          << " microseconds away\n";
        return true;
    }
};

} // namespace std

//////////////////////////////////////////////////////////
//////////// Simple sleep and wait examples //////////////
//////////////////////////////////////////////////////////

std::mutex m;
std::timed_mutex mut;
std::condition_variable cv;

void basic_examples()
{
    std::cout << "Running basic examples\n";
    using namespace std;
    using namespace boost::chrono;
    system_clock::time_point time_limit = system_clock::now() + seconds(4) + milliseconds(500);
    this_thread::sleep_for(seconds(3));
    this_thread::sleep_for(nanoseconds(300));
    this_thread::sleep_until(time_limit);
//    this_thread::sleep_for(time_limit);  // desired compile-time error
//    this_thread::sleep_until(seconds(3)); // desired compile-time error
    mut.try_lock_for(milliseconds(30));
    mut.try_lock_until(time_limit);
//    mut.try_lock_for(time_limit);        // desired compile-time error
//    mut.try_lock_until(milliseconds(30)); // desired compile-time error
    cv.wait_for(m, minutes(1));    // real code would put this in a loop
    cv.wait_until(m, time_limit);  // real code would put this in a loop
    // For those who prefer floating point
    this_thread::sleep_for(duration<double>(0.25));
    this_thread::sleep_until(system_clock::now() + duration<double>(1.5));
}



//////////////////////////////////////////////////////////
//////////////////// User2 Example ///////////////////////
//////////////////////////////////////////////////////////

// Demonstrate User2:
// A "saturating" signed integral type  is developed.  This type has +/- infinity and a nan
// (like IEEE floating point) but otherwise obeys signed integral arithmetic.
// This class is subsequently used as the rep in boost::chrono::duration to demonstrate a
// duration class that does not silently ignore overflow.

namespace User2
{

template <class I>
class saturate
{
public:
    typedef I int_type;

    static const int_type nan = int_type(int_type(1) << (sizeof(int_type) * CHAR_BIT - 1));
    static const int_type neg_inf = nan + 1;
    static const int_type pos_inf = -neg_inf;
private:
    int_type i_;

//     static_assert(std::is_integral<int_type>::value && std::is_signed<int_type>::value,
//                   "saturate only accepts signed integral types");
//     static_assert(nan == -nan && neg_inf < pos_inf,
//                   "saturate assumes two's complement hardware for signed integrals");

public:
    saturate() : i_(nan) {}
    explicit saturate(int_type i) : i_(i) {}
    // explicit
       operator int_type() const;

    saturate& operator+=(saturate x);
    saturate& operator-=(saturate x) {return *this += -x;}
    saturate& operator*=(saturate x);
    saturate& operator/=(saturate x);
    saturate& operator%=(saturate x);

    saturate  operator- () const {return saturate(-i_);}
    saturate& operator++()       {*this += saturate(int_type(1)); return *this;}
    saturate  operator++(int)    {saturate tmp(*this); ++(*this); return tmp;}
    saturate& operator--()       {*this -= saturate(int_type(1)); return *this;}
    saturate  operator--(int)    {saturate tmp(*this); --(*this); return tmp;}

    friend saturate operator+(saturate x, saturate y) {return x += y;}
    friend saturate operator-(saturate x, saturate y) {return x -= y;}
    friend saturate operator*(saturate x, saturate y) {return x *= y;}
    friend saturate operator/(saturate x, saturate y) {return x /= y;}
    friend saturate operator%(saturate x, saturate y) {return x %= y;}

    friend bool operator==(saturate x, saturate y)
    {
        if (x.i_ == nan || y.i_ == nan)
            return false;
        return x.i_ == y.i_;
    }

    friend bool operator!=(saturate x, saturate y) {return !(x == y);}

    friend bool operator<(saturate x, saturate y)
    {
        if (x.i_ == nan || y.i_ == nan)
            return false;
        return x.i_ < y.i_;
    }

    friend bool operator<=(saturate x, saturate y)
    {
        if (x.i_ == nan || y.i_ == nan)
            return false;
        return x.i_ <= y.i_;
    }

    friend bool operator>(saturate x, saturate y)
    {
        if (x.i_ == nan || y.i_ == nan)
            return false;
        return x.i_ > y.i_;
    }

    friend bool operator>=(saturate x, saturate y)
    {
        if (x.i_ == nan || y.i_ == nan)
            return false;
        return x.i_ >= y.i_;
    }

    friend std::ostream& operator<<(std::ostream& os, saturate s)
    {
        switch (s.i_)
        {
        case pos_inf:
            return os << "inf";
        case nan:
            return os << "nan";
        case neg_inf:
            return os << "-inf";
        };
        return os << s.i_;
    }
};

template <class I>
saturate<I>::operator int_type() const
{
    switch (i_)
    {
    case nan:
    case neg_inf:
    case pos_inf:
        throw std::out_of_range("saturate special value can not convert to int_type");
    }
    return i_;
}

template <class I>
saturate<I>&
saturate<I>::operator+=(saturate x)
{
    switch (i_)
    {
    case pos_inf:
        switch (x.i_)
        {
        case neg_inf:
        case nan:
            i_ = nan;
        }
        return *this;
    case nan:
        return *this;
    case neg_inf:
        switch (x.i_)
        {
        case pos_inf:
        case nan:
            i_ = nan;
        }
        return *this;
    }
    switch (x.i_)
    {
    case pos_inf:
    case neg_inf:
    case nan:
        i_ = x.i_;
        return *this;
    }
    if (x.i_ >= 0)
    {
        if (i_ < pos_inf - x.i_)
            i_ += x.i_;
        else
            i_ = pos_inf;
        return *this;
    }
    if (i_ > neg_inf - x.i_)
        i_ += x.i_;
    else
        i_ = neg_inf;
    return *this;
}

template <class I>
saturate<I>&
saturate<I>::operator*=(saturate x)
{
    switch (i_)
    {
    case 0:
        switch (x.i_)
        {
        case pos_inf:
        case neg_inf:
        case nan:
            i_ = nan;
        }
        return *this;
    case pos_inf:
        switch (x.i_)
        {
        case nan:
        case 0:
            i_ = nan;
            return *this;
        }
        if (x.i_ < 0)
            i_ = neg_inf;
        return *this;
    case nan:
        return *this;
    case neg_inf:
        switch (x.i_)
        {
        case nan:
        case 0:
            i_ = nan;
            return *this;
        }
        if (x.i_ < 0)
            i_ = pos_inf;
        return *this;
    }
    switch (x.i_)
    {
    case 0:
        i_ = 0;
        return *this;
    case nan:
        i_ = nan;
        return *this;
    case pos_inf:
        if (i_ < 0)
            i_ = neg_inf;
        else
            i_ = pos_inf;
        return *this;
    case neg_inf:
        if (i_ < 0)
            i_ = pos_inf;
        else
            i_ = neg_inf;
        return *this;
    }
    int s = (i_ < 0 ? -1 : 1) * (x.i_ < 0 ? -1 : 1);
    i_ = i_ < 0 ? -i_ : i_;
    int_type x_i_ = x.i_ < 0 ? -x.i_ : x.i_;
    if (i_ <= pos_inf / x_i_)
        i_ *= x_i_;
    else
        i_ = pos_inf;
    i_ *= s;
    return *this;
}

template <class I>
saturate<I>&
saturate<I>::operator/=(saturate x)
{
    switch (x.i_)
    {
    case pos_inf:
    case neg_inf:
        switch (i_)
        {
        case pos_inf:
        case neg_inf:
        case nan:
            i_ = nan;
            break;
        default:
            i_ = 0;
            break;
        }
        return *this;
    case nan:
        i_ = nan;
        return *this;
    case 0:
        switch (i_)
        {
        case pos_inf:
        case neg_inf:
        case nan:
            return *this;
        case 0:
            i_ = nan;
            return *this;
        }
        if (i_ > 0)
            i_ = pos_inf;
        else
            i_ = neg_inf;
        return *this;
    }
    switch (i_)
    {
    case 0:
    case nan:
        return *this;
    case pos_inf:
    case neg_inf:
        if (x.i_ < 0)
            i_ = -i_;
        return *this;
    }
    i_ /= x.i_;
    return *this;
}

template <class I>
saturate<I>&
saturate<I>::operator%=(saturate x)
{
//    *this -= *this / x * x;  // definition
    switch (x.i_)
    {
    case nan:
    case neg_inf:
    case 0:
    case pos_inf:
        i_ = nan;
        return *this;
    }
    switch (i_)
    {
    case neg_inf:
    case pos_inf:
        i_ = nan;
    case nan:
        return *this;
    }
    i_ %= x.i_;
    return *this;
}

// Demo overflow-safe integral durations ranging from picoseconds resolution to millennium resolution
typedef boost::chrono::duration<saturate<long long>, boost::pico                 > picoseconds;
typedef boost::chrono::duration<saturate<long long>, boost::nano                 > nanoseconds;
typedef boost::chrono::duration<saturate<long long>, boost::micro                > microseconds;
typedef boost::chrono::duration<saturate<long long>, boost::milli                > milliseconds;
typedef boost::chrono::duration<saturate<long long>                            > seconds;
typedef boost::chrono::duration<saturate<long long>, boost::ratio<         60LL> > minutes;
typedef boost::chrono::duration<saturate<long long>, boost::ratio<       3600LL> > hours;
typedef boost::chrono::duration<saturate<long long>, boost::ratio<      86400LL> > days;
typedef boost::chrono::duration<saturate<long long>, boost::ratio<   31556952LL> > years;
typedef boost::chrono::duration<saturate<long long>, boost::ratio<31556952000LL> > millennium;

}  // User2

// Demonstrate custom promotion rules (needed only if there are no implicit conversions)
namespace User2 { namespace detail {

template <class T1, class T2, bool = boost::is_integral<T1>::value>
struct promote_helper;

template <class T1, class T2>
struct promote_helper<T1, saturate<T2>, true>  // integral
{
    typedef typename boost::common_type<T1, T2>::type rep;
    typedef User2::saturate<rep> type;
};

template <class T1, class T2>
struct promote_helper<T1, saturate<T2>, false>  // floating
{
    typedef T1 type;
};

} }

namespace boost
{

template <class T1, class T2>
struct common_type<User2::saturate<T1>, User2::saturate<T2> >
{
    typedef typename common_type<T1, T2>::type rep;
    typedef User2::saturate<rep> type;
};

template <class T1, class T2>
struct common_type<T1, User2::saturate<T2> >
    : User2::detail::promote_helper<T1, User2::saturate<T2> > {};

template <class T1, class T2>
struct common_type<User2::saturate<T1>, T2>
    : User2::detail::promote_helper<T2, User2::saturate<T1> > {};


// Demonstrate specialization of duration_values:

namespace chrono {

template <class I>
struct duration_values<User2::saturate<I> >
{
    typedef User2::saturate<I> Rep;
public:
    static Rep zero() {return Rep(0);}
    static Rep max()  {return Rep(Rep::pos_inf-1);}
    static Rep min()  {return -max();}
};

}  // namespace chrono

}  // namespace boost


void testUser2()
{
    std::cout << "*************\n";
    std::cout << "* testUser2 *\n";
    std::cout << "*************\n";
    using namespace User2;
    typedef seconds::rep sat;
    years yr(sat(100));
    std::cout << "100 years expressed as years = " << yr.count() << '\n';
    nanoseconds ns = yr;
    std::cout << "100 years expressed as nanoseconds = " << ns.count() << '\n';
    ns += yr;
    std::cout << "200 years expressed as nanoseconds = " << ns.count() << '\n';
    ns += yr;
    std::cout << "300 years expressed as nanoseconds = " << ns.count() << '\n';
//    yr = ns;  // does not compile
    std::cout << "yr = ns;  // does not compile\n";
//    picoseconds ps1 = yr;  // does not compile, compile-time overflow in ratio arithmetic
    std::cout << "ps = yr;  // does not compile\n";
    ns = yr;
    picoseconds ps = ns;
    std::cout << "100 years expressed as picoseconds = " << ps.count() << '\n';
    ps = ns / sat(1000);
    std::cout << "0.1 years expressed as picoseconds = " << ps.count() << '\n';
    yr = years(sat(-200000000));
    std::cout << "200 million years ago encoded in years: " << yr.count() << '\n';
    days d = boost::chrono::duration_cast<days>(yr);
    std::cout << "200 million years ago encoded in days: " << d.count() << '\n';
    millennium c = boost::chrono::duration_cast<millennium>(yr);
    std::cout << "200 million years ago encoded in millennium: " << c.count() << '\n';
    std::cout << "Demonstrate \"uninitialized protection\" behavior:\n";
    seconds sec;
    for (++sec; sec < seconds(sat(10)); ++sec)
        ;
    std::cout << sec.count() << '\n';
    std::cout << "\n";
}

void testStdUser()
{
    std::cout << "***************\n";
    std::cout << "* testStdUser *\n";
    std::cout << "***************\n";
    using namespace boost::chrono;
    hours hr = hours(100);
    std::cout << "100 hours expressed as hours = " << hr.count() << '\n';
    nanoseconds ns = hr;
    std::cout << "100 hours expressed as nanoseconds = " << ns.count() << '\n';
    ns += hr;
    std::cout << "200 hours expressed as nanoseconds = " << ns.count() << '\n';
    ns += hr;
    std::cout << "300 hours expressed as nanoseconds = " << ns.count() << '\n';
//    hr = ns;  // does not compile
    std::cout << "hr = ns;  // does not compile\n";
//    hr * ns;  // does not compile
    std::cout << "hr * ns;  // does not compile\n";
    duration<double> fs(2.5);
    std::cout << "duration<double> has count() = " << fs.count() << '\n';
//    seconds sec = fs;  // does not compile
    std::cout << "seconds sec = duration<double> won't compile\n";
    seconds sec = duration_cast<seconds>(fs);
    std::cout << "seconds has count() = " << sec.count() << '\n';
    std::cout << "\n";
}

//  timeval clock demo
//     Demonstrate the use of a timeval-like struct to be used as the representation
//     type for both duraiton and time_point.

namespace timeval_demo
{

class xtime {
private:
    long tv_sec;
    long tv_usec;

    void fixup() {
        if (tv_usec < 0) {
            tv_usec += 1000000;
            --tv_sec;
        }
    }

public:

    explicit xtime(long sec, long usec) {
        tv_sec = sec;
        tv_usec = usec;
        if (tv_usec < 0 || tv_usec >= 1000000) {
            tv_sec += tv_usec / 1000000;
            tv_usec %= 1000000;
            fixup();
        }
    }

    explicit xtime(long long usec)
    {
        tv_usec = static_cast<long>(usec % 1000000);
        tv_sec  = static_cast<long>(usec / 1000000);
        fixup();
    }
    
    // explicit
    operator long long() const {return static_cast<long long>(tv_sec) * 1000000 + tv_usec;}

    xtime& operator += (xtime rhs) {
        tv_sec += rhs.tv_sec;
        tv_usec += rhs.tv_usec;
        if (tv_usec >= 1000000) {
            tv_usec -= 1000000;
            ++tv_sec;
        }
        return *this;
    }

    xtime& operator -= (xtime rhs) {
        tv_sec -= rhs.tv_sec;
        tv_usec -= rhs.tv_usec;
        fixup();
        return *this;
    }

    xtime& operator %= (xtime rhs) {
        long long t = tv_sec * 1000000 + tv_usec;
        long long r = rhs.tv_sec * 1000000 + rhs.tv_usec;
        t %= r;
        tv_sec = static_cast<long>(t / 1000000);
        tv_usec = static_cast<long>(t % 1000000);
        fixup();
        return *this;
    }

    friend xtime operator+(xtime x, xtime y) {return x += y;}
    friend xtime operator-(xtime x, xtime y) {return x -= y;}
    friend xtime operator%(xtime x, xtime y) {return x %= y;}

    friend bool operator==(xtime x, xtime y)
        { return (x.tv_sec == y.tv_sec && x.tv_usec == y.tv_usec); }

    friend bool operator<(xtime x, xtime y) {
        if (x.tv_sec == y.tv_sec)
            return (x.tv_usec < y.tv_usec);
        return (x.tv_sec < y.tv_sec);
    }

    friend bool operator!=(xtime x, xtime y) { return !(x == y); }
    friend bool operator> (xtime x, xtime y) { return y < x; }
    friend bool operator<=(xtime x, xtime y) { return !(y < x); }
    friend bool operator>=(xtime x, xtime y) { return !(x < y); }

    friend std::ostream& operator<<(std::ostream& os, xtime x)
        {return os << '{' << x.tv_sec << ',' << x.tv_usec << '}';}
};

class xtime_clock
{
public:
    typedef xtime                                  rep;
    typedef boost::micro                           period;
    typedef boost::chrono::duration<rep, period>   duration;
    typedef boost::chrono::time_point<xtime_clock> time_point;

    static time_point now();
};

xtime_clock::time_point
xtime_clock::now()
{
#if defined(BOOST_CHRONO_WINDOWS_API)
    time_point t(duration(xtime(0)));
    gettimeofday((timeval*)&t, 0);
    return t;

#elif defined(BOOST_CHRONO_MAC_API)

    time_point t(duration(xtime(0)));
    gettimeofday((timeval*)&t, 0);
    return t;

#elif defined(BOOST_CHRONO_POSIX_API)
    timespec ts;
    ::clock_gettime( CLOCK_REALTIME, &ts );

    xtime xt( ts.tv_sec, ts.tv_nsec/1000);
    return time_point(duration(xt));
      
#endif  // POSIX
}

void test_xtime_clock()
{
    using namespace boost::chrono;
    std::cout << "timeval_demo system clock test\n";
    std::cout << "sizeof xtime_clock::time_point = " << sizeof(xtime_clock::time_point) << '\n';
    std::cout << "sizeof xtime_clock::duration = " << sizeof(xtime_clock::duration) << '\n';
    std::cout << "sizeof xtime_clock::rep = " << sizeof(xtime_clock::rep) << '\n';
    xtime_clock::duration delay(milliseconds(5));
    xtime_clock::time_point start = xtime_clock::now();
    while (xtime_clock::now() - start <= delay)
    {
    }
    xtime_clock::time_point stop = xtime_clock::now();
    xtime_clock::duration elapsed = stop - start;
    std::cout << "paused " << nanoseconds(elapsed).count() << " nanoseconds\n";
}

}  // timeval_demo

// Handle duration with resolution not known until run time

namespace runtime_resolution
{

class duration
{
public:
    typedef long long rep;
private:
    rep rep_;

    static const double ticks_per_nanosecond;

public:
    typedef boost::chrono::duration<double, boost::nano> tonanosec;

    duration() {} // = default;
    explicit duration(const rep& r) : rep_(r) {}

    // conversions
    explicit duration(const tonanosec& d)
            : rep_(static_cast<rep>(d.count() * ticks_per_nanosecond)) {}

    // explicit
       operator tonanosec() const {return tonanosec(rep_/ticks_per_nanosecond);}

    // observer

    rep count() const {return rep_;}

    // arithmetic

    duration& operator+=(const duration& d) {rep_ += d.rep_; return *this;}
    duration& operator-=(const duration& d) {rep_ += d.rep_; return *this;}
    duration& operator*=(rep rhs)           {rep_ *= rhs; return *this;}
    duration& operator/=(rep rhs)           {rep_ /= rhs; return *this;}

    duration  operator+() const {return *this;}
    duration  operator-() const {return duration(-rep_);}
    duration& operator++()      {++rep_; return *this;}
    duration  operator++(int)   {return duration(rep_++);}
    duration& operator--()      {--rep_; return *this;}
    duration  operator--(int)   {return duration(rep_--);}

    friend duration operator+(duration x, duration y) {return x += y;}
    friend duration operator-(duration x, duration y) {return x -= y;}
    friend duration operator*(duration x, rep y)      {return x *= y;}
    friend duration operator*(rep x, duration y)      {return y *= x;}
    friend duration operator/(duration x, rep y)      {return x /= y;}

    friend bool operator==(duration x, duration y) {return x.rep_ == y.rep_;}
    friend bool operator!=(duration x, duration y) {return !(x == y);}
    friend bool operator< (duration x, duration y) {return x.rep_ < y.rep_;}
    friend bool operator<=(duration x, duration y) {return !(y < x);}
    friend bool operator> (duration x, duration y) {return y < x;}
    friend bool operator>=(duration x, duration y) {return !(x < y);}
};

static
double
init_duration()
{
    //mach_timebase_info_data_t MachInfo;
    //mach_timebase_info(&MachInfo);
    //return static_cast<double>(MachInfo.denom) / MachInfo.numer;
    return static_cast<double>(1) / 1000; // Windows FILETIME is 1 per microsec 
}

const double duration::ticks_per_nanosecond = init_duration();

class clock;

class time_point
{
public:
    typedef runtime_resolution::clock clock;
    typedef long long rep;
private:
    rep rep_;


    rep count() const {return rep_;}
public:

    time_point() : rep_(0) {}
    explicit time_point(const duration& d)
        : rep_(d.count()) {}

    // arithmetic

    time_point& operator+=(const duration& d) {rep_ += d.count(); return *this;}
    time_point& operator-=(const duration& d) {rep_ -= d.count(); return *this;}

    friend time_point operator+(time_point x, duration y) {return x += y;}
    friend time_point operator+(duration x, time_point y) {return y += x;}
    friend time_point operator-(time_point x, duration y) {return x -= y;}
    friend duration operator-(time_point x, time_point y) {return duration(x.rep_ - y.rep_);}
};

class clock
{
public:
    typedef duration::rep rep;
    typedef runtime_resolution::duration duration;
    typedef runtime_resolution::time_point time_point;

    static time_point now()
    {
        
#if defined(BOOST_CHRONO_WINDOWS_API)
        timeval tv;
        gettimeofday( &tv, 0 );
        return time_point(duration((static_cast<rep>(tv.tv_sec)<<32) | tv.tv_usec));

#elif defined(BOOST_CHRONO_MAC_API)

        timeval tv;
        gettimeofday( &tv, 0 );
        return time_point(duration((static_cast<rep>(tv.tv_sec)<<32) | tv.tv_usec));

#elif defined(BOOST_CHRONO_POSIX_API)
        timespec ts;
        ::clock_gettime( CLOCK_REALTIME, &ts );
        return time_point(duration((static_cast<rep>(ts.tv_sec)<<32) | ts.tv_nsec/1000));
      
#endif  // POSIX

    }
};

void test()
{
    using namespace boost::chrono;
    std::cout << "runtime_resolution test\n";
    clock::duration delay(boost::chrono::milliseconds(5));
    clock::time_point start = clock::now();
    while (clock::now() - start <= delay)
      ;
    clock::time_point stop = clock::now();
    clock::duration elapsed = stop - start;
    std::cout << "paused " << nanoseconds(duration_cast<nanoseconds>(duration::tonanosec(elapsed))).count()
                           << " nanoseconds\n";
}

}  // runtime_resolution


int main()
{
    runtime_resolution::test();
    return 0;
}

