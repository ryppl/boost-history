// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_REMOTE_CALLER_HPP
#define BOOST_RPC_REMOTE_CALLER_HPP

#include <boost/detail/arity.hpp>

namespace boost {
namespace rpc {

template<typename Signature, typename ArchivePair, int Arity, typename Enable=void>
class marshaled_impl;

#define BOOST_ARITY_SEPARATE_VOID_RETURN
#define BOOST_ARITY_ITERATION_PARAMS \
    (3,(0,BOOST_RPC_MAX_ARGS,<boost/rpc/detail/marshaled_template.hpp>))
#include <boost/detail/arity_iterate.hpp>
#undef BOOST_ARITY_ITERATION_PARAMS
#undef BOOST_ARITY_SEPARATE_VOID_RETURN


} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_REMOTE_CALLER_HPP