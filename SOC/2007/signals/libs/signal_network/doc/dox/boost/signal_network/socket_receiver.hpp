#include <boost/signal_network/storage.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

/** \brief Receives serializable signals through a Boost asio socket.
	\param Signature Signature of the signal sent (and received through the socket).
	\warning Since I can't get Doxygen to unravel the preprocessed definition of this
		class, the documentation was generated through a non-functioning class Doxygen could read.
	\todo socket_sender only works for Signatures of return type void.

	\par Example:
	\dontinclude example.cpp
	\skip asio test
	\until end void asio_test
*/
template<typename T, typename Signature>
class socket_receiver : public storage<Signature>
{
public:
	/// Initializes the socket_sender to use the provided socket.
	socket_receiver(asio::ip::tcp::socket &socket);
	/// Sets the receiver to wait for a packet and send its contents via the signal.
	void operator()();
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE
