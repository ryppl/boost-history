//  Boost string_algo library regex.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REGEX_HPP
#define BOOST_STRING_REGEX_HPP

#include <boost/string_algo/config.hpp>
#include <boost/regex.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace2.hpp>
#include <boost/string_algo/regex2.hpp>
#include <boost/string_algo/format2.hpp>
#include <boost/string_algo/split2.hpp>

/*! \file
	This header defines regex variant of the algorithms. They mostly only
	wrap regex++ functionality to provide uniform interface with
	other algorithms in this library.
	Another reason to include them here is have a proof of concept for
	the library extensibility and customization.
*/

namespace boost {

//  find_regex  -----------------------------------------------//

    //! Find regex algorithm
    /*!
        Search for a subsequence maching the given regex in the input.
		
		\param Input A container which will be searched.
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of 
			the input parameter.
	*/

    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputT>::type >
    find_regex( 
        InputT& Input, 
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::regex_finder(Rx,MatchFlags)(
			string_algo::begin(Input), string_algo::end(Input) );
    }

//  replace_regex --------------------------------------------------------------------//

    //! Replace regex algorithm
	/*!
		Search for a subsequence mathing given regex and format it with 
		the specified format. Result is copied to the given output iterator.

		\param Output A output iterarot to which the result will be copied
		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\param Format Regex format definition
		\param FormatFlags Format flags
		\return An output iterator pointing just after last inserted character
	*/
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline OutputIteratorT replace_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        match_flag_type MatchFlags=match_default,
        match_flag_type FormatFlags=format_default )
    {
        return string_algo::replace_copy( 
            Output,
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    //! Replace regex algorithm
	/*!
		Search for a subsequence mathing given regex and format it with 
		the specified format. Result is a modified copy of the input.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\param Format Regex format definition
		\param FormatFlags Format flags
		\return A modified copy of the input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT replace_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        match_flag_type MatchFlags=match_default,
        match_flag_type FormatFlags=format_default )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    //! Replace regex algorithm
	/*!
		Search for a subsequence mathing given regex and format it with 
		the specified format. Input sequence is modified in-place.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\param Format Regex format definition
		\param FormatFlags Format flags
		\return A reference to the modified input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT& replace_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        match_flag_type MatchFlags=match_default,
        match_flag_type FormatFlags=format_default )
    {
        return string_algo::replace( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

//  replace_all_regex --------------------------------------------------------------------//

    //! Replace all regex algorithm
	/*!
		Format all subsequences, mathing given regex, with the specified format. 
		Result is copied to the given output iterator.

		\param Output A output iterarot to which the result will be copied
		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\param Format Regex format definition
		\param FormatFlags Format flags
		\return An output iterator pointing just after last inserted character
	*/
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline OutputIteratorT replace_all_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        match_flag_type MatchFlags=match_default,
        match_flag_type FormatFlags=format_default )
    {
        return string_algo::replace_all_copy( 
            Output,
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    //! Replace all regex algorithm
	/*!
		Format all subsequences, mathing given regex, with the specified format. 
		Result is a modified copy of the input.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\param Format Regex format definition
		\param FormatFlags Format flags
		\return A modified copy of the input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT replace_all_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        match_flag_type MatchFlags=match_default,
        match_flag_type FormatFlags=format_default )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

    //! Replace all regex algorithm
	/*!
		Format all subsequences, mathing given regex, with the specified format. 
		Input sequence is modified in-place.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\param Format Regex format definition
		\param FormatFlags Format flags
		\return A reference to the modified input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT,
        typename FormatStringTraitsT, typename FormatStringAllocatorT >
    inline InputT& replace_all_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        const std::basic_string<CharT, FormatStringTraitsT, FormatStringAllocatorT>& Format,
        match_flag_type MatchFlags=match_default,
        match_flag_type FormatFlags=format_default )
    {
        return string_algo::replace_all( 
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::regex_formatter( Format, FormatFlags ) );
    }

//  erase_regex --------------------------------------------------------------------//

    //! Erase regex algorithm
	/*!
		Remove a subsequence mathing given regex from the input.
		Result is copied to the given output iterator.

		\param Output A output iterarot to which the result will be copied
		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return An output iterator pointing just after last inserted character
	*/
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline OutputIteratorT erase_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase regex algorithm
	/*!
		Remove a subsequence mathing given regex from the input.
		Result is a modified copy of the input.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return A modified copy of the input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline InputT erase_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
	{
        return string_algo::replace_copy( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase regex algorithm
	/*!
		Remove a subsequence mathing given regex from the input.
		Input sequence is modified in-place.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return A reference to the modified input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline InputT& erase_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
	{
        return string_algo::replace( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

//  erase_all_regex --------------------------------------------------------------------//

    //! Erase all regex algorithm
	/*!
		Erase all subsequences, mathing given regex, from the input.
		Result is copied to the given output iterator.

		\param Output A output iterarot to which the result will be copied
		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return An output iterator pointing just after last inserted character
	*/
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline OutputIteratorT erase_all_regex_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase all regex algorithm
	/*!
		Erase all subsequences, mathing given regex, from the input.
		Result is a modified copy of the input.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return A modified copy of the input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline InputT erase_all_regex_copy( 
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

    //! Erase all regex algorithm
	/*!
		Erase all subsequences, mathing given regex, from the input.
		Input sequence is modified in-place.

		\param Input An input sequence
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return A reference to the modified input
	*/
    template< 
        typename InputT, 
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT>
    inline InputT& erase_all_regex( 
        InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::regex_finder( Rx, MatchFlags ),
            string_algo::empty_formatter( Input ) );
    }

//  find_all_regex ------------------------------------------------------------------//

    //! Find all regex algorithm
	/*!
		This algorithm finds all subsequenced matching the give regex
		in the input. A result is given as a 'container of containers'.
		Each match of the search sequence is represented by one
		element in the result.

		\param Result A 'container container' to container the result of search.
			Both outher and inner container must have constructor taking a pair
			of iterators as an argument.
			Typical type of the result is 
				std::vector< boost::iterator_range<iterator> >.
			( each element of such a vector will container a range delimiting 
			a match )
		\param Input A container which will be searched.
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return A reference the result

		\note Prior content of the result will be overriden.
	*/
	template< 
        typename ResultT, 
        typename InputT,         
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline ResultT& find_all_regex(
        ResultT& Result,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::iter_find(
            Result,
            Input,
            string_algo::regex_finder(Rx,MatchFlags) );         
    }

//  split_regex ------------------------------------------------------------------//

    //! Split regex algorithm
	/*! 
		Tokenize expression. This function is equivalent of C strtok. Input
		sequence is split into tokens, separated  by seperators. Separators 
		is every match of the given regex.

		\param Result A 'container container' to container the result of search.
			Both outher and inner container must have constructor taking a pair
			of iterators as an argument.
			Typical type of the result is 
				std::vector< boost::iterator_range<iterator> >.
			( each element of such a vector will container a range delimiting 
			a match )

		\param Input A container which will be searched.
		\param Rx A regular expression
		\param MatchFlags Regex search options
		\return A reference the result

		\note Prior content of the result will be overriden.
    */
    template< 
        typename ResultT, 
        typename InputT,         
        typename CharT, 
        typename RegexTraitsT, typename RegexAllocatorT >
    inline ResultT& split_regex(
        ResultT& Result,
        const InputT& Input,
        const reg_expression<CharT, RegexTraitsT, RegexAllocatorT>& Rx,
        match_flag_type MatchFlags=match_default )
    {
        return string_algo::iter_split(
            Result,
            Input,
            string_algo::regex_finder(Rx,MatchFlags) );         
    }

} // namespace boost


#endif  // BOOST_STRING_REGEX_HPP
