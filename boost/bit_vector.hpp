//  Boost bit-vector library header file  ------------------------------------//

//  (C) Copyright Daryle Walker 2002.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_VECTOR_HPP
#define BOOST_BIT_VECTOR_HPP

#include <boost/bit_reference.hpp>  // for boost::bit_reference, etc.
#include <boost/config.hpp>         // for BOOST_STATIC_CONSTANT, etc.
#include <boost/cstdint.hpp>        // for boost::uintmax_t
#include <boost/limits.hpp>         // for std::numeric_limits
#include <boost/operators.hpp>      // for boost::orable, etc.

#include <algorithm>  // for std::swap
#include <bitset>     // for std::bitset
#include <cstddef>    // for std::size_t
#include <iterator>   // for std::reverse_iterator, etc.
#include <memory>     // for std::allocator
#include <utility>    // for std::pair
#include <vector>     // for std::vector


namespace boost
{


//  Forward declarations  ----------------------------------------------------//

template < typename BlockType = uintmax_t, class Allocator
 = std::allocator<BlockType> >
    class bit_vector;

typedef bit_vector<>  std_bit_vector;

template < typename BlockType, class Allocator >
    void  swap( bit_vector<BlockType, Allocator> &a, bit_vector<BlockType,
     Allocator> &b );


//  Forward declarations for implementation detail stuff  --------------------//
//  (Just for the stuff that will be needed for the next section)

namespace detail
{

template < typename BlockType, class Allocator >
    class const_bit_vector_iterator;

template < typename BlockType, class Allocator >
    class bit_vector_iterator;

}  // namespace detail


//  Bit-vector (and set) class declaration  ----------------------------------//

template < typename BlockType, class Allocator >
class bit_vector
    : public ordered_euclidian_ring_operators1< bit_vector<BlockType, Allocator>
    , bitwise1< bit_vector<BlockType, Allocator>
    , unit_steppable< bit_vector<BlockType, Allocator>
    , shiftable2< bit_vector<BlockType, Allocator>, typename Allocator::size_type
    > > > >
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
    static  std::pair<self_type, self_type>  div( self_type const &dividend,
     self_type const &divisor );//not implemented

    static  self_type  from_ulong( unsigned long val );//not implemented

    // Structors (use automatic copy ctr. and dtr.)
    explicit  bit_vector( allocator_type const &a = Allocator() );//not implemented
    explicit  bit_vector( size_type n, bool value = false,
     allocator_type const &a = allocator_type() );//not implemented

    #ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
    template < typename InputIterator >
    bit_vector( InputIterator beg, InputIterator end,
     allocator_type const &a = allocator_type() );//not implemented
    #endif

    #ifndef BOOST_NO_MEMBER_TEMPLATES
    template < class Allocator2 >
    explicit  bit_vector( std::vector<bool, Allocator2> const &v,
     allocator_type const &a = allocator_type() );//not implemented

    template < std::size_t N >
    explicit  bit_vector( std::bitset<N> const &b,
     allocator_type const &a = allocator_type() );//not implemented
    #endif

    // Size operations
    size_type  size() const;//not implemented
    bool       empty() const;//not implemented
    size_type  max_size() const;

    // Capacity operations
    size_type  capacity() const;
    void       reserve( size_type n );//not implemented

    // Comparison operations (use operators.hpp for help)
    bool  operator ==( self_type const &x ) const;//not implemented
    bool  operator <( self_type const &x ) const;//not implemented

    // Condition checking
    size_type  count() const;//not implemented
    bool       any() const;//not implemented
    bool       none() const;//not implemented

    // Assignments (use automatic regular-assignment operator)
    void  assign( size_type n, bool value );//not implemented

    #ifndef BOOST_NO_MEMBER_TEMPLATES
    template < typename InputIterator >
    void  assign( InputIterator beg, InputIterator end );//not implemented
    #endif

    void  swap( self_type &c );

    self_type &  operator ^=( self_type const &c );//not implemented
    self_type &  operator |=( self_type const &c );//not implemented
    self_type &  operator &=( self_type const &c );//not implemented
    self_type &  operator <<=( size_type s );//not implemented
    self_type &  operator >>=( size_type s );//not implemented

    self_type &  operator +=( self_type const &c );//not implemented
    self_type &  operator -=( self_type const &c );//not implemented
    self_type &  operator *=( self_type const &c );//not implemented
    self_type &  operator /=( self_type const &c );//not implemented
    self_type &  operator %=( self_type const &c );//not implemented

    // Direct element access
    reference        at( size_type i );//not implemented
    const_reference  at( size_type i ) const;//not implemented

    reference        operator []( size_type i );//not implemented
    const_reference  operator []( size_type i ) const;//not implemented

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

    self_type &  flip();//not implemented
    self_type &  flip( size_type i );

    // Iterator generation
    iterator        begin();//not implemented
    const_iterator  begin() const;//not implemented

    iterator        end();//not implemented
    const_iterator  end() const;//not implemented

    reverse_iterator        rbegin();//not implemented
    const_reverse_iterator  rbegin() const;//not implemented

    reverse_iterator        rend();//not implemented
    const_reverse_iterator  rend() const;//not implemented

    // Element insertion and removal
    iterator  insert( iterator p, bool value );//not implemented
    void      insert( iterator p, size_type n, bool value );//not implemented

    #ifndef BOOST_NO_MEMBER_TEMPLATES
    template < typename InputIterator >
    void  insert( iterator p, InputIterator beg, InputIterator end );//not implemented
    #endif

    void  push_back( bool value );//not implemented
    void  pop_back();//not implemented

    iterator  erase( iterator p );//not implemented
    iterator  erase( iterator beg, iterator end );//not implemented

    void  resize( size_type n, bool value = false );//not implemented
    void  clear();//not implemented

    // More operations
    bool       operator !() const;
    self_type  operator ~() const;//not implemented
    self_type  operator -() const;//not implemented
    self_type  operator +() const;
    
    self_type &  operator --() const;//not implemented
    self_type &  operator ++() const;//not implemented
    
    operator bool_type() const;

    uintmax_t  to_uintmax_t() const;//not implemented

    // Allocator support
    allocator_type  get_allocator() const;

private:
    // Data members
    vector_type  v_;
    int          last_pos_;

};  // boost::bit_vector


//  Implementation detail stuff  ---------------------------------------------//

namespace detail
{

template < typename BlockType, class Allocator >
class const_bit_vector_iterator
    : public bidirectional_iterator_helper< const_bit_vector_iterator<BlockType
    , Allocator>
    , bool
    , typename Allocator::difference_type,
    , typename Allocator::pointer
    , const_bit_reference<BlockType>
    >
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
    // Operations
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
    : public bidirectional_iterator_helper< bit_vector_iterator<BlockType
    , Allocator>
    , bool
    , typename Allocator::difference_type,
    , typename Allocator::pointer
    , bit_reference<BlockType>
    >
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

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::size_type
bit_vector<BlockType, Allocator>::max_size
(
) const
{
    return this->v_.max_size() * self_type::block_size;
}

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::size_type
bit_vector<BlockType, Allocator>::capacity
(
) const
{
    return this->v_.capacity() * self_type::block_size;
}

template < typename BlockType, class Allocator >
inline
void
bit_vector<BlockType, Allocator>::swap
(
    self_type &  c
)
{
    this->v_.swap( c.v_ );
    std::swap( this->last_pos_, c.last_pos_ );
}

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::reference
bit_vector<BlockType, Allocator>::front
(
)
{
    return *this->begin();
}

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::const_reference
bit_vector<BlockType, Allocator>::front
(
) const
{
    return *this->begin();
}

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::reference
bit_vector<BlockType, Allocator>::back
(
)
{
    return *this->rbegin();
}

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::const_reference
bit_vector<BlockType, Allocator>::back
(
) const
{
    return *this->rbegin();
}

template < typename BlockType, class Allocator >
inline
bool
bit_vector<BlockType, Allocator>::test
(
    size_type  i
) const
{
    return this->at( i );
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator> &
bit_vector<BlockType, Allocator>::set
(
)
{
    this->assign( this->size(), true );
    return *this;
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator> &
bit_vector<BlockType, Allocator>::set
(
    size_type  i,
    bool       value  // = true
)
{
    this->at( i ) = value;
    return *this;
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator> &
bit_vector<BlockType, Allocator>::reset
(
)
{
    this->assign( this->size(), false );
    return *this;
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator> &
bit_vector<BlockType, Allocator>::reset
(
    size_type  i
)
{
    return this->set( i, false );
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator> &
bit_vector<BlockType, Allocator>::flip
(
    size_type  i
)
{
    ( this->at(i) ).flip();
    return *this;
}

template < typename BlockType, class Allocator >
inline
bool
bit_vector<BlockType, Allocator>::operator !
(
) const
{
    return this->none();
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator>
bit_vector<BlockType, Allocator>::operator +
(
) const
{
    return *this;
}

template < typename BlockType, class Allocator >
inline
bit_vector<BlockType, Allocator>::operator bool_type
(
) const
{
    return this->any() ? &self_type::set : 0;
}

template < typename BlockType, class Allocator >
inline
typename bit_vector<BlockType, Allocator>::allocator_type
bit_vector<BlockType, Allocator>::get_allocator
(
) const
{
    return this->v_.get_allocator();
}


//  Bit-vector/set support function definitions  -----------------------------//

template < typename BlockType, class Allocator >
inline
void
swap
(
    bit_vector<BlockType, Allocator> &  a,
    bit_vector<BlockType, Allocator> &  b
)
{
    a.swap( b );
}


#endif  // BOOST_BIT_VECTOR_HPP
