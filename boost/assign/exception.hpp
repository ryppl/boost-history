    /**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_ASSIGN_EXCEPTIOM_HPP
#define BOOST_ASSIGN_EXCEPTIOM_HPP

#include <exception>

namespace boost
{    
    namespace assignment
    {
        class assign_exception : public std::exception
        {
            public:
        	assign_exception( const char* what ) : what_( what )
        	{ }
        
        
        
        	virtual const char* what() const throw()
        	{
        	    return what_;
        	}
        
            private:
                const char* what_;
        };
    }
}
#endif
