//  Boost io/streambuf_wrapping.hpp header file  -----------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

#ifndef BOOST_IO_STREAMBUF_WRAPPING_HPP
#define BOOST_IO_STREAMBUF_WRAPPING_HPP

#include <boost/io_fwd.hpp>  // self include

#include <boost/utility/base_from_member.hpp>  // for boost::base_from_member

#include <ios>      // for std::basic_ios
#include <istream>  // for std::basic_istream and std::basic_iostream
#include <ostream>  // for std::basic_ostream


namespace boost
{
namespace io
{


//  Forward declarations  ----------------------------------------------------//

template < class StreamBuf >
    class basic_wrapping_istream;

template < class StreamBuf >
    class basic_wrapping_ostream;

template < class StreamBuf >
    class basic_wrapping_iostream;


//  Implementation detail stuff  ---------------------------------------------//

namespace detail
{

    // Base class (template) for common member functions and data
    template < class StreamBuf >
    class basic_wrapping_ios
        : private ::boost::base_from_member< StreamBuf >
        , public virtual ::std::basic_ios< typename StreamBuf::char_type,
            typename StreamBuf::traits_type >
    {
        typedef ::boost::base_from_member< StreamBuf >  pbase_type;

        typedef ::std::basic_ios< typename StreamBuf::char_type,
          typename StreamBuf::traits_type >  vbase_type;

    public:
        typedef StreamBuf  streambuf_type;

        typedef typename StreamBuf::char_type  char_type;
        typedef typename StreamBuf::int_type    int_type;

        typedef typename StreamBuf::pos_type        pos_type;
        typedef typename StreamBuf::off_type        off_type;
        typedef typename StreamBuf::traits_type  traits_type;

        streambuf_type *  rdbuf()
        { return &this->pbase_type::member; }

        streambuf_type const *  rdbuf() const
        { return &this->pbase_type::member; }

        bool  is_using_internal_streambuf() const
        { return this->rdbuf() == this->vbase_type::rdbuf(); }

    protected:
        explicit  basic_wrapping_ios( streambuf_type const &s )
            : pbase_type( s )
            { this->vbase_type::init( &this->pbase_type::member ); }

        basic_wrapping_ios()
            : pbase_type()
            { this->vbase_type::init( &this->pbase_type::member ); }

        template < typename T1 >
        explicit  basic_wrapping_ios( T1 x1 )
            : pbase_type( x1 )
            { this->vbase_type::init( &this->pbase_type::member ); }

        template < typename T1, typename T2 >
        basic_wrapping_ios( T1 x1, T2 x2 )
            : pbase_type( x1, x2 )
            { this->vbase_type::init( &this->pbase_type::member ); }

        template < typename T1, typename T2, typename T3 >
        basic_wrapping_ios( T1 x1, T2 x2, T3 x3 )
            : pbase_type( x1, x2, x3 )
            { this->vbase_type::init( &this->pbase_type::member ); }

    };  // boost::io::detail::basic_wrapping_ios

}  // namespace detail


//  Streambuf-wrapping stream class template declarations  -------------------//

// Use a macro to template the templates!
#define BOOST_PRIVATE_WRAPPER( Stream ) \
    template < class StreamBuf > \
    class basic_wrapping_##Stream \
        : public detail::basic_wrapping_ios< StreamBuf > \
        , public ::std::basic_##Stream < typename StreamBuf::char_type, \
            typename StreamBuf::traits_type > \
    { \
        typedef detail::basic_wrapping_ios< StreamBuf >  pbase_type; \
        typedef ::std::basic_##Stream< typename StreamBuf::char_type, \
          typename StreamBuf::traits_type >  base_type; \
    public: \
        typedef StreamBuf                        streambuf_type; \
        typedef typename StreamBuf::char_type         char_type; \
        typedef typename StreamBuf::int_type           int_type; \
        typedef typename StreamBuf::pos_type           pos_type; \
        typedef typename StreamBuf::off_type           off_type; \
        typedef typename StreamBuf::traits_type     traits_type; \
        using pbase_type::rdbuf; \
    protected: \
        explicit  basic_wrapping_##Stream ( streambuf_type const &s ) \
            : pbase_type( s ), base_type( this->pbase_type::rdbuf() ) \
            {} \
        basic_wrapping_##Stream () \
            : pbase_type(), base_type( this->pbase_type::rdbuf() ) \
            {} \
        template < typename T1 > \
        explicit  basic_wrapping_##Stream ( T1 x1 ) \
            : pbase_type( x1 ), base_type( this->pbase_type::rdbuf() ) \
            {} \
        template < typename T1, typename T2 > \
        basic_wrapping_##Stream ( T1 x1, T2 x2 ) \
            : pbase_type( x1, x2 ), base_type( this->pbase_type::rdbuf() ) \
            {} \
        template < typename T1, typename T2, typename T3 > \
        basic_wrapping_##Stream ( T1 x1, T2 x2, T3 x3 ) \
            : pbase_type( x1, x2, x3 ), base_type( this->pbase_type::rdbuf() ) \
            {} \
    }


BOOST_PRIVATE_WRAPPER( istream );
BOOST_PRIVATE_WRAPPER( ostream );
BOOST_PRIVATE_WRAPPER( iostream );


// Undo any private macros
#undef BOOST_PRIVATE_WRAPPER


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_STREAMBUF_WRAPPING_HPP
