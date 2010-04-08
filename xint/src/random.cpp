
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Contains the definitions for the random number functions.
*/

#ifdef _WIN32
    #define STRICT
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>

#ifdef XINT_THREADSAFE
    #include <boost/thread.hpp>
#endif

namespace boost {
namespace xint {

using namespace detail;

namespace {

class generator_t {
    public:
    typedef base_random_generator::result_type result_type;
    typedef boost::mt19937 default_random_t;

    #ifdef XINT_THREADSAFE
        generator_t() { mLock.lock(); init(); }
        ~generator_t() { mLock.unlock(); }
    #else
        generator_t() { init(); }
    #endif

    result_type operator()() { return mGeneratorObj->operator()(); }

    static void set_generator(base_random_generator *p) { mGeneratorObj.reset(p); }

    private:
    void init() {
        if (mGeneratorObj.get() == 0) {
            typedef default_random_t T;
            T *genobj(new T(boost::uint32_t(time(0)+clock())));
            random_generator<T> *obj=new random_generator<T>(genobj);
            set_generator(obj);
        }
    }

    static std::auto_ptr<base_random_generator> mGeneratorObj;

    #ifdef XINT_THREADSAFE
        static boost::mutex mLock;
    #endif
};

std::auto_ptr<base_random_generator> generator_t::mGeneratorObj;

#ifdef XINT_THREADSAFE
    boost::mutex generator_t::mLock;
#endif

} // namespace

namespace detail {

void set_random_generator(base_random_generator *obj) {
    generator_t::set_generator(obj);
}

unsigned int get_random() {
    generator_t gen;
    return gen();
}

} // namespace detail

////////////////////////////////////////////////////////////////////////////////
// The secure random generator

//! @cond
#ifdef _WIN32
    struct strong_random_generator::impl_t {
        typedef BOOLEAN (WINAPI *RtlGenRandomFn)(PVOID, ULONG);
        typedef DWORD result_type;

        impl_t(): dll(0), fn(0) {
            // This should work under WinXP, Vista, and Win7. No guarantees about
            // future compatibility, but I doubt that Microsoft will get rid of it
            // (it's too useful), and I doubt that they'll change it now that it's
            // well-known (it would break too many programs). We could also use the
            // rand_s function in more recent versions of Visual C++, but that
            // causes compatibility problems with older versions of Windows.
            dll=LoadLibraryA("Advapi32.dll");
            if (dll != 0) fn=RtlGenRandomFn(GetProcAddress(dll, "SystemFunction036"));
            if (fn == 0) {
                destroy();
                throw no_strong_random();
            }
        }

        ~impl_t() { destroy(); }

        result_type operator()() {
            result_type r=0;
            if (!fn(&r, sizeof(result_type)))
                throw no_strong_random("RtlGenRandom failed");
            return r;
        }

        void destroy() { if (dll) FreeLibrary(dll); }

        HMODULE dll;
        RtlGenRandomFn fn;
    };

    double strong_random_generator::entropy() const { return 32; }
#else
    struct strong_random_generator::impl_t {
        typedef unsigned char result_type;

        impl_t(): rng("/dev/urandom", std::ios::binary) {
            // This should be supported under most non-Windows systems. Note
            // that we're using /dev/urandom, not /dev/random -- /dev/random is
            // more secure, but it can be VERY slow.
            if (!rng) throw no_strong_random();
        }

        result_type operator()() {
            int r=rng.get();
            if (r==EOF) throw no_strong_random("/dev/urandom returned EOF");
            return static_cast<result_type>(r);
        }

        std::ifstream rng;
    };

    double strong_random_generator::entropy() const { return 8; }
#endif
//! @endcond

const bool strong_random_generator::has_fixed_range = true;
const strong_random_generator::result_type strong_random_generator::min_value =
    (std::numeric_limits<impl_t::result_type>::min)();
const strong_random_generator::result_type strong_random_generator::max_value =
    (std::numeric_limits<impl_t::result_type>::max)();
strong_random_generator::strong_random_generator(): impl(new impl_t) { }
strong_random_generator::~strong_random_generator() { delete impl; }
strong_random_generator::result_type strong_random_generator::operator()() {
    return (*impl)(); }

//! @cond
strong_random_generator::result_type strong_random_generator::min
    BOOST_PREVENT_MACRO_SUBSTITUTION () const { return min_value; }
strong_random_generator::result_type strong_random_generator::max
    BOOST_PREVENT_MACRO_SUBSTITUTION () const { return max_value; }
//! @endcond

////////////////////////////////////////////////////////////////////////////////
// Returns a positive (unless told otherwise) integer between zero and
// (1<<bits)-1, inclusive

namespace core {
integer random_by_size(size_t bits, bool high_bit_on, bool low_bit_on, bool
    can_be_negative)
{
    if (bits<=0) return integer::zero();

    generator_t randomGenerator;
    const size_t cBitsPerIteration=std::numeric_limits<generator_t::result_type>::digits;

    // Grab a set of random bits, of at least the specified size
    size_t iterations = (bits+cBitsPerIteration-1) / cBitsPerIteration;
    std::vector<generator_t::result_type> v;
    for (size_t i=0; i<iterations; ++i) v.push_back(randomGenerator());

    const char *vptr=(const char *)&v[0], *vptr_end=vptr+(v.size() * sizeof(generator_t::result_type));
    integer p=from_binary(std::string(vptr, vptr_end));

    // Trim it to the proper length
    size_t index=(bits/bits_per_digit);
    digit_t mask=(digit_t(1) << (bits % bits_per_digit))-1;
    if (mask==0) { mask=digit_mask; --index; }
    p._get_data()->digits[index] &= mask;
    for (digit_t *i=p._get_data()->digits+index+1,
        *ie=p._get_data()->digits+p._get_data()->mLength; i<ie; ++i) *i=0;
    p._get_data()->skipLeadingZeros();

    if (high_bit_on) setbit(p, bits-1);
    if (low_bit_on) setbit(p, 0);
    if (can_be_negative) p._set_negative(randomGenerator() & 0x01);

    return p;
}
} // namespace core

/*! \brief Generates a random integer with specific attributes.

\param[in] bits The maximum number of bits that you want the returned number to
have.
\param[in] high_bit_on If \c true, the returned number will have exactly the
requested size. If \c false, the upper bits may be zero, resulting in a number
that is slightly smaller than requested.
\param[in] low_bit_on If \c true, the returned number will always be odd. If
\c false, it has an equal chance of being odd or even.
\param[in] can_be_negative If \c true, the returned value has an equal chance
of being positive or negative. If \c false, it will always be positive.

\returns A random integer with the requested attributes.

\remarks
This function uses the currently-defined random generator.

\see \ref random
\see xint::set_random_generator
*/
integer random_by_size(size_t bits, bool high_bit_on, bool low_bit_on, bool
    can_be_negative)
{
    try {
        return integer(core::random_by_size(bits, high_bit_on, low_bit_on,
            can_be_negative));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        return integer::nan();
    }
}

} // namespace xint
} // namespace boost
