// function.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FUNCTION_HPP
#define SIGNAL_NETWORK_FUNCTION_HPP

#include <boost/signal_network/modifier.hpp>
#include <boost/function.hpp>
#include <boost/signal_network/storage.hpp>
SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

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
/** \brief Converts a function into a Signal Network filter.
\param Signature Signature of the function to be converted.

The signet::function object will receive signals of signature void(<i>function arguments</i>),
and send signals of signature void(<i>function return type</i>).
*/
template<typename Signature,
    typename FunctionSignature,
    typename OutSignal=default_out_signal,
    typename Combiner = boost::last_value<void>,
    typename Group = int,
    typename GroupCompare = std::less<Group>,
    typename Base = modifier<detail::function_adapter<FunctionSignature, Signature>, Signature, OutSignal, Combiner, Group, GroupCompare>
>
class function : public Base
{
public:
    typedef function<Signature, FunctionSignature, OutSignal, Combiner, Group, GroupCompare, typename Base::unfused > unfused;

    function(const boost::function<FunctionSignature> &f) : Base(f) {}
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_FUNCTION_HPP