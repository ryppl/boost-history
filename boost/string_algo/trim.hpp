//  Boost general library trim.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_TRIM_HPP
#define BOOST_TRIM_HPP

#include <algorithm>
#include <functional>
#include <locale>

namespace boost {


//  trimming functions  -----------------------------------------------//

namespace string_util_impl {

	// isspace implementation
	/*
		this forward is provided here to allow non standard isspace implementations.
		f.e. non character base overrides
	*/
	template< class CharT >
	bool isspace_impl( CharT Ch, const std::locale& Loc )
	{
		return std::isspace( Ch, Loc );
	}

	// an isspace functor 
	template< class CharT >
	    struct isspaceF : public std::binary_function< CharT, std::locale, bool > 
	{
		result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
	    {
			return isspace_impl( Ch, Loc );
		}
	};

} // namespace string_util_impl

//  trim iterators  -----------------------------------------------//

	// Search for first non space character from the beginning of the sequence
	template< class Iterator >
	Iterator trim_begin( Iterator& InBegin, Iterator& InEnd, const std::locale& Loc=std::locale() )
	{
		return std::find_if( 
			InBegin, 
			InEnd, 
			std::not1(std::bind2nd(string_util_impl::isspaceF<Iterator::value_type>(), Loc )));
	}

//  left trim  -----------------------------------------------//

	// const version of left trim
	template< class Seq >
	Seq ltrim( const Seq& Input, const std::locale& Loc=std::locale() )
	{
		return Seq( 
				trim_begin( Input.begin(), Input.end(), Loc ),
				Input.end() );
	}

	// in-place version of left trim
	template< class Seq >
	Seq& ltrim_in( Seq& Input, const std::locale& Loc=std::locale() )
	{
		Input.erase( 
			Input.begin(),
			trim_begin( Input.begin(), Input.end(), Loc ));

		return Input;
	}

//  right trim  -----------------------------------------------//

	// const version of right trim
	template< class Seq >
	Seq rtrim( const Seq& Input, const std::locale& Loc=std::locale() )
	{
		return Seq( 
			Input.begin(),
			trim_begin( Input.rbegin(), Input.rend(), Loc ).base()
			);
	}
	
	// in-place version of right trim
	template< class Seq >
	Seq& rtrim_in( Seq& Input, const std::locale& Loc=std::locale() )
	{
		Input.erase(
			trim_begin( Input.rbegin(), Input.rend(), Loc ).base(),
			Input.end()
			);

		return Input;
	}

//  both side trim  -----------------------------------------------//

	// const version of trim
	template< class Seq >
	Seq trim( const Seq& Input, const std::locale& Loc=std::locale() )
	{
		Seq::const_iterator TrimEnd=trim_begin( Input.rbegin(), Input.rend(), Loc).base();

		return Seq( 
			trim_begin( Input.begin(), TrimEnd, Loc ),
			TrimEnd
			);
	}
	
	// in-place version of trim
	template< class Seq >
	Seq& trim_in( Seq& Input, const std::locale& Loc=std::locale() )
	{
		return ltrim_in( rtrim_in( Input, Loc ), Loc );
	}
	

} // namespace boost


#endif  // BOOST_TRIM_HPP
