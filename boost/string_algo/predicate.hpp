//  Boost string_algo library predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_HPP
#define BOOST_STRING_PREDICATE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/compare.hpp>
#include <boost/string_algo/predicate2.hpp>

/*! \file boost/string_algo/predicate.hpp
	The algorithms are resolving if a substring is contained in 
	the input string under various conditions. These conditions are 
	suffix and prefix check, equality check and containment check. 
*/

namespace boost {

//  starts_with predicate  -----------------------------------------------//

    //! 'Starts with' predicate
	/*!
		This predicate holds when the test container is a prefix of the Input.
		In other word, if the input starts with the test.

		\param Input An input container
		\param Test A test container
		\return A result of the test
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool starts_with( 
        const InputContainerT& Input, 
        const TestContainerT& Test )
    {
        return string_algo::starts_with( Input, Test, string_algo::is_equal() );
    }

    //! 'Starts with' predicate ( case insensitive )
	/*!
		This predicate holds when the test container is a prefix of the Input.
		In other word, if the input starts with the test.
		Elements are compared case insensitively.

		\param Input An input container
		\param Test A test container
		\param Loc a locale used for case insensitive comparison
		\return A result of the test
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool istarts_with( 
        const InputContainerT& Input, 
        const TestContainerT& Test,
		const std::locale& Loc=std::locale() )
    {
        return string_algo::starts_with( Input, Test, string_algo::is_iequal(Loc) );
    }

//  ends_with predicate  -----------------------------------------------//

    //! 'Ends with' predicate
	/*!
		This predicate holds when the test container is a suffix of the Input.
		In other word, if the input ends with the test.

		\param Input An input container
		\param Test A test container
		\return A result of the test
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool ends_with( 
        const InputContainerT& Input, 
        const TestContainerT& Test )
    {
        return string_algo::ends_with( Input, Test, string_algo::is_equal() );
    }

    //! 'Ends with' predicate ( case insensitive )
	/*!
		This predicate holds when the test container is a suffix of the Input.
		In other word, if the input ends with the test.
		Elements are compared case insensitively.

		\param Input An input container
		\param Test A test container
		\param Loc a locale used for case insensitive comparison
		\return A result of the test
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool iends_with( 
        const InputContainerT& Input, 
        const TestContainerT& Test,
		const std::locale& Loc=std::locale() )
    {
        return string_algo::ends_with( Input, Test, string_algo::is_iequal(Loc) );
    }

//  contains predicate  -----------------------------------------------//

    //! 'Contains' predicate
	/*!
		This predicate holds when the test container is contained in the Input.
		
		\param Input An input container
		\param Test A test container
		\return A result of the test
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool contains( 
        const InputContainerT& Input, 
        const TestContainerT& Test )
    {
        return string_algo::contains( Input, Test, string_algo::is_equal() );
    }

    //! 'Contains' predicate ( case insensitive )
	/*!
		This predicate holds when the test container is contained in the Input.
		Elements are compared case insensitively.

		\param Input An input container
		\param Test A test container
		\param Loc a locale used for case insensitive comparison
		\return A result of the test
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool icontains( 
        const InputContainerT& Input, 
        const TestContainerT& Test, 
		const std::locale& Loc=std::locale() )
    {
        return string_algo::contains( Input, Test, string_algo::is_iequal(Loc) );
    }

//  equals predicate  -----------------------------------------------//

    //! 'Equals' predicate
	/*!
		This predicate holds when the test container is equal to the
		input container i.e. all elements in both containers are same.
		
		\param Input An input container
		\param Test A test container
		\return A result of the test

		\note This is two-way version of std::equal algorithm
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool equals( 
        const InputContainerT& Input, 
        const TestContainerT& Test )
    {
        return string_algo::equals( Input, Test, string_algo::is_equal() );
    }

    //! 'Equals' predicate ( casa insensitive )
	/*!
		This predicate holds when the test container is equal to the
		input container i.e. all elements in both containers are same.
		Elements are compared case insensitively.

		\param Input An input container
		\param Test A test container
		\param Loc a locale used for case insensitive comparison
		\return A result of the test

		\note This is two-way version of std::equal algorithm
	*/
    template< typename InputContainerT, typename TestContainerT >
    inline bool iequals( 
        const InputContainerT& Input, 
        const TestContainerT& Test,
		const std::locale& Loc=std::locale() )
    {
        return string_algo::equals( Input, Test, string_algo::is_iequal(Loc) );
    }

//  all predicate  -----------------------------------------------//

    //! 'Equals' predicate
	/*!
		This predicate holds it all its elemets satisfy a given 
		condition, represented by the predicate.
		
		\param Input An input container
		\param Pred A predicate
		\return A result of the test
	*/
    template< typename InputContainerT, typename PredicateT >
    inline bool all( 
        const InputContainerT& Input, 
        PredicateT Pred )
    {
        typedef BOOST_STRING_TYPENAME 
            string_algo::container_const_iterator<InputContainerT>::type Iterator1T;

        Iterator1T InputEnd=string_algo::end(Input);
        for( Iterator1T It=string_algo::begin(Input); It!=InputEnd; It++ )
        {
            if ( !Pred(*It) )
                return false;
        }
        
        return true;
    }

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP
