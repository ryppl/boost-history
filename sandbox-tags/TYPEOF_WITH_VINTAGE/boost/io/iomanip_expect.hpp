//  Boost io/iomanip_expect.hpp header file  ---------------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_IOMANIP_EXPECT_HPP
#define BOOST_IO_IOMANIP_EXPECT_HPP

#include <boost/io_fwd.hpp>  // self include

#include <ios>      // for std::streamsize and std::ios_base
#include <istream>  // for std::basic_istream
#include <locale>   // for std::locale, std::ctype, and std::use_facet
#include <string>   // for std::basic_string


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < typename Ch >
    class expect_cstr;

template < >
    class expect_cstr< char >;

template < typename Ch >
    class expect_ch;

template < >
    class expect_ch< char >;

template < typename Ch, class Tr, class Al >
    class expect_str;

typedef expect_ch<char>  expect_char;

template < typename Ch >
    expect_cstr<Ch>
    expect( Ch const *s );

template < typename Ch >
    expect_ch<Ch>
    expect( Ch c, std::streamsize count = 1 );

template < typename Ch, class Tr, class Al >
    expect_str<Ch, Tr, Al>
    expect( std::basic_string<Ch, Tr, Al> const &s );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &
    operator >>( std::basic_istream<Ch, Tr> &s, expect_cstr<Ch> const &e );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &
    operator >>( std::basic_istream<Ch, Tr> &s, expect_cstr<char> const &e );

template < class Tr >
    std::basic_istream<char, Tr> &
    operator >>( std::basic_istream<char, Tr> &s, expect_cstr<char> const &e );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &
    operator >>( std::basic_istream<Ch, Tr> &s, expect_ch<Ch> const &e );

template < typename Ch, class Tr >
    std::basic_istream<Ch, Tr> &
    operator >>( std::basic_istream<Ch, Tr> &s, expect_ch<char> const &e );

template < class Tr >
    std::basic_istream<char, Tr> &
    operator >>( std::basic_istream<char, Tr> &s, expect_ch<char> const &e );

template < typename Ch, class Tr, class Al >
    std::basic_istream<Ch, Tr> &
    operator >>( std::basic_istream<Ch, Tr> &s, expect_str<Ch, Tr, Al> const
     &e );


//  Implementation detail stuff  ---------------------------------------------//

namespace detail
{

template < typename Ch, class Tr >
struct cstr_element_generator
{
    Ch const *  s_;

    explicit  cstr_element_generator( Ch const *s )  : s_( s )  {}

    operator bool() const
    { return this->s_ && !Tr::eq( Ch(), *this->s_ ); }

    Ch  operator ()()
    { return static_cast<bool>( *this ) ? *this->s_++ : Ch(); }

};  // boost::io::detail::cstr_element_generator

template < class Tr, typename Ch >
cstr_element_generator<Ch, Tr>  make_ceg( Ch const *s )
{
    return cstr_element_generator<Ch, Tr>( s );
}

template < typename Ch >
struct cs_element_generator
{
    char const *            s_;
    std::ctype<Ch> const &  f_;

    cs_element_generator( char const *s, std::locale const &l )
        : s_( s ), f_( std::use_facet< std::ctype<Ch> >(l) )
        {}

    operator bool() const
    { return this->s_ && *this->s_; }

    Ch  operator ()()
    { return static_cast<bool>( *this ) ? f_.widen( *this->s_++ ) : Ch(); }

};  // boost::io::detail::cs_element_generator

template < typename Ch >
cs_element_generator<Ch>  make_ceg2( char const *s, std::locale const &l )
{
    return cs_element_generator<Ch>( s, l );
}

template < typename Ch >
struct char_repeat_generator
{
    Ch               c_;
    std::streamsize  n_;

    char_repeat_generator( Ch c, std::streamsize n )  : c_( c ), n_( n )  {}

        operator bool() const  { return 0 < this->n_; }
    Ch  operator ()()          { return --this->n_, this->c_; }

};  // boost::io::detail::char_repeat_generator

template < typename Ch >
char_repeat_generator<Ch>  make_crg( Ch c, std::streamsize n )
{
    return char_repeat_generator<Ch>( c, n );
}

template < class Iter, typename Ch >
struct iter_element_generator
{
    Iter  b_, e_;

    iter_element_generator( Iter b, Iter e )  : b_( b ), e_( e )  {}

    operator bool() const
    { return this->b_ != this->e_; }

    Ch  operator ()()
    { return static_cast<bool>( *this ) ? *this->b_++ : Ch(); }

};  // boost::io::detail::iter_element_generator

template < typename Ch, class Iter >
iter_element_generator<Iter, Ch>  make_ieg( Iter b, Iter e )
{
    return iter_element_generator<Iter, Ch>( b, e );
}

template < typename Ch, class Tr, class Gen >
void  do_expect_core( std::basic_istream<Ch, Tr> &is, Gen g )
{
    while ( is && g )
    {
        typename Tr::int_type const  i1 = is.peek();

        if ( !Tr::eq_int_type(Tr::eof(), i1) )
        {
            Ch const  c1 = Tr::to_char_type( i1 );
            Ch const  c2 = g();

            if ( Tr::eq(c1, c2) )
            {
                is.ignore();
                continue;
            }
        }

        is.setstate( std::ios_base::failbit );
        break;
    }
}

template < typename Ch >
class expect_cstr_base
{
public:
    // Template argument
    typedef Ch  char_type;

    // Accessor
    char_type const *  expected_string() const  { return this->s_; }

    // Operator
    template < class Tr >
    void  operator ()( std::basic_istream<char_type, Tr> &is ) const
    { do_expect_core( is, make_ceg<Tr>(this->expected_string()) ); }

protected:
    // Lifetime management
    explicit  expect_cstr_base( char_type const *s )  : s_( s )  {}

private:
    // Member datum
    char_type const *  s_;

};  // boost::io::detail::expect_cstr_base

template < typename Ch >
class expect_ch_base
{
public:
    // Template argument
    typedef Ch  char_type;

    // Accessors
    char_type        expected_character() const   { return this->c_; }
    std::streamsize  expected_occurances() const  { return this->n_; }

    // Operator
    template < class Tr >
    void  operator ()( std::basic_istream<char_type, Tr> &is ) const
    {
        do_expect_core( is, make_crg(this->expected_character(),
         this->expected_occurances()) );
    }

protected:
    // Lifetime management
    expect_ch_base( char_type c, std::streamsize count )
        : c_( c ), n_( count )
        {}

private:
    // Member data
    char_type        c_;
    std::streamsize  n_;

};  // boost::io::detail::expect_ch_base

}  // namespace detail


//  Expected-string (C) I/O manipulator class template declarations  ---------//

template < typename Ch >
class expect_cstr
    : private detail::expect_cstr_base< Ch >
{
    typedef detail::expect_cstr_base<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    explicit  expect_cstr( char_type const *s );

    // Accessor
    using base_type::expected_string;

    // Operator
    template < class Tr >
    void  operator ()( std::basic_istream<char_type, Tr> &is ) const;

};  // boost::io::expect_cstr

template < >
class expect_cstr< char >
    : private detail::expect_cstr_base< char >
{
    typedef detail::expect_cstr_base<char>  base_type;

public:
    // Template argument
    typedef char  char_type;

    // Lifetime management
    explicit  expect_cstr( char_type const *s );

    // Accessor
    using base_type::expected_string;

    // Operators
    template < class Tr >
    void  operator ()( std::basic_istream<char_type, Tr> &is ) const;

    template < typename Ch, class Tr >
    void  operator ()( std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::expect_cstr<char>


//  Expected-character I/O manipulator class template declarations  ----------//

template < typename Ch >
class expect_ch
    : private detail::expect_ch_base< Ch >
{
    typedef detail::expect_ch_base<Ch>  base_type;

public:
    // Template argument
    typedef Ch  char_type;

    // Lifetime management
    expect_ch( char_type c, std::streamsize count );

    // Accessors
    using base_type::expected_character;
    using base_type::expected_occurances;

    // Operator
    template < class Tr >
    void  operator ()( std::basic_istream<char_type, Tr> &is ) const;

};  // boost::io::expect_ch

template < >
class expect_ch< char >
    : private detail::expect_ch_base< char >
{
    typedef detail::expect_ch_base<char>  base_type;

public:
    // Template argument
    typedef char  char_type;

    // Lifetime management
    expect_ch( char_type c, std::streamsize count );

    // Accessors
    using base_type::expected_character;
    using base_type::expected_occurances;

    // Operators
    template < class Tr >
    void  operator ()( std::basic_istream<char_type, Tr> &is ) const;

    template < typename Ch, class Tr >
    void  operator ()( std::basic_istream<Ch, Tr> &is ) const;

};  // boost::io::expect_ch<char>


//  Expected-string (C++) I/O manipulator class template declaration  --------//

template < typename Ch, class Tr, class Al >
class expect_str
{
public:
    // Template arguments
    typedef Ch  char_type;
    typedef Tr  traits_type;
    typedef Al  allocator_type;

    // Other types
    typedef std::basic_string<Ch, Tr, Al>   string_type;
    typedef std::basic_istream<Ch, Tr>     istream_type;

    // Lifetime management
    explicit  expect_str( string_type const &s );

    // Accessor
    string_type  expected_string() const;

    // Operator
    void  operator ()( istream_type &is ) const;

private:
    // Member datum
    string_type  s_;

};  // boost::io::expect_str


//  Expected-string (C) I/O manipulator member function definitions  ---------//

template < typename Ch >
inline
expect_cstr<Ch>::expect_cstr
(
    char_type const *  s
)
    : base_type( s )
{
}

template < typename Ch >
template < class Tr >
inline
void
expect_cstr<Ch>::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    this->base_type::operator ()( is );
}

inline
expect_cstr<char>::expect_cstr
(
    char_type const *  s
)
    : base_type( s )
{
}

template < class Tr >
inline
void
expect_cstr<char>::operator ()
(
    std::basic_istream<char, Tr> &  is
) const
{
    this->base_type::operator ()( is );
}

template < typename Ch, class Tr >
inline
void
expect_cstr<char>::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    detail::do_expect_core( is, detail::make_ceg2<Ch>(this->expected_string(),
     is.getloc()) );
}


//  Expected-character I/O manipulator member function definitions  ----------//

template < typename Ch >
inline
expect_ch<Ch>::expect_ch
(
    char_type        c,
    std::streamsize  count
)
    : base_type( c, count )
{
}

template < typename Ch >
template < class Tr >
inline
void
expect_ch<Ch>::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    this->base_type::operator ()( is );
}

inline
expect_ch<char>::expect_ch
(
    char_type        c,
    std::streamsize  count
)
    : base_type( c, count )
{
}

template < class Tr >
inline
void
expect_ch<char>::operator ()
(
    std::basic_istream<char, Tr> &  is
) const
{
    this->base_type::operator ()( is );
}

template < typename Ch, class Tr >
inline
void
expect_ch<char>::operator ()
(
    std::basic_istream<Ch, Tr> &  is
) const
{
    detail::do_expect_core( is, detail::make_crg(is.widen(
     this->expected_character() ), this->expected_occurances()) );
}


//  Expected-string (C++) I/O manipulator member function definitions  -------//

template < typename Ch, class Tr, class Al >
inline
expect_str<Ch, Tr, Al>::expect_str
(
    string_type const &  s
)
    : s_( s )
{
}

template < typename Ch, class Tr, class Al >
inline
typename expect_str<Ch, Tr, Al>::string_type
expect_str<Ch, Tr, Al>::expected_string
(
) const
{
    return s_;
}

template < typename Ch, class Tr, class Al >
inline
void
expect_str<Ch, Tr, Al>::operator ()
(
    istream_type &  is
) const
{
    detail::do_expect_core( is, detail::make_ieg<char_type>(this->s_.begin(),
     this->s_.end()) );
}


//  I/O-manipulator function (template) definitions  -------------------------//

template < typename Ch >
inline
expect_cstr<Ch>
expect
(
    Ch const *  s
)
{
    return expect_cstr<Ch>( s );
}

template < typename Ch >
inline
expect_ch<Ch>
expect
(
    Ch               c,
    std::streamsize  count  // = 1
)
{
    return expect_ch<Ch>( c, count );
}

template < typename Ch, class Tr, class Al >
inline
expect_str<Ch, Tr, Al>
expect
(
    std::basic_string<Ch, Tr, Al> const &  s
)
{
    return expect_str<Ch, Tr, Al>( s );
}


//  I/O-manipulator operator function definitions  ---------------------------//

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  s,
    expect_cstr<Ch> const &       e
)
{
    return e( s ), s;
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  s,
    expect_cstr<char> const &     e
)
{
    return e( s ), s;
}

template < class Tr >
inline
std::basic_istream<char, Tr> &
operator >>
(
    std::basic_istream<char, Tr> &  s,
    expect_cstr<char> const &       e
)
{
    return e( s ), s;
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  s,
    expect_ch<Ch> const &         e
)
{
    return e( s ), s;
}

template < typename Ch, class Tr >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &  s,
    expect_ch<char> const &       e
)
{
    return e( s ), s;
}

template < class Tr >
inline
std::basic_istream<char, Tr> &
operator >>
(
    std::basic_istream<char, Tr> &  s,
    expect_ch<char> const &         e
)
{
    return e( s ), s;
}

template < typename Ch, class Tr, class Al >
inline
std::basic_istream<Ch, Tr> &
operator >>
(
    std::basic_istream<Ch, Tr> &    s,
    expect_str<Ch, Tr, Al> const &  e
)
{
    return e( s ), s;
}


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_IOMANIP_EXPECT_HPP
