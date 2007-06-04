// storage.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_GENERATOR_HPP
#define SIGNAL_NETWORK_GENERATOR_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>

#include <boost/function_types/parameter_types.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>

#include <boost/signal_network/detail/unfused_typed_class.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

namespace detail {

///	Converts a type into a storable type by removing const qualifiers and references.
template<typename T>
struct storable : public boost::remove_const<typename boost::remove_reference<T>::type > {};

}

template<typename Signature>
class storage : public boost::signal_network::signet::filter<Signature>
{
public:
    typedef boost::signal_network::signet::filter<Signature> base_type;

    typedef boost::function_types::parameter_types<Signature> ParTypes;
    typedef typename boost::mpl::transform<
        typename boost::mpl::transform<ParTypes, boost::remove_reference<boost::mpl::_> >::type,
        boost::remove_const<boost::mpl::_> >::type StorableTypes;
    typedef typename boost::fusion::result_of::as_vector<StorableTypes >::type VecStorable;

    typedef boost::fusion::unfused_typed_class<storage<Signature>,
        typename boost::function_types::parameter_types<Signature> > unfused;

    storage(const VecStorable &vec_par) : stored(vec_par), fused_out(filter<Signature>::out) {}
    storage() : fused_out(filter<Signature>::out) {}

    template<class Seq>
    struct result
    { 
        typedef void type;
    };

    void operator()()
    {
        fused_out(stored);
    }
    template <class Seq>
    void operator()(const Seq &vec_par)
    {
        stored = vec_par;
    }
    const VecStorable &value()
    {
        return stored;
    }
    template<int N>
    typename boost::fusion::result_of::at_c<VecStorable, N>::type value_()
    {
        return boost::fusion::at_c<N>(stored);
    }
    template<int N>
    typename boost::mpl::at_c<ParTypes, N>::type value_exact()
    {
        return boost::fusion::at_c<N>(stored);
    }
    template<int N>
   	slot_selector_t<storage<Signature>,// typename boost::call_traits<
        typename boost::mpl::at_c<ParTypes, N>::type ()
        >//::param_type ()>
    slot()
	{
        return boost::signal_network::slot_selector<//typename boost::call_traits<
            typename boost::mpl::at_c<ParTypes, N>::type ()>
        (*this, &storage<Signature>::template value_exact<N>);
    }

protected:
    VecStorable stored;
    boost::fusion::fused<typename base_type::signal_type const &> fused_out;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE


#endif // SIGNAL_NETWORK_GENERATOR_HPP