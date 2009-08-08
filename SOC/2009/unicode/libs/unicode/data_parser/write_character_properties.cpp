// Write Unicode character properties.
// Though this file is under the Boost license, it is NOT (or not yet) part of
// Boost!

// Copyright Graham Barnett, Rogier van Dalen 2005.
// Use, modification, and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

#include <cstring>

#include <boost/spirit/include/classic.hpp>
#include <boost/lambda/lambda.hpp>

#include "write_character_properties.hpp"

using namespace boost::spirit;
using namespace boost::unicode;
using namespace boost::unicode::ucd;
using namespace boost::lambda;

using boost::char32;

namespace boost { namespace unicode { namespace ucd {

const int PROGRESS_INDICATION_EVERY_PREPARE  = 4000;
const int PROGRESS_INDICATION_EVERY_WRITE  = 100;

write_entry::write_entry()
{
	general_category = (category::type)-1;
	combining = -1;
	bidi = (bidi_class::type)-1;
	decomposition_kind = (decomposition_type::type)-1;
	has_decomposition = false;
    grapheme_break = grapheme_cluster_break::any;
    word_break_kind = word_break::any;
    sentence_break_kind = sentence_break::any;
	unknown_char = false;
	sort_variable = false;
    sort_type = sort_type::default_;
    sort_index_or_data1 = 0;
    sort_data2 = 0;
	uppercase = (char32)0;
	lowercase = (char32)0;
	titlecase = (char32)0;
	has_complex_case = false;
	line_break = (line_break::type)-1;
	joining = (join_type::type)-1;
}

write_entry::write_entry(char32 ch, const character_properties& data)
{
	chr = ch;
	name = data.name;
	general_category = data.general_category;
	combining = data.combining;
	bidi = data.bidi;
	decomposition_kind = data.decomposition_kind;
	has_decomposition = (!data.decomposition.empty());
    grapheme_break = data.grapheme_break;
    word_break_kind = data.word_break_kind;
    sentence_break_kind = data.sentence_break_kind;
	unknown_char = data.unknown_char;
	sort_variable = data.sort_variable;
    sort_type = data.sort_type;
    sort_index_or_data1 = data.sort_index_or_data1;
    sort_data2 = data.sort_data2;
	uppercase = data.uppercase;
	lowercase = data.lowercase;
	titlecase = data.titlecase;
	has_complex_case = (!data.complex_case.empty());
	line_break = data.line_break;
	joining = data.joining;
}

void write_entry::calc_properties_checksum(boost::crc_basic<16>& crc)
{
	// it would be very nice to checksum the whole of the entry in one 
	// go but alignment might mean that there is random data interspersed
#ifdef BOOST_UNICODE_UCD_BIG
	crc.process_bytes(name.c_str(), name.size() );
#endif
	crc.process_bytes(&general_category, sizeof(general_category) );
	crc.process_bytes(&combining, sizeof(combining) );
	crc.process_bytes(&bidi, sizeof(bidi) );
	crc.process_bytes(&decomposition_kind, sizeof(decomposition_kind) );
	crc.process_bytes(&has_decomposition, sizeof(has_decomposition) );
	crc.process_bytes(&grapheme_break, sizeof(grapheme_break) );
	crc.process_bytes(&word_break_kind, sizeof(word_break_kind) );
	crc.process_bytes(&sentence_break_kind, sizeof(sentence_break_kind) );
	crc.process_bytes(&unknown_char, sizeof(unknown_char) );
#ifdef BOOST_UNICODE_UCD_BIG    
	crc.process_bytes(&sort_variable, sizeof(sort_variable) );
	crc.process_bytes(&sort_type, sizeof(sort_type) );
	crc.process_bytes(&sort_index_or_data1, sizeof(sort_index_or_data1) );
	crc.process_bytes(&sort_data2, sizeof(sort_data2) );
	crc.process_bytes(&uppercase, sizeof(uppercase) );
	crc.process_bytes(&lowercase, sizeof(lowercase) );
	crc.process_bytes(&titlecase, sizeof(titlecase) );
	crc.process_bytes(&has_complex_case, sizeof(has_complex_case) );
#endif
	crc.process_bytes(&line_break, sizeof(line_break) );
#ifdef BOOST_UNICODE_UCD_BIG
	crc.process_bytes(&joining, sizeof(joining) );
#endif
}

bool write_entry::has_same_properties(const write_entry& other) const
{
	// note: chr should NOT be included as this is used to compare 
    // character properties, not chr
	// note: as the name comparison is the slowest, 
    // it is also the last item in the statement
	return general_category == other.general_category && 
		combining == other.combining && bidi == other.bidi && 
		decomposition_kind == other.decomposition_kind && 
        has_decomposition == other.has_decomposition &&
		grapheme_break == other.grapheme_break &&
		word_break_kind == other.word_break_kind &&
        sentence_break_kind == other.sentence_break_kind && 
        unknown_char == other.unknown_char && 
        sort_type == other.sort_type &&
        sort_variable == other.sort_variable &&
        sort_index_or_data1 == other.sort_index_or_data1 && 
        uppercase == other.uppercase &&
		lowercase == other.lowercase && 
        titlecase == other.titlecase &&
		has_complex_case == other.has_complex_case && 
        line_break == other.line_break &&
		joining == other.joining && name == other.name;
}

/*******************************************************************************************
**** This structure is for internal use only and should not be used by any developer 
**** unless they are providing a custom implementation of the character properties interface
**** Changes to this must be reflected in uni_impl_format_internal.hpp
*******************************************************************************************/
std::fstream& operator << (std::fstream& file, const write_entry& data)
{
	// create the data in a stringstream
	std::stringstream ss;
	// start and comment
	// --- start of character -------------------------------------------------
	ss << "\t{\t// char 0x" << std::hex << data.chr << ",\n";
#ifdef BOOST_UNICODE_UCD_BIG
	ss << "\t\t\"" << data.name << "\",\n";
#endif
	if (data.has_decomposition)
	{
		ss << "\t\t__uni_decomp_data_0x" << std::hex << data.chr << ",\n";
	}
	else
	{
		ss << "\t\tNULL,\n";
	}
#ifdef BOOST_UNICODE_UCD_BIG
	if (data.has_complex_case)
	{
		ss << "\t\t__uni_complex_case_0x" << std::hex << data.chr << ",\n";
	}
	else
	{
		ss << "\t\tNULL,\n";
	}
#endif
    ss << "\t\t{\n";
	ss << "\t\t\tcategory::" << as_string(data.general_category) << ",\n";
#ifdef BOOST_UNICODE_UCD_BIG
	ss << "\t\t\tjoin_type::" << as_string(data.joining) << ",\n";
#endif
	ss << "\t\t\tword_break::" << as_string(data.word_break_kind) << ",\n";
//    ss << "\t\t\t" << std::boolalpha << data.unknown_char << ",\n";
#ifdef BOOST_UNICODE_UCD_BIG
    ss << "\t\t\t" << std::boolalpha << data.sort_variable << ",\n";
    ss << "\t\t\tucd::sort_type::" << as_string(data.sort_type) << ",\n";
    ss << "\t\t\t" << std::dec << data.sort_data2 << ",\n";
#endif
	ss << "\t\t\tbidi_class::" << as_string(data.bidi) << ",\n";
	ss << "\t\t\tdecomposition_type::" << as_string(data.decomposition_kind) << ",\n";
	ss << "\t\t\tline_break::" << as_string(data.line_break) << ",\n";
	ss << "\t\t\t" << std::dec << data.combining << ",\n";
	ss << "\t\t\tsentence_break::" << as_string(data.sentence_break_kind) << ",\n";
    ss << "\t\t\tgrapheme_cluster_break::" << as_string(data.grapheme_break) << ",\n";
    ss << "\t\t},\n";

#ifdef BOOST_UNICODE_UCD_BIG    
    ss << "\t\t" << std::dec << data.sort_index_or_data1 << ",\n";
	ss << "\t\t0x" << std::hex << data.uppercase << ",\n";
	ss << "\t\t0x" << std::hex << data.lowercase << ",\n";
	ss << "\t\t0x" << std::hex << data.titlecase << ",\n";
#endif
	// --- end of character ---------------------------------------------------
	ss << "\t},\n";		

	// write the data to the file
	file << ss.str();

	return file;
}

decomp_entry::decomp_entry()
{
	chr = (char32)-1;
}

decomp_entry::decomp_entry(char32 ch, const std::vector<char32>& decomp)
{
	chr = ch;
	decomposition = decomp;
}

decomp_entry decomp_entry::create_dummy_entry()
{
	decomp_entry temp;
	temp.decomposition.push_back(0);

	return temp;
}

std::fstream& operator << (std::fstream& file, const decomp_entry& data)
{
	// create the data in a stringstream
	std::stringstream ss;
	ss << "static const char32 __uni_decomp_data_0x" << std::hex << data.chr << "[] = { ";
	size_t size = data.decomposition.size();
    ss << size << ", ";
	for (size_t n = 0; n < size; n++)
	{
		ss << "0x" << std::hex << data.decomposition[n] << ", ";
	}
	ss << "};\n";

	// write the data to the file
	file << ss.str();

	return file;
}

write_block::write_block()
{
	decomposition_index_min = 0;
	decomposition_index_max = -1;
	complex_case_index_min = 0;
	complex_case_index_max = -1;
	ch_first = 0;
};

std::fstream& write_block::write_decomp_for_block(std::fstream& file,
	const std::vector<decomp_entry>& tbl_decomp) const
{
	for (int n = decomposition_index_min; n <= decomposition_index_max; n++)
	{
		file << tbl_decomp[n];
	}

	file << "\n\n";

	return file;
}

void write_complex_casing(
	std::fstream& file, const complex_casing& data, bool bFinal)
{
	if (data.complex_uppercase.size() > (size_t)complex_case_size_const)
	{
		std::stringstream ss;
		ss << "Increase in size of complex_case_size_const due to complex_uppercase is required";
		throw std::runtime_error (ss.str());
	}
	if (data.complex_lowercase.size() > (size_t)complex_case_size_const)
	{
		std::stringstream ss;
		ss << "Increase in size of complex_case_size_const due to complex_lowercase is required";
		throw std::runtime_error (ss.str());
	}
	if (data.complex_titlecase.size() > (size_t)complex_case_size_const)
	{
		std::stringstream ss;
		ss << "Increase in size of complex_case_size_const due to complex_titlecase is required";
		throw std::runtime_error (ss.str());
	}

	file << "\t\t" << std::dec << (int)data.complex_uppercase.size() << ", ";
	file << "\t\t{ ";
	int n;
	for (n = 0; n < (int)data.complex_uppercase.size(); n++)
	{
		file << std::hex << "0x" << data.complex_uppercase[n] << ", ";
	}
	file << "},\n";

	file << "\t\t" << std::dec << (int)data.complex_lowercase.size() << ", ";
	file << "\t\t{ ";
	for (n = 0; n < (int)data.complex_lowercase.size(); n++)
	{
		file << std::hex << "0x" << data.complex_lowercase[n] << ", ";
	}
	file << "},\n";

	file << "\t\t" << std::dec << (int)data.complex_titlecase.size() << ", ";
	file << "\t\t{ ";
	for (n = 0; n < (int)data.complex_titlecase.size(); n++)
	{
		file << std::hex << "0x" << data.complex_titlecase[n] << ", ";
	}
	file << "},\n";

	if (bFinal)
	{
		file << "\t\ttrue,\n";
	}
	else
	{
		file << "\t\tfalse,\n";
	}
}

std::fstream& operator << (std::fstream& file, const read_block& data)
{
	file << "\t{ ";
	file << "0x" << std::hex << data.first << ", ";
	file << "0x" << std::hex << data.last << ", ";
	file << "\"" << data.name << "\" ";
	file << "},\n";

	return file;
}

std::fstream& operator << (std::fstream& file,
	const tuple<char32, std::vector<complex_casing> >& data)
{
#ifdef BOOST_UNICODE_UCD_BIG
	// write the block
	file << "static const unichar_complex_case_internal __uni_complex_case_0x";
	file << std::hex << get<0>(data) << "[]= \n{\n";
    
	int n;
	int count = (int)get<1>(data).size();
	for (n = 0; n < count - 1; n++)
	{
        file << "\t{\n";
		write_complex_casing(file, get<1>(data)[n], false);
        file << "\t},\n";
	}
	for (; n < count; n++)
	{
        file << "\t{\n";
		write_complex_casing(file, get<1>(data)[n], true);
        file << "\t},\n";
	}

	// write the data to the file
	file << "};\n\n\n";

#endif
	return file;
}

std::fstream& write_block::write_complex_case_block(std::fstream& file,
	const std::vector<tuple<char32, std::vector<complex_casing> > >&
		tbl_complex_case) const
{
	for (int n = complex_case_index_min; n <= complex_case_index_max; n++)
	{
		file << tbl_complex_case[n];
	}

	file << "\n\n";

	return file;
}

std::fstream& operator << (std::fstream& file, const write_block& data)
{
	// write the block
	file << "static const unichar_data_internal __uni_char_data_";
    file << std::hex << data.ch_first << "[]= \n{\n";
 	
	for (int n = 0; n < block_size_const; n++)
	{
		file << (*data.tbl_entry[n]);
	}

	file << "};\n\n\n";

	return file;
}

void write_block::calc_checksum()
{
	if (decomposition_index_max != -1 || complex_case_index_max != -1)
	{
		checksum_of_entry_properties = (uint32_t)-1;
		return;
	}

	boost::crc_basic<16> crc( 0x1021, 0xFFFF, 0, false, false );

	int x;
	for (x = block_size_const - 1; x >= 0; x--)
	{
		// optimisation - if an entry has decomposition data or complex case there
		// is no point trying to compare with it as these are almost certainly unique
		tbl_entry[x]->calc_properties_checksum(crc);
	}

	checksum_of_entry_properties = (uint32_t)crc.checksum();
}

void write_license(std::fstream& file)
{
	file << "// Though this file is under the Boost license, it is NOT (or not yet) part of\n";
	file << "// Boost!\n";
	file << "\n";
	file << "// Copyright Graham Barnett, Rogier van Dalen 2005.\n";
	file << "// Use, modification, and distribution are subject to the Boost Software\n";
	file << "// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at\n";
	file << "// http://www.boost.org/LICENSE_1_0.txt)\n";
	file << "\n";
	file << "// This file was created using information from the\n";
	file << "// www.unicode.org web site\n";
	file << "// License http://www.unicode.org/copyright.html \n";
	file << "\n";
}

void write_block_tables_and_blocks(const std::vector<read_block>& tbl_block,
	const std::vector<char32>& tbl_block_ident, 
	const char * dest_path, size_t nFiles)
{
	std::cout << " writing " << dest_path << "uni_ucd_interface_impl_data.cpp\n";

	// open file
	std::stringstream fss;
	fss << dest_path;
	fss << "uni_ucd_interface_impl_data.cpp";

	std::fstream file;
	file.exceptions (std::ios_base::failbit | std::ios_base::badbit);
	file.open(fss.str().c_str(), std::ios_base::out);

	write_license(file);
	file << "/**** This file should be added to a project that                     ****/\n";
	file << "/**** implements the unichar interface                                ****/\n";
	file << "/**** This file is automatically generated and should not be modified.****/\n";
	file << "/**** Data in this file should not be accessed directly except        ****/\n";
	file << "/**** through the single published interface as documented in Boost   ****/\n";
    
	file << "\n\n#define BOOST_UNICODE_SOURCE\n";
    file << "#include <boost/assert.hpp>\n";
	file << "#include <boost/static_assert.hpp>\n";
	file << "#include <boost/unicode/ucd/properties.hpp>\n";
	size_t n;
	for (n = 0; n < nFiles; n++)
	{
		file << "#include \"uni_ucd_interface_impl_data_" << std::dec << (int)(n + 1) << ".ipp\"\n";
	}
	file << "#include \"uni_ucd_interface_impl_sort_data.ipp\"\n";
    file << "#include \"uni_ucd_interface_impl_compose_data.ipp\"\n";
	file << "\n\nnamespace boost { namespace unicode { namespace ucd {\n\n";

	// ---- block table -------------------------------------------------------

	file << "BOOST_UNICODE_DECL const unichar_blocks_internal __uni_block_data[]=\n{\n";

	for (n = 0; n < tbl_block.size(); n++)
	{
		file << tbl_block[n];
	}

    /*// write a terminating block
    read_block terminating_data;
    terminating_data.first = 0;
    terminating_data.last = 0;
    terminating_data.name = "";
    file << terminating_data;*/

	file << "};\n\nBOOST_UNICODE_DECL const size_t __uni_block_data_size = sizeof __uni_block_data / sizeof __uni_block_data[0];\n\n";

	// ---- block table -------------------------------------------------------
	
	// ---- block refs --------------------------------------------------------
	file << "BOOST_UNICODE_DECL const unichar_data_internal* __uni_char_data[]=\n{\n";

	for (n = 0; n < tbl_block_ident.size(); n++)
	{
		file << "\t__uni_char_data_" << std::hex << tbl_block_ident[n] << ",\n";
	}

	file << "};\n";

	// ---- block ref --------------------------------------------------------

	file << "}}}	// namespaces\n";
	
	file.flush();
	file.close();
}

void write_block_enum(
	const std::vector<read_block>& tbl_block, const char * dest_path_ucd_props)
{
	std::cout << " writing " << dest_path_ucd_props
		<< "block_types.hpp" << std::endl;

	// open file
	std::stringstream fss;
	fss << dest_path_ucd_props;
	fss << "block_types.hpp";

	std::fstream file;
	file.open(fss.str().c_str(), std::ios_base::out);

	write_license(file);
	file << "/**** This file should be included in any project that uses           ****/\n";
	file << "/**** the boost Unicode character interface                           ****/\n";
	file << "/**** This file is automatically generated and should not be modified.****/\n";
	file << "\n";
	file << "/**** WARNING !! The block enum is a sparse enum to allow for         ****/\n";
	file << "/**** new values to be added to the unicode spec without affecting    ****/\n";
	file << "/**** existing code                                                   ****/\n";
	
    file << "\n\n#ifndef BOOST_UNICODE_UNI_UCD_CHARACTER_PROPERTIES_HPP_INCLUDED\n";
    file << "#define BOOST_UNICODE_UNI_UCD_CHARACTER_PROPERTIES_HPP_INCLUDED\n";
	file << "\n\nnamespace boost { namespace unicode { namespace ucd { \n\n";

	// ---- block table -------------------------------------------------------

	file << "\t\tstruct block\n";
	file << "\t\t{\n";
	file << "\t\t\tenum type\n";
	file << "\t\t\t{\n";
    file << "\t\t\t\tnone = -1,\n\n";

	for (size_t n = 0; n < tbl_block.size(); n++)
	{
		std::string name = tbl_block[n].name;
		// legalise the name into a form that is acceptible for use in a C enum
		for (size_t x = 0; x < name.length();x++)
		{
			switch (name[x])
			{
				case '-':	name[x] = '_';	break;
				case ' ':	name[x] = '_';	break;
				default:	name[x] = tolower(name[x]);	break;
			}
		}
		file << "\t\t\t\t" << name;
		file << " = 0x" << std::hex << tbl_block[n].first;
		file << ",\n";
	}

	file << "\t\t\t};\n";
	file << "\t\t};\n";
    
    file << "\n\t\tBOOST_UNICODE_DECL const char* as_string(block::type);\n\n";

	file << "}}} // namespaces\n\n";
    file << "#endif // BOOST_UNICODE_UNI_UCD_CHARACTER_PROPERTIES_HPP_INCLUDED\n";
	
	file.flush();
	file.close();
}

/*******************************************************************
** get_default_sort_characteristics
** For characters that do not have specific sort entries and that do
** canonically decompose we need to have default sort characteristics
** See tr10 - 7.1.3 Implicit Weights
** Must be kept in sync with version in uni_impl_interface.cpp
*******************************************************************/
void get_default_sort_characteristics(char32 cp, collation_entry & sort_entry)
{
    // the character has no sort entry, nor could it be decomposed

    // check for illegal values
	if (cp == 0xFFFF || cp == 0xFFFE || cp > 0x10FFFF)
	{
        collation_data coll1;
        coll1.variable = false;
        coll1.weight1 = 0;
        coll1.weight2 = 0;
        coll1.weight3 = 0;
        coll1.weight4 = 0;

        sort_entry.data.clear();
        sort_entry.following_chars.clear();

        sort_entry.data.push_back(coll1);

        return;
    }

    uint16_t base;
    // The value for BASE depends on the type of character:
    if (cp >= 0x3400 && cp < 0x4DB5)
    {
        // FB40 	CJK Ideograph
        base = 0xFB40;
    }
    else if (cp >= 0x4DB5 && cp < 0x4DC0)
    {
        // FB80 	CJK Ideograph Extension A/B
        base = 0xFB80;
    }
    else
    {
        // FBC0 	Any other code point
        base = 0xFBC0;
    }

    uint16_t aaaa = (uint16_t)(base + (cp >> 15));
    uint16_t bbbb = (uint16_t)((cp & 0x7FFF) | 0x8000);

    // CP => [.AAAA.0020.0002.cp][.BBBB.0000.0000.cp]
    collation_data coll1;
    coll1.variable = false;
    coll1.weight1 = aaaa;
    coll1.weight2 = 0x0020;
    coll1.weight3 = 0x0002;
    coll1.weight4 = cp;

    collation_data coll2;
    coll2.variable = false;
    coll2.weight1 = bbbb;
    coll2.weight2 = 0x0000;
    coll2.weight3 = 0x0000;
    coll2.weight4 = cp;

    sort_entry.data.clear();
    sort_entry.following_chars.clear();

    sort_entry.data.push_back(coll1);
    sort_entry.data.push_back(coll2);
}

bool decompose_for_sort(const character_properties & props_char, 
                        char32 cp, 
                        const std::map <char32, character_properties> & props,
                                            std::vector<collation_data> & tbl_coll)
{
    std::vector <char32>::const_iterator iter = props_char.decomposition.begin();
    std::vector <char32>::const_iterator end = props_char.decomposition.end();
    for (; iter != end; iter++)
    {
        char32 cp_decomp = *iter;
        const std::map <char32, character_properties>::const_iterator iter_char = 
                                                            props.find(cp_decomp);
        if (iter_char == props.end())
        {
            collation_entry sort_entry;
            get_default_sort_characteristics(cp_decomp, sort_entry);

            tbl_coll.reserve(tbl_coll.size() + sort_entry.data.size());
            copy(sort_entry.data.begin(), sort_entry.data.end(), 
                                                            back_inserter(tbl_coll));
        }
        else
        {
            switch (iter_char->second.sort_type)
            {
            case sort_type::zero_data1_data2_cp:
                {
                    collation_data sort_entry;
                    sort_entry.variable = iter_char->second.sort_variable;
                    sort_entry.weight1 = 0;
                    sort_entry.weight2 = iter_char->second.sort_index_or_data1;
                    sort_entry.weight3 = iter_char->second.sort_data2;
                    sort_entry.weight4 = cp;
                    tbl_coll.push_back(sort_entry);
                }
                break;
            case sort_type::data1_0x0020_data2_cp:
                {
                    collation_data sort_entry;
                    sort_entry.variable = iter_char->second.sort_variable;
                    sort_entry.weight1 = iter_char->second.sort_index_or_data1;
                    sort_entry.weight2 = 0x0020;
                    sort_entry.weight3 = iter_char->second.sort_data2;
                    sort_entry.weight4 = cp;
                    tbl_coll.push_back(sort_entry);
                }
                break;
            case sort_type::default_:
                {
                    collation_entry sort_entry;
                    get_default_sort_characteristics(cp, sort_entry);
                    // this optimisation requires default to be of size 1
                    //BUG assert(sort_entry.data.size() == 1); 
                    tbl_coll.push_back(sort_entry.data[0]);
                }
                break;
            case sort_type::is_index:
                {
                    if (iter_char->second.sort_data.size() > 1)
                    {
                        // For simplicity we do not handle complex sort data
                        return false;
                    }
                    else if (iter_char->second.sort_data.size() == 0)
                    {
                        return decompose_for_sort(iter_char->second, iter_char->first, props, tbl_coll);
                    }

                    tbl_coll.reserve(tbl_coll.size() + iter_char->second.sort_data[0].data.size());
                    copy(iter_char->second.sort_data[0].data.begin(), 
                                                        iter_char->second.sort_data[0].data.end(), 
                                                                    back_inserter(tbl_coll));
                }
                break;
            default:
                // invalid enum
                assert(iter_char->second.sort_type == (sort_type::type)-1);
                break;
            }
        }
    }

    return true;
}

/*******************************************************************
** optimise_char_sort_data
** Try and optimise the sort data by checking the canonical decomposition
** This reduces the amount of data by about a factor of 3
*******************************************************************/
bool optimise_char_sort_data(const std::map <char32, character_properties> & props,
                                  char32 cp, character_properties & props_char_var)
{
    // if sort decomp is available and works - use it
    // if it does not work then no change  

    // sanity check
    assert(props_char_var.sort_data.size() > 0);
     
    // is there a canonical decomp?
    if (props_char_var.decomposition.empty())
        return false;

    // we cannot simply handle complex sort with entries with following _chars
    if (props_char_var.sort_data.size() != 1)
        return false;

    // sanity check - if there is only one entry then there cannot be any following chars
    assert(props_char_var.sort_data[0].following_chars.size() == 0);

    std::vector<collation_data> tbl_coll;
    
    // check for decomp and return false if it is too complex to optimise
    if (!decompose_for_sort(props_char_var, cp, props, tbl_coll))
        return true;

    std::vector<collation_data>::const_iterator iter_src = 
                                                  props_char_var.sort_data[0].data.begin();
    std::vector<collation_data>::const_iterator end_src = 
                                                  props_char_var.sort_data[0].data.end();
    std::vector<collation_data>::const_iterator iter_try = tbl_coll.begin();
    std::vector<collation_data>::const_iterator end_try = tbl_coll.end();

    for (; (iter_src != end_src) && (iter_try != end_try); iter_src++, iter_try++)
    {
        // compare two primitive structures
        // workaround for boost interop issues
        //if (memcmp(&(*iter_src), &(*iter_try), sizeof(collation_data)) != 0)
        if(*iter_src != *iter_try)
        {
            return false;
        }
    }

    // match - clear down the sort data
    // dummy value of zero indicates that the entry should use canonical decomp
    props_char_var.sort_data.clear();
    props_char_var.sort_index_or_data1 = 0;
    props_char_var.sort_data2 = 0;

    return true;
}

/*******************************************************************
** pack_char_sort_data
** Try and optimise the sort data by packing it using predefined values
*******************************************************************/
void pack_char_sort_data(char32 cp, character_properties & props_char_var)
{
    // sanity checks
    assert(props_char_var.sort_type == sort_type::is_index);
    assert(props_char_var.sort_index_or_data1 == (uint16_t)-1);

    if (props_char_var.sort_data.size() == 1 && 
                            props_char_var.sort_data[0].data.size() == 1)
    {
        // sanity check - there must be at least one entry with no
        // following chars - so if there is only one entry ....
        assert(props_char_var.sort_data[0].following_chars.empty());

        const collation_data& coll = props_char_var.sort_data[0].data[0];

        // now that we have the entry we need to decide if it can be optimised

        // possible optimisation candidate
        if (coll.weight1 == 0 && coll.weight4 == cp)
        {
            props_char_var.sort_variable = coll.variable;
            props_char_var.sort_type = sort_type::zero_data1_data2_cp;
            props_char_var.sort_index_or_data1 = coll.weight2;
            props_char_var.sort_data2 = coll.weight3;
            props_char_var.sort_data.clear();
        }
        else if (coll.weight2 == 0x0020 && coll.weight4 == cp)
        {
            props_char_var.sort_variable = coll.variable;
            props_char_var.sort_type = sort_type::data1_0x0020_data2_cp;
            props_char_var.sort_index_or_data1 = coll.weight1;
            props_char_var.sort_data2 = coll.weight3;
            props_char_var.sort_data.clear();
        }
        else
        {
            // check against default
            collation_entry sort_entry;
            get_default_sort_characteristics(cp, sort_entry);

            if (sort_entry.data.size() == 1 && 
                coll.variable == sort_entry.data[0].variable &&
                coll.weight1 == sort_entry.data[0].weight1 &&
                coll.weight2 == sort_entry.data[0].weight2 &&
                coll.weight3 == sort_entry.data[0].weight3 &&
                coll.weight4 == sort_entry.data[0].weight4)
            {
                // default
                props_char_var.sort_variable = coll.variable;
                props_char_var.sort_type = sort_type::default_;
                props_char_var.sort_index_or_data1 = 0;
                props_char_var.sort_data2 = 0;
                props_char_var.sort_data.clear();
            }
        }
    }
}

/*******************************************************************
** prepare
** Takes the character properties and organises them into blocks 
*******************************************************************/
void prepare(std::map <char32, character_properties> & props, 
						const std::vector <read_block>& blocks, write_data& data)
{
	std::cout << "+Preparing to write data\n";

	data.tbl_blocks.clear();
	data.tbl_decomp.clear();
	data.tbl_complex_case.clear();
    data.tbl_sort_data.clear();
    data.tbl_sort_following_chars.clear();
    data.tbl_sort_entries.clear();
    // scope
    {
        sort_data_entry dummy;
        data.tbl_sort_entries.push_back(make_tuple(0, dummy));
    }

    // write empty following chars at index 0
    data.tbl_sort_following_chars.push_back(make_tuple(0, true, 0));

	data.tbl_blocks.reserve(1 + (int)(0x10FFFD / block_size_const));

	int progress = PROGRESS_INDICATION_EVERY_PREPARE;
	std::cout << " ";

	int index_in_block = 0;
	write_block block;
	for (char32 cp = 0; cp <= 0x10FFFD; cp++)
	{	
		std::map <char32, character_properties>::iterator iter_char = 
                                                                    props.find(cp);

		if (iter_char == props.end())
		{
			// Although it may seem strange, it is valid for codepooints
            // to be missing
			// In those cases they are filled in with the first default 
            // character in that block
            int x;
            for (x = (int)blocks.size() - 1; 
                x >= 0 && (cp < blocks[x].first || cp > blocks[x].last); x--);                                                                    
			
			if (x < 0)
			{
				// This is a gap in the unicode range which no existing block
                // occupies at this time.
				// We therefore supply the unknown character
				iter_char = props.find(character_properties::CHARACTER_DOES_NOT_EXIST);

				if (iter_char == props.end())
				{
					std::stringstream ss;
					ss << "Unknown character not found - search for CHARACTER_DOES_NOT_EXIST and ";
					ss << "check why it was not added explicitly";
					throw std::runtime_error (ss.str());
				}
			}
			else
			{
				// We now must look up the default character for the block
				// Although this really makes sense unicode does not make 
                // things easy for us as in some blocks the first character
                // is a few characters in
				// We therefore try up to five characters from the start of the block
				for (x = 0; x < 5; x++)
				{
					iter_char = props.find(blocks[x].first + x);
					if (iter_char != props.end())
					break;
				}

				if (iter_char == props.end())
				{
					std::stringstream ss;
					ss << "codepoint " << std::hex << blocks[x].first;
					ss << " is a block start and no characters were found near the start of the block";
					throw std::runtime_error (ss.str());
				}
			}
		}

		if (progress-- <= 0)
		{
			progress = PROGRESS_INDICATION_EVERY_PREPARE;
			std::cout << '.' << std::flush;
		}
		
		int decomposition_index = 0;
		int complex_case_index = 0;

		const character_properties& props_char = iter_char->second;

		// check for decompostion data
		if (!props_char.decomposition.empty())
		{
			decomposition_index = (int)data.tbl_decomp.size();
			block.decomposition_index_max = decomposition_index;
			
			data.tbl_decomp.push_back(decomp_entry(cp, props_char.decomposition));
		}

		// check for complex case data
		if (!props_char.complex_case.empty())
		{
			complex_case_index = (int)data.tbl_complex_case.size();
			block.complex_case_index_max = complex_case_index;
			
			data.tbl_complex_case.push_back(make_tuple(cp, props_char.complex_case));
		}

        character_properties& props_char_var = iter_char->second;
        if (props_char.sort_type == (sort_type::type)-1)
        {
            collation_entry sort_entry;
            get_default_sort_characteristics(cp, sort_entry);
            props_char_var.sort_data.clear();
            props_char_var.sort_data.push_back(sort_entry);
        }
            
        if (!props_char.sort_data.empty())
        {
            if (!optimise_char_sort_data(props, cp, props_char_var))
            {
                pack_char_sort_data(cp, props_char_var);
            }
        }

        // write the complex sort data
        if (props_char.sort_type == sort_type::is_index)
        {
            character_properties& props_char_upd = iter_char->second;
            props_char_upd.sort_index_or_data1 = (int)data.tbl_sort_entries.size();

            for (size_t n = 0; n < props_char.sort_data.size(); n++)
            {
                sort_data_entry entry;
                entry.following_chars_length = 
                            (uint16_t)props_char.sort_data[n].following_chars.size();
                if (entry.following_chars_length == 0)
                {
                    entry.following_chars_index = 0;
                }
                else
                {
                    entry.following_chars_index = 
                                            (uint16_t)data.tbl_sort_following_chars.size();
        
                    // add the following_chars to the table
                    for (size_t x = 0; x < props_char.sort_data[n].following_chars.size(); x++)
                    {
                        data.tbl_sort_following_chars.push_back(
                          make_tuple(cp, (x == 0), props_char.sort_data[n].following_chars[x]));
                    }

                    // write zero termination
                    data.tbl_sort_following_chars.push_back(make_tuple(cp, false, 0));
                }

                entry.sort_data_index = (uint16_t)data.tbl_sort_data.size();
                // sanity check
                assert((entry.sort_data_index % 6) == 0);

                // note that data us written in blocks of 6 words
                entry.sort_data_length = 
                                        (uint16_t)props_char.sort_data[n].data.size() * 6;

                // prepare the sort data
                std::vector<uint16_t> tbl_data;
                for (size_t x = 0; x < props_char.sort_data[n].data.size(); x++)
                {
                    const collation_data& coll_data = props_char.sort_data[n].data[x];
                    tbl_data.push_back((coll_data.variable) ? 1 : 0);
                    tbl_data.push_back(coll_data.weight1);
                    tbl_data.push_back(coll_data.weight2);
                    tbl_data.push_back(coll_data.weight3);
                    tbl_data.push_back((uint16_t)(coll_data.weight4 >> 16));
                    tbl_data.push_back((uint16_t)(coll_data.weight4 & 0xFFFF));
                }

                // search for an existing entry that is the same
                std::vector<tuple<bool, uint16_t> >::iterator iter =
                                                                data.tbl_sort_data.begin();
                std::vector<tuple<bool, uint16_t> >::iterator end =
                                                                data.tbl_sort_data.end();
                bool already_present = false;
                int index = 0;
                int index_start = index;
                std::vector<uint16_t>::const_iterator iter_entry;
                std::vector<uint16_t>::const_iterator end_entry = tbl_data.end();
                for (; iter != end && !already_present; iter++, index++)
                {
                    // data is 6 aligned so if we are not testing carry on looping
                    // till we are 6 aligned
                    if ((index % 6) != 0)
                        continue;

                    tuple<bool, uint16_t>& data = *iter;
                    if (!get<0>(data))
                        continue;

                    index_start = index;
                    iter_entry = tbl_data.begin();
                    
                    for (; iter != end; iter++, index++)
                    {
                        if (*iter_entry != get<1>(data))
                        {
                            break;
                        }

                        iter_entry++;

                        if (iter_entry == end_entry)
                        {
                            // success
                            entry.sort_data_index = index_start;
                            // sanity check
                            assert((entry.sort_data_index % 6) == 0);

                            already_present = true;
                            break;
                        }
                    }
                }

                if (!already_present)
                {
                    // add the sort data
                    assert((tbl_data.size() % 6) == 0);
                    for (size_t x = 0; x < tbl_data.size(); x++)
                    {
                        if (x == 0)
                        {
                            data.tbl_sort_data.push_back(
                                make_tuple(true, tbl_data[x]));
                        }
                        else
                        {
                            data.tbl_sort_data.push_back(
                                make_tuple(false, tbl_data[x]));
                        }
                    }
                }

                data.tbl_sort_entries.push_back(make_tuple(cp, entry));
            }
        }

		block.tbl_entry[index_in_block].reset(new write_entry(cp, props_char)); 
		if (++index_in_block == block_size_const)
		{
			block.calc_checksum();

			bool skip_duplicate = false;
			
			// now check to see if the block is a duplicate
			for (int y = (int)data.tbl_blocks.size() - 1; y >= 0; y--)
			{
				const write_block& block2 = data.tbl_blocks[y];

				if (block.checksum_of_entry_properties != (uint32_t)-1 &&
								block.checksum_of_entry_properties == block2.checksum_of_entry_properties)
				{
					// yes it has - now check to see if the block 
                    // entries are actually the same
					// or if the checksum was a statistical anomoly
					int z;
					for (z = block_size_const - 1; 
						z >= 0 && block.tbl_entry[z]->has_same_properties(*block2.tbl_entry[z]); z--);

					if (z < 0)
					{
						// identical block
						skip_duplicate = true;
						data.tbl_block_ident.push_back(block2.ch_first);
						break;
					}
				}
			}

			if (!skip_duplicate)
			{
				data.tbl_block_ident.push_back(block.ch_first);
				data.tbl_blocks.push_back(block);
			}

			index_in_block = 0;

			// reset the block ready for reuse
			block.ch_first = cp + 1;
			block.decomposition_index_min = (int)data.tbl_decomp.size();
			block.decomposition_index_max = -1;
			block.complex_case_index_min = (int)data.tbl_complex_case.size();
			block.complex_case_index_max = -1;
		}
	}
	std::cout << "\n";

    data.props = &props;
	std::cout << "-Preparing to write data\n";
}

#ifdef BOOST_UNICODE_UCD_BIG
static char32 cp_last_sort_data_entry = 0xFFFFF;
#endif

std::fstream& operator << (std::fstream& file, 
                           const tuple<char32, sort_data_entry>& data)
{
#ifdef BOOST_UNICODE_UCD_BIG
	// write the entry
    if (get<0>(data) != cp_last_sort_data_entry)
    {
        file << "\t{  // char 0x" << std::hex << get<0>(data);
        cp_last_sort_data_entry = get<0>(data);
    }
    else
    {
        file << "\t{";
    }
 	
    // there is always sort data
	file << "\n\t\t&__uni_sort_data[" << 
            std::dec << get<1>(data).sort_data_index << "],";
	file << "\n\t\t&__uni_sort_data[" <<
            std::dec << ((int)(get<1>(data).sort_data_index +
            get<1>(data).sort_data_length)) << "],";
    
    if (get<1>(data).following_chars_length == 0)
    {
	    file << "\n\t\t&__uni_sort_follow[0],";
    }
    else
    {
	    file << "\n\t\t&__uni_sort_follow[" <<
            std::dec << get<1>(data).following_chars_index << "],";
    }
    file << "\n\t\t" << get<1>(data).following_chars_length << ",";
    	
    file << "\n\t},\n";
    
#endif
	return file;
}

// write the sort table and update the props sort indexes as appropriate
void write_sort(const write_data& data, const char * dest_path)
{
    std::cout << " writing "<< dest_path
		<< "uni_ucd_interface_impl_sort_data.ipp\n";

    // open file
	std::stringstream fss;
	fss << dest_path;
	fss << "uni_ucd_interface_impl_sort_data.ipp";
	std::fstream file;
	file.open(fss.str().c_str(), std::ios_base::out);

	write_license(file);
	file << "/**** This file should not be included in any file manually           ****/\n";
	file << "/**** This file is automatically generated and should not be modified.****/\n";
	file << "/**** Data in this file should not be accessed directly except        ****/\n";
	file << "/**** through the single published interface as documented in Boost   ****/\n";
	file << "\n\nusing namespace boost::unicode;\n";
	file << "\n\nnamespace boost { namespace unicode { namespace ucd {\n";

#ifdef BOOST_UNICODE_UCD_BIG
	// ---- sort data table ------------------------------------------------------

    std::vector<tuple<bool, uint16_t> >::const_iterator iter_sd = 
                                                    data.tbl_sort_data.begin();
    std::vector<tuple<bool, uint16_t> >::const_iterator end_sd = 
                                                    data.tbl_sort_data.end();

    file << "static const uint16_t __uni_sort_data[] = {\n\t";
    int count = 0;
    int index = 0;
    for (; iter_sd != end_sd; iter_sd++, index++)
    {
        const tuple<bool, uint16_t> & entry = *iter_sd;
        if (get<0>(entry))
        {
            file << "\n\t// index " << std::dec << index << "\n\t";
        }
        
        file << "0x" << std::hex << get<1>(entry) << ", ";
        
        // write sort data in groups of six
        if (((count++ - 5) % 6) == 0)
            file << "\n\t";
    }
    file << "\t};\n\n";
    
	// ---- following chars table ------------------------------------------------
    
    std::vector<tuple<char32, bool, char32> >::const_iterator iter_sf = 
                                            data.tbl_sort_following_chars.begin();
    std::vector<tuple<char32, bool, char32> >::const_iterator end_sf = 
                                            data.tbl_sort_following_chars.end();

    file << "static const char32 __uni_sort_follow[] = {\n";
    // write the empty following_characters at index 0
    file << "\t// empty following_characters\n\tNULL,";

    // skip the null as we just wrote it
    iter_sf++;
    for (; iter_sf != end_sf; iter_sf++)
    {
        const tuple<char32, bool, char32> & entry = *iter_sf;
        if (get<1>(entry))
        {
            file << "\n\t// char 0x" << std::hex << get<0>(entry) << "\n\t";
        }
        
        file << "0x" << std::hex << get<2>(entry) << ", ";
    }
    file << "\t};\n\n";
    
	// ---- sort data entry table ------------------------------------------------

	file << "\n\nBOOST_UNICODE_DECL extern const unichar_sort_data_entry __uni_sort_entry[]= {\n";

    // write the dummy entry
    file << "\t{  // entry means use canonical decomp";
	file << "\n\t\tNULL,";
	file << "\n\t\tNULL,";
    file << "\n\t\tNULL,";
    file << "\n\t\t0,";
    file << "\n\t},\n";

    std::vector<tuple<char32, sort_data_entry> >::const_iterator iter_se = 
                                            data.tbl_sort_entries.begin();
    std::vector<tuple<char32, sort_data_entry> >::const_iterator end_se = 
                                            data.tbl_sort_entries.end();
    // skip the null as we just wrote it
    iter_se++;
    while (iter_se != end_se)
    {
        file << *iter_se;
        
        iter_se++;
    }

	file << "\t};\n\n";

    // ----------------------------------------------------------------------------
#endif

    file << "}}}  // namespaces\n\n";
	
	file.flush();
	file.close();
}

struct lexico_sort
{
    bool operator()(const decomp_entry* lft, const decomp_entry* rgt) const
    {
        return std::lexicographical_compare(
            lft->decomposition.begin(), lft->decomposition.end(),
            rgt->decomposition.begin(), rgt->decomposition.end()
        );
    }
};

struct lexico_comp
{
    bool operator()(const decomp_entry* lft, const decomp_entry* rgt) const
    {
        return lft->decomposition.size() == rgt->decomposition.size()
           &&  std::equal(lft->decomposition.begin(), lft->decomposition.end(), rgt->decomposition.begin());
    }
};

const character_properties& get_properties(const std::map<char32, character_properties>& props, char32 ch)
{
    std::map<char32, character_properties>::const_iterator it = props.find(ch);
    if(it == props.end())
    {
		std::stringstream ss;
		ss << "Code points associated with decomposition not found";
		throw std::runtime_error (ss.str());
    }
    return it->second;
}

void write_compose(const write_data& data, const char * dest_path)
{
    std::cout << " writing "<< dest_path
		<< "uni_ucd_interface_impl_compose_data.ipp\n";

    // open file
	std::stringstream fss;
	fss << dest_path;
	fss << "uni_ucd_interface_impl_compose_data.ipp";
	std::fstream file;
	file.open(fss.str().c_str(), std::ios_base::out);

	write_license(file);
	file << "/**** This file should not be included in any file manually           ****/\n";
	file << "/**** This file is automatically generated and should not be modified.****/\n";
	file << "/**** Data in this file should not be accessed directly except        ****/\n";
	file << "/**** through the single published interface as documented in Boost   ****/\n";
	file << "\n\nusing namespace boost::unicode;\n";
	file << "\n\nnamespace boost { namespace unicode { namespace ucd {\n";

	// ---- compose data table ------------------------------------------------------
    
    std::vector<const decomp_entry*> compose;
    compose.reserve(data.tbl_decomp.size());
    
    for(std::vector<decomp_entry>::const_iterator it = data.tbl_decomp.begin(); it != data.tbl_decomp.end(); ++it)
    {
        /* We exclude non-canonical decompositions, singletons,
         * decompositions that start with non-starters,
         * and code points from the exclusion table */
        if(
            get_properties(*data.props, it->chr).decomposition_kind == ucd::decomposition_type::canonical &&
            it->decomposition.size() > 1 &&
            get_properties(*data.props, it->decomposition[0]).combining == 0 &&
            !get_properties(*data.props, it->chr).comp_ex
        )
            compose.push_back(&*it);
    }
    
    std::sort(compose.begin(), compose.end(), lexico_sort());
    std::vector<const decomp_entry*>::iterator end = std::unique(compose.begin(), compose.end(), lexico_comp());
    if(end != compose.end())
    {
		std::stringstream ss;
		ss << "Duplicates found for canonical composition";
		throw std::runtime_error (ss.str());
    }

    file << "\nBOOST_UNICODE_DECL extern const unichar_compose_data_entry __uni_compose_entry[] = {\n";
    
    for(std::vector<const decomp_entry*>::const_iterator it = compose.begin(); it != compose.end(); ++it)
        file << "\t{__uni_decomp_data_0x" << std::hex << (*it)->chr << ", 0x" << (*it)->chr << "},\n";
    
    file << "};\n\n";
    file << "BOOST_UNICODE_DECL extern const size_t __uni_compose_entry_size = sizeof __uni_compose_entry / sizeof __uni_compose_entry[0];\n\n";

    // ----------------------------------------------------------------------------
    
    file << "}}}  // namespaces\n\n";
	
	file.flush();
	file.close();
}

/*******************************************************************
** write
** Writes the blocks to the output file
*******************************************************************/
void write(const std::vector <read_block>& blocks, const write_data& data, 
                              const char * dest_path, const char* dest_path_ucd_props)
{
	std::cout << "+Writing data\n";

	int blocks_in_file = 0;
	int file_index = 0;

	// work out how many blocks will be in each file
	size_t block_size = data.tbl_blocks.size();

	int blocks_per_file = ((int)block_size + (write_block::NUMBER_FILES / 2)) / 
													write_block::NUMBER_FILES;

    std::fstream file;
	file.exceptions (std::ios_base::failbit | std::ios_base::badbit);
	for (size_t block_index = 0; block_index < block_size; block_index++)
	{
		if (blocks_in_file <= 0)
		{
			if (file.is_open())
			{
				file << "}}}	// namespaces\n";

				file.flush();
				file.close();
			}

			std::cout << " writing "<< dest_path
				<< "uni_ucd_interface_impl_data_"
				<< std::dec << (file_index+1) << ".ipp\n";
			// open file
			std::stringstream fss;
			fss << dest_path;
			fss << "uni_ucd_interface_impl_data_" << std::dec << ++file_index
				<< ".ipp";
			file.open(fss.str().c_str(), std::ios_base::out);
			// reset count
			blocks_in_file = blocks_per_file - 1;
			
			write_license(file);
			file << "/**** This file should not be included in any file manually           ****/\n";
			file << "/**** This file is automatically generated and should not be modified.****/\n";
			file << "/**** Data in this file should not be accessed directly except        ****/\n";
			file << "/**** through the single published interface as documented in Boost   ****/\n";
			file << "\n\nusing namespace boost::unicode;\n\n";
			file << "\n\nnamespace boost { namespace unicode { namespace ucd {\n";
		}

		blocks_in_file--;

		// do the write
		data.tbl_blocks[block_index].write_decomp_for_block(file, data.tbl_decomp);
		data.tbl_blocks[block_index].write_complex_case_block(file, data.tbl_complex_case);
		file << data.tbl_blocks[block_index];
	}

	file << "}}}	// namespaces\n";

	// close the file
	file.flush();
	file.close();

	write_block_tables_and_blocks(blocks, data.tbl_block_ident, dest_path, file_index);
	write_block_enum(blocks, dest_path_ucd_props);
    write_sort(data, dest_path);
    write_compose(data, dest_path);

	std::cout << "-Writing data\n";
}


}}}	// namespaces

