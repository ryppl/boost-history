//  (C) Copyright David Abrahams 2000. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

#ifndef BOOST_VECTOR_SET_DWA052900_HPP_
# define BOOST_VECTOR_SET_DWA052900_HPP_

#include <boost/associative_vector.hpp>

namespace boost {

template <typename Key, typename Compare = std::less<Key>,
        typename Alloc = std::allocator<Key>,
        typename Container = boost::associative_vector<Key, Key,
        boost::identity<Key>, Compare, Alloc> >
class vector_set
{
public:
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    typedef Container Rep_type;
    typedef typename Rep_type::iterator Rep_iterator;
public:
    typedef typename Rep_type::const_pointer pointer;
    typedef typename Rep_type::const_pointer const_pointer;
    typedef typename Rep_type::const_reference reference;
    typedef typename Rep_type::const_reference const_reference;
    typedef typename Rep_type::const_iterator const_iterator;
    typedef const_iterator iterator;
    typedef typename Rep_type::const_reverse_iterator reverse_iterator;
    typedef typename Rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename Rep_type::size_type size_type;
    typedef typename Rep_type::difference_type difference_type;
    typedef typename Rep_type::allocator_type allocator_type;
private:
    Rep_type m_impl;  // associative_vector representing vector_set
    Rep_iterator const2iter (const_iterator rhs) {
        Rep_iterator lhs(m_impl.begin());
        std::advance(lhs, std::distance(begin(), rhs));
        return lhs;
        }
public:
                // allocation/deallocation
    vector_set() : m_impl(Compare(), allocator_type()) { }
    explicit vector_set(const Compare& comp,
            const allocator_type& a = allocator_type()) : m_impl(comp, a) { }
    template <typename InputIterator>
    vector_set(InputIterator first, InputIterator last)
            : m_impl(Compare(), allocator_type()) {
        m_impl.insert_unique(first, last);
    }
    template <typename InputIterator>
    vector_set(InputIterator first, InputIterator last, const Compare& comp)
            : m_impl(comp, allocator_type()) {
        m_impl.insert_unique(first, last);
    }
    template <typename InputIterator>
    vector_set(InputIterator first, InputIterator last, const Compare& comp,
            const allocator_type& a) : m_impl(comp, a) {
        m_impl.insert_unique(first, last);
    }
    vector_set(const vector_set<Key,Compare,Alloc>& x) : m_impl(x.m_impl) {}
    vector_set<Key,Compare,Alloc>& operator=(
            const vector_set<Key, Compare, Alloc>& x) { 
        m_impl = x.m_impl;
        return *this;
    }
                    // accessors:
    key_compare key_comp() const { return m_impl.key_comp(); }
    value_compare value_comp() const { return m_impl.key_comp(); }
    allocator_type get_allocator() const { return m_impl.get_allocator(); }
    iterator begin() const { return m_impl.begin(); }
    iterator end() const { return m_impl.end(); }
    reverse_iterator rbegin() const { return m_impl.rbegin(); } 
    reverse_iterator rend() const { return m_impl.rend(); }
    bool empty() const { return m_impl.empty(); }
    size_type size() const { return m_impl.size(); }
    size_type max_size() const { return m_impl.max_size(); }
    void swap(vector_set<Key,Compare,Alloc,Container>& x) {
        m_impl.swap(x.m_impl);
    }
                    // DWA enhancements:
    const value_type& front() const { return *begin(); }
    const value_type& back() const {
        const_iterator it(end());
        return *--it;
    }
    const value_type& operator[](size_type n) const {
        const_iterator it(begin());
        std::advance(it, n);
        return *it;
    }
    void reserve(size_type n) { m_impl.reserve(n); }
    size_type capacity() const { return m_impl.capacity(); }
                    // insert/erase
    std::pair<iterator,bool> insert(const value_type& x) { 
        std::pair<Rep_iterator, bool> p = m_impl.insert_unique(x); 
        return std::pair<iterator, bool>(p.first, p.second);
    }
    iterator insert(iterator position, const value_type& x) {
        return m_impl.insert_unique(const2iter(position), x);
    }
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        m_impl.insert_unique(first, last);
    }
    void erase(iterator position) { 
        return m_impl.erase(const2iter(position));
    }
    size_type erase(const key_type& x) { return m_impl.erase(x); }
    void erase(iterator first, iterator last) { 
        m_impl.erase(const2iter(first), const2iter(last));
    }
    void clear() { m_impl.clear(); }
                    // vector_set operations:
    iterator find(const key_type& x) const { return m_impl.find(x); }
    size_type count(const key_type& x) const { 
        return m_impl.find(x) == m_impl.end() ? 0 : 1; 
    }
    iterator lower_bound(const key_type& x) const {
        return m_impl.lower_bound(x);
    }
    iterator upper_bound(const key_type& x) const {
        return m_impl.upper_bound(x); 
    }
    std::pair<iterator,iterator> equal_range(const key_type& x) const {
        return m_impl.equal_range(x);
    }
};
template <typename Key, typename Compare, typename Alloc, typename Container>
void swap(const vector_set<Key, Compare, Alloc, Container>& x,
        const vector_set<Key, Compare, Alloc, Container>& y) {
    x.swap(y);
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator==(const vector_set<Key, Compare, Alloc, Container>& x, 
        const vector_set<Key, Compare, Alloc, Container>& y) {
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator!=(const vector_set<Key, Compare, Alloc, Container>& x, 
        const vector_set<Key, Compare, Alloc, Container>& y) {
return !(x == y);
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator<(const vector_set<Key, Compare, Alloc, Container>& x, 
        const vector_set<Key, Compare, Alloc, Container>& y) {
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator>(const vector_set<Key, Compare, Alloc, Container>& x, 
        const vector_set<Key, Compare, Alloc, Container>& y) {
    return y < x;
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator<=(const vector_set<Key, Compare, Alloc, Container>& x, 
        const vector_set<Key, Compare, Alloc, Container>& y) {
    return !(x > y);
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator>=(const vector_set<Key, Compare, Alloc, Container>& x, 
        const vector_set<Key, Compare, Alloc, Container>& y) {
    return !(x < y);
}

template <typename Key, typename Compare = std::less<Key>,
        typename Alloc = std::allocator<Key>,
        typename Container = boost::associative_vector<Key, Key,
        std::identity<Key>, Compare, Alloc> >
class vector_multiset
{
public:
                    // typedefs:
    typedef Key     key_type;
    typedef Key     value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    typedef Container Rep_type;
    typedef typename Rep_type::iterator Rep_iterator;
public:
    typedef typename Rep_type::const_pointer pointer;
    typedef typename Rep_type::const_pointer const_pointer;
    typedef typename Rep_type::const_reference reference;
    typedef typename Rep_type::const_reference const_reference;
    typedef typename Rep_type::const_iterator const_iterator;
    typedef const_iterator iterator;
    typedef typename Rep_type::const_reverse_iterator reverse_iterator;
    typedef typename Rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename Rep_type::size_type size_type;
    typedef typename Rep_type::difference_type difference_type;
    typedef typename Rep_type::allocator_type allocator_type;
private:
    Rep_type m_impl;  // associative_vector representing vector_multiset
    Rep_iterator const2iter (const_iterator rhs) {
        Rep_iterator lhs(m_impl.begin());
        std::advance(lhs, std::distance(begin(), rhs));
        return lhs;
        }
public:
                // allocation/deallocation
    vector_multiset() : m_impl(Compare(), allocator_type()) {}
    explicit vector_multiset(const Compare& comp,
            const allocator_type& a = allocator_type()) : m_impl(comp, a) {}
    template <typename InputIterator>
    vector_multiset(InputIterator first, InputIterator last)
            : m_impl(Compare(), allocator_type()) {
        m_impl.insert_equal(first, last);
    }
    template <typename InputIterator>
    vector_multiset(InputIterator first, InputIterator last,
            const Compare& comp) : m_impl(comp, allocator_type()) {
        m_impl.insert_equal(first, last);
    }
    template <typename InputIterator>
    vector_multiset(InputIterator first, InputIterator last,
            const Compare& comp, const allocator_type& a) : m_impl(comp, a) {
        m_impl.insert_equal(first, last);
    }
    vector_multiset(const vector_multiset<Key,Compare,Alloc>& x)
            : m_impl(x.m_impl) {}
    vector_multiset<Key,Compare,Alloc>&
    operator=(const vector_multiset<Key,Compare,Alloc>& x) {
        m_impl = x.m_impl; 
        return *this;
    }
                    // accessors:
    key_compare key_comp() const { return m_impl.key_comp(); }
    value_compare value_comp() const { return m_impl.key_comp(); }
    allocator_type get_allocator() const { return m_impl.get_allocator(); }
    iterator begin() const { return m_impl.begin(); }
    iterator end() const { return m_impl.end(); }
    reverse_iterator rbegin() const { return m_impl.rbegin(); } 
    reverse_iterator rend() const { return m_impl.rend(); }
    bool empty() const { return m_impl.empty(); }
    size_type size() const { return m_impl.size(); }
    size_type max_size() const { return m_impl.max_size(); }
    size_type capacity() const { return m_impl.capacity(); }
    void swap(vector_multiset<Key,Compare,Alloc,Container>& x) {
        m_impl.swap(x.m_impl);
    }
                    // DWA enhancements:
    const value_type& front() const { return *begin(); }
    const value_type& back() const {
        const_iterator it(end());
        return *--it;
    }
    const value_type& operator[](size_type n) const {
        const_iterator it(begin());
        std::advance(it, n);
        return *it;
    }
    void reserve(size_type n) { m_impl.reserve(n); }
                    // insert/erase
    iterator insert(const value_type& x) { return m_impl.insert_equal(x); }
    iterator insert(iterator position, const value_type& x) {
        return m_impl.insert_equal(const2iter(position), x);
    }
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        m_impl.insert_equal(first, last);
    }
    void erase(iterator position) { 
        return m_impl.erase(const2iter(position));
    }
    size_type erase(const key_type& x) { return m_impl.erase(x); }
    void erase(iterator first, iterator last) { 
        m_impl.erase(const2iter(first), const2iter(last));
    }
    void clear() { m_impl.clear(); }
                    // vector_multiset operations:
    iterator find(const key_type& x) const { return m_impl.find(x); }
    size_type count(const key_type& x) const { return m_impl.count(x); }
    iterator lower_bound(const key_type& x) const {
        return m_impl.lower_bound(x);
    }
    iterator upper_bound(const key_type& x) const {
        return m_impl.upper_bound(x); 
    }
    std::pair<iterator,iterator> equal_range(const key_type& x) const {
        return m_impl.equal_range(x);
    }
};
template <typename Key, typename Compare, typename Alloc, typename Container>
void swap(const vector_multiset<Key, Compare, Alloc, Container>& x,
        const vector_multiset<Key, Compare, Alloc, Container>& y) {
    x.swap(y);
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator==(const vector_multiset<Key, Compare, Alloc, Container>& x,
           const vector_multiset<Key, Compare, Alloc, Container>& y) {
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator!=(const vector_multiset<Key, Compare, Alloc, Container>& x,
           const vector_multiset<Key, Compare, Alloc, Container>& y) {
    return !(x == y);
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator<(const vector_multiset<Key, Compare, Alloc, Container>& x,
           const vector_multiset<Key, Compare, Alloc, Container>& y) {
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator>(const vector_multiset<Key, Compare, Alloc, Container>& x,
           const vector_multiset<Key, Compare, Alloc, Container>& y) {
    return y < x;
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator<=(const vector_multiset<Key, Compare, Alloc, Container>& x,
           const vector_multiset<Key, Compare, Alloc, Container>& y) {
    return !(x > y);
}
template <typename Key, typename Compare, typename Alloc, typename Container>
inline bool operator>=(const vector_multiset<Key, Compare, Alloc, Container>& x,
           const vector_multiset<Key, Compare, Alloc, Container>& y) {
    return !(x < y);
}

} // namespace boost

#endif

