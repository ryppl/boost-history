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

/*! \file
	Defines element comparison predicates. Many algoritms in this lib can 
	take an addtion argument with a predicate used to compare elements.
	This maked possible, for instance, to have case insensitive versions
	of the algorithms.
*/

namespace boost {
    namespace string_algo {
    
        //  is_equal functor  -----------------------------------------------//

        //! is_equal functor
        /*!
			Standard STL equal_to does handle only comparison between arguments
			of the same type. This is less restrictive version which wraps == operator.
        */
        struct is_equal
        {
			//! Function operator
			/*!
				Compare two operands for equality
			*/
            template< typename T1, typename T2 >
                bool operator ()( const T1& Arg1, const T2& Arg2 ) const
            {
                return Arg1==Arg2;
            }
        };

        //! case insensitive version of is_equal
		/*!
			Case insensitive comparison predicate. Comparison is done using
			specified locales.
		*/
        struct is_iequal
        {
			//! Constructor
			/*!
				\param Loc locales used for comparison
			*/
            is_iequal( const std::locale& Loc=std::locale() ) : 
                m_Loc( Loc ) {}

			//! Function operator 
			/*!
				Compare two operands. Case is ignored.
			*/
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
