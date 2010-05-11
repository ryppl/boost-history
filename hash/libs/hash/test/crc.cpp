
#include <boost/hash/crc.hpp>
#include <boost/hash/compute_digest.hpp>
#include <boost/hash/digest_io.hpp>

#include <iostream>
#include <string>

#include <cassert>

void test32() {
    typedef boost::hash::crc32_png HASH;
    using boost::hash::compute_digest;

    // Messages from MD4/MD5 test vectors
    
    {
    HASH::digest_type d = compute_digest<HASH>("");
    std::cout << d << "\n";
    assert(d == "00000000");
    }

    // All with 1 MiB or less fo input checked against
    // http://hash.online-convert.com/crc32b-generator
    // All checked against cksfv utility
    // http://www.iki.fi/shd/foss/cksfv/

    {
    HASH::digest_type d = compute_digest<HASH>("a");
    std::cout << d << "\n";
    assert(d == "e8b7be43");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("abc");
    std::cout << d << "\n";
    assert(d == "352441c2");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("message digest");
    std::cout << d << "\n";
    assert(d == "20159d7f");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("abcdefghijklmnopqrstuvwxyz");
    std::cout << d << "\n";
    assert(d == "4c2750bd");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    std::cout << d << "\n";
    assert(d == "1fc2e6d2");
    }

    {
    HASH::digest_type d = compute_digest<HASH>("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    std::cout << d << "\n";
    assert(d == "7ca94a72");
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    HASH::stream_hash<8>::type sh;
    for (unsigned i = 0; i < 1000000; ++i) sh.update_one('a');
    HASH::digest_type d = sh.end_message();
    std::cout << d << "\n";
    assert(d == "dc25bfbc");
    }

    {
    HASH::stream_hash<8>::type sh;
    std::string s(1000, 'a');
    for (unsigned i = 0; i < 1000000; ++i) sh.update_n(s.data(), s.size());
    HASH::digest_type d = sh.end_message();
    std::cout << d << "\n";
    assert(d == "a7943e77");
    }

#ifndef QUICK
    {
    HASH::stream_hash<8>::type sh;
    for (unsigned i = 0; i < 1000000000; ++i) sh.update_one('a');
    HASH::digest_type d = sh.end_message();
    std::cout << d << "\n";
    assert(d == "a7943e77");
    }
#endif // QUICK
#endif

}

int main() {
    using namespace boost::hash;

    test32();
    
}

