// (C) Copyright Jeremy Siek 2001.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.


#include "bitset_test.hpp"
#include "boost/dynamic_bitset.hpp"

#include "boost/limits.hpp"
#include "boost/config.hpp"

template <typename Tests, typename String>
void run_string_tests(const String& s
                      BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tests)
                     )
{

  const std::size_t len = s.length();
  const std::size_t step = len/4 ? len/4 : 1;

  // bitset length determined by the string-related arguments
  std::size_t i;
  for (i = 0; i <= len/2 ; /*++i*/ i += step) {
    Tests::from_string(s, i, len/2);     // len/2 - i bits
    Tests::from_string(s, i, len);       // len - i   bits
    Tests::from_string(s, i, 1 + len*2); // len - i   bits
  }

  // bitset length explicitly specified
  for (i = 0; i <= len/2; i += step) {
    for (std::size_t sz = 0; sz <= len*4; sz+= step*2) {
      Tests::from_string(s, i, len/2, sz);
      Tests::from_string(s, i, len,   sz);
      Tests::from_string(s, i, 1 + len*2, sz);

      }
  }

}

template <typename Block>
void run_test_cases( BOOST_EXPLICIT_TEMPLATE_TYPE(Block) )
{
  typedef boost::dynamic_bitset<Block> bitset_type;
  typedef bitset_test< bitset_type > Tests;
  const int bits_per_block = bitset_type::bits_per_block;

  std::string long_string = get_long_string();

  std::size_t   N,
                ul_width    = std::numeric_limits<unsigned long>::digits,
                block_width = std::numeric_limits<Block>::digits;
  unsigned long numbers[]   = { 0, 40247,
                                std::numeric_limits<unsigned long>::max() };

  //=====================================================================
  // Test construction from unsigned long
  for (std::size_t i = 0; i < 3; ++i) {
    unsigned long number = numbers[i];
    N = 0;
    Tests::from_unsigned_long(N, number);

    N = std::size_t(0.7 * double(ul_width));
    Tests::from_unsigned_long(N, number);

    N = 1 * ul_width;
    Tests::from_unsigned_long(N, number);

    N = std::size_t(1.3 * double(ul_width));
    Tests::from_unsigned_long(N, number);

    N = std::size_t(0.7 * double(block_width));
    Tests::from_unsigned_long(N, number);

    N = block_width;
    Tests::from_unsigned_long(N, number);

    N = std::size_t(1.3 * double(block_width));
    Tests::from_unsigned_long(N, number);

    N = 3 * block_width;
    Tests::from_unsigned_long(N, number);
  }
  //=====================================================================
  // Test construction from a string
  {

      run_string_tests<Tests>(std::string("")); // empty string
      run_string_tests<Tests>(std::string("1"));

      run_string_tests<Tests>(get_long_string());

# if !defined BOOST_NO_STD_WSTRING
      // I need to decide what to do for non "C" locale here. On
      // one hand I should have better tests. On the other one
      // I don't want tests for dynamic_bitset to cope with locales,
      // ctype::widen, etc. (but that's what you deserve when you
      // don't separate concerns at the library level)
      //
      run_string_tests<Tests>(
          std::wstring(L"11111000000111111111010101010101010101010111111"));
# endif

      // Note that this are _valid_ arguments
      Tests::from_string(std::string("x11y"), 1, 2);
      Tests::from_string(std::string("x11"), 1, 10);
      Tests::from_string(std::string("x11"), 1, 10, 10);

  }
  //=====================================================================
  // Test construction from a block range
  {
    std::vector<Block> blocks;
    Tests::from_block_range(blocks);
  }
  {
    std::vector<Block> blocks(3);
    blocks[0] = static_cast<Block>(0);
    blocks[1] = static_cast<Block>(1);
    blocks[2] = ~Block(0);
    Tests::from_block_range(blocks);
  }
  {
    std::vector<Block> blocks(101);
    for (typename std::vector<Block>::size_type i = 0;
         i < blocks.size(); ++i)
      blocks[i] = i;
    Tests::from_block_range(blocks);
  }
  //=====================================================================
  // Test copy constructor
  {
    boost::dynamic_bitset<Block> b;
    Tests::copy_constructor(b);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("0"));
    Tests::copy_constructor(b);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    Tests::copy_constructor(b);
  }
  //=====================================================================
  // Test assignment operator
  {
    boost::dynamic_bitset<Block> a, b;
    Tests::assignment_operator(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1")), b(std::string("0"));
    Tests::assignment_operator(a, b);
  }
  {
    boost::dynamic_bitset<Block> a(long_string), b(long_string);
    Tests::assignment_operator(a, b);
  }
  //=====================================================================
  // Test resize
  {
    boost::dynamic_bitset<Block> a;
    Tests::resize(a);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0"));
    Tests::resize(a);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1"));
    Tests::resize(a);
  }
  {
    boost::dynamic_bitset<Block> a(long_string);
    Tests::resize(a);
  }
  //=====================================================================
  // Test clear
  {
    boost::dynamic_bitset<Block> a;
    Tests::clear(a);
  }
  {
    boost::dynamic_bitset<Block> a(long_string);
    Tests::clear(a);
  }
  //=====================================================================
  // Test append bit
  {
    boost::dynamic_bitset<Block> a;
    Tests::append_bit(a);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0"));
    Tests::append_bit(a);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1"));
    Tests::append_bit(a);
  }
  {
    const int size_to_fill_all_blocks = 4 * bits_per_block;
    boost::dynamic_bitset<Block> a(size_to_fill_all_blocks, 255ul);
    Tests::append_bit(a);
  }
  {
    boost::dynamic_bitset<Block> a(long_string);
    Tests::append_bit(a);
  }
  //=====================================================================
  // Test append block
  {
    boost::dynamic_bitset<Block> a;
    Tests::append_block(a);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0"));
    Tests::append_block(a);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1"));
    Tests::append_block(a);
  }
  {
    const int size_to_fill_all_blocks = 4 * bits_per_block;
    boost::dynamic_bitset<Block> a(size_to_fill_all_blocks, 15ul);
    Tests::append_block(a);
  }
  {
    boost::dynamic_bitset<Block> a(long_string);
    Tests::append_block(a);
  }
  //=====================================================================
  // Test append block range
  {
    boost::dynamic_bitset<Block> a;
    std::vector<Block> blocks;
    Tests::append_block_range(a, blocks);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("0"));
    std::vector<Block> blocks(3);
    blocks[0] = static_cast<Block>(0);
    blocks[1] = static_cast<Block>(1);
    blocks[2] = ~Block(0);
    Tests::append_block_range(a, blocks);
  }
  {
    boost::dynamic_bitset<Block> a(std::string("1"));
    std::vector<Block> blocks(101);
    for (typename std::vector<Block>::size_type i = 0;
         i < blocks.size(); ++i)
      blocks[i] = i;
    Tests::append_block_range(a, blocks);
  }
  {
    boost::dynamic_bitset<Block> a(long_string);
    std::vector<Block> blocks(3);
    blocks[0] = static_cast<Block>(0);
    blocks[1] = static_cast<Block>(1);
    blocks[2] = ~Block(0);
    Tests::append_block_range(a, blocks);
  }
  //=====================================================================
  // Test bracket operator
  {
    boost::dynamic_bitset<Block> b1;
    std::vector<bool> bitvec1;
    Tests::operator_bracket(b1, bitvec1);
  }
  {
    boost::dynamic_bitset<Block> b(std::string("1"));
    std::vector<bool> bit_vec(1, true);
    Tests::operator_bracket(b, bit_vec);
  }
  {
    boost::dynamic_bitset<Block> b(long_string);
    std::size_t n = long_string.size();
    std::vector<bool> bit_vec(n);
    for (std::size_t i = 0; i < n; ++i)
      bit_vec[i] = long_string[n - 1 - i] == '0' ? 0 : 1;
    Tests::operator_bracket(b, bit_vec);
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
