
//#define BOOST_HASH_SHOW_PROGRESS
#include <boost/hash/sha.hpp>
#include <boost/hash/sha1.hpp>
#include <boost/hash/compute_digest.hpp>


#include <cassert>
#include <cstdio>
#include <cstring>
#include <boost/cstdint.hpp>

using namespace boost::hash;

//
// Appendix references are from
// http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
//

//
// Additional test vectors from
// http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA_All.pdf
//

void test_collision_sha0() {
    // Reported 2004-08-12 by Joux, Carribault, Lemuet, and Jalby
    typedef sha::block_hash_type::word_type word_type;
    unsigned bitlength = 2048;
    word_type fic1[] = {
        0xa766a602, 0xb65cffe7, 0x73bcf258, 0x26b322b3,
        0xd01b1a97, 0x2684ef53, 0x3e3b4b7f, 0x53fe3762,
        0x24c08e47, 0xe959b2bc, 0x3b519880, 0xb9286568,
        0x247d110f, 0x70f5c5e2, 0xb4590ca3, 0xf55f52fe,
        0xeffd4c8f, 0xe68de835, 0x329e603c, 0xc51e7f02,
        0x545410d1, 0x671d108d, 0xf5a4000d, 0xcf20a439,
        0x4949d72c, 0xd14fbb03, 0x45cf3a29, 0x5dcda89f,
        0x998f8755, 0x2c9a58b1, 0xbdc38483, 0x5e477185,
        0xf96e68be, 0xbb0025d2, 0xd2b69edf, 0x21724198,
        0xf688b41d, 0xeb9b4913, 0xfbe696b5, 0x457ab399,
        0x21e1d759, 0x1f89de84, 0x57e8613c, 0x6c9e3b24,
        0x2879d4d8, 0x783b2d9c, 0xa9935ea5, 0x26a729c0,
        0x6edfc501, 0x37e69330, 0xbe976012, 0xcc5dfe1c,
        0x14c4c68b, 0xd1db3ecb, 0x24438a59, 0xa09b5db4,
        0x35563e0d, 0x8bdf572f, 0x77b53065, 0xcef31f32,
        0xdc9dbaa0, 0x4146261e, 0x9994bd5c, 0xd0758e3d,
    };
    assert(sizeof(fic1)*8 == bitlength);
    word_type fic2[] = {
        0xa766a602, 0xb65cffe7, 0x73bcf258, 0x26b322b1,
        0xd01b1ad7, 0x2684ef51, 0xbe3b4b7f, 0xd3fe3762,
        0xa4c08e45, 0xe959b2fc, 0x3b519880, 0x39286528,
        0xa47d110d, 0x70f5c5e0, 0x34590ce3, 0x755f52fc,
        0x6ffd4c8d, 0x668de875, 0x329e603e, 0x451e7f02,
        0xd45410d1, 0xe71d108d, 0xf5a4000d, 0xcf20a439,
        0x4949d72c, 0xd14fbb01, 0x45cf3a69, 0x5dcda89d,
        0x198f8755, 0xac9a58b1, 0x3dc38481, 0x5e4771c5,
        0x796e68fe, 0xbb0025d0, 0x52b69edd, 0xa17241d8,
        0x7688b41f, 0x6b9b4911, 0x7be696f5, 0xc57ab399,
        0xa1e1d719, 0x9f89de86, 0x57e8613c, 0xec9e3b26,
        0xa879d498, 0x783b2d9e, 0x29935ea7, 0xa6a72980,
        0x6edfc503, 0x37e69330, 0x3e976010, 0x4c5dfe5c,
        0x14c4c689, 0x51db3ecb, 0xa4438a59, 0x209b5db4,
        0x35563e0d, 0x8bdf572f, 0x77b53065, 0xcef31f30,
        0xdc9dbae0, 0x4146261c, 0x1994bd5c, 0x50758e3d,
    };
    assert(sizeof(fic2)*8 == bitlength);

    assert(sizeof(fic1) == sizeof(fic2));

    sha::digest_type h1 = compute_digest_n<sha>(fic1, bitlength/32);
    printf("%s\n", h1.cstring().data());
    sha::digest_type h2 = compute_digest_n<sha>(fic2, bitlength/32);
    printf("%s\n", h2.cstring().data());
    assert(h1 == h2);

    char const *expected_hash = "c9f160777d4086fe8095fba58b7e20c228a4006b";
    assert(!strcmp(expected_hash, h1.cstring().data()));
    assert(!strcmp(expected_hash, h2.cstring().data()));
}

void test_subbyte_sha1() {
    {
    sha1::stream_hash<1>::type h;
    sha1::digest_type d = h.end_message();
    assert(!strcmp("da39a3ee5e6b4b0d3255bfef95601890afd80709",
                   d.cstring().data()));
    }

    {
    // echo -n "abc" | sha1sum
    sha1::stream_hash<4>::type h;
    h.update_one(0x6).update_one(0x1)
     .update_one(0x6).update_one(0x2)
     .update_one(0x6).update_one(0x3);
    sha1::digest_type d = h.end_message();
    assert(!strcmp("a9993e364706816aba3e25717850c26c9cd0d89d",
                   d.cstring().data()));
    }

    // More from http://csrc.nist.gov/groups/STM/cavp/documents/shs/SHAVS.pdf

    {
    // A.1/1
    bool a[] = {1, 0, 0, 1, 1};
    sha1::digest_type d = compute_digest_n<sha1>(a, sizeof(a)/sizeof(*a));
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("29826b003b906e660eff4027ce98af3531ac75ba",
                   d.cstring().data()));
    }

    {
    // A.1/2
    bool a[] = {0, 1, 0, 1,
                1, 1, 1, 0};
    sha1::digest_type d = compute_digest_n<sha1>(a, sizeof(a)/sizeof(*a));
    std::printf("%s\n", d.cstring().data());
    assert(d == compute_digest_n<sha1>("\x5e", 1));
    }

#define B0 0, 0, 0, 0
#define B1 0, 0, 0, 1
#define B2 0, 0, 1, 0
#define B3 0, 0, 1, 1
#define B4 0, 1, 0, 0
#define B5 0, 1, 0, 1
#define B6 0, 1, 1, 0
#define B7 0, 1, 1, 1
#define B8 1, 0, 0, 0
#define B9 1, 0, 0, 1
#define BA 1, 0, 1, 0
#define BB 1, 0, 1, 1
#define BC 1, 1, 0, 0
#define BD 1, 1, 0, 1
#define BE 1, 1, 1, 0
#define BF 1, 1, 1, 1

    {
    // A.1/3
    unsigned n = 123;
    bool a[] = {B4, B9, BB, B2, BA, BE, BC, B2,
                B5, B9, B4, BB, BB, BE, B3, BA,
                B3, BB, B1, B1, B7, B5, B4, B2,
                BD, B9, B4, BA, BC, B8, B8};
    sha1::digest_type d = compute_digest_n<sha1>(a, n);
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("6239781e03729919c01955b3ffa8acb60b988340",
                   d.cstring().data()));
    }

    {
    // A.2/1
    unsigned n = 611;
    bool a[] = {
        B6, B5, BF, B9, B3, B2, B9, B9,
        B5, BB, BA, B4, BC, BE, B2, BC,
        BB, B1, BB, B4, BA, B2, BE, B7,
        B1, BA, BE, B7, B0, B2, B2, B0,
        BA, BA, BC, BE, BC, B8, B9, B6,
        B2, BD, BD, B4, B4, B9, B9, BC,
        BB, BD, B7, BC, B8, B8, B7, BA,
        B9, B4, BE, BA, BA, BA, B1, B0,
        B1, BE, BA, B5, BA, BA, BB, BC,
        B5, B2, B9, BB, B4, BE, B7, BE,
        B4, B3, B6, B6, B5, BA, B5, BA,
        BF, B2, BC, BD, B0, B3, BF, BE,
        B6, B7, B8, BE, BA, B6, BA, B5,
        B0, B0, B5, BB, BB, BA, B3, BB,
        B0, B8, B2, B2, B0, B4, BC, B2,
        B8, BB, B9, B1, B0, B9, BF, B4,
        B6, B9, BD, BA, BC, B9, B2, BA,
        BA, BA, BB, B3, BA, BA, B7, BC,
        B1, B1, BA, B1, BB, B3, B2, BA,
        BE
    };
    sha1::digest_type d = compute_digest_n<sha1>(a, n);
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("8c5b2a5ddae5a97fc7f9d85661c672adbf7933d4",
                   d.cstring().data()));
    }

    {
    // A.1/4
    unsigned n = 128;
    uint32_t a[] = {
        0x9a7dfdf1, 0xecead06e, 0xd646aa55, 0xfe757146,
    };
    sha1::stream_hash<4>::type h;
    for (unsigned i = 0; i < n; i += 4) {
        h.update_one((a[i/32] >> (32-4-i%32)) % 0x10);
    }
    sha1::digest_type d = h.end_message();
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("82abff6605dbe1c17def12a394fa22a82b544a35",
                   d.cstring().data()));
    }

    {
    // A.2/2
    unsigned n = 1304;
    uint32_t a[] = {
        0xf78f9214, 0x1bcd170a, 0xe89b4fba, 0x15a1d59f, 0x3fd84d22, 0x3c9251bd,
        0xacbbae61, 0xd05ed115, 0xa06a7ce1, 0x17b7beea, 0xd24421de, 0xd9c32592,
        0xbd57edea, 0xe39c39fa, 0x1fe8946a, 0x84d0cf1f, 0x7beead17, 0x13e2e095,
        0x9897347f, 0x67c80b04, 0x00c20981, 0x5d6b10a6, 0x83836fd5, 0x562a56ca,
        0xb1a28e81, 0xb6576654, 0x631cf165, 0x66b86e3b, 0x33a108b0, 0x5307c00a,
        0xff14a768, 0xed735060, 0x6a0f85e6, 0xa91d396f, 0x5b5cbe57, 0x7f9b3880,
        0x7c7d523d, 0x6d792f6e, 0xbc24a4ec, 0xf2b3a427, 0xcdbbfb00
    };
    sha1::stream_hash<4>::type h;
    for (unsigned i = 0; i < n; i += 4) {
        h.update_one((a[i/32] >> (32-4-i%32)) % 0x10);
    }
    sha1::digest_type d = h.end_message();
    std::printf("%s\n", d.cstring().data());
    assert(!strcmp("cb0082c8f197d260991ba6a460e76e202bad27b3",
                   d.cstring().data()));
    }
}

void test_accumulator_sha1() {
    sha1::block_hash_type a;

    {
    sha1::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    }

    a.reset();
    {
    // 0-length input: echo -n | sha1sum

    // A single 1 bit after the (empty) message,
    // then pad with 0s,
    // then add the length, which is also 0
    sha1::block_hash_type::block_type m = {{0x80000000u}};
    a.update(m);

    sha1::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert(!strcmp("da39a3ee5e6b4b0d3255bfef95601890afd80709",
                   s.cstring().data()));
    }

    a.reset();
    {
    // Example from appendix A.1: echo -n "abc" | sha1sum
    sha1::block_hash_type::block_type m = {{}};
    m[ 0] = 0x61626380;
    m[15] = 0x00000018;
    a.update(m);

    sha1::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert(!strcmp("a9993e364706816aba3e25717850c26c9cd0d89d",
                   s.cstring().data()));
    }

    a.reset();
    {
    // Example from appendix A.2:
    // echo -n "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" | sha1sum
    sha1::block_hash_type::block_type m1 = {{
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
    assert(!strcmp("f4286818c37b27ae0408f581846771484a566572",
                   a.digest().cstring().data()));
    sha1::block_hash_type::block_type m2 = {{}};
    m2[15] = 0x000001c0;
    a.update(m2);
    sha1::digest_type s = a.end_message();
    std::printf("%s\n", s.cstring().data());
    assert(!strcmp("84983e441c3bd26ebaae4aa1f95129e5e54670f1",
                   s.cstring().data()));
    }
}

void test_preprocessor_sha1() {

    {
    sha1::stream_hash<8>::type h;
    sha1::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert(!strcmp("da39a3ee5e6b4b0d3255bfef95601890afd80709",
                   s.cstring().data()));
    }

    {
    // Example from Appendix A.1
    sha1::stream_hash<8>::type h;
    h.update_one('a').update_one('b').update_one('c');
    sha1::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert(!strcmp("a9993e364706816aba3e25717850c26c9cd0d89d",
                   s.cstring().data()));
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    // Example from Appendix A.3
    sha1::stream_hash<8>::type h;
    for (unsigned i = 0; i < 1000000; ++i) {
        h.update_one('a');
    }
    sha1::digest_type s = h.end_message();
    std::printf("%s\n", s.cstring().data());
    assert(!strcmp("34aa973cd4c4daa4f61eeb2bdbad27316534016f",
                   s.cstring().data()));
    }
#endif
}

int main() {
    test_collision_sha0();
    test_subbyte_sha1();
    test_accumulator_sha1();
    test_preprocessor_sha1();

    {
    sha1::digest_type h = compute_digest<sha1>((int*)1, (int*)1);
    std::printf("%s\n", h.cstring().data());
    assert(!strcmp("da39a3ee5e6b4b0d3255bfef95601890afd80709",
                   h.cstring().data()));
    }

    {
    sha1::digest_type h = compute_digest_n<sha0>("abc", 3);
    std::printf("%s\n", h.cstring().data());
    assert(!strcmp("0164b8a914cd2a5e74c4f7ff082c4d97f1edf880",
                   h.cstring().data()));
    }
}

