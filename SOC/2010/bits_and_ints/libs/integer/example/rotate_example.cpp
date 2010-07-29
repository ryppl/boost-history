#include <boost/assert.hpp>
#include <boost/integer/rotate.hpp>

int main()
{
	using boost::rotate_left;
	using boost::rotate_right;
	
	BOOST_ASSERT((rotate_left(0xABCDEF12u, 24) == 0x12ABCDEFu));
	BOOST_ASSERT((rotate_left(0x12345678u,  4) == 0x23456781u));
	BOOST_ASSERT((rotate_left(0x0FF00000u,  6) == 0xFC000003u));
	BOOST_ASSERT((rotate_left(0x00000000u, 31) == 0x00000000u));
	BOOST_ASSERT((rotate_left(0xF0F0F0F0u,  4) == 0x0F0F0F0Fu));
	
	BOOST_ASSERT((rotate_right(0xABCDEF12u,  8) == 0x12ABCDEFu));
	BOOST_ASSERT((rotate_right(0x0000000Fu,  1) == 0x80000007u));
	BOOST_ASSERT((rotate_right(0xFF00FF00u,  8) == 0x00FF00FFu));
	BOOST_ASSERT((rotate_right(0xAABBCCDDu, 16) == 0xCCDDAABBu));
	BOOST_ASSERT((rotate_right(0xFFFFFFFFu, 31) == 0xFFFFFFFFu));
}