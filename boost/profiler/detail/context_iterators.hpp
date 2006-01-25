#ifndef BOOST_PROFILER_DETAIL_CONTEXT_ITERATORS_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_CONTEXT_ITERATORS_HPP_INCLUDED

#include <boost/profiler/detail/context.hpp>
#include <boost/profiler/detail/iterator.hpp>

namespace boost { namespace profiler { namespace detail
{

    inline iterator context::begin() const
    {
        return iterator(*this, 0);
    }

    inline iterator context::end() const
    {
        return iterator(*this, m_entries.size());
    }

} } }

#endif
