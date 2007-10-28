// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_CHAIN_HPP
#define SIGNAL_NETWORK_CHAIN_HPP

#include <boost/dataflow/signal/component/filter_base.hpp>
#include <boost/dataflow/signal/component/detail/unfused_inherited.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/adapted/mpl.hpp>
#include <boost/function_types/parameter_types.hpp>

namespace boost { namespace signals {

namespace detail
{
    template<typename Signature, typename T>
    class chain_impl : public filter_base<typename T::signal_type>
    {
    protected:
        typedef typename boost::function_types::parameter_types<Signature>::type parameter_types;
        typedef typename boost::fusion::result_of::as_vector<parameter_types >::type parameter_vector;
public:
        typedef typename T::signal_type signal_type;
        typedef typename T::signature_type signature_type;
        typedef T proxy_producer_for;
    
        chain_impl(size_t copies, T *component=NULL)
        {
            initialize(copies, component);
        }
        ~chain_impl()
        {
            delete[] components;
        }
        template <class Seq>
        struct result
        {
            typedef typename boost::function_traits<Signature>::result_type type;
        };
        /// Sending a signal to the chain will forward it to the first component in the chain.
        typename boost::function_traits<Signature>::result_type 
        operator()(const parameter_vector &vec_par)
        {
            return boost::fusion::fused<T &>(components[0])(vec_par);
        }
        ///	The default signal coming out of the chain is the default signal of the last component in the chain.
        typename T::signal_type &default_signal() const
        {
            return components[size-1].default_signal();
        }
        typename T::signal_type &get_proxied_producer() const
        {
            return boost::dataflow::get_port<dataflow::signals::mechanism, dataflow::ports::producer>(components[size-1]);
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
}

/** \brief Connects a number of components of the same type in a chain.
\param T Type of the component.
\param Signature Signature of the signal sent and received.

*/
template<typename Signature, typename T>
class chain : public boost::fusion::unfused_inherited<detail::chain_impl<Signature, T>,
typename boost::function_types::parameter_types<Signature>::type >
{
protected:
    typedef boost::fusion::unfused_inherited<detail::chain_impl<Signature, T>,
        typename boost::function_types::parameter_types<Signature>::type > base_type;
public:
    ///	Constructs a chain composed of instances of T.
    ///	Constructs a chain composed of copies of component.
	chain(size_t copies, T *component=NULL) : base_type(copies, component) {}
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_CHAIN_HPP
