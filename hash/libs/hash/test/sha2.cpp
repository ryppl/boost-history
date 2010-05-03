
//#define BOOST_HASH_SHOW_PROGRESS
#include <boost/hash/sha2.hpp>
#include <boost/hash/compute_digest.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

using namespace boost::hash;

//
// Appendix references are from
// http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
//

//
// Additional test vectors from
// http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA_All.pdf
//

void test_subbyte_sha2() {
    // From http://csrc.nist.gov/groups/STM/cavp/documents/shs/SHAVS.pdf

    {
    // B.1/1
    unsigned n = 5;
    bool a[] = {0, 1, 1, 0, 1};
    sha2<224>::digest_type d = compute_digest_n<sha2<224> >(a, n);
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("e3b048552c3c387bcab37f6eb06bb79b"
                   "96a4aee5ff27f51531a9551c",
                   d.cstring().data()));
    }

    {
    // C.1/1
    unsigned n = 5;
    bool a[] = {0, 1, 1, 0, 1};
    sha2<256>::digest_type d = compute_digest_n<sha2<256> >(a, n);
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("d6d3e02a31a84a8caa9718ed6c2057be"
                   "09db45e7823eb5079ce7a573a3760f95",
                   d.cstring().data()));
    }

    {
    // D.1/1
    unsigned n = 5;
    bool a[] = {0, 0, 0, 1, 0};
    sha2<384>::digest_type d = compute_digest_n<sha2<384> >(a, n);
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("8d17be79e32b6718e07d8a603eb84ba0478f7fcfd1bb9399"
                   "5f7d1149e09143ac1ffcfc56820e469f3878d957a15a3fe4",
                   d.cstring().data()));
    }

    {
    // E.1/1
    unsigned n = 5;
    bool a[] = {1, 0, 1, 1, 0};
    sha2<512>::digest_type d = compute_digest_n<sha2<512> >(a, n);
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("d4ee29a9e90985446b913cf1d1376c836f4be2c1cf3cada0"
                   "720a6bf4857d886a7ecb3c4e4c0fa8c7f95214e41dc1b0d2"
                   "1b22a84cc03bf8ce4845f34dd5bdbad4",
                   d.cstring().data()));
    }
}

void test_accumulator_sha256() {
    typedef sha2<256> HASH;
    HASH::block_hash_type a;

    {
    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    }

    a.reset();
    {
    // 0-length input: echo -n | sha256sum

    // A single 1 bit after the (empty) message,
    // then pad with 0s,
    // then add the length, which is also 0
    HASH::block_hash_type::block_type m = {{0x80000000u}};
    a.update(m);

    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" == s);
    }

    a.reset();
    {
    // Example from appendix B.1: echo -n "abc" | sha256sum
    HASH::block_hash_type::block_type m = {{}};
    m[ 0] = 0x61626380;
    m[15] = 0x00000018;
    a.update(m);

    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" == s);
    }

    a.reset();
    {
    // Example from appendix B.2:
    // echo -n "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" | sha256sum
    HASH::block_hash_type::block_type m1 = {{
        0x61626364,
        0x62636465,
        0x63646566,
        0x64656667,
        0x65666768,
        0x66676869,
        0x6768696a,
        0x68696a6b,
        0x696a6b6c,
        0x6a6b6c6d,
        0x6b6c6d6e,
        0x6c6d6e6f,
        0x6d6e6f70,
        0x6e6f7071,
        0x80000000,
        0x00000000,
    }};
    a.update(m1);
    assert("85e655d6417a17953363376a624cde5c76e09589cac5f811cc4b32c1f20e533a" == a.digest());
    HASH::block_hash_type::block_type m2 = {{}};
    m2[15] = 0x000001c0;
    a.update(m2);
    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1" == s);
    }
}

void test_accumulator_sha384() {
    typedef sha2<384> HASH;
    HASH::block_hash_type a;

    {
    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    }

    a.reset();
    {
    // 0-length input: echo -n | sha256sum

    // A single 1 bit after the (empty) message,
    // then pad with 0s,
    // then add the length, which is also 0
    HASH::block_hash_type::block_type m = {{0x8000000000000000ul}};
    a.update(m);

    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("38b060a751ac96384cd9327eb1b1e36a21fdb71114be0743"
           "4c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" == s);
    }

    a.reset();
    {
    // Example from appendix D.1: echo -n "abc" | sha384sum
    HASH::block_hash_type::block_type m = {{}};
    m[ 0] = 0x6162638000000000L;
    m[15] = 0x0000000000000018L;
    a.update(m);

    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("cb00753f45a35e8bb5a03d699ac65007272c32ab0eded163"
           "1a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" == s);
    }

    a.reset();
    {
    // Example from appendix D.2:
    // echo -n "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn (continues)
    //          hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" | sha384sum
    HASH::block_hash_type::block_type m1 = {{
        0x6162636465666768L,
        0x6263646566676869L,
        0x636465666768696aL,
        0x6465666768696a6bL,
        0x65666768696a6b6cL,
        0x666768696a6b6c6dL,
        0x6768696a6b6c6d6eL,
        0x68696a6b6c6d6e6fL,
        0x696a6b6c6d6e6f70L,
        0x6a6b6c6d6e6f7071L,
        0x6b6c6d6e6f707172L,
        0x6c6d6e6f70717273L,
        0x6d6e6f7071727374L,
        0x6e6f707172737475L,
        0x8000000000000000L,
        0x0000000000000000L,
    }};
    a.update(m1);
    assert("2a7f1d895fd58e0beaae96d1a673c741015a2173796c1a88"
           "f6352ca156acaff7c662113e9ebb4d6417b61a85e2ccf0a9" == a.digest());
    HASH::block_hash_type::block_type m2 = {{}};
    m2[15] = 0x0000000000000380L;
    a.update(m2);
    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("09330c33f71147e83d192fc782cd1b4753111b173b3b05d2"
           "2fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039" == s);
    }
}

void test_accumulator_sha512() {
    typedef sha2<512> HASH;
    HASH::block_hash_type a;

    {
    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    }

    a.reset();
    {
    // 0-length input: echo -n | sha512sum

    // A single 1 bit after the (empty) message,
    // then pad with 0s,
    // then add the length, which is also 0
    HASH::block_hash_type::block_type m = {{0x8000000000000000ul}};
    a.update(m);

    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
           "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" == s);
    }

    a.reset();
    {
    // Example from appendix C.1: echo -n "abc" | sha512sum
    HASH::block_hash_type::block_type m = {{}};
    m[ 0] = 0x6162638000000000L;
    m[15] = 0x0000000000000018L;
    a.update(m);

    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
           "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" == s);
    }

    a.reset();
    {
    // Example from appendix C.2:
    // echo -n "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn (continues)
    //          hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" | sha512sum
    HASH::block_hash_type::block_type m1 = {{
        0x6162636465666768L,
        0x6263646566676869L,
        0x636465666768696aL,
        0x6465666768696a6bL,
        0x65666768696a6b6cL,
        0x666768696a6b6c6dL,
        0x6768696a6b6c6d6eL,
        0x68696a6b6c6d6e6fL,
        0x696a6b6c6d6e6f70L,
        0x6a6b6c6d6e6f7071L,
        0x6b6c6d6e6f707172L,
        0x6c6d6e6f70717273L,
        0x6d6e6f7071727374L,
        0x6e6f707172737475L,
        0x8000000000000000L,
        0x0000000000000000L,
    }};
    a.update(m1);
    assert("4319017a2b706e69cd4b05938bae5e890186bf199f30aa956ef8b71d2f810585"
           "d787d6764b20bda2a26014470973692000ec057f37d14b8e06add5b50e671c72" == a.digest());
    HASH::block_hash_type::block_type m2 = {{}};
    m2[15] = 0x0000000000000380L;
    a.update(m2);
    HASH::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018"
           "501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909" == s);
    }

}

void test_preprocessor_sha256() {
    typedef sha2<256> HASH;

    {
    HASH::stream_hash<8>::type h;
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" == s);
    }

    {
    // Example from Appendix B.1
    HASH::stream_hash<8>::type h;
    h.update_one('a').update_one('b').update_one('c');
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" == s);
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    // Example from Appendix B.3
    HASH::stream_hash<8>::type h;
    for (unsigned i = 0; i < 1000000; ++i) {
        h.update_one('a');
    }
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0" == s);
    }
#endif
}

void test_preprocessor_sha384() {
    typedef sha2<384> HASH;

    {
    HASH::stream_hash<8>::type h;
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("38b060a751ac96384cd9327eb1b1e36a21fdb71114be0743"
           "4c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" == s);
    }

    {
    // Example from Appendix D.1
    HASH::stream_hash<8>::type h;
    h.update_one('a').update_one('b').update_one('c');
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("cb00753f45a35e8bb5a03d699ac65007272c32ab0eded163"
           "1a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" == s);
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    // Example from Appendix D.3
    HASH::stream_hash<8>::type h;
    for (unsigned i = 0; i < 1000000; ++i) {
        h.update_one('a');
    }
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("9d0e1809716474cb086e834e310a4a1ced149e9c00f24852"
           "7972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985" == s);
    }
#endif
}

void test_preprocessor_sha512() {
    typedef sha2<512> HASH;

    {
    HASH::stream_hash<8>::type h;
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
           "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" == s);
    }

    {
    // Example from Appendix C.1
    HASH::stream_hash<8>::type h;
    h.update_one('a').update_one('b').update_one('c');
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
           "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" == s);
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    // Example from Appendix C.3
    HASH::stream_hash<8>::type h;
    for (unsigned i = 0; i < 1000000; ++i) {
        h.update_one('a');
    }
    HASH::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
           "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b" == s);
    }
#endif
}

int main() {
    test_subbyte_sha2();

    test_accumulator_sha256();
    test_accumulator_sha384();
    test_accumulator_sha512();

    test_preprocessor_sha256();
    test_preprocessor_sha384();
    test_preprocessor_sha512();

    {
    sha2<224>::digest_type h = compute_digest_n<sha2<224> >("abc", 3);
    std::printf("%s\n", h.cstring().data());
    assert(!strcmp("23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7",
                   h.cstring().data()));
    }

    {
    sha2<512>::stream_hash<16>::type pp;
    for (unsigned i = 0; i < 1000000/2; ++i) {
        pp.update_one(('a'<<8) | 'a');
    }
    sha2<512>::digest_type h = pp.end_message();
    std::printf("%s\n", h.cstring().data());
    assert(!strcmp("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
                   "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b",
                   h.cstring().data()));
    }

    {
    sha2<512>::stream_hash<32>::type pp;
    for (unsigned i = 0; i < 1000000/4; ++i) {
        pp.update_one(('a'<<24) | ('a'<<16) | ('a'<<8) | 'a');
    }
    sha2<512>::digest_type h = pp.end_message();
    std::printf("%s\n", h.cstring().data());
    assert(!strcmp("e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
                   "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b",
                   h.cstring().data()));
    }
}

