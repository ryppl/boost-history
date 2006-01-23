#ifndef BOOST_PROFILER_DETAIL_ITERATOR_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_ITERATOR_HPP_INCLUDED

#include <boost/profiler/detail/report.hpp>
#include <boost/profiler/detail/entry.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/assert.hpp>
#include <vector>

namespace boost { namespace profiler { namespace detail
{

    class context;
    
    class iterator: 
        public boost::iterator_facade<iterator, report, 
                                      boost::forward_traversal_tag, report>
    {
        friend class boost::iterator_core_access;
        friend class context;
    public:
        iterator();
    private:
        iterator(const std::vector<entry> &entries, std::vector<entry>::size_type index);
        void increment();
        bool equal(const iterator &rhs) const;
        report dereference() const;
        void skip();
        const std::vector<entry> *m_entries;
        std::vector<entry>::size_type m_index;
    };
    
    inline iterator::iterator():
        m_entries(0)
    {
    }

    inline iterator::iterator(const std::vector<entry> &entries, 
             std::vector<entry>::size_type index):
       m_entries(&entries), m_index(index)
    {
        skip();
    }

    inline void iterator::increment()
    {
        BOOST_ASSERT(m_entries && m_index < m_entries->size());
        ++m_index;
        skip();
    }

    inline bool iterator::equal(const iterator &rhs) const
    {
        BOOST_ASSERT(m_entries && m_entries == rhs.m_entries);
        return m_index == rhs.m_index;
    }

    inline report iterator::dereference() const
    {
        BOOST_ASSERT(m_entries && m_index < m_entries->size());
        const entry &e = (*m_entries)[m_index];
        return report(e);
    }

    inline void iterator::skip()
    {
        while (m_index < m_entries->size() && !(*m_entries)[m_index].p)
            ++m_index;
    }

} } }

namespace boost { namespace profiler
{
    using detail::iterator;
} }

#endif
