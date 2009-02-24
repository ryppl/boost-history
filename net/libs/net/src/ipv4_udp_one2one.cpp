#include <boost/net/ipv4/udp/one2one.hpp>

#include <boost/net/detail/move.hpp>

#include <boost/net/ipv4/udp/one2many.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace udp
{
	one2one::one2one( net::detail::socket const& s)
	:
	socket_base_type( s),
	net::detail::options( member),
	ipv4::detail::options( member)
	{}

const one2many
one2one::disconnect()
{
	BOOST_ASSERT( is_valid() );

	sockaddr_in4_type addr;
	sockaddr_len_type len( sizeof( sockaddr_in4_type) );
	::bzero( & addr, len);
	addr.sin_family = family_unspecified;
	void * vp( & addr);
	system::error_code ec;
	net::detail::connect(
		native_handle(),
		static_cast< sockaddr_type * >( vp),
		len,
		ec);

	return one2many( net::detail::socket( member.move() ) );
}
}}}}
