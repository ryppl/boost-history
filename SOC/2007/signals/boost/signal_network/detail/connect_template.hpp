#ifndef SIGNAL_NETWORK_TEMPLATE_BODY
		static void connect(boost::signal<Signature> &signal, T &link)
		{
			signal.connect(boost::bind(
			(typename slot_type<T, Signature>::type) &T::operator(),
				boost::ref(link) SIGNAL_NETWORK_BIND_ARGS(SIGNAL_NETWORK_TEMPLATE_ARITY)));
		}
#else

#endif
