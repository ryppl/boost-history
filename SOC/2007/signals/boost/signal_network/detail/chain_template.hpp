#ifndef SIGNAL_NETWORK_TEMPLATE_BODY

	chain_impl(size_t copies, T *component=NULL)
	{
		initialize(copies, component);
	}
	~chain_impl()
	{
		delete[] components;
	}
	SIGNAL_NETWORK_FUNCTION(SIGNAL_NETWORK_DEFAULT_SLOT,Signature,SIGNAL_NETWORK_TEMPLATE_ARITY)
	{
		SIGNAL_NETWORK_RETURN(_return) components[0](SIGNAL_NETWORK_ARG_NAMES(SIGNAL_NETWORK_TEMPLATE_ARITY));
	}
	boost::signal<Signature> &default_signal()
	{
		return components[size-1].default_signal();
	}
private:
	void initialize(size_t copies, T *component=NULL)
	{
		size = copies;
		components = new T[copies];
		for (size_t i=0; i<copies-1; i++)
		{
			if (component) components[i] = *component;
			components[i] >>= components[i+1];
		}
		if (component) components[size-1] = *component;
	}
	T *components;
	size_t size;

#else
public:
	chain(size_t copies) : SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE(copies) {}
	chain(size_t copies, T &component) : SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE(copies, &component) {}

#endif