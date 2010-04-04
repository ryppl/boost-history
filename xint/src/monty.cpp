
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for functions based on the Montgomery
    reduction. Used for an extra-fast powerMod.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"

#include <vector>

namespace xint {

using namespace detail;

digit_t inverse0(const integer& n) {
    // Using the Dussé and Kalisk simplification
    doubledigit_t x = 2, y = 1;
    digit_t n0 = n._get_digit(0);
    for (size_t i = 2; i <= bits_per_digit; ++i, x <<= 1)
        if (x < ((n0 * y) & ((x << 1) - 1)))
            y += x;
    return digit_t(x - y);
}

integer montgomeryR(const integer& n) {
    return integer::one() << (bits_per_digit * n._get_data()->mLength);
}

integer toMontgomeryForm(const integer& n, const integer& m) {
    return (n * montgomeryR(m) % m);
}

integer fromMontgomeryForm(const integer& n, const integer& m) {
    integer inv=invmod(montgomeryR(m), m);
    if (inv.nan()) {
        if (exceptions_allowed()) throw invalid_modulus("modulus has no inverse");
        else return integer(not_a_number());
    }
    return (n * inv % m);
}

//integer montgomeryReduction(const integer& m, const integer& mPrime, const
//    integer& T)
//{
//    // Unstated parameter b is digit_overflowbit, a power of 2
//    // Unstated parameter n is m.data->mLength
//    // Unstated parameter R is b^n
//    // gcd(m, b)==1, or in other words, m must be an odd number
//    // m'=-m^(-1) mod b (precalculated)
//    // T is any arbitrary number greater than zero and <= m*R
//
//    int n=m._get_data()->mLength;
//    doubledigit_t mprime = mPrime._get_data()->digits[0];
//
//    integer A(T);
//    for (int i=0; i < n; ++i) {
//        integer ui((A._get_data()->digits[i] * mprime) & digit_mask);
//        ui <<= (bits_per_digit * i); // Fast-multiply by b^i
//        A+=(ui*m);
//    }
//    A >>= (bits_per_digit * n); // Fast-divide by b^n
//    if (A >= m) A -= m;
//    return A;
//}

integer montgomeryMultiplyMod(const integer& a, const integer& b, const integer& n,
    digit_t nPrime0)
{
    // Using the Dussé and Kalisk simplification
    // Unstated parameter B is a power of two representing the number of values
    //   that a single digit can hold, i.e. digit_overflowbit
    // Unstated parameter L is the number of digits in the modulus, i.e.
    //   n._get_length()
    // Unstated parameter r is B^L
    // nPrime0 is nPrime mod B, or digit zero of nPrime

    const integer B(digit_overflowbit);
    const size_t L(n._get_length()), L1(L-1);

    integer t=a*b;
    size_t i=0;

    do {
        digit_t mi=digit_t(doubledigit_t(t._get_digit(i))*nPrime0);
        t += (n * mi) << (bits_per_digit * i);
    } while (++i <= L1);

    t >>= (bits_per_digit * L); // Fast divide by r
    return (t >= n ? t - n : t);
}

namespace {

// cMaxK sets the balance between memory/precalculations required and the number
// of calculations required for an exponentiation. Increasing it can only reduce
// the calculations by a small amount, whereas it increases the memory
// requirements and number of precalculations by an exponential amount. 8
// provides a good balance.
const size_t cMaxK=8;
typedef boost::uint_t<cMaxK>::fast kbitdigit_t; // k bits have to fit into it
typedef std::vector<kbitdigit_t> vkbitdigit_t;
typedef std::vector<integer> vxint_t;
#define ddPowerOfTwo(p) (doubledigit_t(1) << p)

// The splitIntoKBitDigits function assumes that cMaxK <= bits_per_digit+1,
// it won't work properly if it isn't.
BOOST_STATIC_ASSERT(cMaxK <= bits_per_digit+1);

class TUTable {
    public:
    typedef std::pair<int, int> value_t;

    const value_t& operator[](size_t x) const { return mTable[x]; }

    static const TUTable& get() {
        // Construct a singleton instance on demand
        if (mPtr==0) mPtr=new TUTable;
        return *mPtr;
    }

    private:
    TUTable(): mTable(new value_t[ddPowerOfTwo(cMaxK)]) {
        value_t *p=&mTable[0], *pe=p+ddPowerOfTwo(cMaxK);
        *p++=std::make_pair(0, 0);
        int i=1;
        while (p!=pe) *p++=calculateValues(i++);
    }
    ~TUTable() { delete[] mTable; }

    std::pair<int, int> calculateValues(int x) {
        int r=0;
        while (1) {
            if (x & 0x01) return std::make_pair(r, x);
            ++r;
            x >>= 1;
        }
    }

    static TUTable *mPtr;

    value_t *mTable;
};

TUTable *TUTable::mPtr=0;

int mostEfficientK(const integer& e) {
    doubledigit_t k=cMaxK, kTarget=log2(e)-1;
    while (k > 1 && ((k - 1) * (k << ((k - 1) << 1))
        / (ddPowerOfTwo(k) - k - 1)) >= kTarget)
            --k;
    return int(k);
}

vxint_t precalculateOddPowersOfAa(const integer& a, const integer&
    r, const integer& n, size_t k)
{
    integer aa=a*r%n, aSquared=a*a%n;

    vxint_t rval;
    rval.reserve(size_t(ddPowerOfTwo(k)));
    rval.push_back(integer::one());     // Anything to the zeroth power is one
    rval.push_back(aa);                 // Anything to the first power is itself

    for (doubledigit_t i=3, ie=(ddPowerOfTwo(k)); i<ie; i+=2) {
        aa=aa*aSquared%n;
        rval.push_back(integer::zero()); // Even powers not needed or calculated
        rval.push_back(aa);              // Odd power
    }

    return rval;
}

vkbitdigit_t splitIntoKBitDigits(const integer& e, size_t k) {
    size_t eBits=log2(e), eDigits=(eBits+k-1)/k, i=0, bitsInHopper=0;

    vkbitdigit_t rval;
    rval.reserve(eDigits);

    doubledigit_t hopper=0, mask=(doubledigit_t(1)<<k)-1;
    while (eDigits-- > 0) {
        if (bitsInHopper < k && i < e._get_length()) {
            // Add more bits to the hopper
            hopper = hopper | (doubledigit_t(e._get_digit(i++)) << bitsInHopper);
            bitsInHopper += bits_per_digit;
        }

        // Grab k bits off the bottom
        if (bitsInHopper > 0) {
            rval.push_back(kbitdigit_t(hopper & mask));
            hopper >>= k;
            bitsInHopper-=k;
        } else {
            rval.push_back(0);
        }
    }

    return rval;
}

} // namespace

integer montgomeryPowerMod(const integer& a, const integer& e, const integer& n)
{
    // 0 <= a < n, n is odd
    // Returns a^e mod n

    const TUTable &tuTable(TUTable::get());

    if (e.sign()==0) return integer::one();
    if (n.even()) {
        if (exceptions_allowed()) throw invalid_modulus("montgomeryPowerMod "
            "requires an odd modulus");
        else return integer(not_a_number());
    }

    // Precalculate some values
    const size_t k(mostEfficientK(e));
    const integer r(montgomeryR(n));
    const digit_t nPrime0(inverse0(n));
    const vxint_t oddPowersOfAa(precalculateOddPowersOfAa(a, r, n, k));

    // Slice the exponent (e) up into k-bit digits
    vkbitdigit_t eDigits(splitIntoKBitDigits(e, k));

    integer pp;

    kbitdigit_t i=eDigits.back();
    eDigits.pop_back();
    if (i == 0) {
        pp=r%n;
    } else {
        std::pair<int, int> tu=tuTable[i];
        pp=oddPowersOfAa[tu.second];
        while (tu.first-- > 0) pp=montgomeryMultiplyMod(pp, pp, n, nPrime0);
    }

    while (!eDigits.empty()) {
        i=eDigits.back();
        eDigits.pop_back();

        if (i == 0) {
            int t=int(k);
            while (t-- > 0) pp=montgomeryMultiplyMod(pp, pp, n, nPrime0);
        } else {
            std::pair<int, int> tu=tuTable[i];

            size_t s=k-tu.first;
            while (s-- > 0) pp=montgomeryMultiplyMod(pp, pp, n, nPrime0);

            pp=montgomeryMultiplyMod(pp, oddPowersOfAa[tu.second], n, nPrime0);

            while (tu.first-- > 0) pp=montgomeryMultiplyMod(pp, pp, n, nPrime0);
        }
    }

    return montgomeryMultiplyMod(pp, integer::one(), n, nPrime0);
}

} // namespace xint
