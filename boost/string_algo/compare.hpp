//  Boost string_algo library compare.hpp header file  -------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_COMPARE_HPP
#define BOOST_STRING_COMPARE_HPP

#include <boost/string_algo/config.hpp>
#include <locale>

// Element comparison predicates

namespace boost {
    namespace string_algo {
    
        //  is_equal functor  -----------------------------------------------//

        // is_equal functor
        /*
        standard STL equal_to does handle only comparison between arguments
        of the same type. This is less restrictive version which wraps == operator.
        */
        struct is_equal
        {
            template< typename T1, typename T2 >
                bool operator ()( const T1& Arg1, const T2& Arg2 ) const
            {
                return Arg1==Arg2;
            }
        };

        // case insensitive version of is_equal
        struct is_iequal
        {
            is_iequal( const std::locale& Loc=std::locale() ) : 
                m_Loc( Loc ) {}

            template< typename T1, typename T2 >
                bool operator ()( const T1& Arg1, const T2& Arg2 ) const
            {
                return toupper(Arg1,m_Loc)==toupper(Arg2,m_Loc);
            }

        private:
            std::locale m_Loc;
        };

    } // namespace string_algo 
} // namespace boost


#endif  // BOOST_STRING_COMPARE_HPP
