//  Boost string_algo library predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_HPP
#define BOOST_STRING_PREDICATE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/find.hpp>
#include <boost/string_algo/detail/predicate.hpp>

namespace boost {

//  starts_with predicate  -----------------------------------------------//

    // start_with predicate
    template< typename SeqT1, typename SeqT2 >
    inline bool starts_with( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        typedef BOOST_STRING_TYPENAME 
			string_algo::container_traits<SeqT1>::const_iterator Iterator1T;
		typedef BOOST_STRING_TYPENAME 
			string_algo::container_traits<SeqT2>::const_iterator Iterator2T;
			
		Iterator1T InputEnd=string_algo::end(Input);
		Iterator2T SubstrEnd=string_algo::end(Substr);

		Iterator1T it=string_algo::begin(Input);
        Iterator2T pit=string_algo::begin(Substr);
        for(;
            it!=InputEnd && pit!=SubstrEnd;
            it++,pit++)
        {
            if( !( (*it)==(*pit) ) )
                return false;
        }

        return pit==SubstrEnd;
    }

//  ends_with predicate  -----------------------------------------------//

    // end_with predicate
    template< typename SeqT1, typename SeqT2 >
    inline bool ends_with( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        typedef BOOST_STRING_TYPENAME 
			string_algo::container_traits<SeqT1>::const_iterator Iterator1T;
		typedef BOOST_STRING_TYPENAME boost::detail::
            iterator_traits<Iterator1T>::iterator_category category;

        return string_algo::detail::
            ends_with_iter_select( 
                string_algo::begin(Input), 
				string_algo::end(Input), 
				string_algo::begin(Substr), 
				string_algo::end(Substr), 
				category() );
    }

//  contains predicate  -----------------------------------------------//

    // contains iterator version
    template< typename ForwardIterator1T, typename ForwardIterator2T >
    inline bool contains( 
        ForwardIterator1T Begin, 
        ForwardIterator1T End, 
        ForwardIterator2T SubBegin,
        ForwardIterator2T SubEnd )
    {
        if ( SubBegin==SubEnd )
        {
            // Empty range is contained always
            return true;
        }
        
        return (
			!find_first( 
				make_range(Begin, End), 
				make_range(SubBegin, SubEnd) ).empty() );
    }

    // contains sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool contains( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
		if ( string_algo::empty(Substr) )
        {
            // Empty range is contained always
            return true;
        }
        
        return (
			!find_first( Input,	Substr ).empty() );
    }


//  equal predicate  -----------------------------------------------//

    // equals predicate
    template< typename SeqT1, typename SeqT2 >
    inline bool equals( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        typedef BOOST_STRING_TYPENAME 
			string_algo::container_traits<SeqT1>::const_iterator Iterator1T;
		typedef BOOST_STRING_TYPENAME 
			string_algo::container_traits<SeqT2>::const_iterator Iterator2T;
			
		Iterator1T InputEnd=string_algo::end(Input);
		Iterator2T SubstrEnd=string_algo::end(Substr);

		Iterator1T it=string_algo::begin(Input);
        Iterator2T pit=string_algo::begin(Substr);
        for(;
            it!=InputEnd && pit!=SubstrEnd;
            it++,pit++)
        {
            if( !( (*it)==(*pit) ) )
                return false;
        }

        return ( pit==SubstrEnd ) && ( it==InputEnd );
    }

//  all predicate  -----------------------------------------------//

    // all predicate sequence version
    template< typename SeqT1, typename PredicateT >
    inline bool all( 
        const SeqT1& Input, 
        PredicateT Predicate )
    {
        typedef BOOST_STRING_TYPENAME 
			string_algo::container_traits<SeqT1>::const_iterator Iterator1T;

		Iterator1T InputEnd=string_algo::end(Input);
		for( Iterator1T It=string_algo::begin(Input); It!=InputEnd; It++ )
        {
            if ( !Predicate(*It) )
                return false;
        }
        
        return true;
    }


} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP
