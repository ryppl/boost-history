//  Boost general library case_conv.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_CASE_CONV_HPP
#define BOOST_CASE_CONV_HPP

#include <algorithm>
#include <functional>
#include <locale>

namespace boost {


//  case conversion functions  -----------------------------------------------//

namespace string_util_impl {

	// a tolower functor 
	template< typename CharT >
	    struct tolowerF : public std::binary_function< CharT, std::locale, CharT > 
	{
		result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
	    {
			return std::tolower( Ch, Loc );
		}
	};

	// a toupper functor 
	template< typename CharT >
	    struct toupperF : public std::binary_function< CharT, std::locale, CharT > 
	{
		result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
	    {
			return std::toupper( Ch, Loc );
		}
	};

} // namespace string_util_impl


//  tolower  -----------------------------------------------//

	// const version of tolower
	template< class Seq >
	Seq tolower( const Seq& Input, const std::locale& Loc=std::locale() )
	{
		Seq Output;
		std::transform( 
			Input.begin(), 
			Input.end(), 
			std::back_inserter<Seq>( Output ),
			std::bind2nd( string_util_impl::tolowerF<typename Seq::value_type>(), Loc ) );

		return Output;
	}

	// in-place version of tolower
	template< class Seq >
	Seq& tolower_in( Seq& Input, const std::locale& Loc=std::locale() )
	{
		std::transform( 
			Input.begin(), 
			Input.end(), 
			Input.begin(),
			std::bind2nd( string_util_impl::tolowerF<typename Seq::value_type>(), Loc ) );
		
		return Input;
	}
	
//  toupper  -----------------------------------------------//

	// const version of toupper
	template< class Seq >
	Seq toupper( const Seq& Input, const std::locale& Loc=std::locale() )
	{
		Seq Output;
		std::transform( 
			Input.begin(), 
			Input.end(), 
			std::back_inserter<Seq>( Output ),
			std::bind2nd( string_util_impl::toupperF<typename Seq::value_type>(), Loc ) );

		return Output;
	}

	// in-place version of toupper
	template< class Seq >
	Seq& toupper_in( Seq& Input, const std::locale& Loc=std::locale() )
	{
		std::transform( 
			Input.begin(), 
			Input.end(), 
			Input.begin(),
			std::bind2nd( string_util_impl::toupperF<typename Seq::value_type>(), Loc ) );
		
		return Input;
	}
	
} // namespace boost


#endif  // BOOST_CASE_CONV_HPP
