#ifndef SIGNAL_NETWORK_TEMPLATE_BODY
		static void connect_slot(boost::signal<Signature> &signal, const slot_selector_t<T, Signature> &slot)
		{
			signal.connect(boost::bind(
			(typename slot_type<T, Signature>::type) slot.func,
				boost::ref(slot.link) SIGNAL_NETWORK_BIND_ARGS(SIGNAL_NETWORK_TEMPLATE_ARITY)));
		}
#else

#endif
