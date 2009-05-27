#include <iostream>
#include <boost/foreach.hpp>
#include <boost/unicode/utf_conversion.hpp>

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
	
	
	BOOST_FOREACH(char cp, make_reverse_range(boost::make_u32_to_u8_range(v)))
		std::cout << std::hex << (int)(unsigned char)cp << std::endl;
}
