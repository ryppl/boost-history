#ifndef SIGNAL_NETWORK_TEMPLATE_BODY
public:
	function_impl() {}

	void operator()(SIGNAL_NETWORK_ARG_TYPENAMES(SIGNAL_NETWORK_TEMPLATE_ARITY))
	{
		out(func(SIGNAL_NETWORK_ARG_NAMES(SIGNAL_NETWORK_TEMPLATE_ARITY)));
	}

protected:
	boost::function<Signature> func;
#else
public:
	function(boost::function<Signature> f) {SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE::func = f;}
#endif