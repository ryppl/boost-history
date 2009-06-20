#include <boost/unicode/ucd/properties.hpp>
#include <algorithm>

using namespace boost::unicode::ucd;
using namespace boost;

namespace detail
{
    struct block_find
    {
        bool operator()(const boost::unicode::ucd::unichar_blocks_internal& a, const boost::unicode::ucd::unichar_blocks_internal& b) const
        {
            return a.first < b.first;
        }
        
        bool operator()(const boost::unicode::ucd::unichar_blocks_internal& a, char32 b) const
        {
            return a.first < b;
        }
    };
}

boost::unicode::ucd::block::type boost::unicode::ucd::get_block(char32 ch)
{
    const unichar_blocks_internal* end = __uni_block_data + __uni_block_data_size;
    
    const unichar_blocks_internal* b = std::lower_bound(
        __uni_block_data, end,
        ch, ::detail::block_find()
    );
    
    if(b == __uni_block_data || b == end)
        return block::none;
    
    --b;
    if(ch > b->last)
        return block::none;
    
    return (boost::unicode::ucd::block::type)b->first;
}

const char* boost::unicode::ucd::as_string(boost::unicode::ucd::block::type type)
{
    const unichar_blocks_internal* b = std::lower_bound(
        __uni_block_data, __uni_block_data + __uni_block_data_size,
        type, ::detail::block_find()
    );
    
    return b->name;
}
