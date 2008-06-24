// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_NETWORK_HPP
#define BOOST_DATAFLOW_MANAGED_NETWORK_HPP

#include <vector>
#include <set>

#include <boost/dataflow/managed/component.hpp>

namespace boost { namespace dataflow { namespace managed {


namespace detail {
    struct component_ptr_cmp
    {
        bool operator()(const component * const lhs, const component * const rhs) const
        {
            return lhs->topological_sort_index() < rhs->topological_sort_index();
        }
    };
}

class network
{
public:
    void notify_change(component &changed)
    {
        m_changed.insert(&changed);
    }
    typedef std::set<component *, detail::component_ptr_cmp> changed_type;
    const changed_type &changed_components()
    {
        return m_changed;
    }
    typedef std::vector<component *> topological_sort_type;
    void topological_sort(const topological_sort_type &topological_sort)
    {
        m_changed.clear();
        for(topological_sort_type::const_iterator it=topological_sort.begin(); it!=topological_sort.end(); it++)
            (*it)->topological_sort_index(it - topological_sort.begin());
        for(topological_sort_type::const_iterator it=topological_sort.begin(); it!=topological_sort.end(); it++)
            m_changed.insert(*it);
    }
    void update()
    {
        while(!m_changed.empty())
        {
            component *next = *m_changed.begin();
            m_changed.erase(m_changed.begin());
            next->invoke();
        }
    }
private:
    changed_type m_changed;
};

void notify_change(component &c)
{
    c.network_context().notify_change(c);
}

} } }

#endif // BOOST_DATAFLOW_MANAGED_PORT_HPP