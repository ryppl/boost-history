// (C) Copyright Chuck Allison and Jeremy Siek 2001, 2002.
//
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.

// With optimizations, bug fixes, and improvements by Gennaro Prota.

// See http://www.boost.org/libs/dynamic_bitset for documentation.

// -------------------------------------
// CHANGE LOG:
//
//  The changes up to the marking "---" line below are referred
//  to:
//       v. 1.25   of    boost/dynamic_bitset.hpp
//       v. 1.10   of    boost/detail/dynamic_bitset.hpp
//       v. 1.3    of    boost/dynamic_bitset_fwd.hpp    [no change]
//
// into the main CVS repository.

// - Major implementation change:
//
//    Everything rewritten in terms of std::vector<>:
//     note that the code is now much shorter and that the exception
//     safety guarantees of the size-changing operations (resize,
//     clear, push_back, append(Block), append(Iter, Iter)) are
//     easily identifiable in terms of the guarantees provided by
//     vector<Block>'s operations (with Block having a no-throw
//     copy constructor)
//
//     Note that dynamic_bitset::reference has been changed too:
//     a reference is now basically a reference into a Block,
//     and it remains valid if an exception is thrown by
//     a strong-guarantee operation.

//
// - [... see CVS logs ...]

// - several comment fixes and additions (others are needed though)
// - changed operator>() implementation
// - included istream and ostream instead of iosfwd
//     (we use sentry, setstate, rdbuf, etc. so iosfwd isn't enough)
//
// - boost/config.hpp now included after most std headers.
// - several workarounds updated and BOOST_WORKAROUND() macro used
// - small changes to count()
// - object_representation() added to boost::detail
// - inserter and extractor for iostreams rewritten
// - manipulator to extract "infinite" bits added
// - added again #include<vector>, for now.
// - Changes related to initial_num_blocks:
//      - all overloads made inline
//      - added overload for random access iterators
//      - removed superfluous declarations (fwd refs)
//
//  - to_string and dump_to_string reimplemented
//
//  - from_string reimplemented

//  Minor:
//
//  - added local workaround for boost/config/stdlib/stlport.hpp bug
//    I hope this is a temporary one.
//  - set_(size_type bit, bool val) made void
//  - added parens in calc_num_blocks()
//
// -------------------------------------------------------------

// - corrected workaround for Dinkum lib's allocate() [GP]
// - changed macro test for old iostreams [GP]
// - removed #include <vector> for now. [JGS]
// - Added __GNUC__ to compilers that cannot handle the constructor from basic_string. [JGS]
// - corrected to_block_range [GP]
// - corrected from_block_range [GP]
// - Removed __GNUC__ from compilers that cannot handle the constructor
//     from basic_string and added the workaround suggested by GP. [JGS]
// - Removed __BORLANDC__ from the #if around the basic_string
//     constructor. Luckily the fix by GP for g++ also fixes Borland. [JGS]
//

#ifndef BOOST_DYNAMIC_BITSET_HPP
#define BOOST_DYNAMIC_BITSET_HPP

#include <cassert>
#include <string>
#include <stdexcept>           // for std::overflow_error
#include <algorithm>           // for std::swap, std::min, std::copy, std::fill
#include <vector>

#include "boost/config.hpp"

#ifndef BOOST_NO_STD_LOCALE
# include <locale> // G.P.S
#endif


//  The purpose of this BOOST_OLD_IOSTREAMS macro is basically to support
//  pre-standard implementations of the g++ library (the best place for such
//  a test would be in the boost config files, but there isn't probably any
//  intent for a general old iostreams support). The test here exploits the
//  fact that old (in fact, pre 3.0) versions of libstdc++ are based on the
//  old sgi [Thanks to Phil Edwards for useful info about libstdc++ history]
//
#if defined (__STL_CONFIG_H) && !defined (__STL_USE_NEW_IOSTREAMS)
#  define BOOST_OLD_IOSTREAMS
#  include <iostream.h>
#  include <ctype.h> // for isspace
#else
#  include <istream>
#  include <ostream>
#endif

#include "boost/dynamic_bitset_fwd.hpp" //G.P.S.
#include "boost/detail/dynamic_bitset.hpp"
#include "boost/limits.hpp" // [gps] now included here
#include "boost/lowest_bit.hpp" // used by find_first/next



#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)                   // 1300 == VC++ 7.0
  //  in certain situations VC++ requires a redefinition of
  //  default template arguments, in contrast with 14.1/12
# define BOOST_WORKAROUND_REPEAT_DEFAULT_TEMPLATE_ARGUMENTS // local macro
#endif


// Helps getting a '0' or '1' character.
//
#if defined (BOOST_USE_FACET)
# define BOOST_BITSET_CHAR(type, c) \
           BOOST_USE_FACET(std::ctype<type>, std::locale()).widen(c)
#else
# define BOOST_BITSET_CHAR(type, c)  c
#endif




namespace boost {

template

# ifdef BOOST_WORKAROUND_REPEAT_DEFAULT_TEMPLATE_ARGUMENTS
   <typename Block = unsigned long, typename Allocator = std::allocator<Block> >
# else
   <typename Block, typename Allocator>
# endif

class dynamic_bitset
{
  // Portability note: member function templates are defined inside
  // this class definition to avoid problems with VC++. Similarly,
  // with the member functions of the nested class.


public:
    typedef Block block_type;
    typedef Allocator allocator_type; // [gps] to be documented
    typedef std::size_t size_type;
    BOOST_STATIC_CONSTANT(int, bits_per_block = (std::numeric_limits<Block>::digits));
    BOOST_STATIC_CONSTANT(size_type, npos = -1);


public:

    // A proxy class to simulate lvalues of bit type.
    // Shouldn't it be private? [gps]
    //
    class reference
    {
        friend class dynamic_bitset<Block, Allocator>;


        // the one and only non-copy ctor
        reference(block_type & b, int pos)
            :m_block(b), m_mask(block_type(1) << pos)
        {}

        void operator&(); // not defined

    public:

        // copy constructor: compiler generated

        operator bool() const { return (m_block & m_mask) != 0; }
        bool operator~() const { return (m_block & m_mask) == 0; }

        reference& flip() { do_flip(); return *this; }

        reference& operator=(bool x)               { do_assign(x);   return *this; } // for b[i] = x
        reference& operator=(const reference& rhs) { do_assign(rhs); return *this; } // for b[i] = b[j]

        reference& operator|=(bool x) { if  (x) do_set();   return *this; }
        reference& operator&=(bool x) { if (!x) do_reset(); return *this; }
        reference& operator^=(bool x) { if  (x) do_flip();  return *this; }
        reference& operator-=(bool x) { if  (x) do_reset(); return *this; }

     private:
        block_type & m_block;
        const block_type m_mask;

        void do_set() { m_block |= m_mask; }
        void do_reset() { m_block &= ~m_mask; }
        void do_flip() { m_block ^= m_mask; }
        void do_assign(bool x) { x? do_set() : do_reset(); }
    };

    typedef bool const_reference;

    // constructors, etc.
    explicit
    dynamic_bitset(const Allocator& alloc = Allocator());

    explicit
    dynamic_bitset(size_type num_bits, unsigned long value = 0,
               const Allocator& alloc = Allocator());


    // The presence of this constructor is a concession to ease of
    // use, especially for the novice user. Strictly speaking, it
    // remains a design error: a conversion from string is, for
    // most types, formatting, and should be done by the standard
    // formatting convention: operator>>.
    //
    // That way you have a neat separation of concerns between the class
    // that stores the data (dynamic_bitset in this case) and the one that
    // stores their textual representation (basic_string), a generic name,
    // operator>>, for the free function that connects them (which is
    // important for template programming) and none of the two classes
    // encumbered with knowledge of the other. Also, you can easily deal
    // with locale-related issues.
    //
    // NOTE:
    // The parentheses around std::basic_string<CharT, Traits, Alloc>::npos
    // in the code below are to avoid a g++ 3.2 bug and a Borland bug. -JGS
    template <typename CharT, typename Traits, typename Alloc>
    explicit
    dynamic_bitset(const std::basic_string<CharT, Traits, Alloc>& s,
        typename std::basic_string<CharT, Traits, Alloc>::size_type pos = 0,
        typename std::basic_string<CharT, Traits, Alloc>::size_type n   =
                   (std::basic_string<CharT, Traits, Alloc>::npos),
        size_type num_bits = npos,
        const Allocator& alloc = Allocator())

    :m_bits(alloc),
     m_num_bits(0)
    {
        assert(pos <= s.size());

        typedef typename std::basic_string<CharT, Traits, Alloc> StrT;
        typedef typename StrT::traits_type Tr;

        const typename StrT::size_type rlen = std::min(n, s.size() - pos);
        const size_type sz = ( num_bits != npos? num_bits : rlen);
        m_bits.resize(calc_num_blocks(sz));
        m_num_bits = sz;


        const size_type m = num_bits < rlen ? num_bits : rlen; // [gps]
        const CharT one = BOOST_BITSET_CHAR(CharT, '1');

        typename StrT::size_type i = 0;
        for( ; i < m; ++i) {

            const CharT c = s[(pos + m - 1) - i];

            assert( Tr::eq(c, one)
                    || Tr::eq(c, BOOST_BITSET_CHAR(CharT, '0')) );

            if (Tr::eq(c, one))
                set(i);

        }

    }

    // The first bit in *first is the least significant bit, and the
    // last bit in the block just before *last is the most significant bit.
    template <typename BlockInputIterator>
    dynamic_bitset(BlockInputIterator first, BlockInputIterator last,
                   const Allocator& alloc = Allocator())

    :m_bits(first, last, alloc),
     m_num_bits(m_bits.size() * bits_per_block)
    {}


    // copy constructor
    dynamic_bitset(const dynamic_bitset& b);

    void swap(dynamic_bitset& b);

    dynamic_bitset& operator=(const dynamic_bitset& b);

    // size changing operations
    void resize(size_type num_bits, bool value = false);
    void clear();
    void push_back(bool bit);
    void append(Block block);

    template <typename BlockInputIterator>
    void append(BlockInputIterator first, BlockInputIterator last) // basic guarantee (for now?)
    {
        // to be optimized? [gps]
        for ( ; first != last; ++first)
            append(*first);
    }


    // bitset operations
    dynamic_bitset& operator&=(const dynamic_bitset& b);
    dynamic_bitset& operator|=(const dynamic_bitset& b);
    dynamic_bitset& operator^=(const dynamic_bitset& b);
    dynamic_bitset& operator-=(const dynamic_bitset& b);
    dynamic_bitset& operator<<=(size_type n);
    dynamic_bitset& operator>>=(size_type n);
    dynamic_bitset operator<<(size_type n) const;
    dynamic_bitset operator>>(size_type n) const;

    // basic bit operations
    dynamic_bitset& set(size_type n, bool val = true);
    dynamic_bitset& set();
    dynamic_bitset& reset(size_type n);
    dynamic_bitset& reset();
    dynamic_bitset& flip(size_type n);
    dynamic_bitset& flip();
    bool test(size_type n) const;
    bool any() const;
    bool none() const;
    dynamic_bitset operator~() const;
    size_type count() const;

    // subscript
    reference operator[](size_type pos) {
        return reference(this->m_bits[this->block_index(pos)], this->bit_index(pos));
    }
    bool operator[](size_type pos) const { return test(pos); }

    unsigned long to_ulong() const;

    size_type size() const;
    size_type num_blocks() const;

    bool is_subset_of(const dynamic_bitset& a) const;
    bool is_proper_subset_of(const dynamic_bitset& a) const;

    // lookup
    size_type find_first() const;
    size_type find_next(size_type pos) const;


#ifdef BOOST_DYN_BITSET_USE_FRIENDS
    // lexicographical comparison
    template <typename B, typename A>
    friend bool operator==(const dynamic_bitset<B, A>& a,
                           const dynamic_bitset<B, A>& b);

    template <typename B, typename A>
    friend bool operator<(const dynamic_bitset<B, A>& a,
                          const dynamic_bitset<B, A>& b);


    template <typename B, typename A, typename BlockOutputIterator>
    friend void to_block_range(const dynamic_bitset<B, A>& b,
                               BlockOutputIterator result);

    template <typename BlockIterator, typename B, typename A>
    friend void from_block_range(BlockIterator first, BlockIterator last,
                                 dynamic_bitset<B, A>& result);


    template <typename CharT, typename Traits, typename B, typename A>
    friend std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is,
                                                         dynamic_bitset<B, A>& b);

    template <typename B, typename A, typename stringT>
    friend void to_string_helper (const dynamic_bitset<B, A> & b, stringT & s, std::size_t len);


#endif


private:
    BOOST_STATIC_CONSTANT(int, ulong_width = std::numeric_limits<unsigned long>::digits);
    typedef std::vector<block_type, allocator_type> buffer_type;

    void m_zero_unused_bits();
    size_type m_do_find_from(size_type first_block) const;

    static size_type block_index(size_type pos) { return pos / bits_per_block; }
    static int bit_index(size_type pos) { return pos % bits_per_block; }
    static Block bit_mask(size_type pos) { return Block(1) << bit_index(pos); }



BOOST_DYN_BITSET_PRIVATE:

    void m_rearrange_extracted_bits(size_type num_bits);
    static size_type calc_num_blocks(size_type num_bits)
        { return (num_bits + (bits_per_block - 1)) / bits_per_block; }
    Block& m_highest_block();


    buffer_type m_bits; // [gps] to be renamed
    size_type   m_num_bits;

};

// Global Functions:

// comparison
template <typename Block, typename Allocator>
bool operator!=(const dynamic_bitset<Block, Allocator>& a,
                const dynamic_bitset<Block, Allocator>& b);

template <typename Block, typename Allocator>
bool operator<=(const dynamic_bitset<Block, Allocator>& a,
                const dynamic_bitset<Block, Allocator>& b);

template <typename Block, typename Allocator>
bool operator>(const dynamic_bitset<Block, Allocator>& a,
               const dynamic_bitset<Block, Allocator>& b);

template <typename Block, typename Allocator>
bool operator>=(const dynamic_bitset<Block, Allocator>& a,
                const dynamic_bitset<Block, Allocator>& b);

// stream operators
#ifdef BOOST_OLD_IOSTREAMS
template <typename Block, typename Allocator>
std::ostream& operator<<(std::ostream& os,
                         const dynamic_bitset<Block, Allocator>& b);

template <typename Block, typename Allocator>
std::istream& operator>>(std::istream& is, dynamic_bitset<Block,Allocator>& b);
#else
template <typename CharT, typename Traits, typename Block, typename Allocator>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const dynamic_bitset<Block, Allocator>& b);

template <typename CharT, typename Traits, typename Block, typename Allocator>
std::basic_istream<CharT, Traits>&
operator>>(std::basic_istream<CharT, Traits>& is,
           dynamic_bitset<Block, Allocator>& b);
#endif

// bitset operations
template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator&(const dynamic_bitset<Block, Allocator>& b1,
          const dynamic_bitset<Block, Allocator>& b2);

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator|(const dynamic_bitset<Block, Allocator>& b1,
          const dynamic_bitset<Block, Allocator>& b2);

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator^(const dynamic_bitset<Block, Allocator>& b1,
          const dynamic_bitset<Block, Allocator>& b2);

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator-(const dynamic_bitset<Block, Allocator>& b1,
          const dynamic_bitset<Block, Allocator>& b2);


template <typename Block, typename Allocator, typename stringT>
void
to_string(const dynamic_bitset<Block, Allocator>& b, stringT & s); // G.P.S.

template <typename Block, typename Allocator, typename BlockOutputIterator>
void
to_block_range(const dynamic_bitset<Block, Allocator>& b,
               BlockOutputIterator result);


// G.P.S. check docs with Jeremy
//
template <typename BlockIterator, typename B, typename A>
inline void
from_block_range(BlockIterator first, BlockIterator last,
                 dynamic_bitset<B, A>& result)
{
    // PRE: distance(first, last) == numblocks()
    std::copy (first, last, result.m_bits.begin()); //[gps]
}

//=============================================================================
// dynamic_bitset implementation

#ifdef BOOST_OLD_IOSTREAMS
template <typename Block, typename Allocator>
inline std::ostream&
operator<<(std::ostream& os,
           const typename dynamic_bitset<Block, Allocator>::reference& br)
{
    return os << (bool)br;
}
#else
template <typename CharT, typename Traits, typename Block, typename Allocator>
inline std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const typename dynamic_bitset<Block, Allocator>::reference& br)
{
    return os << (bool)br;
}
#endif

//-----------------------------------------------------------------------------
// constructors, etc.

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>::dynamic_bitset(const Allocator& alloc)
  : m_bits(alloc), m_num_bits(0)
{

}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>::
dynamic_bitset(size_type num_bits, unsigned long value, const Allocator& alloc)
  : m_bits(calc_num_blocks(num_bits), Block(0), alloc),
    m_num_bits(num_bits)
{

  if (num_bits == 0)
      return;

  typedef unsigned long num_type;

  // cut off all bits in value that have pos >= num_bits, if any
  if (num_bits < ulong_width) {
      const num_type mask = (num_type(1) << num_bits) - 1;
      value &= mask;
  }

  if (bits_per_block >= ulong_width) {
      m_bits[0] = static_cast<block_type>(value);
  }
  else {
      for(size_type i = 0; value != 0; ++i) {

          m_bits[i] = static_cast<block_type>(value);
          value >>= bits_per_block;
      }
  }

}

// copy constructor
template <typename Block, typename Allocator>
inline dynamic_bitset<Block, Allocator>::
dynamic_bitset(const dynamic_bitset& b)
  : m_bits(b.m_bits), m_num_bits(b.m_num_bits)  // [gps]
{

}

template <typename Block, typename Allocator>
inline void dynamic_bitset<Block, Allocator>::
swap(dynamic_bitset<Block, Allocator>& b)
{
    std::swap(m_bits, b.m_bits);
    std::swap(m_num_bits, b.m_num_bits);
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>& dynamic_bitset<Block, Allocator>::
operator=(const dynamic_bitset<Block, Allocator>& b)
{
    dynamic_bitset<Block, Allocator> tmp(b);
    this->swap(tmp);
    return *this;
}

//-----------------------------------------------------------------------------
// size changing operations

template <typename Block, typename Allocator>
void dynamic_bitset<Block, Allocator>::
resize(size_type num_bits, bool value) // strong guarantee
{

  const size_type old_num_blocks = num_blocks();
  const size_type required_blocks = calc_num_blocks(num_bits);

  const block_type v = value? ~Block(0) : Block(0);

  if (required_blocks != old_num_blocks) {
    m_bits.resize(required_blocks, v); // s.g. (copy) [gps]
  }


  // At this point:
  //
  //  - if the buffer was shrunk, there's nothing to do, except
  //    a call to m_zero_unused_bits()
  //
  //  - if it it is enlarged, all the (used) bits in the new blocks have
  //    the correct value, but we should also take care of the bits,
  //    if any, that were 'unused bits' before enlarging: if value == true,
  //    they must be set.

  if (value && (num_bits > m_num_bits)) {

    const size_type extra_bits = m_num_bits % bits_per_block; // G.P.S.
    if (extra_bits) {
        assert(old_num_blocks >= 1 && old_num_blocks <= m_bits.size());

        // Set them.
        m_bits[old_num_blocks - 1] |= (v << extra_bits); // G.P.S.
    }

  }



  m_num_bits = num_bits;
  m_zero_unused_bits();

}

template <typename Block, typename Allocator>
void dynamic_bitset<Block, Allocator>::
clear() // no throw
{
  m_bits.clear();
  m_num_bits = 0;
}


template <typename Block, typename Allocator>
void dynamic_bitset<Block, Allocator>::
push_back(bool bit)
{
  this->resize(this->size() + 1);
  set(this->size() - 1, bit);
}

template <typename Block, typename Allocator>
void dynamic_bitset<Block, Allocator>::
append(Block value) // strong guarantee
{

    // G.P.S. to be reviewed...

     // if != 0 this is the number of bits used in the last block
    const size_type excess_bits = this->m_num_bits % bits_per_block;

    if (excess_bits == 0) {
        // the buffer was empty, or blocks are all filled
        m_bits.push_back(value);
    }
    else {
        m_highest_block() |= (value << excess_bits);
        m_bits.push_back(value >> (bits_per_block - excess_bits));
    }

    m_num_bits += bits_per_block;

}


//-----------------------------------------------------------------------------
// bitset operations
template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::operator&=(const dynamic_bitset& rhs)
{
    assert(size() == rhs.size());
    for (size_type i = 0; i < num_blocks(); ++i)
        this->m_bits[i] &= rhs.m_bits[i];
    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::operator|=(const dynamic_bitset& rhs)
{
    assert(size() == rhs.size());
    for (size_type i = 0; i < num_blocks(); ++i)
        this->m_bits[i] |= rhs.m_bits[i];
    //m_zero_unused_bits();
    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::operator^=(const dynamic_bitset& rhs)
{
    assert(size() == rhs.size());
    for (size_type i = 0; i < this->num_blocks(); ++i)
        this->m_bits[i] ^= rhs.m_bits[i];
    //m_zero_unused_bits();
    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::operator-=(const dynamic_bitset& rhs)
{
    assert(size() == rhs.size());
    for (size_type i = 0; i < this->num_blocks(); ++i)
        this->m_bits[i] &= ~rhs.m_bits[i];
    //m_zero_unused_bits();
    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::operator<<=(size_type n)
{
    if (n >= this->m_num_bits)
        return reset();
    //else
    if (n > 0)
    {
        size_type  const last = this->num_blocks() - 1; // num_blocks() is >= 1
        size_type  const div  = n / bits_per_block;   // div is <= last
        size_type  const r    = n % bits_per_block;
        block_type * b        = &m_bits[0];

        // PRE: div != 0  or  r != 0

        if (r != 0) {

            block_type const rs = bits_per_block - r;

            for (size_type i = last-div; i>0; --i) {
                b[i+div] = (b[i] << r) | (b[i-1] >> rs);
            }
            b[div] = b[0] << r;

        }
        else {
            for (size_type i = last-div; i>0; --i) {
                b[i+div] = b[i];
            }
            b[div] = b[0];
        }


        // div blocks are zero filled at the less significant end
        std::fill_n(b, div, static_cast<block_type>(0));


    }

    return *this;


}


//
// NOTE:
//
//      this function (as well as operator <<=) assumes that
//      within each block bits are arranged 'from right to left'.
//
//             static int bit_index(size_type pos)
//             { return pos % bits_per_block; }
//
//  Note also the 'if (r != 0)' in the implementation below:
//  besides being an optimization, it avoids the undefined behavior that
//  could result, when r==0, from shifting a block by ls = bits_per_block
//  bits (we says 'could' because actually, depending on its type, the left
//  operand of << or >> can be promoted to a type with more than bits_per_block
//  value-bits; but the if-else ensures we have well-defined behavior regardless
//  of integral promotions) - G.P.S.
//
template <typename B, typename A>
dynamic_bitset<B, A> & dynamic_bitset<B, A>::operator>>=(size_type n) {
    if (n >= this->m_num_bits) {
        return reset();
    }
    //else
    if (n>0){

        size_type  const last  = this->num_blocks() - 1; // num_blocks() is >= 1
        size_type  const div   = n / bits_per_block;   // div is <= last
        size_type  const r     = n % bits_per_block;
        block_type * b         = &m_bits[0];

        // PRE: div != 0  or  r != 0

        if (r != 0) {

            block_type const ls = bits_per_block - r;

            for (size_type i = div; i < last; ++i) {
                b[i-div] = (b[i] >> r) | (b[i+1]  << ls);
            }
            // r bits go to zero
            b[last-div] = b[last] >> r;
        }

        else {
            for (size_type i = div; i <= last; ++i) {
                b[i-div] = b[i];
            }
            // note the '<=': the last iteration 'absorbs'
            // b[last-div] = b[last] >> 0;
        }



        // div blocks are zero filled at the most significant end
        std::fill_n(b + (this->num_blocks()-div), div, static_cast<block_type>(0));
    }

    return *this;
}







template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
dynamic_bitset<Block, Allocator>::operator<<(size_type n) const
{
    dynamic_bitset r(*this);
    return r <<= n;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
dynamic_bitset<Block, Allocator>::operator>>(size_type n) const
{
    dynamic_bitset r(*this);
    return r >>= n;
}


//-----------------------------------------------------------------------------
// basic bit operations

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::set(size_type pos, bool val)
{
    // [gps]
    //
    // Below we have no set(size_type) function to call when
    // value == true; instead of using a helper, I think
    // overloading set (rather than giving it a default bool
    // argument) would be more elegant.

    assert(pos < this->m_num_bits);

    if (val)
        this->m_bits[this->block_index(pos)] |= this->bit_mask(pos);
    else
        reset(pos);

    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::set()
{
  std::fill(m_bits.begin(), m_bits.end(), ~Block(0));
  m_zero_unused_bits();
  return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::reset(size_type pos)
{
    assert(pos < this->m_num_bits);
    this->m_bits[this->block_index(pos)] &= ~this->bit_mask(pos);
    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::reset()
{
  std::fill(m_bits.begin(), m_bits.end(), Block(0));
  return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::flip(size_type pos)
{
    assert(pos < this->m_num_bits);
    this->m_bits[this->block_index(pos)] ^= this->bit_mask(pos);
    return *this;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>&
dynamic_bitset<Block, Allocator>::flip()
{
    for (size_type i = 0; i < this->num_blocks(); ++i)
        this->m_bits[i] = ~this->m_bits[i];
    m_zero_unused_bits();
    return *this;
}

template <typename Block, typename Allocator>
bool dynamic_bitset<Block, Allocator>::test(size_type pos) const
{
    assert(pos < this->m_num_bits);
    return (this->m_bits[this->block_index(pos)] & this->bit_mask(pos)) != 0;
}

template <typename Block, typename Allocator>
bool dynamic_bitset<Block, Allocator>::any() const
{
    for (size_type i = 0; i < this->num_blocks(); ++i)
        if (this->m_bits[i])
            return 1;
    return 0;
}

template <typename Block, typename Allocator>
inline bool dynamic_bitset<Block, Allocator>::none() const
{
    return !any();
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
dynamic_bitset<Block, Allocator>::operator~() const
{
    dynamic_bitset b(*this);
    b.flip();
    return b;
}


/*

The following is the straightforward implementation of count(), which
we leave here in a comment for documentation purposes.

template <typename Block, typename Allocator>
typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::count() const
{
    size_type sum = 0;
    for (size_type i = 0; i != this->m_num_bits; ++i)
        if (test(i))
            ++sum;
    return sum;
}

The actual algorithm uses a lookup table.


  The basic idea of the method is to pick up X bits at a time
  from the internal array of blocks and consider those bits as
  the binary representation of a number N. Then, to use a table
  of 1<<X elements where table[N] is the number of '1' digits
  in the binary representation of N (i.e. in our X bits).


  In this implementation X is 8 (but can be easily changed: you
  just have to modify the definition of table_width and shrink/enlarge
  the table accordingly - it could be useful, for instance, to expand
  the table to 512 elements on an implementation with 9-bit bytes) and
  the internal array of blocks is seen, if possible, as an array of bytes.
  In practice the "reinterpretation" as array of bytes is possible if and
  only if X >= CHAR_BIT and Block has no padding bits (that would be counted
  together with the "real ones" if we saw the array as array of bytes).
  Otherwise we simply 'extract' X bits at a time from each Block.

*/


template <typename Block, typename Allocator>
typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::count() const
{
    using namespace detail::dynamic_bitset_count_impl;

    const bool no_padding = bits_per_block == CHAR_BIT * sizeof(Block);
    const mode m = table_width >= CHAR_BIT && no_padding? access_by_bytes : access_by_blocks;

    return do_count(m_bits.begin(), this->num_blocks(), Block(0), (mode_to_type<m>*) 0 );

}


//-----------------------------------------------------------------------------
// conversions


template <typename B, typename A, typename stringT>
void to_string_helper (const dynamic_bitset<B, A> & b, stringT & s, std::size_t len)
{
    typedef typename stringT::traits_type Tr;
    typedef typename stringT::value_type  Ch;

    const Ch zero = BOOST_BITSET_CHAR(Ch, '0');
    const Ch one  = BOOST_BITSET_CHAR(Ch, '1');

    // Note that this function is implemented through operator[] and
    // uses only the public interface. Anyhow, this is a deceit because
    // it may call operator[] itself with an index >= b.size() and so
    // relies on an implementation detail of dynamic_bitset.
    // To highlight this, the dependency on the "internals" is made
    // explicit by granting it friendship.
    //
    assert (b.size() <= len /*&& len <= [G.P.S.]*/ );
    s.assign (len, zero);
    for (std::size_t i = 0; i < len; ++i) {
        if (b[i]) // G.P.S. da decidere con Jeremy
            Tr::assign(s[len - 1 - i], one);

    }

}



// A comment similar to the one about the constructor from
// basic_string can be done here. Thanks to James Kanze for
// making me (Gennaro) realize this and many other things
// about internationalization.
//
template <typename Block, typename Allocator, typename stringT> // G.P.S.
inline void
to_string(const dynamic_bitset<Block, Allocator>& b, stringT& s)
{
    to_string_helper (b, s, b.size());
}


// Differently from to_string this function dumps out
// every bit of the internal representation (useful
// for debugging purposes)
//
template <typename B, typename A, typename stringT>
inline void
dump_to_string(const dynamic_bitset<B, A>& b,
               /*std::basic_string<CharT, Alloc>*/stringT& s) // G.P.S.
{
    to_string_helper(b, s, b.m_num_blocks * (dynamic_bitset<B, A>::bits_per_block) );
}

template <typename Block, typename Allocator, typename BlockOutputIterator>
inline void
to_block_range(const dynamic_bitset<Block, Allocator>& b,
               BlockOutputIterator result)
{
    assert (b.m_bits != 0 || b.m_num_blocks == 0);
    std::copy (b.m_bits, b.m_bits + b.m_num_blocks, result); // [gps]
}

template <typename Block, typename Allocator>
unsigned long dynamic_bitset<Block, Allocator>::
to_ulong() const
{

  if (m_num_bits == 0)
      return 0; // convention

  // Check for overflows. This may be a performance burden on very
  // large bitsets but is required by the specification, sorry
  if (find_next(ulong_width - 1) != npos)
    throw std::overflow_error("boost::dynamic_bitset::to_ulong overflow");


  // Ok, from now on we can be sure there's no "on" bit beyond
  // the allowed positions

  if (bits_per_block >= ulong_width)
      return m_bits[0];


  size_type last_block = block_index(std::min(m_num_bits-1,
                                    (size_type)(ulong_width-1)));
  unsigned long result = 0;
  for (size_type i = 0; i <= last_block; ++i) {

    assert(bits_per_block * i < ulong_width);

    unsigned long piece = m_bits[i];
    result |= (piece << (bits_per_block * i));
  }

  return result;

}


template <typename Block, typename Allocator>
inline typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::size() const
{
    return this->m_num_bits;
}

template <typename Block, typename Allocator>
inline typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::num_blocks() const
{
    return m_bits.size();
}

template <typename Block, typename Allocator>
bool dynamic_bitset<Block, Allocator>::
is_subset_of(const dynamic_bitset<Block, Allocator>& a) const
{
    assert(this->size() == a.size());
    for (size_type i = 0; i < this->num_blocks(); ++i)
        if (this->m_bits[i] & ~a.m_bits[i])
            return false;
    return true;
}

template <typename Block, typename Allocator>
bool dynamic_bitset<Block, Allocator>::
is_proper_subset_of(const dynamic_bitset<Block, Allocator>& a) const
{
    assert(this->size() == a.size());
    bool proper = false;
    for (size_type i = 0; i < this->num_blocks(); ++i) {
        Block bt = this->m_bits[i], ba = a.m_bits[i];
        if (ba & ~bt)
            proper = true;
        if (bt & ~ba)
            return false;
    }
    return proper;
}

// --------------------------------
// lookup


// look for the first bit "on", starting
// from the block with index first_block
//
template <typename Block, typename Allocator>
typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::m_do_find_from(size_type first_block) const
{
    size_type i = first_block;

    // skip null blocks
    while (i < this->num_blocks() && this->m_bits[i] == 0)
        ++i;

    if (i>=this->num_blocks())
        return npos; // not found

    return i * bits_per_block + boost::lowest_bit(this->m_bits[i]);

}



template <typename Block, typename Allocator>
typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::find_first() const
{
    return m_do_find_from(0);

}



template <typename Block, typename Allocator>
typename dynamic_bitset<Block, Allocator>::size_type
dynamic_bitset<Block, Allocator>::find_next(size_type pos) const
{

    const size_type sz = size();
    if (pos >= (sz-1) || sz == 0)
        return npos;

    ++pos;

    const size_type blk = this->block_index(pos);
    const int       ind = this->bit_index(pos);

    // mask out bits before pos
    const Block fore = this->m_bits[blk] & ( ~Block(0) << ind );

    return fore?
        blk * bits_per_block + lowest_bit(fore)
        :
        m_do_find_from(blk + 1);

}



//-----------------------------------------------------------------------------
// comparison

template <typename Block, typename Allocator>
bool operator==(const dynamic_bitset<Block, Allocator>& a,
                const dynamic_bitset<Block, Allocator>& b)
{
    return (a.m_num_bits == b.m_num_bits)
           && (a.m_bits == b.m_bits); // [gps]
}

template <typename Block, typename Allocator>
inline bool operator!=(const dynamic_bitset<Block, Allocator>& a,
                       const dynamic_bitset<Block, Allocator>& b)
{
    return !(a == b);
}

template <typename Block, typename Allocator>
bool operator<(const dynamic_bitset<Block, Allocator>& a,
               const dynamic_bitset<Block, Allocator>& b)
{
    assert(a.size() == b.size());
    typedef typename dynamic_bitset<Block, Allocator>::size_type size_type;

    if (a.size() == 0)
      return false;

    // Since we are storing the most significant bit
    // at pos == size() - 1, we need to do the comparisons in reverse.

    // Compare a block at a time
    for (size_type i = a.num_blocks() - 1; i > 0; --i)
      if (a.m_bits[i] < b.m_bits[i])
        return true;
      else if (a.m_bits[i] > b.m_bits[i])
        return false;

    if (a.m_bits[0] < b.m_bits[0])
      return true;
    else
      return false;
}

template <typename Block, typename Allocator>
inline bool operator<=(const dynamic_bitset<Block, Allocator>& a,
                       const dynamic_bitset<Block, Allocator>& b)
{
    return !(a > b);
}

template <typename Block, typename Allocator>
inline bool operator>(const dynamic_bitset<Block, Allocator>& a,
                      const dynamic_bitset<Block, Allocator>& b)
{
    return b < a;
}

template <typename Block, typename Allocator>
inline bool operator>=(const dynamic_bitset<Block, Allocator>& a,
                       const dynamic_bitset<Block, Allocator>& b)
{
    return !(a < b);
}

//-----------------------------------------------------------------------------
// stream operations

#ifdef BOOST_OLD_IOSTREAMS
template < typename Block, typename Allocator>
std::ostream&
operator<<(std::ostream& os, const dynamic_bitset<Block, Allocator>& b)
{
    std::string s;
    to_string(b, s);
    os << s.c_str(); // To Jeremy: should we support bs_limit() for old iostreams too? - G.P.S.
    return os;
}
#else

// Two words on the implementation: we use the stream buffer directly here
// so we have to deal with some details that are normally handled by the
// stream. In particular, with the standard format parameters (27.4.2.1.2, table 83).
// Fortunately, only a few of them have or may have a meaning for dynamic_bitset
// Also, exception handling needs a bit of care because setting state flags is
// itself an operation that may throw exceptions.
//
template <typename CharT, typename Traits, typename Block, typename Allocator>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const dynamic_bitset<Block, Allocator>& b)
{
#if 0 // old - excluded
    std::basic_string<CharT, Traits> s;
    to_string(b, s);
    os << s;
    return os;
#endif

    using namespace std;

    CharT const zero = os.widen('0');
    CharT const one  = os.widen('1');

    ios_base::iostate err = ios_base::goodbit; // G.P.S.
    typename basic_ostream<CharT, Traits>::sentry oprefix(os);
    if (oprefix) {

        try {


            typedef typename dynamic_bitset<Block, Allocator>::size_type bitsetsize_type;
            typedef basic_streambuf<CharT, Traits> buffer_type; // G.P.S.

            buffer_type * buf = os.rdbuf();
            size_t npad = os.width() <= 0  // careful: os.width() is signed (and can be < 0)
                || (bitsetsize_type) os.width() <= b.size()? 0 : os.width() - b.size(); //- G.P.S.

            // eventually fill at left; pad is decresed along the way
            bool const pad_left = (os.flags() & ios_base::adjustfield) == ios_base::left;
            if (npad && !pad_left) { // fills at left
                for (; err == ios_base::goodbit && 0 < npad; --npad)
                    if (Traits::eq_int_type(Traits::eof(),
                        buf->sputc(os.fill())))
                          err |= ios_base::failbit;   // G.P.S.
            }

            // output the bitset
            for (bitsetsize_type i = b.size(); i>0; --i) {// G.P.S.
                typename buffer_type::int_type ret = buf->sputc(b.test(i-1)? one : zero);
                if (Traits::eq_int_type(Traits::eof(), ret) ) { // - G.P.S.
                    err |= ios_base::failbit;
                    break;
                }
            }

            // in case fill at right
            for (; err == ios_base::goodbit && 0 < npad; --npad)
                if (Traits::eq_int_type(Traits::eof(),
                    buf->sputc(os.fill())))
                      err |= ios_base::failbit;


            os.width(0);

        } catch (...) {
            // note that if setstate() throws we rethrow the
            // original exception coming from the stream buffer
            //
            bool rethrow_original = false;
            try { os.setstate(ios_base::failbit); } catch (...) { rethrow_original = true; }

            if (rethrow_original)
                throw;
        }
    }

    // set the error bits: note that this may throw
    if(err != ios_base::goodbit)
        os.setstate(err);
    return os;

}
#endif


#ifdef BOOST_OLD_IOSTREAMS
template <typename Block, typename Allocator>
std::istream&
operator>>(std::istream& is, dynamic_bitset<Block, Allocator>& b)
{
    typedef char CharT;
    std::string buf;
    typedef typename std::string::size_type size_type;
    const size_type N = b.size();
    buf.reserve(N);

    // skip whitespace
    if (is.flags() & std::ios::skipws) {
        char c;
        do {
            is.get(c);
        }
        while (is && isspace(c));

        if (is)
            is.putback(c);
    }

    size_type i;
    for (i = 0; i < N; ++i)
    {
        CharT c;
        is.get(c);
        if (c == '0' || c == '1')
            buf += c;
        else
        {
            is.putback(c);
            break;
        }
    }

    if (i == 0)
        is.clear(is.rdstate() | std::ios::failbit);
    else
    {
        dynamic_bitset<Block, Allocator> tmp(buf);
        b.swap(tmp);
    }
    return is;
}
#else // BOOST_OLD_IOSTREAMS


// Helper for stream extraction: since dynamic_bitset offers no
// efficient way to append to the less significant end we store blocks
// backward during extraction and then reverse them at the end.
//
template <typename Block, typename Allocator>
void
dynamic_bitset<Block, Allocator>::m_rearrange_extracted_bits(size_type
                                                             bits_read)
{
    // Reverse the order of the blocks and, if needed, shift to the right
    std::reverse(m_bits.begin(), m_bits.end());
    
    const int offset = bit_index(bits_read);
    if (offset)
        (*this) >>= (bits_per_block - offset);
    resize(bits_read); // this is never a grow
    
}


template <typename Ch, typename Tr, typename Block, typename Alloc>
std::basic_istream<Ch, Tr>&
operator>>(std::basic_istream<Ch, Tr>& is, dynamic_bitset<Block, Alloc>& b)
{
    // G.P.S. size_t vs size_type to be inspected

    using namespace std;

    const streamsize w = is.width();
    const size_t limit = w > 0 ? w : static_cast<size_t>(-1); // G.P.S. stream_size?

    const ios_base::iostate ok = ios_base::goodbit;
    ios_base::iostate err = ok; // G.P.S.

    // in accordance with prop. resol. of lib dr 303.
    //
    const Ch zero = is.widen('0');
    const Ch one  = is.widen('1');

    typename basic_istream<Ch, Tr>::sentry iprefix(is); // skips whitespaces
    size_t bits_stored = 0;
    try {

        if (iprefix) {
            
            b.clear();
            
            basic_streambuf <Ch, Tr> * buf = is.rdbuf();
            Block * current = 0;
            const Block max_mask
               = Block(1) << (dynamic_bitset<Block, Alloc>::bits_per_block - 1);
            Block mask = 0;

            typename Tr::int_type c = buf->sgetc(); // G.P.S.
            for( ; err == ok && bits_stored < limit; c = buf->snextc() ) {
                
                if (Tr::eq_int_type(Tr::eof(), c))
                    err |= ios_base::eofbit; // G.P.S.
                else {
                    const Ch to_c = Tr::to_char_type(c);
                    const bool is_one = Tr::eq(to_c, one);
                    
                    if (!is_one && !Tr::eq(to_c, zero))
                        break; // non digit character
                    
                    // need a new block; put it at the most
                    // significant end, for now
                    //
                    if (mask == 0) {
                        b.append(0);
                        current = &b.m_highest_block();
                        mask = max_mask;
                    }
                    
                    if(is_one)
                        *current |= mask;
                    
                    mask >>= 1;
                    ++bits_stored;
                }
                
            } // for


            if (bits_stored) {
                b.m_rearrange_extracted_bits(bits_stored);
            }
            else {
                err |= ios_base::failbit;
            }

        }

    } catch (...) {
        // catches from stream buf, or from vector:
        //
        // bits_stored bits have been extracted and stored, and
        // either no further character is extractable or we can't
        // append to the underlying vector (out of memory)

        b.m_rearrange_extracted_bits(bits_stored);

        bool rethrow_original = false;
        try {
          is.setstate(ios_base::failbit);
        } catch(...) {
            rethrow_original = true;
        }

        if (rethrow_original)
            throw;

    }

    // Set error bits: note that this may throw
    if (err != ios_base::goodbit)
        is.setstate (err);

    return is;

}
#endif


//-----------------------------------------------------------------------------
// bitset operations

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator&(const dynamic_bitset<Block, Allocator>& x,
          const dynamic_bitset<Block, Allocator>& y)
{
    dynamic_bitset<Block, Allocator> b(x);
    return b &= y;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator|(const dynamic_bitset<Block, Allocator>& x,
          const dynamic_bitset<Block, Allocator>& y)
{
    dynamic_bitset<Block, Allocator> b(x);
    return b |= y;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator^(const dynamic_bitset<Block, Allocator>& x,
          const dynamic_bitset<Block, Allocator>& y)
{
    dynamic_bitset<Block, Allocator> b(x);
    return b ^= y;
}

template <typename Block, typename Allocator>
dynamic_bitset<Block, Allocator>
operator-(const dynamic_bitset<Block, Allocator>& x,
          const dynamic_bitset<Block, Allocator>& y)
{
    dynamic_bitset<Block, Allocator> b(x);
    return b -= y;
}


//-----------------------------------------------------------------------------
// private member functions (on conforming compilers)

template <typename Block, typename Allocator>
inline Block& dynamic_bitset<Block, Allocator>::m_highest_block()
{
    assert(size() > 0 && num_blocks() > 0);
    return *(m_bits.end() - 1);
}



// If size() is not a multiple of bits_per_block
// then not all the bits in the last block are used.
// This function resets the unused bits (convenient
// for the implementation of many member functions)
//
template <typename Block, typename Allocator>
inline void dynamic_bitset<Block, Allocator>::m_zero_unused_bits()
{
    assert (this->num_blocks() == this->calc_num_blocks(this->m_num_bits));

    // if != 0 this is the number of bits used in the last block
    size_type const extra_bits = size() % bits_per_block;

    if (extra_bits != 0)
        m_highest_block() &= ~(~static_cast<Block>(0) << extra_bits);

}


} // namespace boost


#undef BOOST_WORKAROUND_REPEAT_DEFAULT_TEMPLATE_ARGUMENTS

#endif // BOOST_DYNAMIC_BITSET_HPP


