//  Boost string_algo library predicate2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE2_HPP
#define BOOST_STRING_PREDICATE2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/find.hpp>
#include <boost/string_algo/detail/predicate.hpp>

namespace boost {
    namespace string_algo {

//  starts_with predicate  -----------------------------------------------//

        // start_with predicate
        template< typename SeqT1, typename SeqT2, typename PredicateT >
        inline bool starts_with( 
            const SeqT1& Input, 
            const SeqT2& Test,
            PredicateT Comp )
        {
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_traits<SeqT1>::const_iterator Iterator1T;
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_traits<SeqT2>::const_iterator Iterator2T;
                
            Iterator1T InputEnd=string_algo::end(Input);
            Iterator2T TestEnd=string_algo::end(Test);

            Iterator1T it=string_algo::begin(Input);
            Iterator2T pit=string_algo::begin(Test);
            for(;
                it!=InputEnd && pit!=TestEnd;
                it++,pit++)
            {
                if( !( Comp(*it,*pit) ) )
                    return false;
            }

            return pit==TestEnd;
        }

//  ends_with predicate  -----------------------------------------------//

        // end_with predicate
        template< typename SeqT1, typename SeqT2, typename PredicateT >
        inline bool ends_with( 
            const SeqT1& Input, 
            const SeqT2& Test,
            PredicateT Comp )
        {
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_traits<SeqT1>::const_iterator Iterator1T;
            typedef BOOST_STRING_TYPENAME boost::detail::
                iterator_traits<Iterator1T>::iterator_category category;

            return string_algo::detail::
                ends_with_iter_select( 
                    string_algo::begin(Input), 
                    string_algo::end(Input), 
                    string_algo::begin(Test), 
                    string_algo::end(Test), 
                    Comp,
                    category() );
        }

//  contains predicate  -----------------------------------------------//

        // contains sequence version
        template< typename SeqT1, typename SeqT2, typename PredicateT >
        inline bool contains( 
            const SeqT1& Input, 
            const SeqT2& Test,
            PredicateT Comp )
        {
            if ( string_algo::empty(Test) )
            {
                // Empty range is contained always
                return true;
            }
            
            return ( ! (first_finder(Test,Comp)(Input)).empty() );
        }


//  equal predicate  -----------------------------------------------//

        // equals predicate
        template< typename SeqT1, typename SeqT2, typename PredicateT >
        inline bool equals( 
            const SeqT1& Input, 
            const SeqT2& Test,
            PredicateT Comp )
        {
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_traits<SeqT1>::const_iterator Iterator1T;
            typedef BOOST_STRING_TYPENAME 
                string_algo::container_traits<SeqT2>::const_iterator Iterator2T;
                
            Iterator1T InputEnd=string_algo::end(Input);
            Iterator2T TestEnd=string_algo::end(Test);

            Iterator1T it=string_algo::begin(Input);
            Iterator2T pit=string_algo::begin(Test);
            for(;
                it!=InputEnd && pit!=TestEnd;
                it++,pit++)
            {
                if( !( Comp(*it,*pit) ) )
                    return false;
            }

            return ( pit==TestEnd ) && ( it==InputEnd );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_PREDICATE2_HPP
