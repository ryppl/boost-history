//  Boost io/streambuf_wrapping.hpp header file  -----------------------------//

//  (C) Copyright Daryle Walker 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  See http://www.boost.org for updates, documentation, and revision history. 

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


//  Streambuf-wrapping stream class template declarations  -------------------//

// Use a macro to template the templates!
#define BOOST_PRIVATE_WRAPPER( Stream ) \
    template < class StreamBuf > \
    class basic_wrapping_##Stream \
        : private boost::base_from_member< StreamBuf > \
        , public std::basic_##Stream < typename StreamBuf::char_type, \
            typename StreamBuf::traits_type > \
    { \
        typedef ::boost::base_from_member< StreamBuf >  pbase_type; \
        typedef ::std::basic_##Stream< typename StreamBuf::char_type, \
          typename StreamBuf::traits_type >  base_type; \
    public: \
        typedef StreamBuf                        streambuf_type; \
        typedef typename StreamBuf::char_type    char_type; \
        typedef typename StreamBuf::traits_type  traits_type; \
        streambuf_type *  rdbuf() \
        { return &this->pbase_type::member; } \
        streambuf_type const *  rdbuf() const \
        { return &this->pbase_type::member; } \
        bool  is_using_internal_streambuf() const \
        { return this->rdbuf() == this->base_type::rdbuf(); } \
    protected: \
        explicit  basic_wrapping_##Stream ( streambuf_type const &s ) \
            : pbase_type( s ), base_type( &this->pbase_type::member ) \
            {} \
        basic_wrapping_##Stream () \
            : pbase_type(), base_type( &this->pbase_type::member ) \
            {} \
        template < typename T1 > \
        explicit  basic_wrapping_##Stream ( T1 x1 ) \
            : pbase_type( x1 ), base_type( &this->pbase_type::member ) \
            {} \
        template < typename T1, typename T2 > \
        basic_wrapping_##Stream ( T1 x1, T2 x2 ) \
            : pbase_type( x1, x2 ), base_type( &this->pbase_type::member ) \
            {} \
        template < typename T1, typename T2, typename T3 > \
        basic_wrapping_##Stream ( T1 x1, T2 x2, T3 x3 ) \
            : pbase_type( x1, x2, x3 ), base_type( &this->pbase_type::member ) \
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
