#include <iostream>
#include <boost/foreach.hpp>
#include <boost/unicode/unicode_iterator.hpp>

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
	std::vector<boost::uint32_t> v;
	
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
	
	
	BOOST_FOREACH(uint32_t cp, boost::make_u8_to_u32_range(v))
		std::cout << std::hex << cp << std::endl;
}
