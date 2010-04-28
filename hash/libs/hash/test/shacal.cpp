
//#define BOOST_HASH_SHOW_PROGRESS
#include <boost/hash/block_cyphers/shacal1.hpp>
#include <boost/hash/block_cyphers/shacal2.hpp>
#include <boost/hash/davies_meyer_compressor.hpp>
#include <boost/hash/detail/sha_policy.hpp>
#include <boost/hash/detail/sha1_policy.hpp>
#include <boost/hash/detail/sha2_policy.hpp>
#include <boost/hash/merkle_damgard_block_hash.hpp>
#include <boost/hash/stream_preprocessor.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>

struct state_adder {
    template <typename T>
    void operator()(T &s1, T const &s2) {
        typedef typename T::size_type size_type;
        size_type n = (s2.size() < s1.size() ? s2.size() : s1.size());
        for (typename T::size_type i = 0; i < n; ++i) {
            s1[i] += s2[i];
        }
    }
};

void test_shacal1_block_cypher() {
    typedef boost::hash::block_cyphers::shacal1 bct;

    {
    // Test with the equivalent of SHA-1("")
    bct::block_type plaintext = {{
        0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476,
        0xc3d2e1f0,
    }};
    bct::key_type key = {{0x80000000, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0}};
    bct cypher(key);
    bct::block_type cyphertext = cypher.encypher(plaintext);
    bct::block_type expected_cyphertext = {{
        0x72f480ed,
        0x6e9d9f84,
        0x999ae2f1,
        0x852dc41a,
        0xec052519,
    }};
    assert(cyphertext == expected_cyphertext);
    bct::block_type new_plaintext = cypher.decypher(cyphertext);
    assert(plaintext == new_plaintext);
    }

    typedef boost::hash::davies_meyer_compressor<bct, state_adder> owcft;

    {
    // Test with the equivalent of SHA-256("")
    owcft::state_type const H0 = {{
        0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476,
        0xc3d2e1f0,
    }};
    owcft::block_type block = {{0x80000000, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0}};
    owcft::state_type H = H0;
    owcft f; f(H, block);
    owcft::state_type const H1 = {{
        0xda39a3ee,
        0x5e6b4b0d,
        0x3255bfef,
        0x95601890,
        0xafd80709,
    }};
    assert(H == H1);
    }

    typedef boost::hash::merkle_damgard_block_hash<
                boost::hash::detail::sha1_policy::iv_generator,
                owcft,
                boost::hash::digest_from_state<
                    boost::hash::digest<160>,
                    boost::hash::bitstream_endian::big_byte_big_bit>
            > bht;

    {
    // Test with the equivalent of SHA-1("")
    bht::block_type block = {{0x80000000, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0}};
    bht bh;
    bh.update(block);
    bht::digest_type h = bh.end_message();
    printf("%s\n", h.cstring().data());
    char const *eh = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
    assert(!strcmp(eh, h.cstring().data()));
    }
}

void test_shacal256_block_cypher() {
    typedef boost::hash::block_cyphers::shacal2<256> bct;

    {
    // Test with the equivalent of SHA-256("")
    bct::block_type plaintext = {{
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19,
    }};
    bct::key_type key = {{0x80000000, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0}};
    bct cypher(key);
    bct::block_type cyphertext = cypher.encypher(plaintext);
    bct::block_type expected_cyphertext = {{
        0x79a6dddb,
        0xdd946d8f,
        0x5e8d0156,
        0xf41fc3ea,
        0xd69fef65,
        0xc9962ac0,
        0x8511bf70,
        0x1c71eb3c,
    }};
    assert(cyphertext == expected_cyphertext);
    bct::block_type new_plaintext = cypher.decypher(cyphertext);
    assert(plaintext == new_plaintext);
    }

    typedef boost::hash::davies_meyer_compressor<bct, state_adder> owcft;

    {
    // Test with the equivalent of SHA-256("")
    owcft::state_type const H0 = {{
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19,
    }};
    owcft::block_type block = {{0x80000000, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0}};
    owcft::state_type H = H0;
    owcft f; f(H, block);
    owcft::state_type const H1 = {{
        0xe3b0c442,
        0x98fc1c14,
        0x9afbf4c8,
        0x996fb924,
        0x27ae41e4,
        0x649b934c,
        0xa495991b,
        0x7852b855,
    }};
    assert(H == H1);
    }

    typedef boost::hash::merkle_damgard_block_hash<
                boost::hash::detail::sha2_policy<256>::iv_generator,
                owcft,
                boost::hash::digest_from_state<
                    boost::hash::digest<256>,
                    boost::hash::bitstream_endian::big_byte_big_bit>
            > bht;

    {
    // Test with the equivalent of SHA-256("")
    bht::block_type block = {{0x80000000, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0}};
    bht bh;
    bh.update(block);
    bht::digest_type h = bh.end_message();
    printf("%s\n", h.cstring().data());
    char const *eh = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";
    assert(!strcmp(eh, h.cstring().data()));
    }
}

void test_sha1() {
    using namespace boost::hash;
    typedef merkle_damgard_block_hash<
                detail::sha1_policy::iv_generator,
                davies_meyer_compressor<block_cyphers::shacal1, state_adder>,
                digest_from_state<digest<160>, bitstream_endian::big_byte_big_bit>
            > block_hash_type;
    typedef stream_preprocessor<
                bitstream_endian::big_byte_big_bit,
                8,
                block_hash_type::word_bits * 2,
                block_hash_type
            > sha1_byte_hash;
    typedef sha1_byte_hash::digest_type digest_type;

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    // perl -e 'for ($x = 1000000000; $x--;) {print "a";}' | sha1sum
    sha1_byte_hash h;
    for (unsigned n = 1000000000; n--; ) h.update('a');
    digest_type d = h.end_message();
    printf("%s\n", d.cstring().data());
    char const *ed = "d0f3e4f2f31c665abbd8f518e848d5cb80ca78f7";
    assert(!strcmp(ed, d.cstring().data()));
    }
#endif
}

void test_sha512() {
    using namespace boost::hash;
    unsigned const SHA = 512;
    typedef merkle_damgard_block_hash<
                detail::sha2_policy<SHA>::iv_generator,
                davies_meyer_compressor<block_cyphers::shacal2<SHA>, state_adder>,
                digest_from_state<digest<SHA>, bitstream_endian::big_byte_big_bit>
            > block_hash_type;
    typedef stream_preprocessor<
                bitstream_endian::big_byte_big_bit,
                8,
                block_hash_type::word_bits * 2,
                block_hash_type
            > sha512_byte_hash;
    typedef sha512_byte_hash::digest_type digest_type;

    {
    sha512_byte_hash h;
    digest_type d = h.end_message();
    printf("%s\n", d.cstring().data());
    char const *ed = "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
                     "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
    assert(!strcmp(ed, d.cstring().data()));
    }

    {
    sha512_byte_hash h;
    h.update('a').update('b').update('c');
    digest_type d = h.end_message();
    printf("%s\n", d.cstring().data());
    char const *ed = "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
                     "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f";
    assert(!strcmp(ed, d.cstring().data()));
    }

    {
    sha512_byte_hash h;
    char const *m = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
                    "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    for (char const *p = m; *p; ++p) {
        h.update(*p);
    }
    digest_type d = h.end_message();
    printf("%s\n", d.cstring().data());
    char const *ed = "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018"
                     "501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909";
    assert(!strcmp(ed, d.cstring().data()));
    }

#ifndef BOOST_HASH_SHOW_PROGRESS
    {
    sha512_byte_hash h;
    for (unsigned n = 1000000; n--; ) h.update('a');
    digest_type d = h.end_message();
    printf("%s\n", d.cstring().data());
    char const *ed = "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
                     "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b";
    assert(!strcmp(ed, d.cstring().data()));
    }

#ifndef QUICK
    {
    // perl -e 'for ($x = 1000000000; $x--;) {print "a";}' | sha512sum
    sha512_byte_hash h;
    for (unsigned n = 1000000000; n--; ) h.update('a');
    digest_type d = h.end_message();
    printf("%s\n", d.cstring().data());
    char const *ed = "7cc86d7e06edc6a2029b8c0fa0e3ffb013888fd360f8faf681c7cffd08eacffb"
                     "f09ae159827fc6e4c03894e6ecf4616395d3479f80d66ed3ac81a64ea0445f32";
    assert(!strcmp(ed, d.cstring().data()));
    }
#endif // QUICK
#endif
}

int main() {
    test_shacal1_block_cypher();
    test_shacal256_block_cypher();
    test_sha1();
    test_sha512();
}

