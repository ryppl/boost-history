//  Boost io/value_stream.hpp header file  -----------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_VALUE_STREAM_HPP
#define BOOST_IO_VALUE_STREAM_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/io/streambuf_wrapping.hpp>  // for basic_wrapping_istream, etc.
#include <boost/limits.hpp>                 // for std::numeric_limits

#include <ios>        // for std::streamsize
#include <streambuf>  // for std::basic_streambuf
#include <string>     // for std::char_traits


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_valuebuf;

typedef basic_valuebuf<char>      valuebuf;
typedef basic_valuebuf<wchar_t>  wvaluebuf;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ivaluestream;

typedef basic_ivaluestream<char>      ivaluestream;
typedef basic_ivaluestream<wchar_t>  wivaluestream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ovaluestream;

typedef basic_ovaluestream<char>      ovaluestream;
typedef basic_ovaluestream<wchar_t>  wovaluestream;


//  Constant-value stream-buffer class template declaration  -----------------//

template < typename Ch, class Tr >
class basic_valuebuf
    : public std::basic_streambuf<Ch, Tr>
{
    typedef std::basic_streambuf<Ch, Tr>  base_type;
    typedef basic_valuebuf                self_type;

public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;

    // Other types
    typedef typename Tr::int_type  int_type;
    typedef typename Tr::pos_type  pos_type;
    typedef typename Tr::off_type  off_type;

    // Constructor (use automatically-defined destructor)
    explicit  basic_valuebuf( char_type const &value = char_type() );

    // Accessors
    std::streamsize  gcount() const;

    char_type  gvalue() const;

protected:
    // Overriden virtual functions
    virtual  std::streamsize  showmanyc();
    virtual  std::streamsize  xsgetn( char_type *s, std::streamsize n );
    virtual  int_type         underflow();
    virtual  int_type         uflow();
    virtual  int_type         pbackfail( int_type c = traits_type::eof() );

private:
    // Member data
    std::streamsize  count_;
    int_type         ci_;

};  // boost::io::basic_valuebuf


//  Constant-value stream class template declarations  -----------------------//

// Macro to template the templates!
#define BOOST_PRIVATE_WRAPPER( SuffixF, SuffixB ) \
    template < typename Ch, class Tr > \
    class basic_##SuffixF \
        : public basic_wrapping_##SuffixB< basic_valuebuf<Ch, Tr> > \
    { \
        typedef basic_valuebuf<Ch, Tr>               streambuf_type; \
        typedef basic_wrapping_##SuffixB<streambuf_type>  base_type; \
    public: \
        typedef Ch  char_type; \
        typedef Tr  traits_type; \
        typedef typename Tr::int_type  int_type; \
        typedef typename Tr::pos_type  pos_type; \
        typedef typename Tr::off_type  off_type; \
        explicit  basic_##SuffixF( char_type const &value = char_type() ) \
            : base_type( value ) \
            {} \
        char_type  gvalue() const \
            { return this->rdbuf()->gvalue(); } \
    }

BOOST_PRIVATE_WRAPPER( ivaluestream, istream );
BOOST_PRIVATE_WRAPPER( ovaluestream, ostream );

#undef BOOST_PRIVATE_WRAPPER


//  Constant-value stream-buffer class template member function definitions  -//

template < typename Ch, class Tr >
inline
basic_valuebuf<Ch, Tr>::basic_valuebuf
(
    typename basic_valuebuf<Ch, Tr>::char_type const &  value  // = char_type()
)
    : count_( 0 ), ci_( traits_type::to_int_type(value) )
{
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_valuebuf<Ch, Tr>::gcount
(
) const
{
    return this->count_;
}

template < typename Ch, class Tr >
inline
typename basic_valuebuf<Ch, Tr>::char_type
basic_valuebuf<Ch, Tr>::gvalue
(
) const
{
    return traits_type::to_char_type( this->ci_ );
}

template < typename Ch, class Tr >
inline
std::streamsize
basic_valuebuf<Ch, Tr>::showmanyc
(
)
{
    return std::numeric_limits<std::streamsize>::max();
}

template < typename Ch, class Tr >
std::streamsize
basic_valuebuf<Ch, Tr>::xsgetn
(
    typename basic_valuebuf<Ch, Tr>::char_type *  s,
    std::streamsize                               n
)
{
    if ( s && (n >= 0) )
    {
        traits_type::assign( s, n, this->gvalue() );
        this->count_ += n;
        return n;
    }
    else
    {
        return 0;
    }
}

template < typename Ch, class Tr >
inline
typename basic_valuebuf<Ch, Tr>::int_type
basic_valuebuf<Ch, Tr>::underflow
(
)
{
    return this->ci_;
}

template < typename Ch, class Tr >
inline
typename basic_valuebuf<Ch, Tr>::int_type
basic_valuebuf<Ch, Tr>::uflow
(
)
{
    return ++this->count_, this->ci_;
}

template < typename Ch, class Tr >
typename basic_valuebuf<Ch, Tr>::int_type
basic_valuebuf<Ch, Tr>::pbackfail
(
    typename basic_valuebuf<Ch, Tr>::int_type  c  // = traits_type::eof()
)
{
    if ( traits_type::eq_int_type(c, traits_type::eof())
     || traits_type::eq_int_type(c, this->ci_) )
    {
        return --this->count_, this->ci_;
    }
    else
    {
        return traits_type::eof();
    }
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_VALUE_STREAM_HPP
