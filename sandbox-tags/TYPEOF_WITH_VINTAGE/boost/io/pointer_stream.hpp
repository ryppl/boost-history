//  Boost io/pointer_stream.hpp header file  ---------------------------------//

//  Copyright 2003-2004 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_POINTER_STREAM_HPP
#define BOOST_IO_POINTER_STREAM_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/io/streambuf_wrapping.hpp>  // for basic_wrapping_istream, etc.

#include <cstddef>    // for NULL
#include <ios>        // for std::streamsize, std::ios_base
#include <streambuf>  // for std::basic_streambuf
#include <string>     // for std::char_traits


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_pointerbuf;

typedef basic_pointerbuf<char>      pointerbuf;
typedef basic_pointerbuf<wchar_t>  wpointerbuf;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_constpointerbuf;

typedef basic_constpointerbuf<char>      constpointerbuf;
typedef basic_constpointerbuf<wchar_t>  wconstpointerbuf;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ipointerstream;

typedef basic_ipointerstream<char>      ipointerstream;
typedef basic_ipointerstream<wchar_t>  wipointerstream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_opointerstream;

typedef basic_opointerstream<char>      opointerstream;
typedef basic_opointerstream<wchar_t>  wopointerstream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_pointerstream;

typedef basic_pointerstream<char>      pointerstream;
typedef basic_pointerstream<wchar_t>  wpointerstream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_iconstpointerstream;

typedef basic_iconstpointerstream<char>      iconstpointerstream;
typedef basic_iconstpointerstream<wchar_t>  wiconstpointerstream;


//  Pointer-using stream-buffer class template declarations  -----------------//

template < typename Ch, class Tr >
class basic_pointerbuf
    : public std::basic_streambuf<Ch, Tr>
{
    typedef std::basic_streambuf<Ch, Tr>   base_type;
    typedef basic_pointerbuf               self_type;

public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Other types
    typedef typename Tr::int_type  int_type;
    typedef typename Tr::pos_type  pos_type;
    typedef typename Tr::off_type  off_type;

    // Lifetime management (use automatically-defined destructor)
    basic_pointerbuf( char_type *b, char_type *e,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

    // Accessors
    char_type *  begin_pointer() const;
    char_type *  end_pointer() const;

    std::streamsize  pcount() const;
    std::streamsize  gcount() const;

    std::ios_base::openmode  open_mode() const;

protected:
    // Overriden virtual functions
    virtual  base_type *  setbuf( char_type *s, std::streamsize n );

    virtual  pos_type  seekoff( off_type off, std::ios_base::seekdir way,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

    virtual  pos_type  seekpos( pos_type sp,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

    virtual  int_type  pbackfail( int_type c = traits_type::eof() );

};  // boost::io::basic_pointerbuf

template < typename Ch, class Tr >
class basic_constpointerbuf
    : public std::basic_streambuf<Ch, Tr>
{
    typedef std::basic_streambuf<Ch, Tr>   base_type;
    typedef basic_constpointerbuf          self_type;

public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Other types
    typedef typename Tr::int_type  int_type;
    typedef typename Tr::pos_type  pos_type;
    typedef typename Tr::off_type  off_type;

    // Lifetime management (use automatically-defined destructor)
    basic_constpointerbuf( char_type const *b, char_type const *e );

    // Extra buffer management
    self_type *  pubsetbuf( char_type const *s, std::streamsize n );

    // Accessors
    char_type const *  begin_pointer() const;
    char_type const *  end_pointer() const;

    std::streamsize  gcount() const;

protected:
    // Overriden virtual functions
    virtual  base_type *  setbuf( char_type *s, std::streamsize n );

    virtual  pos_type  seekoff( off_type off, std::ios_base::seekdir way,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

    virtual  pos_type  seekpos( pos_type sp,
     std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );

};  // boost::io::basic_constpointerbuf


//  Pointer-using stream class template declarations  ------------------------//

// Macro to template the templates!
#define BOOST_PRIVATE_WRAPPER( SuffixF, SuffixB, ModeC, ModeM ) \
    template < typename Ch, class Tr > \
    class basic_##SuffixF \
        : public basic_wrapping_##SuffixB< basic_pointerbuf<Ch, Tr> > \
    { \
        typedef basic_pointerbuf<Ch, Tr>             streambuf_type; \
        typedef basic_wrapping_##SuffixB<streambuf_type>  base_type; \
        typedef std::ios_base::openmode                    openmode; \
    public: \
        typedef Ch  char_type; \
        typedef Tr  traits_type; \
        typedef typename Tr::int_type  int_type; \
        typedef typename Tr::pos_type  pos_type; \
        typedef typename Tr::off_type  off_type; \
        basic_##SuffixF( char_type *b, char_type *e, \
            openmode which = ModeC ) : base_type( b, e, which | ModeM ) {} \
        char_type *  begin_pointer() const \
            { return this->rdbuf()->begin_pointer(); } \
        char_type *  end_pointer() const \
            { return this->rdbuf()->end_pointer(); } \
    }

BOOST_PRIVATE_WRAPPER( ipointerstream, istream, std::ios_base::in,
 std::ios_base::in );
BOOST_PRIVATE_WRAPPER( opointerstream, ostream, std::ios_base::out,
 std::ios_base::out );
BOOST_PRIVATE_WRAPPER( pointerstream, iostream, (std::ios_base::in
 | std::ios_base::out), openmode(0) );

#undef BOOST_PRIVATE_WRAPPER

template < typename Ch, class Tr >
class basic_iconstpointerstream
    : public basic_wrapping_istream< basic_constpointerbuf<Ch, Tr> >
{
    typedef basic_constpointerbuf<Ch, Tr>      streambuf_type;
    typedef basic_wrapping_istream<streambuf_type>  base_type;

public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Other types
    typedef typename Tr::int_type  int_type;
    typedef typename Tr::pos_type  pos_type;
    typedef typename Tr::off_type  off_type;

    // Lifetime management (use automatically-defined destructor)
    basic_iconstpointerstream( char_type const *b, char_type const *e )
        : base_type( b, e )
    {
    }

    // Accessors
    char_type const *  begin_pointer() const
    {
        return this->rdbuf()->begin_pointer();
    }

    char_type const *  end_pointer() const
    {
        return this->rdbuf()->end_pointer();
    }

};  // boost::io::basic_iconstpointerstream


//  Pointer-using stream-buffer class template member function definitions  --//

template < typename Ch, class Tr >
inline
basic_pointerbuf<Ch, Tr>::basic_pointerbuf
(
    typename basic_pointerbuf<Ch, Tr>::char_type *  b,
    typename basic_pointerbuf<Ch, Tr>::char_type *  e,
    std::ios_base::openmode                         which
      // = std::ios_base::in | std::ios_base::out
)
{
    using std::ios_base;

    if ( (which & ios_base::in) != 0 )
    {
        this->setg( b, b, e );
    }

    if ( (which & ios_base::out) != 0 )
    {
        this->setp( b, e );
    }
}

template < typename Ch, class Tr >
inline
typename basic_pointerbuf<Ch, Tr>::char_type *
basic_pointerbuf<Ch, Tr>::begin_pointer
(
) const
{
    return this->gptr() ? this->eback() : this->pbase();
}

template <typename Ch, class Tr >
inline
typename basic_pointerbuf<Ch, Tr>::char_type *
basic_pointerbuf<Ch, Tr>::end_pointer
(
) const
{
    return this->pptr() ? this->epptr() : this->egptr();
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_pointerbuf<Ch, Tr>::pcount
(
) const
{
    return this->pptr() ? ( this->pptr() - this->pbase() ) : 0;
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_pointerbuf<Ch, Tr>::gcount
(
) const
{
    return this->gptr() ? ( this->gptr() - this->eback() ) : 0;
}

template < typename Ch, class Tr >
inline
std::ios_base::openmode
basic_pointerbuf<Ch, Tr>::open_mode
(
) const
{
    using std::ios_base;

    ios_base::openmode const  zero = static_cast<ios_base::openmode>( 0 );

    return ( this->gptr() ? ios_base::in : zero ) | ( this->pptr()
     ? ios_base::out : zero );
}

template < typename Ch, class Tr >
inline
std::basic_streambuf<Ch, Tr> *
basic_pointerbuf<Ch, Tr>::setbuf
(
    typename basic_pointerbuf<Ch, Tr>::char_type *  s,
    std::streamsize                                 n
)
{
    if ( this->gptr() )
    {
        this->setg( s, s, s + n );
    }

    if ( this->pptr() )
    {
        this->setp( s, s + n );
    }

    return this;
}

template < typename Ch, class Tr >
typename basic_pointerbuf<Ch, Tr>::pos_type
basic_pointerbuf<Ch, Tr>::seekoff
(
    typename basic_pointerbuf<Ch, Tr>::off_type  off,
    std::ios_base::seekdir                       way,
    std::ios_base::openmode                      which
      // = std::ios_base::in | std::ios_base::out
)
{
    using std::ios_base;

    char_type * const    old_begin = this->begin_pointer();
    char_type * const    old_end = this->end_pointer();
    char_type * const    old_gptr = this->gptr();
    char_type * const    old_pptr = this->pptr();
    bool const           do_input = ( (which & ios_base::in) != 0 );
    bool const           do_output = ( (which & ios_base::out) != 0 );
    pos_type const       invalid( static_cast<off_type>(-1) );

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
        if ( newindex > off_type(old_end - old_begin) )  goto bail;

        this->gbump( newindex - off_type(this->gcount()) );
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
            // Possible FALL-THROUGH
        default :
            goto bail;
        }
        newindex = newoff + off;

        if ( newindex < off_type(0) )  goto bail;
        if ( newindex > off_type(old_end - old_begin) )  goto bail;

        // make sure answers are consistent with both areas if neccessary
        if ( do_input && (pos_type( newindex ) != answer) )  goto bail;

        this->pbump( newindex - off_type(this->pcount()) );
        answer = pos_type( newindex );
    }

    // At this point, either the sole area successfully changed, both areas
    // successfully changed to the same position, or neither area changed.
    return answer;

bail:
    if ( old_gptr )
    {
        this->setg( old_begin, old_gptr, old_end );
    }

    if ( old_pptr )
    {
        this->setp( old_begin, old_end );
        this->pbump( old_pptr - old_begin );
    }

    return invalid;
}

template < typename Ch, class Tr >
inline
typename basic_pointerbuf<Ch, Tr>::pos_type
basic_pointerbuf<Ch, Tr>::seekpos
(
    typename basic_pointerbuf<Ch, Tr>::pos_type  sp,
    std::ios_base::openmode                      which
      // = std::ios_base::in | std::ios_base::out
)
{
    return this->self_type::seekoff( off_type(sp), std::ios_base::beg, which );
}

template < typename Ch, class Tr >
inline
typename basic_pointerbuf<Ch, Tr>::int_type
basic_pointerbuf<Ch, Tr>::pbackfail
(
    typename basic_pointerbuf<Ch, Tr>::int_type  c  // = traits_type::eof()
)
{
    if ( this->gptr() > this->eback() )  // also covers no-get-area case
    {
        this->gbump( -1 );

        if ( !traits_type::eq_int_type(c, traits_type::eof()) )
        {
            traits_type::assign( *this->gptr(), traits_type::to_char_type(c) );
        }

        return traits_type::to_int_type( *this->gptr() );
    }

    return traits_type::eof();
}

template < typename Ch, class Tr >
inline
basic_constpointerbuf<Ch, Tr>::basic_constpointerbuf
(
    typename basic_constpointerbuf<Ch, Tr>::char_type const *  b,
    typename basic_constpointerbuf<Ch, Tr>::char_type const *  e
)
{
    char_type * const  mb = const_cast<char_type *>( b );

    this->setg( mb, mb, const_cast<char_type *>(e) );
}

template < typename Ch, class Tr >
inline
basic_constpointerbuf<Ch, Tr> *
basic_constpointerbuf<Ch, Tr>::pubsetbuf
(
    typename basic_constpointerbuf<Ch, Tr>::char_type const *  s,
    std::streamsize                                            n
)
{
    return static_cast<self_type *>(
     this->base_type::pubsetbuf(const_cast<char_type *>( s ), n) );
}

template < typename Ch, class Tr >
inline
typename basic_constpointerbuf<Ch, Tr>::char_type const *
basic_constpointerbuf<Ch, Tr>::begin_pointer
(
) const
{
    return const_cast<char_type const *>( this->eback() );
}

template <typename Ch, class Tr >
inline
typename basic_constpointerbuf<Ch, Tr>::char_type const *
basic_constpointerbuf<Ch, Tr>::end_pointer
(
) const
{
    return const_cast<char_type const *>( this->egptr() );
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_constpointerbuf<Ch, Tr>::gcount
(
) const
{
    return static_cast<std::streamsize>( this->gptr() - this->eback() );
}

template < typename Ch, class Tr >
inline
std::basic_streambuf<Ch, Tr> *
basic_constpointerbuf<Ch, Tr>::setbuf
(
    typename basic_constpointerbuf<Ch, Tr>::char_type *  s,
    std::streamsize                                      n
)
{
    this->setg( s, s, s + n );
    return this;
}

template < typename Ch, class Tr >
typename basic_constpointerbuf<Ch, Tr>::pos_type
basic_constpointerbuf<Ch, Tr>::seekoff
(
    typename basic_constpointerbuf<Ch, Tr>::off_type  off,
    std::ios_base::seekdir                            way,
    std::ios_base::openmode                           which
      // = std::ios_base::in | std::ios_base::out
)
{
    using std::ios_base;

    char_type * const  begin_ptr = this->eback();
    char_type * const  end_ptr = this->egptr();
    char_type * const  cur_ptr = this->gptr();
    pos_type const     invalid( static_cast<off_type>(-1) );
    pos_type           answer = invalid;

    if ( char_type * const  base_ptr = (ios_base::beg == way) ? begin_ptr
     : (ios_base::end == way) ? end_ptr : (ios_base::cur == way) ? cur_ptr
     : NULL )
    {
        if ( (which & ios_base::in) != 0 )
        {
            off_type const  new_index = off_type( base_ptr - begin_ptr ) + off;

            if ( (new_index >= off_type( 0 )) && (new_index <= off_type( end_ptr
             - begin_ptr )) )
            {
                this->gbump( new_index - off_type(cur_ptr - begin_ptr) );
                answer = pos_type( new_index );
            }
        }
    }

    return answer;
}

template < typename Ch, class Tr >
inline
typename basic_constpointerbuf<Ch, Tr>::pos_type
basic_constpointerbuf<Ch, Tr>::seekpos
(
    typename basic_constpointerbuf<Ch, Tr>::pos_type  sp,
    std::ios_base::openmode                           which
      // = std::ios_base::in | std::ios_base::out
)
{
    return this->self_type::seekoff( off_type(sp), std::ios_base::beg, which );
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_POINTER_STREAM_HPP
