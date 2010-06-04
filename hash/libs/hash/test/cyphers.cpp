
#include <boost/hash/block_cyphers/md4.hpp>
#include <boost/hash/block_cyphers/md5.hpp>
#include <boost/hash/block_cyphers/shacal.hpp>
#include <boost/hash/block_cyphers/shacal1.hpp>
#include <boost/hash/block_cyphers/shacal2.hpp>
#include <boost/hash/block_cyphers/threefish.hpp>

#include <limits>

#include <cassert>
#include <cstdlib>

#ifndef TRIALS
#ifdef QUICK
#define TRIALS 1000
#else
#define TRIALS 100000
#endif
#endif

template <typename T>
T myrand() {
    T x = 0;
    unsigned n = std::numeric_limits<T>::digits
               + std::numeric_limits<T>::is_signed;
    for (unsigned i = 0; i < n; i += 16) {
        x <<= 16;
        x ^= std::rand();
    }
    return x;
}
template <typename A>
A myrandarray() {
    A a;
    for (unsigned i = 0; i < a.size(); ++i) {
        a[i] = myrand<typename A::value_type>();
    }
    return a;
}

template <typename block_cypher_type>
void test_block_cypher() {
    std::srand(0);

    typedef typename block_cypher_type::key_type key_type;
    typedef typename block_cypher_type::block_type block_type;

    for (unsigned i = 0; i < TRIALS; ++i) {
        key_type key = myrandarray<key_type>();
        block_cypher_type cypher(key);
        block_type plaintext = myrandarray<block_type>();
        block_type cyphertext = cypher.encypher(plaintext);
        assert(plaintext == cypher.decypher(cyphertext));
    }
}

int main() {
    using namespace boost::hashes::block_cyphers;
    test_block_cypher<md4>();
    test_block_cypher<md5>();
    test_block_cypher<shacal>();
    test_block_cypher<shacal1>();
    test_block_cypher<shacal2<256> >();
    test_block_cypher<shacal2<512> >();
    test_block_cypher<threefish<256> >();
    test_block_cypher<threefish<512> >();
    test_block_cypher<threefish<1024> >();
}

