//  Boost bit-vector library header file  ------------------------------------//

//  (C) Copyright Daryle Walker 2002.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_VECTOR_HPP
#define BOOST_BIT_VECTOR_HPP

#include <boost/config.hpp>         // for BOOST_STATIC_CONSTANT, etc.
#include <boost/static_assert.hpp>  // for BOOST_STATIC_ASSERT

#include <bitset>    // for std::bitset
#include <cstddef>   // for std::size_t
#include <iterator>  // for std::reverse_iterator, etc.
#include <memory>    // for std::allocator
#include <utility>   // for std::pair
#include <vector>    // for std::vector

#include <boost/cstdint.hpp>  // for boost::uintmax_t
#include <boost/limits.hpp>   // for std::numeric_limits


namespace boost
{


//  Forward declarations  ----------------------------------------------------//

template < typename BlockType >
    class const_bit_reference;

template < typename BlockType >
    class bit_reference;

template < typename BlockType = uintmax_t, class Allocator
 = std::allocator<BlockType> >
    class bit_vector;

typedef bit_vector<>  std_bit_vector;


//  Forward declarations for implementation detail stuff  --------------------//
//  (Just for the stuff that will be needed for the next 3 sections)

namespace detail
{

template < typename BlockType, class Allocator >
    class const_bit_vector_iterator;

template < typename BlockType, class Allocator >
    class bit_vector_iterator;

}  // namespace detail


//  Constant bit reference class declaration  --------------------------------//

template < typename BlockType >
class const_bit_reference
{
    typedef std::numeric_limits<BlockType>  limits_type;

    BOOST_STATIC_ASSERT( limits_type::is_specialized && !limits_type::is_signed
     && limits_type::is_integer && (limits_type::radix == 2) );

    friend class bit_reference<BlockType>;

    BlockType const &  ref_;
    BlockType const    mask_;

    const_bit_reference( BlockType const &ref, BlockType const &mask )
        : ref_( ref ), mask_( mask )
        {}

public:
    typedef BlockType  block_type;

    const_bit_reference( block_type const &ref, std::size_t bit_pos )
        : ref_( ref ), mask_( static_cast<block_type>(1u) << bit_pos )
        {}

    operator bool() const
        { return ref_ & mask_; }

    bool  operator !() const
        { return !this->operator bool(); }

};  // boost::const_bit_reference


//  Bit reference class declaration  -----------------------------------------//

template < typename BlockType >
class bit_reference
{
    BlockType &      ref_;
    BlockType const  mask_;

public:
    typedef BlockType  block_type;

    typedef const_bit_reference<BlockType>  const_reference_type;

    bit_reference( block_type &ref, std::size_t bit_pos )
        : ref_( ref ), mask_( static_cast<BlockType>(1u) << bit_pos )
        {}

    operator bool() const
        { return ref_ & mask_; }
    operator const_reference_type() const
        { return const_reference_type( ref_, mask_ ); }

    bool  operator !() const
        { return !this->operator bool(); }

    bit_reference &  operator =( const_reference_type const &ref )
        { if ( !ref != !(*this) ) this->flip(); return *this; }
    bit_reference &  operator =( bit_reference const &ref )
        { if ( !ref != !(*this) ) this->flip(); return *this; }
    bit_reference &  operator =( bool val )
        { if ( !val != !(*this) ) this->flip(); return *this; }

    void  flip()
        { ref_ ^= mask_; }

};  // boost::bit_reference


//  Bit-vector (and set) class declaration  ----------------------------------//

template < typename BlockType, class Allocator >
class bit_vector
{
    friend class detail::const_bit_vector_iterator<BlockType, Allocator>;
    friend class detail::bit_vector_iterator<BlockType, Allocator>;

    typedef std::vector<BlockType, Allocator>  vector_type;
    typedef std::numeric_limits<BlockType>     limits_type;
    typedef bit_vector<BlockType, Allocator>   self_type;

    BOOST_STATIC_CONSTANT( int, block_size = limits_type::digits );

public:
    // Types
    typedef bool  value_type;

    typedef bit_reference<BlockType>        reference;
    typedef const_bit_reference<BlockType>  const_reference;

    typedef detail::bit_vector_iterator<BlockType, Allocator>  iterator;

    typedef detail::const_bit_vector_iterator<BlockType, Allocator>
      const_iterator;

    typedef std::reverse_iterator<iterator>        reverse_iterator;
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;

    typedef typename Allocator::size_type        size_type;
    typedef typename Allocator::difference_type  difference_type;

    typedef BlockType  block_type;
    typedef Allocator  allocator_type;

    typedef self_type & (self_type::*)()  bool_type;

    // Class-static functions
    std::pair<self_type, self_type>  div( self_type const &dividend,
     self_type const &divisor );

    // Structors (use automatic copy ctr. and dtr.)
    explicit  bit_vector( allocator_type const &a = Allocator() );
    explicit  bit_vector( size_type n, bool value = false,
     allocator_type const &a = allocator_type() );

    #ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
    template < typename InputIterator >
    bit_vector( InputIterator beg, InputIterator end,
     allocator_type const &a = allocator_type() );
    #endif

    #ifndef BOOST_NO_MEMBER_TEMPLATES
    template < class Allocator2 >
    explicit  bit_vector( std::vector<bool, Allocator2> const &v );

    template < std::size_t N >
    explicit  bit_vector( std::bitset<N> const &b );
    #endif

    // Size operations
    size_type  size() const;
    bool       empty() const;
    size_type  max_size() const;

    // Capacity operations
    size_type  capacity() const;
    void       reserve( size_type n );

    // Comparison operations (use operators.hpp for help)
    bool  operator ==( self_type const &x ) const;
    bool  operator <( self_type const &x ) const;

    // Condition checking
    size_type  count() const;
    bool       any() const;
    bool       none() const;

    // Assignments (use automatic regular-assignment operator)
    void  assign( size_type n, bool value );

    #ifndef BOOST_NO_MEMBER_TEMPLATES
    template < typename InputIterator >
    void  assign( InputIterator beg, InputIterator end );
    #endif

    void  swap( self_type &c );

    self_type &  operator ^=( self_type const &c );
    self_type &  operator |=( self_type const &c );
    self_type &  operator &=( self_type const &c );
    self_type &  operator <<=( self_type const &c );
    self_type &  operator >>=( self_type const &c );

    self_type &  operator +=( self_type const &c );
    self_type &  operator -=( self_type const &c );
    self_type &  operator *=( self_type const &c );
    self_type &  operator /=( self_type const &c );
    self_type &  operator %=( self_type const &c );

    // Direct element access
    reference        at( size_type i );
    const_reference  at( size_type i ) const;

    reference        operator []( size_type i );
    const_reference  operator []( size_type i ) const;

    reference        front();
    const_reference  front() const;

    reference        back();
    const_reference  back() const;

    // Other element access
    bool  test( size_type i ) const;

    self_type &  set();
    self_type &  set( size_type i, bool value = true );

    self_type &  reset();
    self_type &  reset( size_type i );

    self_type &  flip();
    self_type &  flip( size_type i );

    // Iterator generation
    iterator        begin();
    const_iterator  begin() const;

    iterator        end();
    const_iterator  end() const;

    reverse_iterator        rbegin();
    const_reverse_iterator  rbegin() const;

    reverse_iterator        rend();
    const_reverse_iterator  rend() const;

    // Element insertion and removal
    iterator  insert( iterator p, bool value );
    void      insert( iterator p, size_type n, bool value );

    #ifndef BOOST_NO_MEMBER_TEMPLATES
    template < typename InputIterator >
    void  insert( iterator p, InputIterator beg, InputIterator end );
    #endif

    void  push_back( bool value );
    void  pop_back();

    iterator  erase( iterator p );
    iterator  erase( iterator beg, iterator end );

    void  resize( size_type n, bool value = false );
    void  clear();

    // More operations
    bool       operator !() const;
    self_type  operator ~() const;
    self_type  operator -() const;
    self_type  operator +() const;
    
    self_type &  operator --() const;
    self_type &  operator ++() const;
    
    operator bool_type() const;

    uintmax_t  to_uintmax_t() const;

    // Allocator support
    allocator_type  get_allocator() const;

};  // boost::bit_vector


//  Implementation detail stuff  ---------------------------------------------//

namespace detail
{

template < typename BlockType, class Allocator >
class const_bit_vector_iterator
{
    friend class bit_vector_iterator<BlockType, Allocator>
    friend class bit_vector<BlockType, Allocator>

    typedef const_bit_vector_iterator<BlockType, Allocator>  self_type;

    typedef bit_vector<BlockType, Allocator>      container_type;
    typedef typename container_type::vector_type  vector_type;
    typedef typename vector_type::const_iterator  const_iterator;
    typedef typename container_type::size_type    size_type;

    const_iterator  vi_;
    size_type       pos_;

    const_bit_vector_iterator( const_iterator vi, size_type pos )
        : vi_( vi ), pos_( pos )
        {}

public:
    // Types
    typedef typename container_type::difference_type  difference_type;
    typedef typename container_type::value_type       value_type;
    typedef typename const_iterator::pointer          pointer;
    typedef const_bit_reference<BlockType>            reference;
    typedef std::bidirectional_iterator_tag           iterator_category;

    reference  operator *() const
        { return reference( &*vi_, pos_ ); }

    self_type &  operator ++()
        { if ( ++pos_ >= container_type::block_size ) { ++vi_; pos_ = 0; } return *this; }

    self_type &  operator --()
        { if ( pos_-- <= 0 ) { --vi_; pos_ = container_type::block_size - 1; } return *this; }

    bool  operator ==( self_type const &c ) const
        { return ( vi_ == c.vi_ ) && ( pos_ == c.pos_ ); }

};

template < typename BlockType, class Allocator >
class bit_vector_iterator
{
    friend class bit_vector<BlockType, Allocator>

    typedef bit_vector_iterator<BlockType, Allocator>  self_type;

    typedef bit_vector<BlockType, Allocator>      container_type;
    typedef typename container_type::vector_type  vector_type;
    typedef typename vector_type::iterator        iterator;
    typedef typename container_type::size_type    size_type;

    typedef const_bit_vector_iterator<BlockType, Allocator>
      const_iterator_type;

    iterator   vi_;
    size_type  pos_;

    bit_vector_iterator( iterator vi, size_type pos )
        : vi_( vi ), pos_( pos )
        {}

public:
    // Types
    typedef typename container_type::difference_type  difference_type;
    typedef typename container_type::value_type       value_type;
    typedef typename iterator::pointer                pointer;
    typedef bit_reference<BlockType>                  reference;
    typedef std::bidirectional_iterator_tag           iterator_category;

    // Operations
    operator const_iterator_type() const
        { return const_iterator_type( vi_, pos_ ); }

    reference  operator *() const
        { return reference( &*vi_, pos_ ); }

    self_type &  operator ++()
        { if ( ++pos_ >= container_type::block_size ) { ++vi_; pos_ = 0; } return *this; }

    self_type &  operator --()
        { if ( pos_-- <= 0 ) { --vi_; pos_ = container_type::block_size - 1; } return *this; }

    bool  operator ==( self_type const &c ) const
        { return ( vi_ == c.vi_ ) && ( pos_ == c.pos_ ); }

};

}  // namespace detail


//  Bit-vector (and set) class function definitions  -------------------------//

// put something here...


#endif  // BOOST_BIT_VECTOR_HPP
