//  Boost memory/auto_temp_buf.hpp header file  ------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/memory/> for the library's home page.

#ifndef BOOST_MEMORY_AUTO_TEMP_BUF_HPP
#define BOOST_MEMORY_AUTO_TEMP_BUF_HPP

#include <boost/memory_fwd.hpp>  // self include

#include <algorithm>  // for std::swap
#include <cstddef>    // for std::ptrdiff_t
#include <memory>     // for std::get_temporary_buffer, etc.
#include <utility>    // for std::pair


namespace boost
{
namespace memory
{


//  Temporary buffer management template class declaration  ------------------//

template < typename T >
class auto_temp_buf
{
    struct auto_temp_buf_ref;

public:
    // Template arguments
    typedef T  element_type;

    // Other types
    typedef std::ptrdiff_t  size_type;

    typedef std::pair<element_type *, size_type>  block_type;

    typedef block_type auto_temp_buf::*  safe_bool;

    // Lifetime
              auto_temp_buf();
    explicit  auto_temp_buf( block_type b );
              auto_temp_buf( element_type *p, size_type n );

    explicit  auto_temp_buf( size_type n );

    auto_temp_buf( auto_temp_buf &c );
    auto_temp_buf( auto_temp_buf_ref r );

    ~auto_temp_buf();

    // Observers
    element_type *  get() const;
    size_type       size() const;

    bool  valid() const;

    // Mutators
    block_type  release();

    void  reset( block_type b );
    void  reset( element_type *p, size_type n );
    void  reset();

    // Operators
    operator auto_temp_buf_ref();

    auto_temp_buf &  operator =( auto_temp_buf &c );
    auto_temp_buf &  operator =( auto_temp_buf_ref r );

          operator safe_bool() const;
    bool  operator !() const;

private:
    // Member data
    block_type  b_;

    // Auxillary type to enable some copies and assignments through temporaries
    struct auto_temp_buf_ref
        : block_type
    {
        auto_temp_buf_ref( block_type b )
            : block_type( b )
            {}

    };  // boost::memory::auto_temp_buf<T>::auto_temp_buf_ref

};  // boost::memory::auto_temp_buf


//  Temporary buffer management member function definitions  -----------------//

template < typename T >
inline
auto_temp_buf<T>::auto_temp_buf
(
)
    : b_( 0, 0 )
{
}

template < typename T >
inline
auto_temp_buf<T>::auto_temp_buf
(
    block_type  b
)
    : b_( b )
{
}

template < typename T >
inline
auto_temp_buf<T>::auto_temp_buf
(
    element_type *  p,
    size_type       n
)
    : b_( p, n )
{
}

template < typename T >
inline
auto_temp_buf<T>::auto_temp_buf
(
    size_type  n
)
    : b_( std::get_temporary_buffer<element_type>(n) )
{
}

template < typename T >
inline
auto_temp_buf<T>::auto_temp_buf
(
    auto_temp_buf<T> &  c
)
    : b_( c.release() )
{
}

template < typename T >
inline
auto_temp_buf<T>::auto_temp_buf
(
    auto_temp_buf_ref  r
)
    : b_( static_cast<block_type &>(r) )
{
}

template < typename T >
inline
auto_temp_buf<T>::~auto_temp_buf
(
)
{
    std::return_temporary_buffer<element_type>( this->b_.first );
}

template < typename T >
inline
typename auto_temp_buf<T>::element_type *
auto_temp_buf<T>::get
(
) const
{
    return this->b_.first;
}

template < typename T >
inline
typename auto_temp_buf<T>::size_type
auto_temp_buf<T>::size
(
) const
{
    return this->b_.second;
}

template < typename T >
inline
bool
auto_temp_buf<T>::valid
(
) const
{
    return this->b_.first && ( this->b_.second > 0 );
}

template < typename T >
inline
typename auto_temp_buf<T>::block_type
auto_temp_buf<T>::release
(
)
{
    block_type  temp( 0, 0 );

    std::swap( temp, this->b_ );
    return temp;
}

template < typename T >
inline
void
auto_temp_buf<T>::reset
(
    block_type  b
)
{
    if ( this->b_ != b )
    {
        std::return_temporary_buffer<element_type>( this->b_.first );
        this->b_ = b;
    }
}

template < typename T >
inline
void
auto_temp_buf<T>::reset
(
    element_type *  p,
    size_type       n
)
{
    this->reset( std::make_pair(p, n) );
}

template < typename T >
inline
void
auto_temp_buf<T>::reset
(
)
{
    this->reset( 0, 0 );
}

template < typename T >
inline
auto_temp_buf<T>::operator typename auto_temp_buf<T>::auto_temp_buf_ref
(
)
{
    return auto_temp_buf_ref( this->release() );
}

template < typename T >
inline
auto_temp_buf<T> &
auto_temp_buf<T>::operator =
(
    auto_temp_buf<T> &  c
)
{
    this->reset( c.release() );
    return *this;
}

template < typename T >
inline
auto_temp_buf<T> &
auto_temp_buf<T>::operator =
(
    auto_temp_buf_ref  r
)
{
    this->reset( static_cast<block_type &>(r) );
    return *this;
}

template < typename T >
inline
auto_temp_buf<T>::operator typename auto_temp_buf<T>::safe_bool
(
) const
{
    return this->valid() ? &auto_temp_buf<T>::b_ : 0;
}

template < typename T >
inline
bool
auto_temp_buf<T>::operator !
(
) const
{
    return !static_cast<bool>( *this );
}


}  // namespace memory
}  // namespace boost


#endif  // BOOST_MEMORY_AUTO_TEMP_BUF_HPP
