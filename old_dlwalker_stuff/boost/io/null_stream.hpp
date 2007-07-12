//  Boost io/null_stream.hpp header file  ------------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_NULL_STREAM_HPP
#define BOOST_IO_NULL_STREAM_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/io/streambuf_wrapping.hpp>  // for basic_wrapping_ostream, etc.

#include <ios>        // for std::streamsize
#include <streambuf>  // for std::basic_streambuf
#include <string>     // for std::char_traits


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_nullbuf;

typedef basic_nullbuf<char>      nullbuf;
typedef basic_nullbuf<wchar_t>  wnullbuf;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_onullstream;

typedef basic_onullstream<char>      onullstream;
typedef basic_onullstream<wchar_t>  wonullstream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_inullstream;

typedef basic_inullstream<char>      inullstream;
typedef basic_inullstream<wchar_t>  winullstream;


//  Voided stream-buffer class template declaration  -------------------------//

template < typename Ch, class Tr >
class basic_nullbuf
    : public std::basic_streambuf<Ch, Tr>
{
    typedef std::basic_streambuf<Ch, Tr>  base_type;
    typedef basic_nullbuf                 self_type;

public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Other types
    typedef typename Tr::int_type  int_type;
    typedef typename Tr::pos_type  pos_type;
    typedef typename Tr::off_type  off_type;

    // Constructor (use automatically-defined destructor)
    basic_nullbuf();

    // Accessors
    std::streamsize  pcount() const;

protected:
    // Overriden virtual functions
    virtual  std::streamsize  xsputn( char_type const *s, std::streamsize n );
    virtual  int_type         overflow( int_type c = traits_type::eof() );

private:
    // Member data
    std::streamsize  count_;

};  // boost::io::basic_nullbuf


//  Voided stream class template declarations  -------------------------------//

// Macro to template the templates!
#define BOOST_PRIVATE_WRAPPER( SuffixF, SuffixB ) \
    template < typename Ch, class Tr > \
    class basic_##SuffixF \
        : public basic_wrapping_##SuffixB< basic_nullbuf<Ch, Tr> > \
    { \
        typedef basic_nullbuf<Ch, Tr>                streambuf_type; \
        typedef basic_wrapping_##SuffixB<streambuf_type>  base_type; \
    public: \
        typedef Ch  char_type; \
        typedef Tr  traits_type; \
        typedef typename Tr::int_type  int_type; \
        typedef typename Tr::pos_type  pos_type; \
        typedef typename Tr::off_type  off_type; \
    }

BOOST_PRIVATE_WRAPPER( onullstream, ostream );
BOOST_PRIVATE_WRAPPER( inullstream, istream );

#undef BOOST_PRIVATE_WRAPPER


//  Voided stream-buffer class template member function definitions  ---------//

template < typename Ch, class Tr >
inline
basic_nullbuf<Ch, Tr>::basic_nullbuf
(
)
    : count_( 0 )
{
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_nullbuf<Ch, Tr>::pcount
(
) const
{
    return this->count_;
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_nullbuf<Ch, Tr>::xsputn
(
    typename basic_nullbuf<Ch, Tr>::char_type const *  s,
    std::streamsize                                    n
)
{
    return ( s && (n >= 0) ) ? ( (this->count_ += n), n ) : 0;
}

template < typename Ch, class Tr >
inline
typename basic_nullbuf<Ch, Tr>::int_type
basic_nullbuf<Ch, Tr>::overflow
(
    typename basic_nullbuf<Ch, Tr>::int_type  c  // = traits_type::eof()
)
{
    if ( !traits_type::eq_int_type(c, traits_type::eof()) )
    {
        ++this->count_;
    }

    return traits_type::not_eof( c );
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_NULL_STREAM_HPP
