#ifndef BOOST_RPC_SERIALIZE
#define BOOST_RPC_SERIALIZE

#include <boost/rpc/config.hpp>
#include <boost/detail/arity.hpp>
#include <boost/call_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <sstream>

namespace boost {
namespace rpc {
namespace detail {

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0,BOOST_RPC_MAX_ARGS,<boost/rpc/detail/serialize_template.hpp>))
#include <boost/detail/arity_iterate.hpp>
#undef BOOST_PP_ITERATION_PARAMS_1

} // namespace detail
} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_SERIALIZE
