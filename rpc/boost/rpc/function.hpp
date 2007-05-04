#ifndef BOOST_RPC_FUNCTION_HPP
#define BOOST_RPC_FUNCTION_HPP

#include <boost/rpc/binary_archive.hpp>
#include <boost/rpc/call_options.hpp>
#include <sstream>

namespace boost {
namespace rpc {
namespace detail {

template<typename ArchivePair = binary_archive>
class function
{
public:
    virtual void call(const boost::rpc::call_options &options,
        typename ArchivePair::iarchive_type &params,
        typename ArchivePair::oarchive_type *results = 0) {};
};

} // namespace detail
} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_FUNCTION_HPP
