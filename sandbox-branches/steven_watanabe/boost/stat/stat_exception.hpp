// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_STAT_STAT_EXCEPTION
#define BOOST_STAT_STAT_EXCEPTION

#include <exception>
#include <boost/assert.hpp>

namespace boost
{
    namespace stat_constants
    {
        typedef int error_type;
        static const error_type empty_range    = 0;
        static const error_type mismatch_range = 1;
    }
    
    class stat_exception : public std::exception
    {
        stat_constants::error_type error_;
        
    public:
        stat_exception( stat_constants::error_type error ) : error_( error )
        { }
        
        virtual const char* what() const throw()
        {
            switch( error_ )
            {
                case stat_constants::empty_range    : return "empty range";
                case stat_constants::mismatch_range : return "mismatched range";
                default                             : BOOST_ASSERT( false ); return "";
            }
        }
    };
}

#endif

