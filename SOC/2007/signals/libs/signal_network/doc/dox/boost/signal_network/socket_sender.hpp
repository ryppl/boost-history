#include <boost/signal_network/detail/defines.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Sends serializable signals through a Boost asio socket.
	\param Signature Signature of the signal received (and sent through the socket).
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.
	\todo socket_sender only works for Signatures of return type void.

	\par Example:
	\dontinclude example.cpp
	\skip asio test
	\until end void asio_test
*/
template<typename T, typename Signature>
class socket_sender
{
public:
	/// Initializes the socket_sender to use the provided socket.
	socket_sender(asio::ip::tcp::socket &socket);
	/// Serializes each of the arguments and sends them in a single packet through the socket.
	void operator()(...);
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
