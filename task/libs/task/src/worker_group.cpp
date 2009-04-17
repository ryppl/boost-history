
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/task/detail/worker_group.hpp"

#include <boost/foreach.hpp>
#include <boost/utility.hpp>

namespace boost { namespace task
{
namespace detail
{
worker_group::worker_group()
:
cont_(),
id_idx_( cont_.get< id_idx_tag >() ),
rnd_idx_( cont_.get< rnd_idx_tag >() )
{}

const worker
worker_group::operator[]( std::size_t pos) const
{ return rnd_idx_[pos]; }

std::size_t
worker_group::size() const
{ return cont_.size(); }

const worker_group::iterator
worker_group::begin()
{ return id_idx_.begin(); }

const worker_group::const_iterator
worker_group::begin() const
{ return id_idx_.begin(); }

const worker_group::iterator
worker_group::end()
{ return id_idx_.end(); }

const worker_group::const_iterator
worker_group::end() const
{ return id_idx_.end(); }

const worker_group::const_iterator
worker_group::find( thread::id const& id) const
{ return id_idx_.find( id); }

void
worker_group::join_all()
{
	BOOST_FOREACH( worker w, cont_)
	{ w.join(); }
}

void
worker_group::interrupt_all()
{
	BOOST_FOREACH( worker w, cont_)
	{ w.interrupt(); }
}

void
worker_group::insert( worker const& w)
{ cont_.insert( w); }

void
worker_group::signal_shutdown_all()
{
	BOOST_FOREACH( worker w, cont_)
	{ w.signal_shutdown(); }
}

void
worker_group::signal_shutdown_now_all()
{
	BOOST_FOREACH( worker w, cont_)
	{ w.signal_shutdown_now(); }
}
} } }

