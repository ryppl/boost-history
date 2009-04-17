
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TASK_DETAIL_WSQ_H
#define BOOST_TASK_DETAIL_WSQ_H

#include <boost/shared_array.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/utility.hpp>

#include <boost/task/detail/callable.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace task
{
namespace detail
{
class wsq : private noncopyable
{
private:
	const int					initial_size_;
	shared_array< callable >	array_;
	int							capacity_;
	int							mask_;
	volatile uint32_t			head_idx_;
	volatile uint32_t			tail_idx_;
	recursive_mutex				mtx_;

public:
	wsq();

	bool empty() const;

	std::size_t size() const;

	void put( callable const&);

	bool try_take( callable &);

	bool try_steal( callable &);
};
}}}

#include <boost/config/abi_suffix.hpp>

#endif // BOOST_TASK_DETAIL_WSQ_H

