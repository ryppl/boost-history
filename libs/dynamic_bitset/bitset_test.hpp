// (C) Copyright Jeremy Siek 2001.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.

#include <vector>
#include <fstream>
#include <string>
#include <algorithm> // for std::min

#include "boost/config.hpp" // for BOOST_STATIC_CONSTANT
#include "boost/limits.hpp"
#include "boost/test/minimal.hpp"

// Extract the bit at position n from num.
template <typename Block>
inline bool nth_bit(Block num, std::size_t n)
{
  // Move the nth bit to position 0 and then mask out all other bits.
  return (num >> n) & 1;
}


// A long, 'irregular', string useful for various tests
std::string get_long_string()
{
  const char * p =
  //    6         5         4         3         2         1
  // 3210987654321098765432109876543210987654321098765432109876543210
    "1110011100011110000011110000011111110000000000000110101110000000"
    "1010101000011100011101010111110000101011111000001111100011100011"
    "0000000110000001000000111100000111100010101111111000000011100011"
    "1111111111111111111111111111111111111111111111111111111111111100"
    "1000001100000001111111111111110000000011111000111100001010100000"
    "101000111100011010101110011011000000010";

  return std::string(p);
}


// constructors
//   default (can't do this generically)

//   from unsigned long

template <typename Bitset>
struct bitset_test {

  static void from_unsigned_long(std::size_t N, unsigned long num)
  {
    // initializes the first M bit position to the cooresponding bit
    // values in val. M is the smaller of N and the width of unsigned
    // long

    // missing from the std?
    //   if M < N then the remaining bit positions are initialized to zero

    Bitset b(N, num);
    BOOST_CHECK(b.size() == N);

    const std::size_t ulong_width = std::numeric_limits<unsigned long>::digits;
    std::size_t M = std::min(N, ulong_width);
    std::size_t I;
    for (I = 0; I < M; ++I)
      BOOST_CHECK(b[I] == nth_bit(num, I));
    for (; I < N; ++I)
      BOOST_CHECK(b[I] == 0);
  }

  // from string
  //
  // Note: The corresponding function in dynamic_bitset (constructor
  // from a string) has several default arguments. Actually we don't
  // test the correct working of those defaults here (except for the
  // default of num_bits). I'm not sure what to do in this regard.
  //
  template <typename Ch, typename Tr, typename Al>
  static void from_string(const std::basic_string<Ch, Tr, Al>& str,
                          std::size_t pos,
                          std::size_t max_char,
                          std::size_t num_bits = -1)
  {

      std::size_t rlen = std::min(max_char, str.size() - pos); // [gps]

      // The resulting size N of the bitset is num_bits, if
      // that is different from the default arg, rlen otherwise.
      // Put M = the smaller of N and rlen, then character
      // position pos + M - 1 corresponds to bit position zero.
      // Subsequent decreasing character positions correspond to
      // increasing bit positions.

      const bool size_upon_string = num_bits == (std::size_t)(-1);
      Bitset b = size_upon_string ?
                    Bitset(str, pos, max_char)
                  : Bitset(str, pos, max_char, num_bits);

      const std::size_t actual_size = size_upon_string? rlen : num_bits;
      BOOST_CHECK(b.size() == actual_size);
      std::size_t m = std::min(num_bits, rlen);
      std::size_t j;
      for (j = 0; j < m; ++j)
          BOOST_CHECK(b[j] == (str[pos + m - 1 - j] == '1')); // [gps]
      // If M < N, remaining bit positions are zero
      for (; j < actual_size; ++j)
          BOOST_CHECK(b[j] == 0);


  }

  typedef typename Bitset::block_type Block;
  BOOST_STATIC_CONSTANT(int, bits_per_block = Bitset::bits_per_block);

  // PRE: std::equal(first1, last1, first2) == true
  static void from_block_range(std::vector<Block> blocks)
  {
    {
      Bitset bset(blocks.begin(), blocks.end());
      std::size_t n = blocks.size();
      for (std::size_t b = 0; b < n; ++b) {
        for (std::size_t i = 0; i < bits_per_block; ++i) {
          std::size_t bit = b * bits_per_block + i;
          BOOST_CHECK(bset[bit] == nth_bit(blocks[b], i));
        }
      }
    }
    {
      Bitset bset(blocks.size() * bits_per_block);
      boost::from_block_range(blocks.begin(), blocks.end(), bset);
      std::size_t n = blocks.size();
      for (std::size_t b = 0; b < n; ++b) {
        for (std::size_t i = 0; i < bits_per_block; ++i) {
          std::size_t bit = b * bits_per_block + i;
          BOOST_CHECK(bset[bit] == nth_bit(blocks[b], i));
        }
      }
    }
  }

  // copy constructor (absent from std::bitset)
  static void copy_constructor(const Bitset& b)
  {
    Bitset copy(b);
    BOOST_CHECK(b == copy);

    // Changes to the copy do not affect the original
    if (b.size() > 0) {
      std::size_t pos = copy.size() / 2;
      copy.flip(pos);
      BOOST_CHECK(copy[pos] != b[pos]);
    }
  }

  // assignment operator (absent from std::bitset)
  static void assignment_operator(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset b(lhs);
    b = rhs;
    BOOST_CHECK(b == rhs);

    // Changes to the copy do not affect the original
    if (b.size() > 0) {
      std::size_t pos = b.size() / 2;
      b.flip(pos);
      BOOST_CHECK(b[pos] != rhs[pos]);
    }
  }

  static void resize(const Bitset& lhs)
  {
    Bitset b(lhs);

    // Test no change in size
    b.resize(lhs.size());
    BOOST_CHECK(b == lhs);

    // Test increase in size
    b.resize(lhs.size() * 2, true);

    std::size_t i;
    for (i = 0; i < lhs.size(); ++i)
      BOOST_CHECK(b[i] == lhs[i]);
    for (; i < b.size(); ++i)
      BOOST_CHECK(b[i] == true);

    // Test decrease in size
    b.resize(lhs.size());
    for (i = 0; i < lhs.size(); ++i)
      BOOST_CHECK(b[i] == lhs[i]);
  }

  static void clear(const Bitset& lhs)
  {
    Bitset b(lhs);
    b.clear();
    BOOST_CHECK(b.size() == 0);
  }

  static void append_bit(const Bitset& lhs)
  {
    Bitset b(lhs);
    b.push_back(true);
    BOOST_CHECK(b.size() == lhs.size() + 1);
    BOOST_CHECK(b[b.size() - 1] == true);
    for (std::size_t i = 0; i < lhs.size(); ++i)
      BOOST_CHECK(b[i] == lhs[i]);

    b.push_back(false);
    BOOST_CHECK(b.size() == lhs.size() + 2);
    BOOST_CHECK(b[b.size() - 1] == false);
    BOOST_CHECK(b[b.size() - 2] == true);
    for (std::size_t j = 0; j < lhs.size(); ++j)
      BOOST_CHECK(b[j] == lhs[j]);
  }

  static void append_block(const Bitset& lhs)
  {
    Bitset b(lhs);
    Block value(128);
    b.append(value);
    BOOST_CHECK(b.size() == lhs.size() + Bitset::bits_per_block);
    for (std::size_t i = 0; i < Bitset::bits_per_block; ++i)
      BOOST_CHECK(b[lhs.size() + i] == bool((value >> i) & 1));
  }

  static void append_block_range(const Bitset& lhs, std::vector<Block> blocks)
  {
    Bitset b(lhs), c(lhs);
    b.append(blocks.begin(), blocks.end());
    for (typename std::vector<Block>::iterator i = blocks.begin();
         i != blocks.end(); ++i)
      c.append(*i);
    BOOST_CHECK(b == c);
  }

  // operator[] and reference members
  // PRE: b[i] == bit_vec[i]
  static void operator_bracket(const Bitset& lhs, const std::vector<bool>& bit_vec)
  {
    Bitset b(lhs);
    std::size_t i, j, k;

    // x = b[i]
    // x = ~b[i]
    for (i = 0; i < b.size(); ++i) {
      bool x = b[i];
      BOOST_CHECK(x == bit_vec[i]);
      x = ~b[i];
      BOOST_CHECK(x == !bit_vec[i]);
    }
    Bitset prev(b);

    // b[i] = x
    for (j = 0; j < b.size(); ++j) {
      bool x = !prev[j];
      b[j] = x;
      for (k = 0; k < b.size(); ++k)
        if (j == k)
          BOOST_CHECK(b[k] == x);
        else
          BOOST_CHECK(b[k] == prev[k]);
      b[j] = prev[j];
    }
    b.flip();

    // b[i] = b[j]
    for (i = 0; i < b.size(); ++i) {
      b[i] = prev[i];
      for (j = 0; j < b.size(); ++j) {
        if (i == j)
          BOOST_CHECK(b[j] == prev[j]);
        else
          BOOST_CHECK(b[j] == !prev[j]);
      }
      b[i] = !prev[i];
    }

    // b[i].flip()
    for (i = 0; i < b.size(); ++i) {
      b[i].flip();
      for (j = 0; j < b.size(); ++j) {
        if (i == j)
          BOOST_CHECK(b[j] == prev[j]);
        else
          BOOST_CHECK(b[j] == !prev[j]);
      }
      b[i].flip();
    }
  }

  //===========================================================================
  // bitwise operators

  // bitwise and assignment

  // PRE: b.size() == rhs.size()
  static void and_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs &= rhs;
    // Clears each bit in lhs for which the corresponding bit in rhs is
    // clear, and leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 0)
        BOOST_CHECK(lhs[I] == 0);
      else
        BOOST_CHECK(lhs[I] == prev[I]);
  }

  // PRE: b.size() == rhs.size()
  static void or_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs |= rhs;
    // Sets each bit in lhs for which the corresponding bit in rhs is set, and
    // leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 1)
        BOOST_CHECK(lhs[I] == 1);
      else
        BOOST_CHECK(lhs[I] == prev[I]);
  }

  // PRE: b.size() == rhs.size()
  static void xor_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs ^= rhs;
    // Flips each bit in lhs for which the corresponding bit in rhs is set,
    // and leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 1)
        BOOST_CHECK(lhs[I] == !prev[I]);
      else
        BOOST_CHECK(lhs[I] == prev[I]);
  }

  // PRE: b.size() == rhs.size()
  static void sub_assignment(const Bitset& b, const Bitset& rhs)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs -= rhs;
    // Resets each bit in lhs for which the corresponding bit in rhs is set,
    // and leaves all other bits unchanged.
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (rhs[I] == 1)
        BOOST_CHECK(lhs[I] == 0);
      else
        BOOST_CHECK(lhs[I] == prev[I]);
  }

  static void shift_left_assignment(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs <<= pos;
    // Replaces each bit at position I in lhs with the following value:
    // - If I < pos, the new value is zero
    // - If I >= pos, the new value is the previous value of the bit at
    //   position I - pos
    for (std::size_t I = 0; I < lhs.size(); ++I)
      if (I < pos)
        BOOST_CHECK(lhs[I] == 0);
      else
        BOOST_CHECK(lhs[I] == prev[I - pos]);
  }

  static void shift_right_assignment(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    Bitset prev(lhs);
    lhs >>= pos;
    // Replaces each bit at position I in lhs with the following value:
    // - If pos >= N - I, the new value is zero
    // - If pos < N - I, the new value is the previous value of the bit at
    //    position I + pos
    std::size_t N = lhs.size();
    for (std::size_t I = 0; I < N; ++I)
      if (pos >= N - I)
        BOOST_CHECK(lhs[I] == 0);
      else
        BOOST_CHECK(lhs[I] == prev[I + pos]);
  }


  static void set_all(const Bitset& b)
  {
    Bitset lhs(b);
    lhs.set();
    for (std::size_t I = 0; I < lhs.size(); ++I)
      BOOST_CHECK(lhs[I] == 1);
  }

  static void set_one(const Bitset& b, std::size_t pos, bool value)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      Bitset prev(lhs);
      // Stores a new value in the bit at position pos in lhs.
      lhs.set(pos, value);
      BOOST_CHECK(lhs[pos] == value);

      // All other values of lhs remain unchanged
      for (std::size_t I = 0; I < N; ++I)
        if (I != pos)
          BOOST_CHECK(lhs[I] == prev[I]);
    } else {
      // Not in range, doesn't satisfy precondition.
    }
  }

  static void reset_all(const Bitset& b)
  {
    Bitset lhs(b);
    // Resets all bits in lhs
    lhs.reset();
    for (std::size_t I = 0; I < lhs.size(); ++I)
      BOOST_CHECK(lhs[I] == 0);
  }

  static void reset_one(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      Bitset prev(lhs);
      lhs.reset(pos);
      // Resets the bit at position pos in lhs
      BOOST_CHECK(lhs[pos] == 0);

      // All other values of lhs remain unchanged
      for (std::size_t I = 0; I < N; ++I)
        if (I != pos)
          BOOST_CHECK(lhs[I] == prev[I]);
    } else {
      // Not in range, doesn't satisfy precondition.
    }
  }

  static void operator_flip(const Bitset& b)
  {
    Bitset lhs(b);
    Bitset x(lhs);
    BOOST_CHECK(~lhs == x.flip());
  }

  static void flip_all(const Bitset& b)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    Bitset prev(lhs);
    lhs.flip();
    // Toggles all the bits in lhs
    for (std::size_t I = 0; I < N; ++I)
      BOOST_CHECK(lhs[I] == !prev[I]);
  }

  static void flip_one(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      Bitset prev(lhs);
      lhs.flip(pos);
      // Toggles the bit at position pos in lhs
      BOOST_CHECK(lhs[pos] == !prev[pos]);

      // All other values of lhs remain unchanged
      for (std::size_t I = 0; I < N; ++I)
        if (I != pos)
          BOOST_CHECK(lhs[I] == prev[I]);
    } else {
      // Not in range, doesn't satisfy precondition.
    }
  }

  // to_ulong()
  static void to_ulong(const Bitset& lhs)
  {
    std::size_t N = lhs.size();
    std::size_t n = std::numeric_limits<unsigned long>::digits;
    bool will_overflow = false;
    for (std::size_t I = n; I < N; ++I)
      if (lhs[I] != 0)
        will_overflow = true;
    if (will_overflow) {
      try {
        (void)lhs.to_ulong();
        BOOST_CHECK(false); // It should have thrown and exception
      } catch (std::overflow_error) {
        // Good!
      } catch (...) {
        BOOST_CHECK(false); // threw the wrong exception
      }
    } else {
      unsigned long num = lhs.to_ulong();
      // Make sure the number is right
      for (std::size_t I = 0; I < N; ++I)
        BOOST_CHECK(lhs[I] == nth_bit(num, I));
    }
  }

  // to_string()
  static void to_string(const Bitset& b)
  {
    // Construct a string object of the appropriate type and initializes
    // it to a string of length N characters. Each character is determined
    // by the value of its corresponding bit position in b. Character
    // position N - 1 corresponds to bit position zero. Sebsequent
    // decreasing character positions correspond to increasing bit
    // positions. Bit value zero becomes the charactet 0, bit value one
    // becomes the character 1.
    std::string str;
    boost::to_string(b, str);
    std::size_t N = b.size();
    BOOST_CHECK(str.size() == b.size());
    for (std::size_t I = 0; I < b.size(); ++I)
      BOOST_CHECK(b[I] == 0 ? (str[N - 1 - I] == '0') : (str[N - 1 - I] == '1'));
  }

  static void count(const Bitset& b)
  {
    std::size_t c = b.count();
    std::size_t c_real = 0;
    for (std::size_t I = 0; I < b.size(); ++I)
      if (b[I])
        ++c_real;
    BOOST_CHECK(c == c_real);
  }

  static void size(const Bitset& b)
  {
    BOOST_CHECK(Bitset(b).set().count() == b.size());
  }

  static void any(const Bitset& b)
  {
    BOOST_CHECK(b.any() == (b.count() > 0));
  }

  static void none(const Bitset& b)
  {
    BOOST_CHECK(b.none() == (b.count() == 0));
  }

  static void subset(const Bitset& a, const Bitset& b)
  {
    if (a.is_subset_of(b)) {
      for (std::size_t I = 0; I < a.size(); ++I)
        if (a[I])
          BOOST_CHECK(b[I]);
    } else {
      bool is_subset = true;
      for (std::size_t I = 0; I < a.size(); ++I)
        if (a[I] && !b[I]) {
          is_subset = false;
          break;
        }
      BOOST_CHECK(is_subset == false);
    }
  }

  static void proper_subset(const Bitset& a, const Bitset& b)
  {
    if (a.is_proper_subset_of(b)) {
      for (std::size_t I = 0; I < a.size(); ++I)
        if (a[I])
          BOOST_CHECK(b[I]);
      BOOST_CHECK(a.count() < b.count());
    } else {
      bool is_subset = true;
      for (std::size_t I = 0; I < a.size(); ++I)
        if (a[I] && !b[I]) {
          is_subset = false;
          break;
        }
      BOOST_CHECK(is_subset == false || a.count() >= b.count());
    }
  }

  static void find_first(const Bitset& b)
  {
      // find first non-null bit, if any
      typename Bitset::size_type i = 0;
      while (i < b.size() && b[i] == 0)
          ++i;

      if (i == b.size())
        BOOST_CHECK(b.find_first() == Bitset::npos); // not found;
      else {
        BOOST_CHECK(b.find_first() == i);
        BOOST_CHECK(b.test(i) == true);
      }

  }

  static void find_next(const Bitset& b, typename Bitset::size_type prev)
  {
    BOOST_CHECK(next_bit_on(b, prev) == b.find_next(prev));
  }

  static void operator_equal(const Bitset& a, const Bitset& b)
  {
    if (a == b) {
      for (std::size_t I = 0; I < a.size(); ++I)
        BOOST_CHECK(a[I] == b[I]);
    } else {
      if (a.size() == b.size()) {
        bool diff = false;
        for (std::size_t I = 0; I < a.size(); ++I)
          if (a[I] != b[I]) {
            diff = true;
            break;
          }
        BOOST_CHECK(diff);
      }
    }
  }

  static void operator_not_equal(const Bitset& a, const Bitset& b)
  {
    if (a != b) {
      if (a.size() == b.size()) {
        bool diff = false;
        for (std::size_t I = 0; I < a.size(); ++I)
          if (a[I] != b[I]) {
            diff = true;
            break;
          }
        BOOST_CHECK(diff);
      }
    } else {
      for (std::size_t I = 0; I < a.size(); ++I)
        BOOST_CHECK(a[I] == b[I]);
    }
  }

  static bool less_than(const Bitset& a, const Bitset& b)
  {
    // Compare from most significant to least.
    // Careful, don't send unsigned int into negative territory!
    if (a.size() == 0)
      return false;

    std::size_t I;
    for (I = a.size() - 1; I > 0; --I)
      if (a[I] < b[I])
        return true;
      else if (a[I] > b[I])
        return false;
      // if (a[I] = b[I]) skip to next

    if (a[0] < b[0])
      return true;
    else
      return false;
  }

  static typename Bitset::size_type next_bit_on(const Bitset& b, typename Bitset::size_type prev)
  {
      // helper function for find_next()
      //

      if (b.none() == true || prev == Bitset::npos)
          return Bitset::npos;

      ++prev;

      if (prev >= b.size())
          return Bitset::npos;

      typename Bitset::size_type i = prev;
      while (i < b.size() && b[i] == 0)
          ++i;

      return i==b.size() ? Bitset::npos : i;

  }

  static void operator_less_than(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b))
      BOOST_CHECK(a < b);
    else
      BOOST_CHECK(!(a < b));
  }

  static void operator_greater_than(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b) || a == b)
      BOOST_CHECK(!(a > b));
    else
      BOOST_CHECK(a > b);
  }

  static void operator_less_than_eq(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b) || a == b)
      BOOST_CHECK(a <= b);
    else
      BOOST_CHECK(!(a <= b));
  }

  static void operator_greater_than_eq(const Bitset& a, const Bitset& b)
  {
    if (less_than(a, b))
      BOOST_CHECK(!(a >= b));
    else
      BOOST_CHECK(a >= b);
  }

  static void test_bit(const Bitset& b, std::size_t pos)
  {
    Bitset lhs(b);
    std::size_t N = lhs.size();
    if (pos < N) {
      BOOST_CHECK(lhs.test(pos) == lhs[pos]);
    } else {
      // Not in range, doesn't satisfy precondition.
    }
  }

  static void operator_shift_left(const Bitset& lhs, std::size_t pos)
  {
    Bitset x(lhs);
    BOOST_CHECK((lhs << pos) == (x <<= pos));
  }

  static void operator_shift_right(const Bitset& lhs, std::size_t pos)
  {
    Bitset x(lhs);
    BOOST_CHECK((lhs >> pos) == (x >>= pos));
  }

  // operator|
  static
  void operator_or(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_CHECK((lhs | rhs) == (x |= rhs));
  }

  // operator&
  static
  void operator_and(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_CHECK((lhs & rhs) == (x &= rhs));
  }

  // operator^
  static
  void operator_xor(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_CHECK((lhs ^ rhs) == (x ^= rhs));
  }

  // operator-
  static
  void operator_sub(const Bitset& lhs, const Bitset& rhs)
  {
    Bitset x(lhs);
    BOOST_CHECK((lhs - rhs) == (x -= rhs));
  }

  // operator<<(ostream,
  // operator>>(istream,

  static
  void stream_read_write(const Bitset& out, const Bitset& in)
  {
    Bitset x(in);
    {
      std::ofstream f("tmp");
      f << out;
    }
    {
      std::ifstream f("tmp");
      f >> x;
      BOOST_CHECK(out == x);
    }
  }

};
