#ifndef SIGNAL_NETWORK_TEMPLATE_BODY

	selector_impl() : selected(0) {}
	void select(int source) {selected = source;}
	BOOST_PP_REPEAT_FROM_TO(1,_inputs_inc,SIGNAL_NETWORK_SELECTOR_INPUT,SIGNAL_NETWORK_TEMPLATE_ARITY)
private:
	volatile int selected;

#else

#endif