// function.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_FUNCTION_HPP
#define SIGNAL_NETWORK_FUNCTION_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/function.hpp>
#include <boost/signal_network/detail/unfused_inherited.hpp>
#include <boost/mpl/vector.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

template<typename Signature>
class function : public filter<void (typename boost::function_traits<Signature>::result_type)>
{
    typedef boost::function_types::parameter_types<Signature> ParTypes;
public:
    typedef boost::fusion::unfused_inherited<function<Signature>,
        typename mpl::vector<boost::function<Signature> >::type,
        typename boost::function_types::parameter_types<Signature> > unfused;

    function(boost::function<Signature> f) :
      func(f),
      fused_func(boost::fusion::fused<boost::function<Signature> const &>(func)) {}
    template <class Seq>
    struct result
    {
        typedef void type;
    };
    template <class Seq>
    void operator()(const Seq &vec_par)
    {
        out(fused_func(vec_par));
    }
protected:
   	boost::function<Signature> func;
    boost::fusion::fused<boost::function<Signature> const &> fused_func;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_FUNCTION_HPP