
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the random number functions.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"

#include <vector>
#include <sstream>
#include <fstream>

#include <boost/crc.hpp>
#include <boost/random/mersenne_twister.hpp>

#ifdef XINT_THREADSAFE
    #include <boost/thread.hpp>
#endif

#ifdef _WIN32
    #define STRICT
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    #include <TCHAR.h>
#endif

namespace xint {

using namespace detail;

namespace {

class generator_t {
    typedef boost::mt19937 internal_generator_t;

    public:
    typedef internal_generator_t::result_type result_type;

    #ifdef XINT_THREADSAFE
        generator_t() { mLock.lock(); init(); }
        ~generator_t() { mLock.unlock(); }
    #else
        generator_t() { init(); }
    #endif

    void seed_manual(const std::string& bytes);
    bool seed_secure();
    void seed_fallback();
    result_type operator()();

    private:
    class SeedGenerator {
        public:
        SeedGenerator(const std::string& seedstring): mString(seedstring.substr(0,
            cSeedMaximumBytes)), mNumber(0) { }
        internal_generator_t::result_type operator()() {
            std::ostringstream s1;
            s1 << mNumber++ << mString;
            std::string s2=s1.str();

            boost::crc_32_type crc;
            crc.process_bytes(s2.c_str(), s2.length());
            return crc.checksum();
        }

        #ifdef XINT_SECURE
            static char zero(char) { return 0; }

            ~SeedGenerator() {
                mNumber=0;
                std::transform(mString.begin(), mString.end(), mString.begin(),
                    zero);
            }
        #endif

        private:
        static const size_t cSeedMaximumBytes=4096;

        std::string mString;
        int mNumber;
    };

    void init();

    static internal_generator_t *mGen;
    static bool mSeeded;

    #ifdef XINT_THREADSAFE
        static boost::mutex mLock;
    #endif
};

generator_t::internal_generator_t *generator_t::mGen=0;
bool generator_t::mSeeded=false;

#ifdef XINT_THREADSAFE
    boost::mutex generator_t::mLock;
#endif

void generator_t::seed_manual(const std::string& bytes) {
    SeedGenerator gen(bytes);
    mGen->seed(gen);
    mSeeded=true;
}

bool generator_t::seed_secure() {
    const int cBitsRequested=256,
        cBytesRequested=cBitsRequested / std::numeric_limits<char>::digits;
    bool success=false;

    #ifdef _WIN32
        // This should work under WinXP, Vista, and Win7. No guarantees about
        // future compatibility, but I doubt that Microsoft will get rid of it
        // (it's too useful), and I doubt that they'll change it now that it's
        // well-known (it would break too many programs). We could also use the
        // rand_s function in more recent versions of Visual C++, but that
        // causes compatibility problems with older versions of Windows.
        typedef BOOLEAN (WINAPI *RtlGenRandomFn)(PVOID, ULONG);
        HMODULE dll=LoadLibrary(_T("Advapi32.dll"));
        if (dll != 0) {
            RtlGenRandomFn RtlGenRandom=RtlGenRandomFn(GetProcAddress(dll,
                "SystemFunction036"));
            if (RtlGenRandom != 0) {
                std::vector<char> buffer(cBytesRequested, '\0');
                if (RtlGenRandom(&buffer[0], cBytesRequested)) {
                    char *c=&buffer[0];
                    seed_manual(std::string(c, c+cBytesRequested));
                    success=true;
                }
            }
            FreeLibrary(dll);
        }
    #else
        // This should be supported under most non-Windows systems. Note that
        // we're using /dev/urandom, not /dev/random -- /dev/random is more
        // secure, but it can be VERY slow.
        std::ifstream rng("/dev/urandom");
        if (rng) {
            std::string rstr;
            for (int i=0; i < cBytesRequested; ++i)
                rstr.push_back(rng.get());
            seed_manual(rstr);
            success=true;
        }
    #endif

    return success;
}

void generator_t::seed_fallback() {
    // No cryptographically-secure device available. Fall back onto the
    // system clock. It's not much, but it's portable, fast, and will
    // provide at least a *little* entropy.
    std::ostringstream out;
    out << time(0) << clock();
    seed_manual(out.str());
}

generator_t::result_type generator_t::operator()() {
    if (!mSeeded)
        if (!seed_secure())
            seed_fallback();
    return (*mGen)();
}

void generator_t::init() {
    if (!mGen) mGen=new internal_generator_t();
}

} // namespace

bool seed_secure() {
    generator_t gen;
    return gen.seed_secure();
}

void seed_fallback() {
    generator_t gen;
    gen.seed_fallback();
}

void seed_manual(const std::string& value) {
    generator_t gen;
    gen.seed_manual(value);
}

// Returns a positive (unless told otherwise) integer between zero and
// (1<<bits)-1, inclusive
integer random_by_size(size_t bits, bool highBitOn, bool lowBitOn, bool canBeNegative) {
    if (bits<=0) return integer::zero();

    generator_t randomGenerator;

    const size_t cBitsPerIteration=std::numeric_limits<generator_t::result_type>::digits;

    // Grab a set of random bits, of at least the specified size
    int iterations = (bits+cBitsPerIteration-1) / cBitsPerIteration;
    std::vector<generator_t::result_type> v;
    for (int i=0; i<iterations; ++i) v.push_back(randomGenerator());

    const char *vptr=(const char *)&v[0], *vptr_end=vptr+(v.size() * sizeof(generator_t::result_type));
    integer p=from_binary(std::string(vptr, vptr_end));

    // Trim it to the proper length
    int index=(bits/bits_per_digit);
    digit_t mask=(digit_t(1) << (bits % bits_per_digit))-1;
    if (mask==0) { mask=digit_mask; --index; }
    p._get_data()->digits[index] &= mask;
    for (digit_t *i=p._get_data()->digits+index+1,
        *ie=p._get_data()->digits+p._get_data()->mLength; i<ie; ++i) *i=0;
    p._get_data()->skipLeadingZeros();

    if (highBitOn) setbit(p, bits-1);
    if (lowBitOn) setbit(p, 0);
    if (canBeNegative) p._set_negative(randomGenerator() & 0x01);

    return p;
}

template <>
integer random<integer>(const integer& lowest, const integer& highest) {
    integer range(abs(highest-lowest+1));
    return lowest+(random_by_size(log2(range)) % range);
}

} // namespace xint
