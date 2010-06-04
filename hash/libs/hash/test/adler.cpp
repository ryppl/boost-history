
#include <boost/hash/adler.hpp>
#include <boost/hash/compute_digest.hpp>
#include <boost/hash/detail/primes.hpp>
#include <boost/hash/digest_io.hpp>
#include <boost/static_assert.hpp>

#include <iostream>
#include <string>

#include <cassert>

namespace primes_check {
    using boost::hashes::detail::largest_prime;

    BOOST_STATIC_ASSERT(largest_prime< 2>::value == 3);
    BOOST_STATIC_ASSERT(largest_prime< 3>::value == 7);
    BOOST_STATIC_ASSERT(largest_prime< 4>::value == 13);
    BOOST_STATIC_ASSERT(largest_prime< 5>::value == 31);
    BOOST_STATIC_ASSERT(largest_prime< 6>::value == 61);
    BOOST_STATIC_ASSERT(largest_prime< 7>::value == 127);
    BOOST_STATIC_ASSERT(largest_prime< 8>::value == 251);
    BOOST_STATIC_ASSERT(largest_prime< 9>::value == 509);
    BOOST_STATIC_ASSERT(largest_prime<10>::value == 1021);
    BOOST_STATIC_ASSERT(largest_prime<11>::value == 2039);
    BOOST_STATIC_ASSERT(largest_prime<12>::value == 4093);
    BOOST_STATIC_ASSERT(largest_prime<13>::value == 8191);
    BOOST_STATIC_ASSERT(largest_prime<14>::value == 16381);
    BOOST_STATIC_ASSERT(largest_prime<15>::value == 32749);
    BOOST_STATIC_ASSERT(largest_prime<16>::value == 65521);

    BOOST_STATIC_ASSERT(largest_prime<17>::value == 131071);
    BOOST_STATIC_ASSERT(largest_prime<18>::value == 262139);
    BOOST_STATIC_ASSERT(largest_prime<19>::value == 524287);
    BOOST_STATIC_ASSERT(largest_prime<20>::value == 1048573);
    BOOST_STATIC_ASSERT(largest_prime<21>::value == 2097143);
    BOOST_STATIC_ASSERT(largest_prime<22>::value == 4194301);
    BOOST_STATIC_ASSERT(largest_prime<23>::value == 8388593);
    BOOST_STATIC_ASSERT(largest_prime<24>::value == 16777213);
    BOOST_STATIC_ASSERT(largest_prime<25>::value == 33554393);
    BOOST_STATIC_ASSERT(largest_prime<26>::value == 67108859);
    BOOST_STATIC_ASSERT(largest_prime<27>::value == 134217689);
    BOOST_STATIC_ASSERT(largest_prime<28>::value == 268435399);
    BOOST_STATIC_ASSERT(largest_prime<29>::value == 536870909);
    BOOST_STATIC_ASSERT(largest_prime<30>::value == 1073741789);
    BOOST_STATIC_ASSERT(largest_prime<31>::value == 2147483647);
    BOOST_STATIC_ASSERT(largest_prime<32>::value == 4294967291U);

}

void test32() {
    typedef boost::hashes::adler<32> HASH;
    using boost::hashes::compute_digest;

    {
    HASH::digest_type d = compute_digest<HASH>("\x2");
    std::cout << d << "\n";
    assert(d == "00030003");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("\x2\x4");
    std::cout << d << "\n";
    assert(d == "000a0007");
    }
    
    // Messages from MD4/MD5 test vectors
    
    {
    HASH::digest_type d = compute_digest<HASH>("");
    std::cout << d << "\n";
    assert(d == "00000001");
    }

    // All with 1 MiB or less fo input checked against
    // http://hash.online-convert.com/adler32-generator

    {
    HASH::digest_type d = compute_digest<HASH>("a");
    std::cout << d << "\n";
    assert(d == "00620062");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("abc");
    std::cout << d << "\n";
    assert(d == "024d0127");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("message digest");
    std::cout << d << "\n";
    assert(d == "29750586");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("abcdefghijklmnopqrstuvwxyz");
    std::cout << d << "\n";
    assert(d == "90860b20");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    std::cout << d << "\n";
    assert(d == "8adb150c");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    std::cout << d << "\n";
    assert(d == "97b61069");
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    HASH::stream_hash<8>::type sh;
    for (unsigned i = 0; i < 1000000; ++i) sh.update_one('a');
    HASH::digest_type d = sh.end_message();
    std::cout << d << "\n";
    assert(d == "15d870f9");
    }

    {
    HASH::stream_hash<8>::type sh;
    std::string s(1000, 'a');
    for (unsigned i = 0; i < 1000000; ++i) sh.update_n(s.data(), s.size());
    HASH::digest_type d = sh.end_message();
    std::cout << d << "\n";
    assert(d == "bbc26298");
    }

#ifndef QUICK
    {
    HASH::stream_hash<8>::type sh;
    for (unsigned i = 0; i < 1000000000; ++i) sh.update_one('a');
    HASH::digest_type d = sh.end_message();
    std::cout << d << "\n";
    assert(d == "bbc26298");
    }
#endif // QUICK
#endif

}

int main() {
    using namespace boost::hashes;

    test32();
    
}

