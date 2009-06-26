#include <iostream>
#include <boost/foreach.hpp>
#include <boost/unicode/utf.hpp>
#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/graphemes.hpp>

#include <boost/typeof/typeof.hpp>

#include <algorithm>

template<typename Range>
std::pair<
	boost::reverse_iterator<typename boost::range_iterator<Range>::type>,
	boost::reverse_iterator<typename boost::range_iterator<Range>::type>
> make_reverse_range(const Range& r)
{
	return std::make_pair(
		boost::make_reverse_iterator(boost::end(r)),
		boost::make_reverse_iterator(boost::begin(r))
	);
}

template<typename Range, typename OutputIterator>
void copy(const Range& range, OutputIterator out)
{
    std::copy(boost::begin(range), boost::end(range), out);
}

template<typename Range>
size_t count(const Range& range)
{
    size_t count = 0;
    for(typename boost::range_iterator<const Range>::type it = boost::begin(range); it != boost::end(range); ++it)
        count++;
        
    return count;
}

#define FOREACH_AUTO(name, range)                                      \
if(boost::begin(range) != boost::end(range))                           \
if(bool _once_##__LINE__ = 1)                                          \
for(                                                                   \
    BOOST_AUTO(name, *boost::begin(range));                            \
    _once_##__LINE__;                                                  \
    _once_##__LINE__ = 0                                               \
)                                                                      \
for(                                                                   \
    BOOST_AUTO(_it_##__LINE__, boost::begin(range));                   \
    name = *_it_##__LINE__, _it_##__LINE__ != boost::end(range);       \
    ++_it_##__LINE__                                                   \
)                                                                      \

int main()
{
	std::vector<boost::char32> v;
	
	/*v.push_back(122);
	v.push_back(27700);
	v.push_back(119070);
	v.push_back(123);*/
	
	v.push_back(1);
	v.push_back(0xE9);
	v.push_back(3);
	
	
	/*v.push_back(0x7a);
	v.push_back(0x6c34);
	v.push_back(0xd834);
	v.push_back(0xdd1e);
	v.push_back(0x7b);*/
	
	
	BOOST_FOREACH(char cp, make_reverse_range(boost::u8_encoded(v)))
		std::cout << std::hex << (int)(unsigned char)cp << std::endl;
        
    std::vector<char> v2;
    copy(v, boost::u8_encoded_out(std::back_inserter(v2)));
    
    BOOST_FOREACH(char cp, boost::u8_decoded(v2))
		std::cout << std::hex << (int)(unsigned char)cp << std::endl;
        
    std::cout << std::endl;
    
    BOOST_AUTO(range, boost::u8_bounded( boost::u8_encoded(v) ) );
    FOREACH_AUTO(code_points, range)
    {
        FOREACH_AUTO(cu, code_points)
            std::cout << ' ' << std::hex << (int)(unsigned char)cu;
            
        std::cout << ',';
    }
    
    std::cout << "\n";
    
    char foo[] = "eoaéôn";
    FOREACH_AUTO(subrange, boost::u8_bounded(foo))
    {
        BOOST_FOREACH(unsigned char c, subrange)
            std::cout << c;

        std::cout << ' ';
    }
    std::cout << std::endl;
        
    std::cout << "\n" << boost::unicode::ucd::get_name(0xE9) << std::endl;
    std::cout << boost::unicode::ucd::as_string(boost::unicode::ucd::get_block(0xE9)) << std::endl;
    
    boost::char32 grapheme_test[] = {
        'f', 'o', 'o', '\r', '\n', 275, 769, ' ', 0x1e17
    };
    
    FOREACH_AUTO(code_points, boost::grapheme_bounded(grapheme_test))
    {
        BOOST_FOREACH(unsigned char c, boost::u8_encoded(code_points))
            std::cout << c;
        std::cout << '(' << count(code_points) << ')';
    }
    
    std::cout << std::endl;
}
