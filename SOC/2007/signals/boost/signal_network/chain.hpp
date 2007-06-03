// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CHAIN_HPP
#define SIGNAL_NETWORK_CHAIN_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/signal_network/detail/unfused_inherited.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

template<typename T, typename Signature>
class chain : public filter_base
{
public:
    typedef boost::function_types::parameter_types<Signature> parameter_types;
    typedef typename boost::fusion::result_of::as_vector<parameter_types >::type parameter_vector;

    typedef boost::fusion::unfused_inherited<chain<T, Signature>,
        typename mpl::vector<size_t, T *>::type,
        typename boost::function_types::parameter_types<Signature> > unfused;

	chain(size_t copies, T *component=NULL)
	{
		initialize(copies, component);
	}
	~chain()
	{
		delete[] components;
	}
    template <class Seq>
    struct result
    {
        typedef typename boost::function_traits<Signature>::result_type type;
    };
    typename boost::function_traits<Signature>::result_type 
    operator()(const parameter_vector &vec_par)
    {
        return boost::fusion::fused<T &>(components[0])(vec_par);
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

};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_CHAIN_HPP
