//
// detail/impl/kqueue_reactor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2005 Stefan Arentz (stefan at soze dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_IMPL_KQUEUE_REACTOR_HPP
#define BOOST_ASIO_DETAIL_IMPL_KQUEUE_REACTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#if defined(BOOST_ASIO_HAS_KQUEUE)

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace detail {

template <typename Time_Traits>
void kqueue_reactor::add_timer_queue(timer_queue<Time_Traits>& queue)
{
  do_add_timer_queue(queue);
}

// Remove a timer queue from the reactor.
template <typename Time_Traits>
void kqueue_reactor::remove_timer_queue(timer_queue<Time_Traits>& queue)
{
  do_remove_timer_queue(queue);
}

template <typename Time_Traits>
void kqueue_reactor::schedule_timer(timer_queue<Time_Traits>& queue,
    const typename Time_Traits::time_type& time, timer_op* op, void* token)
{
  boost::asio::detail::mutex::scoped_lock lock(mutex_);
  if (!shutdown_)
  {
    bool earliest = queue.enqueue_timer(time, op, token);
    io_service_.work_started();
    if (earliest)
      interrupter_.interrupt();
  }
}

template <typename Time_Traits>
std::size_t kqueue_reactor::cancel_timer(
    timer_queue<Time_Traits>& queue, void* token)
{
  boost::asio::detail::mutex::scoped_lock lock(mutex_);
  op_queue<operation> ops;
  std::size_t n = queue.cancel_timer(token, ops);
  lock.unlock();
  io_service_.post_deferred_completions(ops);
  return n;
}

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // defined(BOOST_ASIO_HAS_KQUEUE)

#endif // BOOST_ASIO_DETAIL_IMPL_KQUEUE_REACTOR_HPP
