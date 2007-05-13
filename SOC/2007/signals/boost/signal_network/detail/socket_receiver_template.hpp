#ifndef SIGNAL_NETWORK_TEMPLATE_BODY

public:
	socket_receiver_impl(asio::ip::tcp::socket &socket) :
		socket_(socket),
		stream(std::ios::in | std::ios::out | std::ios::binary) {}
	void operator()()
	{
		int received_size = socket_.receive(asio::buffer(buffer, bufferSize));
		stream.str(empty_string);
		stream.write((char *)buffer, received_size);
		boost::archive::binary_iarchive archive(stream);
		BOOST_PP_REPEAT_FROM_TO(1,SIGNAL_NETWORK_TEMPLATE_ARITY_INC,SIGNAL_NETWORK_SOCKET_UNPACK,_)
		
		storage<Signature>::operator()();
	}

private:
	asio::ip::tcp::socket &socket_;
	std::stringstream stream;
	std::string empty_string;
	boost::archive::binary_iarchive *archive;
	enum { bufferSize = 20000 };
	unsigned char buffer[bufferSize];
#else

public:
	socket_receiver(asio::ip::tcp::socket &socket) : SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE (socket) {}

#endif