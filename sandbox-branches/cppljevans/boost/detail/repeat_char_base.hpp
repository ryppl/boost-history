//  Boost detail/repeat_char_base.hpp header file  ---------------------------//

//  Copyright 2004 Daryle Walker.  Distributed under the Boost Software
//  License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  No documentation; used for <boost/io/iomanip*.hpp>

#ifndef BOOST_DETAIL_REPEAT_CHAR_BASE_HPP
#define BOOST_DETAIL_REPEAT_CHAR_BASE_HPP

#include <ios>  // for std::streamsize


namespace boost
{
namespace detail
{


//  Base class template for repeated character streaming  --------------------//

template < typename Ch = char >
class repeated_character_streamer_base
{
public:
    // Template argument
    typedef Ch  char_type;

    // Accessors
    char_type          repeated_char() const { return this->c_; }
    ::std::streamsize  repeat_count() const { return this->count_; }

    bool  will_synchronize_afterwards() const { return this->sync_; }

protected:
    // Lifetime management
    repeated_character_streamer_base( char_type c, ::std::streamsize count,
     bool synchronize_afterwards )
        : c_( c ), count_( count ), sync_( synchronize_afterwards )
        {}

private:
     // Member data
    char_type          c_;
    ::std::streamsize  count_;
    bool               sync_;

};  // boost::detail::repeated_character_streamer_base


}  // namespace detail
}  // namespace boost


#endif  // BOOST_DETAIL_REPEAT_CHAR_BASE_HPP
