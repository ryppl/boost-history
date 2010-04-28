
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_MERKLE_DAMGARD_BLOCK_HASH_HPP
#define BOOST_HASH_MERKLE_DAMGARD_BLOCK_HASH_HPP

namespace boost {
namespace hash {

//
// The Merkle-Damgård construction builds a block hash from a
// one-way compressor.  As this version operated on the block
// level, it doesn't contain any padding or other strengthening.
// For a Wide Pipe construction, use a digest_type that will
// truncate the internal state.
//

struct nop_finalizer {
    template <typename T>
    void
    operator()(T &) {}
};

template <typename iv_G,
          typename compressor_F,
          typename digest_C,
          typename finalizer_F = nop_finalizer>
class merkle_damgard_block_hash {
  public:
    typedef iv_G iv_generator;
    typedef compressor_F compressor_functor;
    typedef digest_C digest_creator;
    typedef typename digest_creator::digest_type digest_type;
    typedef finalizer_F finalizer_functor;

    static unsigned const word_bits = compressor_functor::word_bits;
    typedef typename compressor_functor::word_type word_type;

    static unsigned const state_bits = compressor_functor::state_bits;
    static unsigned const state_words = compressor_functor::state_words;
    typedef typename compressor_functor::state_type state_type;

    static unsigned const block_bits = compressor_functor::block_bits;
    static unsigned const block_words = compressor_functor::block_words;
    typedef typename compressor_functor::block_type block_type;

  public:
    merkle_damgard_block_hash &update(block_type const &block) {
        compressor_functor compressor;
        compressor(state_, block);
        return *this;
    }
    digest_type end_message() {
        finalizer_functor finalizer;
        finalizer(state_);
        digest_type digest = digest_creator::template from_state<state_bits, word_bits>(state_);
        reset();
        return digest;
    }
    digest_type digest() const {
        return merkle_damgard_block_hash(*this).end_message();
    }

  public:
    merkle_damgard_block_hash() { reset(); }
    void reset(state_type const &s) { state_ = s; }
    void reset() {
        iv_generator iv;
        reset(iv());
    }
    state_type const &state() const { return state_; }
  private:
    state_type state_;
};

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_MERKLE_DAMGARD_BLOCK_HASH_HPP
