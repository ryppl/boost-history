// Read Unicode character properties from the database.
// This file contains the signatures of the actual file-reading functions.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*
These files are required as inputs:

UNIDATA/Blocks.txt
UNIDATA/UnicodeData.txt
UNIDATA/SpecialCasing.txt
UNIDATA/LineBreak.txt
UNIDATA/ArabicShaping.txt
UCA/allkeys.txt
*/

#ifndef BOOST_UNICODE_READ_CHARACTER_PROPERTIES_FILES_HPP_INCLUDED
#define BOOST_UNICODE_READ_CHARACTER_PROPERTIES_FILES_HPP_INCLUDED

#include <stdexcept>

#include <boost/spirit/include/classic_file_iterator.hpp>

#include "read_character_properties.hpp"

namespace boost
{

	namespace unicode
	{

		namespace ucd
		{

			// Helper routines
			class file_read_error : std::runtime_error
			{
				static std::string make_message (const std::string & filename);
			public:
				file_read_error (const std::string & a_filename)
					: std::runtime_error (make_message (a_filename)) {}

				~file_read_error() throw() {}
			};

			class syntax_error : std::runtime_error
			{
				static std::string make_message (
					const char* filename,
					const boost::spirit::classic::file_iterator<> & first,
					const boost::spirit::classic::file_iterator<> & last);
			public:
				syntax_error (const char* filename,
								const boost::spirit::classic::file_iterator<> & first,
								const boost::spirit::classic::file_iterator<> & last)
					: std::runtime_error (make_message (filename, first, last))
				{}

				~syntax_error() throw() {}
			};

			boost::spirit::classic::file_iterator<> get_file_iterator
				(const char * name);

			void eat_comments (boost::spirit::classic::file_iterator<> & current,
							   const boost::spirit::classic::file_iterator<> & last);

			/// Read from UnicodeData.txt
			void read_character_properties_unicodedata (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

			/// Read from ArabicShaping.txt
			void read_character_properties_arabicshaping (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

			/// Read from LineBreak.txt
			void read_character_properties_linebreak (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

			/// Read from CaseMappings.txt
			void read_character_properties_specialcasing (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

			/// Read from GraphemeBreakProperty.txt
			void read_character_properties_grapheme_break (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

			/// Read from WordBreakProperty.txt
			void read_character_properties_word_break (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

			/// Read from SentenceBreakProperty.txt
			void read_character_properties_sentence (
				std::map <char32, character_properties> & prop, const char* filename, bool required);

            // Read from Allkeys.txt
            void read_collation_data (
				std::map <char32, character_properties> & props, 
                                const char* filename, bool required);


		}	// namespace ucd

	}	// namespace unicode

}	// namespace boost

#endif	// BOOST_UNICODE_READ_CHARACTER_PROPERTIES_FILES_HPP_INCLUDED
