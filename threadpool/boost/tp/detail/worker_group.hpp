//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_DETAIL_WORKER_GROUP_H
#define BOOST_TP_DETAIL_WORKER_GROUP_H

#include <cstddef>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/thread.hpp>

#include <boost/tp/detail/worker.hpp>

namespace boost { namespace tp {
namespace detail
{
class worker_group
{
private:
	struct id_idx_tag {};
	struct rnd_idx_tag {};

	typedef multi_index::multi_index_container<
		worker,
		multi_index::indexed_by<
			multi_index::ordered_unique<
				multi_index::tag< id_idx_tag >,
				multi_index::const_mem_fun<
					worker,
					const thread::id,
					& worker::get_id
				>
			>,
			multi_index::random_access< multi_index::tag< rnd_idx_tag > >
		>
	>				container;

	typedef container::index< id_idx_tag >::type		id_idx;
	typedef container::index< rnd_idx_tag >::type		rnd_idx;

	container		cont_;
	id_idx		&	id_idx_;
	rnd_idx		&	rnd_idx_;

public:
	typedef id_idx::iterator		iterator;
	typedef id_idx::const_iterator	const_iterator;

	worker_group();

	const worker operator[]( std::size_t pos) const;

	std::size_t size() const;

	const iterator begin();

	const const_iterator begin() const;

	const iterator end();

	const const_iterator end() const;

	const const_iterator find( thread::id const& id) const;

	void join_all();

	void interrupt_all();

	void insert( worker const& w);

	void signal_shutdown_all();

	void signal_shutdown_now_all();
};
} } }

#endif // BOOST_TP_DETAIL_WORKER_GROUP_H

