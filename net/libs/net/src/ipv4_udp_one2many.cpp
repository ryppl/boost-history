#include <boost/net/ipv4/udp/one2many.hpp>

#include <boost/net/detail/move.hpp>

#include <boost/net/ipv4/udp/one2one.hpp>

namespace boost { namespace net {
namespace ipv4 { namespace udp
{
	one2many::one2many( net::detail::socket const& s)
	:
	socket_base_type( s),
	net::detail::options( member),
	ipv4::detail::options( member)
	{}

const one2one
one2many::connect( address const& addr)
{
	BOOST_ASSERT( is_valid() );

	system::error_code ec;
	net::detail::connect(
		native_handle(),
		addr.sockaddr_ptr(),
		addr.size(),
		ec);
	if ( ec) throw_exception( system::system_error( ec) );

	return one2one( net::detail::socket( member.move() ) );
}
}}}}
