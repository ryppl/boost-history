// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_ASYNC_CALL_HPP
#define BOOST_RPC_ASYNC_CALL_HPP

#include <boost/rpc/call.hpp>
#include <boost/rpc/detail/archive_out_param.hpp>
#include <boost/future/future.hpp>

namespace boost {
namespace rpc {

template<typename ReturnType>
class async_returning_call : public returning_call<ReturnType>
{
public:
    virtual void assign_promises() = 0;
    const promise<ReturnType> &return_promise() {return return_prom;}
protected:
    promise<ReturnType> return_prom;
};

template<typename Id, typename Signature, typename ArchivePair = binary_archive,
    typename Enable=void, typename Enable2=void>
class async_call
#ifdef DOXYGEN_DOCS_ONLY
#include <boost/rpc/detail/async_call_template.hpp>
#endif // DOXYGEN_DOCS_ONLY
;

#define BOOST_ARITY_SEPARATE_VOID_RETURN
#define BOOST_ARITY_ITERATION_PARAMS \
    (3,(0,BOOST_RPC_MAX_ARGS,<boost/rpc/detail/async_call_template.hpp>))
#include <boost/detail/arity_iterate.hpp>
#undef BOOST_ARITY_ITERATION_PARAMS
#undef BOOST_ARITY_SEPARATE_VOID_RETURN


} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_CALL_HPP
