//  (C) Copyright David Abrahams 2000. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  (C) Copyright John Potter 2000. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this copyright
//  notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

#ifndef BOOST_ASSOCIATIVE_VECTOR_DWA052800_HPP_
# define BOOST_ASSOCIATIVE_VECTOR_DWA052800_HPP_

# include <vector>
# include <utility>
# include <algorithm>
# include <boost/operators.hpp>
# include <boost/compose.hpp>
# include <boost/identity.hpp>

namespace boost {

template <class Key, class Value, class KeyOfValue,
    class Compare = std::less<Key>, class Alloc = std::allocator<Value>,
    class Container = std::vector<Value, Alloc> >
class associative_vector
{
private:
    typedef Container Impl;
    typedef associative_vector<Key, Value, KeyOfValue, Compare, Alloc,
            Container> Self;
public:
    typedef typename Impl::iterator iterator;
    typedef typename Impl::const_iterator const_iterator;
    typedef typename Impl::reverse_iterator reverse_iterator;
    typedef typename Impl::const_reverse_iterator const_reverse_iterator;
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag Iterator_category;
    typedef Alloc allocator_type;
private :
    iterator const2iter (const_iterator rhs) {
        Self const& me(*this);
        iterator lhs(begin());
        std::advance(lhs, std::distance(me.begin(), rhs));
        return lhs;
        }
public :
    associative_vector() {}
    associative_vector(const Compare& comp)
            : m_key_compare(comp) {}
    associative_vector(const Compare& comp, const allocator_type& a)
            : m_key_compare(comp), m_impl(a) {}
    Self& operator=(Self rhs) { swap(rhs); return *this; } // strong guarantee
    const Compare& key_comp() const { return m_key_compare; }
    const allocator_type get_allocator() const {
            return m_impl.get_allocator(); }
    iterator begin() { return m_impl.begin(); }
    iterator end() { return m_impl.end(); }
    const_iterator begin() const { return m_impl.begin(); }
    const_iterator end() const { return m_impl.end(); }
    reverse_iterator rbegin() { return m_impl.rbegin(); }
    reverse_iterator rend() { return m_impl.rend(); }
    const_reverse_iterator rbegin() const { return m_impl.rbegin(); }
    const_reverse_iterator rend() const { return m_impl.rend(); }
    bool empty() const { return m_impl.empty(); }
    size_type size() const { return m_impl.size(); }
    size_type max_size() const { return m_impl.max_size(); }
    size_type capacity () const { return m_impl.capacity(); }
    void swap(Self& x) {
        std::swap(m_impl, x.m_impl);
        std::swap(m_key_compare, x.m_key_compare);
    }
                                // insert/erase
    std::pair<iterator,bool> insert_unique(const value_type& x) {
        iterator pos = lower_bound(KeyOfValue()(x));
        if (pos == end()
                || m_key_compare(KeyOfValue()(x), KeyOfValue()(*pos)))
            return std::make_pair(m_impl.insert(pos, x), true);
        else
            return std::make_pair(pos, false);
    }
    iterator insert_equal(const value_type& x) {
        return m_impl.insert(upper_bound(KeyOfValue()(x)), x);
    }
    iterator insert_unique(const_iterator position, const value_type& x) {
        iterator pos = const2iter(position);
        // Try to insert at the hint position, *(pos-1) < x < *pos
        if (pos == end()
                || m_key_compare(KeyOfValue()(x), KeyOfValue()(*pos)))
        {
            iterator pre(pos);
            if (pos == begin()
                    || m_key_compare(KeyOfValue()(*--pre), KeyOfValue()(x)))
                return m_impl.insert(pos, x);
        }
        // See if it is already there, ! (x < *pos) && ! (*pos < x)
        else if (! m_key_compare(KeyOfValue()(*pos), KeyOfValue()(x)))
            return pos;
        // Bad hint, let insert(value) handle it
        return insert_unique(x).first;
    }
    iterator insert_equal(const_iterator position, const value_type& x) {
        iterator pos = const2iter(position);
        iterator pre(pos);
        // Try to insert at the hint position, *(pos - 1) <= x <= *pos
        if ((pos == end()
                || ! m_key_compare(KeyOfValue()(*pos), KeyOfValue()(x)))
                && (pos == begin()
                || ! m_key_compare(KeyOfValue()(x), KeyOfValue()(*--pre))))
            return m_impl.insert(pos, x);
        else
            return insert_equal(x);
    }
    template<class Iterator>
    void insert_equal(Iterator first, Iterator last) {
        for ( ; first != last; ++first)
            insert_equal(*first);
    }
    template<class Iterator>
    void insert_unique(Iterator first, Iterator last) {
        for ( ; first != last; ++first)
            insert_unique(*first);
    }
    void erase(const_iterator position) {
        m_impl.erase(const2iter(position));
    }
    size_type erase(const key_type& x) {
        std::pair<iterator,iterator> p = equal_range(x);
        size_type n = std::distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }
    void erase(const_iterator first, const_iterator last) {
        m_impl.erase(const2iter(first), const2iter(last));
    }
    void clear() { m_impl.clear(); }
                        // set operations
    iterator find(const key_type& x) {
        const Self& me = *this;
        return const2iter(me.find(x));
    }
    const_iterator find(const key_type& x) const {
        const_iterator finish = end();
        const_iterator pos = std::lower_bound(begin(), finish, x,
                compose_f_gx_hy(m_key_compare, KeyOfValue(),
                boost::identity<key_type>()));
        if (pos == finish || m_key_compare(x, KeyOfValue()(*pos)))
            return finish;
        else
            return pos;
    }
    size_type count(const key_type& x) const {
        return std::distance(lower_bound(x), upper_bound(x));
    }
    iterator lower_bound(const key_type& x) {
        return std::lower_bound(begin(), end(), x,
                compose_f_gx_hy(m_key_compare, KeyOfValue(),
                boost::identity<key_type>()));
    }
    const_iterator lower_bound(const key_type& x) const {
        return std::lower_bound(begin(), end(), x,
                compose_f_gx_hy(m_key_compare, KeyOfValue(),
                boost::identity<key_type>()));
    }
    iterator upper_bound(const key_type& x) {
        return std::upper_bound(begin(), end(), x,
                compose_f_gx_hy(m_key_compare, boost::identity<key_type>(),
                KeyOfValue()));
    }
    const_iterator upper_bound(const key_type& x) const {
        return std::upper_bound(begin(), end(), x,
                compose_f_gx_hy(m_key_compare, boost::identity<key_type>(),
                KeyOfValue()));
    }
    
    std::pair<iterator,iterator> equal_range(const key_type& x) {
        return std::pair<iterator, iterator>(lower_bound(x), upper_bound(x));
    }
    std::pair<const_iterator, const_iterator> equal_range(
            const key_type& x) const {
        return std::pair<const_iterator,const_iterator>(
                lower_bound(x), upper_bound(x));
    }
private: // data members
    Compare m_key_compare;
    Impl m_impl;
};

// global swap, though possibly of limited utility pending resolution of
// standard issues w.r.t. "partial specialization of template functions".
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
void swap(
        associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x,
        associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    x.swap(y);
}
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
inline bool operator==(
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x, 
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    return x.size() == y.size()
            && std::equal(x.begin(), x.end(), y.begin());
}
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
inline bool operator!=(
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x, 
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    return !(x == y);
}
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
inline bool operator<(
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x, 
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    return std::lexicographical_compare(x.begin(), x.end(),
            y.begin(), y.end());
}
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
inline bool operator>(
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x, 
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    return y < x;
}
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
inline bool operator<=(
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x, 
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    return !(x > y);
}
template <class Key, class Value, class KeyOfValue, class Compare,
        class Alloc, class Container>
inline bool operator>=(
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& x, 
        const associative_vector<Key, Value, KeyOfValue, Compare,
        Alloc, Container>& y) {
    return !(x < y);
}
    
} // namespace boost

#endif

