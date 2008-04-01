#ifndef BOOST_RPC_RPCED_HPP
#define BOOST_RPC_RPCED_HPP

#include <boost/rpc/config.hpp>
#include <boost/detail/arity.hpp>
#include <boost/function.hpp>
#include <boost/rpc/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/rpc/detail/archive_out_param.hpp>
#include <boost/detail/storable_arity.hpp>

namespace boost {
namespace rpc {
namespace detail {

template<typename Signature, typename ArchivePair, int Arity, typename Enable=void>
class marshaled_impl;

#define BOOST_ARITY_SEPARATE_VOID_RETURN
#define BOOST_ARITY_ITERATION_PARAMS \
    (3,(0,BOOST_RPC_MAX_ARGS,<boost/rpc/detail/marshaled_template.hpp>))
#include <boost/detail/arity_iterate.hpp>
#undef BOOST_ARITY_ITERATION_PARAMS
#undef BOOST_ARITY_SEPARATE_VOID_RETURN
} // namespace detail

template<typename Signature, typename ArchivePair = binary_archive>
class marshaled : public detail::marshaled_impl<Signature, ArchivePair, boost::function_traits<Signature>::arity>
{
public:
    marshaled(const boost::function<Signature> &f)
        : detail::marshaled_impl<Signature, ArchivePair, boost::function_traits<Signature>::arity>(f)
    {}
};

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_RPCED_HPP
