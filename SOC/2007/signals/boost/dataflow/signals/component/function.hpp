// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FUNCTION_HPP
#define SIGNAL_NETWORK_FUNCTION_HPP

#include <boost/dataflow/signals/component/modifier.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/function.hpp>

namespace boost { namespace signals {

namespace detail
{
    template<typename FunctionSignature, typename Signature>
    struct function_adapter
    {
        function_adapter(const boost::function<FunctionSignature> &f) :
            func(f),
            fused_func(boost::fusion::fused<boost::function<FunctionSignature> const &>(func)) {}
        function_adapter(const function_adapter &fa) :
            func(fa.func),
        fused_func(boost::fusion::fused<boost::function<FunctionSignature> const &>(func)) {}
        template<typename Seq>
        const typename storage<Signature>::storable_vector &
        operator()(const Seq &seq)
        {
            boost::fusion::at_c<0>(vector) = fused_func(seq);
            return vector;
        }
    protected:
        typename storage<Signature>::storable_vector vector;
        boost::function<FunctionSignature> func;
        boost::fusion::fused<boost::function<FunctionSignature> const &> fused_func;
    };
}
/** \brief Converts a function into a [DataflowSignals] filter.
\param Signature Signature of the function to be converted.

The signals::function object will receive signals of signature void(<i>function arguments</i>),
and send signals of signature void(<i>function return type</i>).
*/
template<
    typename Signature,
    typename FunctionSignature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename SignalArgs=typename default_signal_args<Signature>::type
> 
class function
    : public modifier<
        function<Signature, FunctionSignature, OutSignal, SignalArgs>,
        detail::function_adapter<FunctionSignature, Signature>, Signature, OutSignal, SignalArgs>
{
public:
    typedef modifier<
        function<Signature, FunctionSignature, OutSignal, SignalArgs>,
        detail::function_adapter<FunctionSignature, Signature>, Signature, OutSignal, SignalArgs> base_type;

    function(const boost::function<FunctionSignature> &f) : base_type(f) {}
};

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_FUNCTION_HPP
