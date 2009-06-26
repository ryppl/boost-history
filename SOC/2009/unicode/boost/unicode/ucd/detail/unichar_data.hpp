#ifndef BOOST_UNICODE_UNICHAR_DATA_HPP
#define BOOST_UNICODE_UNICHAR_DATA_HPP

#include <boost/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/integer/static_log2.hpp>

#include <boost/cuchar.hpp>
#include <boost/unicode/ucd/properties_types.hpp>

#define BOOST_UNICODE_ENUM_SIZE(en) (boost::static_log2<boost::unicode::ucd::en::_count>::value + 1)

namespace boost
{
namespace unicode
{
namespace ucd
{
    namespace detail
    {
        template<int X, int Y>
        struct pow : boost::mpl::int_< X * pow<X, Y-1>::value >
        {
        };
    
        template<int X>
        struct pow<X, 0> : boost::mpl::int_<1>
        {
        };
    } // namespace detail

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
    const int block_size_const = detail::pow<2, block_size_bits_const>::value;

    struct unichar_sort_data_entry
    {
        // to optimise the structure across different targets we move the
        // pointers to the front
        const uint16_t *    sort_data_begin;
        const uint16_t *    sort_data_end;
        const char32 *      following_chars;
        const uint16_t      following_chars_count;
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
    const char* as_string(sort_type::type);

    struct unichar_data_internal
    {
        // to optimise the structure across different targets we move the
        // pointers to the front
        const char *     name;
        const char32 *   decomp;
        const unichar_complex_case_internal* complex_case;

        struct
        { 
            unsigned  category : BOOST_UNICODE_ENUM_SIZE(category);
            unsigned  join_type : BOOST_UNICODE_ENUM_SIZE(join_type);
            unsigned  word_break : BOOST_UNICODE_ENUM_SIZE(word_break);
            unsigned  unknown_char : 1;
            // first value for sort_variable
            unsigned  sort_variable : 1;
            unsigned  sort_data_type : 2;
            unsigned  sort_data2 : 16;

            unsigned  bidi_class : BOOST_UNICODE_ENUM_SIZE(bidi_class);           
            unsigned  decomposition_type : BOOST_UNICODE_ENUM_SIZE(decomposition_type);    
            unsigned  break_class : BOOST_UNICODE_ENUM_SIZE(break_class);
            unsigned  combining : 8;
            unsigned  sentence_break : BOOST_UNICODE_ENUM_SIZE(sentence_break); 
            unsigned  grapheme_cluster_break : BOOST_UNICODE_ENUM_SIZE(grapheme_cluster_break);
        };
        // the meaning of this is controlled by predefined_sort
        const uint16_t   sort_index_or_data1;
        const char32     uppercase;
        const char32     lowercase;
        const char32     titlecase;
    };

    struct unichar_blocks_internal
    {
        const char32     first;
        const char32     last;
        const char*      name;
    };

    extern const boost::unicode::ucd::unichar_data_internal* __uni_char_data[];
    extern const boost::unicode::ucd::unichar_blocks_internal __uni_block_data[];
    extern const boost::unicode::ucd::unichar_sort_data_entry __uni_sort_entry[];
    
    extern const size_t __uni_block_data_size;
    
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

#endif
