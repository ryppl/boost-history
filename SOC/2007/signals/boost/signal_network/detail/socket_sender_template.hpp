#ifndef SIGNAL_NETWORK_TEMPLATE_BODY

public:
	socket_sender_impl(asio::ip::tcp::socket &socket) :
		socket_(socket),
		stream(std::ios::in | std::ios::out | std::ios::binary) {};
	SIGNAL_NETWORK_FUNCTION(SIGNAL_NETWORK_DEFAULT_SLOT,Signature,SIGNAL_NETWORK_TEMPLATE_ARITY)
	{
		boost::archive::binary_oarchive archive (stream);
		BOOST_PP_REPEAT_FROM_TO(1,SIGNAL_NETWORK_TEMPLATE_ARITY_INC,SIGNAL_NETWORK_SOCKET_PACK,_)
		boost::asio::write(socket_, asio::buffer(stream.str()),
		boost::asio::transfer_all());
		stream.str(empty_string);
	}
private:
	asio::ip::tcp::socket &socket_;
	std::stringstream stream;
	std::string empty_string;

#else

public:
	socket_sender(asio::ip::tcp::socket &socket) : SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE(socket) {}
#endif