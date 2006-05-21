//  Boost memory/auto_array.hpp header file  ---------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/memory/> for the library's home page.

#ifndef BOOST_MEMORY_AUTO_ARRAY_HPP
#define BOOST_MEMORY_AUTO_ARRAY_HPP

#include <boost/memory_fwd.hpp>  // self include

#include <cstddef>  // for std::size_t


namespace boost
{
namespace memory
{


//  Array-pointing auto_ptr variant, template class declaration  -------------//

template < typename T >
class auto_array
{
    struct auto_array_ref;

public:
    // Template arguments
    typedef T  element_type;

    // Other types
    typedef std::size_t  size_type;

    // Constructors and destructor
    explicit  auto_array( element_type *p = 0 ) throw();
    explicit  auto_array( size_type s );

    auto_array( auto_array &c ) throw();
    auto_array( auto_array_ref r ) throw();

    ~auto_array() throw();

    // Observers
    element_type *  get() const throw();

    // Mutators
    element_type *  release() throw();
    void            reset( element_type *p = 0 ) throw();

    // Operators
    element_type &  operator []( size_type i ) const throw();

    operator auto_array_ref() throw();

    auto_array &  operator =( auto_array &c ) throw();
    auto_array &  operator =( auto_array_ref r ) throw();

private:
    // Member data
    element_type *  ap_;

    // Auxillary type to enable some copies and assignments through temporaries
    struct auto_array_ref
    {
        T *  rp_;

        explicit  auto_array_ref( T *p )
            : rp_( p )
            {}

    };  // boost::memory::auto_array<T>::auto_array_ref

};  // boost::memory::auto_array


//  Array-pointing auto_ptr variant, member function definitions  ------------//

template < typename T >
inline
auto_array<T>::auto_array
(
    typename auto_array<T>::element_type *  p  // = 0
) throw()
    : ap_( p )
{
}

template < typename T >
auto_array<T>::auto_array
(
    typename auto_array<T>::size_type  s
)
    : ap_( auto_array<T>(new T[ s ]).release() )
{
}

template < typename T >
inline
auto_array<T>::auto_array
(
    auto_array<T> &  c
) throw()
    : ap_( c.release() )
{
}

template < typename T >
inline
auto_array<T>::auto_array
(
    typename auto_array<T>::auto_array_ref  r
) throw()
    : ap_( r.rp_ )
{
}

template < typename T >
inline
auto_array<T>::~auto_array
(
) throw()
{
    delete [] this->ap_;
}

template < typename T >
inline
typename auto_array<T>::element_type *
auto_array<T>::get
(
) const throw()
{
    return this->ap_;
}

template < typename T >
typename auto_array<T>::element_type *
auto_array<T>::release
(
) throw()
{
    element_type * const  temp = this->ap_;

    this->ap_ = 0;
    return temp;
}

template < typename T >
inline
void
auto_array<T>::reset
(
    typename auto_array<T>::element_type *  p  // = 0
) throw()
{
    if ( this->ap_ != p )
    {
        delete [] this->ap_;
        this->ap_ = p;
    }
}

template < typename T >
inline
typename auto_array<T>::element_type &
auto_array<T>::operator []
(
    typename auto_array<T>::size_type  i
) const throw()
{
    return this->ap_[ i ];
}

template < typename T >
inline
auto_array<T>::operator typename auto_array<T>::auto_array_ref
(
) throw()
{
    return auto_array_ref( this->release() );
}

template < typename T >
inline
auto_array<T> &
auto_array<T>::operator =
(
    auto_array<T> &  c
) throw()
{
    this->reset( c.release() );
    return *this;
}

template < typename T >
inline
auto_array<T> &
auto_array<T>::operator =
(
    typename auto_array<T>::auto_array_ref  r
) throw()
{
    this->reset( r.rp_ );
    return *this;
}


}  // namespace memory
}  // namespace boost


#endif  // BOOST_MEMORY_AUTO_ARRAY_HPP
