#ifndef BOOST_UNICODE_UNICHAR_DATA_HPP
#define BOOST_UNICODE_UNICHAR_DATA_HPP

#include <boost/assert.hpp>
#include <boost/integer/static_pow.hpp>
#include <boost/integer/static_log2.hpp>

#include <boost/cuchar.hpp>
#include <boost/unicode/ucd/properties_types.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#define BOOST_UNICODE_ENUM_SIZE(en) (boost::static_log2<boost::unicode::ucd::en::_count>::value + 1)

//#define BOOST_UNICODE_UCD_BIG 1

namespace boost
{
namespace unicode
{
namespace ucd
{
    /***************************************************************
    *** These structure are for internal use only and should not be
    **** used by any developer unless they are providing a custom
    **** implementation of the character properties interface
    **** Changes to this must be reflected
    **** in write_character_properties.cpp:
    **** fnct: operator << (std::fstream& file, const write_entry& data)
    ****************************************************************/
    const int complex_case_size_const = 4;
    const int block_size_bits_const = 7;
    const int block_size_const = static_pow<2, block_size_bits_const>::value;

    struct unichar_sort_data_entry
    {
        // to optimise the structure across different targets we move the
        // pointers to the front
        const uint16_t *    sort_data_begin;
        const uint16_t *    sort_data_end;
        const char32 *      following_chars;
        const uint16_t      following_chars_count;
    };
    
    struct unichar_compose_data_entry
    {
        const char32* decomp;
        char32 ch;
    };

    struct unichar_complex_case_internal
    {
        const int       length_uppercase;
        const char32    uppercase[complex_case_size_const]; 
        const int       length_lowercase;
        const char32    lowercase[complex_case_size_const]; 
        const int       length_titlecase;
        const char32    titlecase[complex_case_size_const]; 
        // There may be several sequential entries for complex case
        // for a single character
        // Entries will be parsed in order until an entry with 
        // finalEntry is found 
        const bool      final_entry;
        // TODO local and conditions
    };

    struct sort_type
    {
        enum type
        {
            // this enum is used to reduce the data size required
            // to represent the sort data from the uca

            // sort data is default value for the character range for the character
            default_ = 0, 

            // sort data is defined in the unichar_sort_data_entry table
            // and sort_index_or_data1 is the index into the table
            is_index,

            // sort data is 0000.data1.data2.char32
            zero_data1_data2_cp,

            // sort data is data1.0020.data2.char32
            data1_0x0020_data2_cp, 

            _count
        };
        enum {  data_0x0020_0x000D_cp_offset = 5   };
    };
    BOOST_UNICODE_DECL const char* as_string(sort_type::type);

    struct unichar_data_internal
    {
        // to optimise the structure across different targets we move the
        // pointers to the front
#ifdef BOOST_UNICODE_UCD_BIG
        const char *     name;
#endif
        const char32 *   decomp;
#ifdef BOOST_UNICODE_UCD_BIG
        const unichar_complex_case_internal* complex_case;
#endif

        struct
        { 
            unsigned  category : BOOST_UNICODE_ENUM_SIZE(category);
#ifdef BOOST_UNICODE_UCD_BIG
            unsigned  join_type : BOOST_UNICODE_ENUM_SIZE(join_type);
#endif
            unsigned  word_break : BOOST_UNICODE_ENUM_SIZE(word_break);
#ifdef BOOST_UNICODE_UCD_BIG
            // first value for sort_variable
            unsigned  sort_variable : 1;
            unsigned  sort_data_type : 2;
            unsigned  sort_data2 : 16;
#endif

            unsigned  bidi_class : BOOST_UNICODE_ENUM_SIZE(bidi_class);           
            unsigned  decomposition_type : BOOST_UNICODE_ENUM_SIZE(decomposition_type);    
            unsigned  line_break : BOOST_UNICODE_ENUM_SIZE(line_break);
            unsigned  combining : 8;
            unsigned  sentence_break : BOOST_UNICODE_ENUM_SIZE(sentence_break); 
            unsigned  grapheme_cluster_break : BOOST_UNICODE_ENUM_SIZE(grapheme_cluster_break);
        };
#ifdef BOOST_UNICODE_UCD_BIG
        // the meaning of this is controlled by predefined_sort
        const uint16_t   sort_index_or_data1;
        const char32     uppercase;
        const char32     lowercase;
        const char32     titlecase;
#endif
    };

    struct unichar_blocks_internal
    {
        const char32     first;
        const char32     last;
        const char*      name;
    };

    BOOST_UNICODE_DECL extern const unichar_data_internal* __uni_char_data[];
    BOOST_UNICODE_DECL extern const unichar_blocks_internal __uni_block_data[];
    BOOST_UNICODE_DECL extern const size_t __uni_block_data_size;
#ifdef BOOST_UNICODE_UCD_BIG
    BOOST_UNICODE_DECL extern const unichar_sort_data_entry __uni_sort_entry[];
#endif
    BOOST_UNICODE_DECL extern const unichar_compose_data_entry __uni_compose_entry[];
    BOOST_UNICODE_DECL extern const size_t __uni_compose_entry_size;

    
    inline const unichar_data_internal& get_data_internal(char32 ch)
    {
        BOOST_ASSERT(ch <= 0x10FFFD);
        
        return __uni_char_data
            [ch >> block_size_bits_const]
            [ch & (block_size_const-1)];
    }

} // namespace ucd
} // namespace unicode
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
