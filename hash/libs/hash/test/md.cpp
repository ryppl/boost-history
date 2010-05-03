
//#define BOOST_HASH_SHOW_PROGRESS
#include <boost/hash/compute_digest.hpp>
#include <boost/hash/md4.hpp>
#include <boost/hash/md5.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace boost::hash;

void test_accumulator_md4() {
    md4::block_hash_type a;

    {
    md4::block_hash_type::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("0123456789abcdeffedcba9876543210" == s);
    }

    a.reset();
    {
    // 0-length input: echo -n | md4sum

    // A single 1 bit after the (empty) message,
    // then pad with 0s,
    // then add the length, which is also 0.
    // Remember that MD5 is little-octet, big-bit endian
    md4::block_hash_type::block_type m = {{0x00000080u}};
    a.update(m);
    md4::block_hash_type::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("31d6cfe0d16ae931b73c59d7e0c089c0" == s);
    }

    a.reset();
    {
    // echo -n "abc" | md4sum
    md4::block_hash_type::block_type m = {{}};
    m[ 0] = 0x80636261;
    // little-octet, big-bit endian also means the size isn't in the last word
    m[14] = 0x00000018;
    a.update(m);
    md4::block_hash_type::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("a448017aaf21d8525fc10ae87aa6729d" == s);
    }
}

void test_accumulator_md5() {
    md5::block_hash_type a;

    {
    md5::block_hash_type::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("0123456789abcdeffedcba9876543210" == s);
    }

    a.reset();
    {
    // 0-length input: echo -n | md4sum

    // A single 1 bit after the (empty) message,
    // then pad with 0s,
    // then add the length, which is also 0.
    // Remember that MD5 is little-octet, big-bit endian
    md5::block_hash_type::block_type m = {{0x00000080u}};
    a.update(m);
    md5::block_hash_type::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("d41d8cd98f00b204e9800998ecf8427e" == s);
    }

    a.reset();
    {
    // echo -n "abc" | md4sum
    md5::block_hash_type::block_type m = {{}};
    m[ 0] = 0x80636261;
    // little-octet, big-bit endian also means the size isn't in the last word
    m[14] = 0x00000018;
    a.update(m);
    md5::block_hash_type::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("900150983cd24fb0d6963f7d28e17f72" == s);
    }
}

void test_preprocessor_md4() {

    {
    md4::stream_hash<8>::type h;
    md4::block_hash_type::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("31d6cfe0d16ae931b73c59d7e0c089c0" == s);
    }

    {
    md4::stream_hash<8>::type h;
    h.update_one('a').update_one('b').update_one('c');
    assert(h.digest() == h.digest());
    md4::block_hash_type::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("a448017aaf21d8525fc10ae87aa6729d" == s);
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    md4::stream_hash<8>::type h;
    for (unsigned i = 0; i < 1000000; ++i) {
        h.update_one('a');
    }
    md4::block_hash_type::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("bbce80cc6bb65e5c6745e30d4eeca9a4" == s);
    }
#endif
}

void test_preprocessor_md5() {

    {
    md5::stream_hash<8>::type h;
    md5::block_hash_type::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("d41d8cd98f00b204e9800998ecf8427e" == s);
    }

    {
    md5::stream_hash<8>::type h;
    h.update_one('a').update_one('b').update_one('c');
    assert(!(h.digest() != h.digest()));
    md5::block_hash_type::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("900150983cd24fb0d6963f7d28e17f72" == s);
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    // perl -e 'for (1..1000000) { print "a"; }' | md5sum
    md5::stream_hash<8>::type h;
    for (unsigned i = 0; i < 1000000; ++i) {
        h.update_one('a');
    }
    md5::block_hash_type::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("7707d6ae4e027c70eea2a935c2296f21" == s);
    }
#endif
}

void test_md4() {
    // Test suite from A.5 http://www.faqs.org/rfcs/rfc1320.html
    assert("31d6cfe0d16ae931b73c59d7e0c089c0" == compute_digest<md4>(""));
    assert("bde52cb31de33e46245e05fbdbd6fb24" == compute_digest<md4>("a"));
    assert("a448017aaf21d8525fc10ae87aa6729d" == compute_digest<md4>("abc"));
    assert("d9130a8164549fe818874806e1c7014b" == compute_digest<md4>("message digest"));
    assert("d79e1c308aa5bbcdeea8ed63df412da9" == compute_digest<md4>("abcdefghijklmnopqrstuvwxyz"));
    assert("043f8582f241db351ce627e153e7f0e4" == compute_digest<md4>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
    assert("e33b4ddc9c38f2199c3e7b164fcc0536" == compute_digest<md4>("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
}

void test_md5() {
    // Test suite from A.5 http://www.faqs.org/rfcs/rfc1321.html
    assert("d41d8cd98f00b204e9800998ecf8427e" == compute_digest<md5>(""));
    assert("0cc175b9c0f1b6a831c399e269772661" == compute_digest<md5>("a"));
    assert("900150983cd24fb0d6963f7d28e17f72" == compute_digest<md5>("abc"));
    assert("f96b697d7cb7938d525a2f31aaf161d0" == compute_digest<md5>("message digest"));
    assert("c3fcd3d76192e4007dfb496cca67e13b" == compute_digest<md5>("abcdefghijklmnopqrstuvwxyz"));
    assert("d174ab98d277d9f5a5611c2c9f419d9f" == compute_digest<md5>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
    assert("57edf4a22be3c955ac49da2e2107b67a" == compute_digest<md5>("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
}

int main() {
    test_accumulator_md4();
    test_accumulator_md5();

    test_preprocessor_md4();
    test_preprocessor_md5();

    test_md4();
    test_md5();
}
