//  Boost io/array_stream.hpp header file  -----------------------------------//

//  (C) Copyright Daryle Walker 2002.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_IO_ARRAY_STREAM_HPP
#define BOOST_IO_ARRAY_STREAM_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/config.hpp>                 // for BOOST_STATIC_CONSTANT
#include <boost/io/streambuf_wrapping.hpp>  // for basic_wrapping_istream, etc.

#include <cstddef>    // for std::size_t
#include <streambuf>  // for std::basic_streambuf
#include <string>     // for std::char_traits


namespace boost
{
namespace io
{


//  Array-using streambuf class template declaration  ------------------------//

template < std::size_t N, typename Ch, class Tr >
class basic_array_streambuf
    : public std::basic_streambuf<Ch, Tr>
{
    typedef std::basic_streambuf<Ch, Tr>   base_type;
    typedef basic_array_streambuf          self_type;

public:
    // Template arguments
    BOOST_STATIC_CONSTANT( std::size_t, array_size = N );

    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Constructors
    basic_array_streambuf();
    basic_array_streambuf( self_type const &c );

    // Accessors
    char_type *  array_begin();
    char_type *  array_end();

    char_type const *  array_begin() const;
    char_type const *  array_end() const;

private:
    // Member data
    char_type  array_[ array_size ];

};  // boost::io::basic_array_streambuf


//  Array-using stream class template declarations  --------------------------//

template < std::size_t N, typename Ch, class Tr >
class basic_array_istream
    : public basic_wrapping_istream< basic_array_streambuf<N, Ch, Tr> >
{
public:
    // Template arguments
    BOOST_STATIC_CONSTANT( std::size_t, array_size = N );

    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Accessors
    char_type *  array_begin();
    char_type *  array_end();

    char_type const *  array_begin() const;
    char_type const *  array_end() const;

};  // boost::io::basic_array_istream

template < std::size_t N, typename Ch, class Tr >
class basic_array_ostream
    : public basic_wrapping_ostream< basic_array_streambuf<N, Ch, Tr> >
{
public:
    // Template arguments
    BOOST_STATIC_CONSTANT( std::size_t, array_size = N );

    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Accessors
    char_type *  array_begin();
    char_type *  array_end();

    char_type const *  array_begin() const;
    char_type const *  array_end() const;

};  // boost::io::basic_array_ostream

template < std::size_t N, typename Ch, class Tr >
class basic_array_stream
    : public basic_wrapping_iostream< basic_array_streambuf<N, Ch, Tr> >
{
public:
    // Template arguments
    BOOST_STATIC_CONSTANT( std::size_t, array_size = N );

    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Accessors
    char_type *  array_begin();
    char_type *  array_end();

    char_type const *  array_begin() const;
    char_type const *  array_end() const;

};  // boost::io::basic_array_stream


//  Array-using streambuf class template member function definitions  --------//

template < std::size_t N, typename Ch, class Tr >
inline
basic_array_streambuf<N, Ch, Tr>::basic_array_streambuf
(
)
{
    this->setg( this->array_, this->array_, this->array_
     + self_type::array_size );
    this->setp( this->array_, this->array_ + self_type::array_size );
}

template < std::size_t N, typename Ch, class Tr >
inline
basic_array_streambuf<N, Ch, Tr>::basic_array_streambuf
(
    basic_array_streambuf<N, Ch, Tr> const &  c
)
{
    traits_type::copy( this->array_, c.array_, self_type::array_size );

    this->setg( this->array_, this->array_, this->array_
     + self_type::array_size );
    this->setp( this->array_, this->array_ + self_type::array_size );
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_streambuf<N, Ch, Tr>::char_type *
basic_array_streambuf<N, Ch, Tr>::array_begin
(
)
{
    return this->array_;
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_streambuf<N, Ch, Tr>::char_type *
basic_array_streambuf<N, Ch, Tr>::array_end
(
)
{
    return this->array_ + self_type::array_size;
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_streambuf<N, Ch, Tr>::char_type const *
basic_array_streambuf<N, Ch, Tr>::array_begin
(
) const
{
    return this->array_;
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_streambuf<N, Ch, Tr>::char_type const *
basic_array_streambuf<N, Ch, Tr>::array_end
(
) const
{
    return this->array_ + self_type::array_size;
}


//  Array-using stream class template member function definitions  -----------//

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_istream<N, Ch, Tr>::char_type *
basic_array_istream<N, Ch, Tr>::array_begin
(
)
{
    return this->get_internal_streambuf()->array_begin();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_istream<N, Ch, Tr>::char_type *
basic_array_istream<N, Ch, Tr>::array_end
(
)
{
    return this->get_internal_streambuf()->array_end();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_istream<N, Ch, Tr>::char_type const *
basic_array_istream<N, Ch, Tr>::array_begin
(
) const
{
    return this->get_internal_streambuf()->array_begin();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_istream<N, Ch, Tr>::char_type const *
basic_array_istream<N, Ch, Tr>::array_end
(
) const
{
    return this->get_internal_streambuf()->array_end();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_ostream<N, Ch, Tr>::char_type *
basic_array_ostream<N, Ch, Tr>::array_begin
(
)
{
    return this->get_internal_streambuf()->array_begin();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_ostream<N, Ch, Tr>::char_type *
basic_array_ostream<N, Ch, Tr>::array_end
(
)
{
    return this->get_internal_streambuf()->array_end();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_ostream<N, Ch, Tr>::char_type const *
basic_array_ostream<N, Ch, Tr>::array_begin
(
) const
{
    return this->get_internal_streambuf()->array_begin();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_ostream<N, Ch, Tr>::char_type const *
basic_array_ostream<N, Ch, Tr>::array_end
(
) const
{
    return this->get_internal_streambuf()->array_end();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_stream<N, Ch, Tr>::char_type *
basic_array_stream<N, Ch, Tr>::array_begin
(
)
{
    return this->get_internal_streambuf()->array_begin();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_stream<N, Ch, Tr>::char_type *
basic_array_stream<N, Ch, Tr>::array_end
(
)
{
    return this->get_internal_streambuf()->array_end();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_stream<N, Ch, Tr>::char_type const *
basic_array_stream<N, Ch, Tr>::array_begin
(
) const
{
    return this->get_internal_streambuf()->array_begin();
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_stream<N, Ch, Tr>::char_type const *
basic_array_stream<N, Ch, Tr>::array_end
(
) const
{
    return this->get_internal_streambuf()->array_end();
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_ARRAY_STREAM_HPP
