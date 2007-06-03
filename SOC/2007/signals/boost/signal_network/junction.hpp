// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_JUNCTION_HPP
#define SIGNAL_NETWORK_JUNCTION_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/signal_network/detail/unfused_inherited.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

template<typename Signature>
class junction : public fused_filter<Signature>
{
public:
    typedef boost::fusion::unfused_inherited<junction<Signature>,
        typename mpl::vector<>::type,
        typename boost::function_types::parameter_types<Signature> > unfused;

    junction() : enabled(true) {}
    void enable() {enabled = true;}
    void disable() {enabled = false;}

    template <class Seq>
    struct result
    {
        typedef typename boost::function_traits<Signature>::result_type type;
    };

    typename boost::function_traits<Signature>::result_type 
    operator()(const typename fused_filter<Signature>::parameter_vector &vec_par)
    {
    	if (enabled)
             return static_cast<typename boost::function_traits<Signature>::result_type>
                 (fused_out(vec_par));
    }
private:
    volatile bool enabled;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif