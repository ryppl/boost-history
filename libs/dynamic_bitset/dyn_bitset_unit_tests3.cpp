// (C) Copyright Jeremy Siek 2001.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.


#include <cmath> // for pow

#include "boost/limits.hpp"
#include "boost/dynamic_bitset.hpp"
#include "bitset_test.hpp"
#include "boost/config.hpp"


// The usual workaround for VC6 bug related
// to template parameters that don't appear
// in the function's argument list.
// We should have this available somewhere,
// to avoid repeating it in every place. [gps]
//
#define BOOST_DUMMY_DEFAULT_ARGUMENT(t) t* = 0


template <typename Block>
void run_test_cases(BOOST_DUMMY_DEFAULT_ARGUMENT(Block))
{
  // a bunch of typedefs to have handy later on
  typedef boost::dynamic_bitset<Block> bitset_type;
  typedef typename bitset_type::size_type size_type;
  typedef bitset_test< boost::dynamic_bitset<Block> > Tests;

  std::string long_string(101, '0');
  for (std::size_t i = 0; i < long_string.size(); ++i)
    long_string[i] = '0' + (i % 2);

  std::size_t ul_width = std::numeric_limits<unsigned long>::digits;

  //=====================================================================
  // Test b.to_long()
  {
    boost::dynamic_bitset<Block> b;
    Tests::to_ulong(b);
  }
  {
    std::string ul_str(ul_width, '1');
    boost::dynamic_bitset<Block> b(ul_str);
    Tests::to_ulong(b);
  }
  { // case overflow
    boost::dynamic_bitset<Block> b(long_string);
    Tests::to_ulong(b);
  }
  //=====================================================================
  // Test to_string(b, str)
  {
    boost::dynamic_bitset<Block> b;
    Tests::to_string(b);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::to_string(b);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    Tests::to_string(b);
  }
  //=====================================================================
  // Test b.count()
  {
    boost::dynamic_bitset<Block> b;
    Tests::count(b);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::count(b);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    Tests::count(b);
  }
  //=====================================================================
  // Test b.size()
  {
    boost::dynamic_bitset<Block> b;
    Tests::size(b);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::size(b);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    Tests::size(b);
  }
  //=====================================================================
  // Test b.any()
  {
    boost::dynamic_bitset<Block> b;
    Tests::any(b);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::any(b);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    Tests::any(b);
  }
  //=====================================================================
  // Test b.none()
  {
    boost::dynamic_bitset<Block> b;
    Tests::none(b);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::none(b);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    Tests::none(b);
  }
  //=====================================================================
  // Test a.is_subset_of(b)
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::subset(a, b);
  }
  //=====================================================================
  // Test a.is_proper_subset_of(b)
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::proper_subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::proper_subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::proper_subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::proper_subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::proper_subset(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::proper_subset(a, b);
  }

  //=====================================================================
  // Test find_first
  {
      // empty bitset
      bitset_type b;
      Tests::find_first(b);
  }
  {
      // all-0s bitset
      bitset_type b(4 * bitset_type::bits_per_block, 0ul);
      Tests::find_first(b);
  }
  {
      // first bit on
      bitset_type b(1, 1ul);
      Tests::find_first(b);
  }
  {
      // last bit on
      bitset_type b(4 * bitset_type::bits_per_block - 1, 0ul);
      b.set(b.size() - 1);
      Tests::find_first(b);
  }

  //=====================================================================
  // Test find_next
  {
      // empty bitset
      bitset_type b;

      // check
      Tests::find_next(b, 0);
      Tests::find_next(b, 1);
      Tests::find_next(b, 200);
      Tests::find_next(b, b.npos); // G.P.S.
  }
  {
      // all-1s bitset
      bitset_type b(16 * bitset_type::bits_per_block);
      b.set();

      // check
      for(size_type i = 0; i < b.size(); ++i) {
          Tests::find_next(b, i);
      }
  }
  {
      // bitset with 1s at block boundary only
      const int num_blocks = 32;
      const int block_width = bitset_type::bits_per_block;

      bitset_type b(num_blocks * block_width);
      size_type i = block_width - 1;
      for ( ; i < b.size(); i += block_width) {

        b.set(i);
        size_type first_in_block = i - (block_width - 1);
        b.set(first_in_block);
      }

      // check
      for (i = 0; i < b.size(); ++i) {
          Tests::find_next(b, i);
      }

  }
  {
      // bitset with alternate 1s and 0s
      const size_type sz = 1000;
      bitset_type b(sz);

      size_type i = 0;
      for ( ; i < sz; ++i) {
        b[i] = (i%2 == 0);
      }

      // check
      for (i = 0; i < sz; ++i) {
          Tests::find_next(b, i);
      }

  }


  //=====================================================================
  // Test operator==
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::operator_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::operator_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::operator_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::operator_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::operator_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::operator_equal(a, b);
  }
  //=====================================================================
  // Test operator!=
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::operator_not_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::operator_not_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::operator_not_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::operator_not_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::operator_not_equal(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::operator_not_equal(a, b);
  }
  //=====================================================================
  // Test operator<
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::operator_less_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::operator_less_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::operator_less_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::operator_less_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::operator_less_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::operator_less_than(a, b);
  }
  // check for consistency with ulong behaviour
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 5ul);
    assert(a < b);
  }
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 4ul);
    assert(!(a < b));
  }
  {
    boost::dynamic_bitset<Block> a(3, 5ul), b(3, 4ul);
    assert(!(a < b));
  }
  //=====================================================================
  // Test operator<=
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::operator_less_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::operator_less_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::operator_less_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::operator_less_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::operator_less_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::operator_less_than_eq(a, b);
  }
  // check for consistency with ulong behaviour
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 5ul);
    assert(a <= b);
  }
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 4ul);
    assert(a <= b);
  }
  {
    boost::dynamic_bitset<Block> a(3, 5ul), b(3, 4ul);
    assert(!(a <= b));
  }
  //=====================================================================
  // Test operator>
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::operator_greater_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::operator_greater_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::operator_greater_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::operator_greater_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::operator_greater_than(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::operator_greater_than(a, b);
  }
  // check for consistency with ulong behaviour
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 5ul);
    assert(!(a > b));
  }
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 4ul);
    assert(!(a > b));
  }
  {
    boost::dynamic_bitset<Block> a(3, 5ul), b(3, 4ul);
    assert(a > b);
  }
  //=====================================================================
  // Test operator<=
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::operator_greater_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0")), b(std::string("0"));
    Tests::operator_greater_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("1"));
    Tests::operator_greater_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::operator_greater_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    a[long_string.size()/2].flip();
    Tests::operator_greater_than_eq(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    b[long_string.size()/2].flip();
    Tests::operator_greater_than_eq(a, b);
  }
  // check for consistency with ulong behaviour
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 5ul);
    assert(!(a >= b));
  }
  {
    boost::dynamic_bitset<Block> a(3, 4ul), b(3, 4ul);
    assert(a >= b);
  }
  {
    boost::dynamic_bitset<Block> a(3, 5ul), b(3, 4ul);
    assert(a >= b);
  }
  //=====================================================================
  // Test b.test(pos)
  { // case pos >= b.size()
    boost::dynamic_bitset<Block> b;
    Tests::test_bit(b, 0);
  }
  { // case pos < b.size()
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::test_bit(b, 0);
  }
  { // case pos == b.size() / 2
    boost::dynamic_bitset<Block> b(long_string);
    Tests::test_bit(b, long_string.size()/2);
  }
  //=====================================================================
  // Test b << pos
  { // case pos == 0
    std::size_t pos = 0;
    boost::dynamic_bitset<Block> b(std::string("1010"));
    Tests::operator_shift_left(b, pos);
  }
  { // case pos == size()/2
    std::size_t pos = long_string.size() / 2;
    boost::dynamic_bitset<Block> b(long_string);
    Tests::operator_shift_left(b, pos);
  }
  { // case pos >= n
    std::size_t pos = long_string.size();
    boost::dynamic_bitset<Block> b(long_string);
    Tests::operator_shift_left(b, pos);
  }
  //=====================================================================
  // Test b >> pos
  { // case pos == 0
    std::size_t pos = 0;
    boost::dynamic_bitset<Block> b(std::string("1010"));
    Tests::operator_shift_right(b, pos);
  }
  { // case pos == size()/2
    std::size_t pos = long_string.size() / 2;
    boost::dynamic_bitset<Block> b(long_string);
    Tests::operator_shift_right(b, pos);
  }
  { // case pos >= n
    std::size_t pos = long_string.size();
    boost::dynamic_bitset<Block> b(long_string);
    Tests::operator_shift_right(b, pos);
  }
  //=====================================================================
  // Test a & b
  {
    boost::dynamic_bitset<Block> lhs, rhs;
    Tests::operator_and(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(std::string("1")), rhs(std::string("0"));
    Tests::operator_and(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 0), rhs(long_string);
    Tests::operator_and(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 1), rhs(long_string);
    Tests::operator_and(lhs, rhs);
  }
  //=====================================================================
  // Test a | b
  {
    boost::dynamic_bitset<Block> lhs, rhs;
    Tests::operator_or(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(std::string("1")), rhs(std::string("0"));
    Tests::operator_or(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 0), rhs(long_string);
    Tests::operator_or(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 1), rhs(long_string);
    Tests::operator_or(lhs, rhs);
  }
  //=====================================================================
  // Test a^b
  {
    boost::dynamic_bitset<Block> lhs, rhs;
    Tests::operator_xor(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(std::string("1")), rhs(std::string("0"));
    Tests::operator_xor(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 0), rhs(long_string);
    Tests::operator_xor(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 1), rhs(long_string);
    Tests::operator_xor(lhs, rhs);
  }
  //=====================================================================
  // Test a-b
  {
    boost::dynamic_bitset<Block> lhs, rhs;
    Tests::operator_sub(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(std::string("1")), rhs(std::string("0"));
    Tests::operator_sub(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 0), rhs(long_string);
    Tests::operator_sub(lhs, rhs);
  }
  {
    boost::dynamic_bitset<Block> lhs(long_string.size(), 1), rhs(long_string);
    Tests::operator_sub(lhs, rhs);
  }
  //=====================================================================
  // Test stream operator<< and operator>>
  {
    boost::dynamic_bitset<Block> b;
    boost::dynamic_bitset<Block> x(b.size());
    Tests::stream_read_write(b, x);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    boost::dynamic_bitset<Block> x(b.size());
    Tests::stream_read_write(b, x);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    boost::dynamic_bitset<Block> x(b.size());
    Tests::stream_read_write(b, x);
  }
}

int
test_main(int, char*[])
{
  run_test_cases<unsigned char>();
  run_test_cases<unsigned short>();
  run_test_cases<unsigned int>();
  run_test_cases<unsigned long>();
# ifdef BOOST_HAS_LONG_LONG
  run_test_cases<unsigned long long>();
# endif

  return EXIT_SUCCESS;
}
