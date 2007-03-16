//  Boost io/array_stream.hpp header file  -----------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_ARRAY_STREAM_HPP
#define BOOST_IO_ARRAY_STREAM_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/config.hpp>                 // for BOOST_STATIC_CONSTANT
#include <boost/io/streambuf_wrapping.hpp>  // for basic_wrapping_istream, etc.
#include <boost/noncopyable.hpp>            // for boost::noncopyable

#include <cstddef>    // for std::size_t, NULL
#include <ios>        // for std::streamsize, std::ios_base
#include <streambuf>  // for std::basic_streambuf
#include <string>     // for std::char_traits


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_streambuf;

template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_istream;
template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_ostream;
template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_stream;


//  Array-using streambuf class template declaration  ------------------------//

template < std::size_t N, typename Ch, class Tr >
class basic_array_streambuf
    : public std::basic_streambuf<Ch, Tr>
    , private boost::noncopyable
{
    typedef std::basic_streambuf<Ch, Tr>   base_type;
    typedef basic_array_streambuf          self_type;

public:
    // Template arguments
    BOOST_STATIC_CONSTANT( std::size_t, array_size = N );

    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Other types
    typedef typename Tr::int_type  int_type;
    typedef typename Tr::pos_type  pos_type;
    typedef typename Tr::off_type  off_type;

    // Constructors
    explicit  basic_array_streambuf( std::ios_base::openmode which
     = std::ios_base::in | std::ios_base::out );

    basic_array_streambuf( char_type const *b, char_type const *e,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

    template < typename InputIterator >
    basic_array_streambuf
    (
        InputIterator            b,
        InputIterator            e,
        std::ios_base::openmode  which = std::ios_base::in | std::ios_base::out
    )
    {
        for ( std::size_t i = 0 ; (i < self_type::array_size) && (b != e)
         ; ++i, ++b )
        {
            traits_type::assign( this->array_[i], *b );
        }
        this->setup_buffers( which );
    }

    // Accessors
    char_type *  array_begin();
    char_type *  array_end();

    char_type const *  array_begin() const;
    char_type const *  array_end() const;

    std::streamsize  characters_written() const;
    std::streamsize  characters_read() const;

    std::ios_base::openmode  open_mode() const;

protected:
    // Overriden virtual functions
    virtual  pos_type  seekoff( off_type off, std::ios_base::seekdir way,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

    virtual  pos_type  seekpos( pos_type sp,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

private:
    // Helpers
    void  setup_buffers( std::ios_base::openmode which );

    // Member data
    char_type  array_[ array_size ];

};  // boost::io::basic_array_streambuf


//  Array-using stream class template declarations  --------------------------//

// Macro to template the templates!
#define BOOST_PRIVATE_WRAPPER( SuffixF, SuffixB, ModeC, ModeM ) \
    template < std::size_t N, typename Ch, class Tr > \
    class basic_array_##SuffixF \
        : public basic_wrapping_##SuffixB< basic_array_streambuf<N, Ch, Tr> > \
    { \
        typedef basic_array_streambuf<N, Ch, Tr>     streambuf_type; \
        typedef basic_wrapping_##SuffixB<streambuf_type>  base_type; \
        typedef std::ios_base::openmode                    openmode; \
    public: \
        BOOST_STATIC_CONSTANT( std::size_t, array_size = N ); \
        typedef Ch  char_type; \
        typedef Tr  traits_type; \
        typedef typename Tr::int_type  int_type; \
        typedef typename Tr::pos_type  pos_type; \
        typedef typename Tr::off_type  off_type; \
        explicit  basic_array_##SuffixF( openmode which = ModeC ) \
            : base_type( which | ModeM ) {} \
        basic_array_##SuffixF( char_type const *b, char_type const *e, \
            openmode which = ModeC ) : base_type( b, e, which | ModeM ) {} \
        template < typename InputIterator > \
        basic_array_##SuffixF( InputIterator b, InputIterator e, \
            openmode which = ModeC ) : base_type( b, e, which | ModeM ) {} \
        char_type *  array_begin() \
            { return this->rdbuf()->array_begin(); } \
        char_type *  array_end() \
            { return this->rdbuf()->array_end(); } \
        char_type const *  array_begin() const \
            { return this->rdbuf()->array_begin(); } \
        char_type const *  array_end() const \
            { return this->rdbuf()->array_end(); } \
    }

BOOST_PRIVATE_WRAPPER( istream, istream, std::ios_base::in, std::ios_base::in );
BOOST_PRIVATE_WRAPPER( ostream, ostream, std::ios_base::out,
 std::ios_base::out );
BOOST_PRIVATE_WRAPPER( stream, iostream, (std::ios_base::in
 | std::ios_base::out), openmode(0) );

#undef BOOST_PRIVATE_WRAPPER


//  Array-using streambuf class template member function definitions  --------//

template < std::size_t N, typename Ch, class Tr >
inline
basic_array_streambuf<N, Ch, Tr>::basic_array_streambuf
(
   std::ios_base::openmode  which  // = std::ios_base::in | std::ios_base::out
)
{
    traits_type::assign( this->array_, self_type::array_size, char_type() );
    this->setup_buffers( which );
}

template < std::size_t N, typename Ch, class Tr >
inline
basic_array_streambuf<N, Ch, Tr>::basic_array_streambuf
(
    typename basic_array_streambuf<N, Ch, Tr>::char_type const *  b,
    typename basic_array_streambuf<N, Ch, Tr>::char_type const *  e,
    std::ios_base::openmode                                       which
      // = std::ios_base::in | std::ios_base::out
)
{
    traits_type::copy( this->array_, b, (e - b) );
    this->setup_buffers( which );
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

template < std::size_t N, typename Ch, class Tr >
inline
std::streamsize
basic_array_streambuf<N, Ch, Tr>::characters_written
(
) const
{
    return this->pptr() ? ( this->pptr() - this->pbase() ) : 0;
}

template < std::size_t N, typename Ch, class Tr >
inline
std::streamsize
basic_array_streambuf<N, Ch, Tr>::characters_read
(
) const
{
    return this->gptr() ? ( this->gptr() - this->eback() ) : 0;
}

template < std::size_t N, typename Ch, class Tr >
inline
std::ios_base::openmode
basic_array_streambuf<N, Ch, Tr>::open_mode
(
) const
{
    using std::ios_base;

    ios_base::openmode const  zero = static_cast<ios_base::openmode>( 0 );

    return ( this->gptr() ? ios_base::in : zero ) | ( this->pptr()
     ? ios_base::out : zero );
}

template < std::size_t N, typename Ch, class Tr >
typename basic_array_streambuf<N, Ch, Tr>::pos_type
basic_array_streambuf<N, Ch, Tr>::seekoff
(
    typename basic_array_streambuf<N, Ch, Tr>::off_type  off,
    std::ios_base::seekdir                               way,
    std::ios_base::openmode                              which
      // = std::ios_base::in | std::ios_base::out
)
{
    using std::ios_base;

    char * const    old_gptr = this->gptr();
    char * const    old_pptr = this->pptr();
    bool const      do_input = ( (which & ios_base::in) != 0 );
    bool const      do_output = ( (which & ios_base::out) != 0 );
    pos_type const  invalid( static_cast<off_type>(-1) );

    pos_type  answer = invalid;

    if ( do_input )
    {
        off_type  newoff, newindex;

        if ( NULL == old_gptr )  goto bail;

        switch ( way )
        {
        case ios_base::beg :
            newoff = 0;
            break;
        case ios_base::end :
            newoff = this->egptr() - this->eback();
            break;
        case ios_base::cur :
            newoff = this->gptr() - this->eback();
            break;
        default :
            goto bail;
        }
        newindex = newoff + off;

        if ( newindex < off_type(0) )  goto bail;
        if ( newindex > off_type(self_type::array_size) )  goto bail;

        this->gbump( newindex - off_type(this->characters_read()) );
        answer = pos_type( newindex );
    }

    if ( do_output )
    {
        off_type  newoff, newindex;

        if ( NULL == old_pptr )  goto bail;

        switch ( way )
        {
        case ios_base::beg :
            newoff = 0;
            break;
        case ios_base::end :
            newoff = this->epptr() - this->pbase();
            break;
        case ios_base::cur :
            newoff = this->pptr() - this->pbase();
            if ( !do_input ) break;  // can't do both areas with "cur"!
        default :
            goto bail;
        }
        newindex = newoff + off;

        if ( newindex < off_type(0) )  goto bail;
        if ( newindex > off_type(self_type::array_size) )  goto bail;

        // make sure answers are consistent with both areas if neccessary
        if ( do_input && (pos_type( newindex ) != answer) )  goto bail;

        this->pbump( newindex - off_type(this->characters_written()) );
        answer = pos_type( newindex );
    }

    // At this point, either sole area successfully changed, both areas
    // successfully changed to the same position, or neither area changed.
    return answer;

bail:
    if ( old_gptr )
    {
        this->setg( this->array_, old_gptr, this->array_
         + self_type::array_size );
    }

    if ( old_pptr )
    {
        this->setp( this->array_, this->array_ + self_type::array_size );
        this->pbump( old_pptr - this->array_ );
    }

    return invalid;
}

template < std::size_t N, typename Ch, class Tr >
inline
typename basic_array_streambuf<N, Ch, Tr>::pos_type
basic_array_streambuf<N, Ch, Tr>::seekpos
(
    typename basic_array_streambuf<N, Ch, Tr>::pos_type  sp,
    std::ios_base::openmode                              which
      // = std::ios_base::in | std::ios_base::out
)
{
    return this->self_type::seekoff( off_type(sp), std::ios_base::beg, which );
}

template < std::size_t N, typename Ch, class Tr >
inline
void
basic_array_streambuf<N, Ch, Tr>::setup_buffers
(
    std::ios_base::openmode  which
)
{
    using std::ios_base;

    if ( (which & ios_base::in) != 0 )
    {
        this->setg( this->array_, this->array_, this->array_
         + self_type::array_size );
    }

    if ( (which & ios_base::out) != 0 )
    {
        this->setp( this->array_, this->array_ + self_type::array_size );
    }
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_ARRAY_STREAM_HPP
