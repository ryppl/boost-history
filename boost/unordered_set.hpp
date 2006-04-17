
// JTC1/SC22/WG21 N1456 Hash table implementation
// http://std.dkuug.dk/jtc1/sc22/wg21/docs/papers/2003/n1456.html

// boost/unordered_set.hpp

// Copyright � 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright � 2005-2006 Daniel James.

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNORDERED_SET_HPP_INCLUDED
#define BOOST_UNORDERED_SET_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#include <functional>
#include <memory>

#include <boost/unordered/detail/hash_table.hpp>
#include <boost/functional/hash.hpp>

namespace boost
{
    //! An unordered associative container that stores unique values.
    /*! For full details see section 6.3.4.3 of the Technical report.
     *  http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1836.pdf
     */

    template <class Value,
        class Hash = hash<Value>,
        class Pred = std::equal_to<Value>,
        class Alloc = std::allocator<Value> >
    class unordered_set
    {
        // Named for the benefit of Doxygen.
        typedef boost::unordered_detail::hash_types<
            Value, Value, Hash,
            Pred, Alloc, false
        > implementation_defined;

        typename implementation_defined::hash_table base;

    public:
        // types
        typedef Value key_type;
        typedef Value value_type;
        typedef Hash hasher;
        typedef Pred key_equal;

        typedef Alloc allocator_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;

        typedef typename implementation_defined::size_type size_type;
        typedef typename implementation_defined::difference_type difference_type;

        typedef typename implementation_defined::const_iterator iterator;
        typedef typename implementation_defined::const_iterator const_iterator;
        typedef typename implementation_defined::const_local_iterator local_iterator;
        typedef typename implementation_defined::const_local_iterator const_local_iterator;

        // construct/destroy/copy

        explicit unordered_set(
                size_type n = boost::unordered_detail::default_initial_bucket_count,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
            : base(n, hf, eql, a)
        {
        }

        template <class InputIterator>
        unordered_set(InputIterator f, InputIterator l)
            : base(f, l, boost::unordered_detail::default_initial_bucket_count,
                hasher(), key_equal(), allocator_type())
        {
        }

        template <class InputIterator>
        unordered_set(InputIterator f, InputIterator l, size_type n,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
            : base(f, l, n, hf, eql, a)
        {
        }

    private:

        typename implementation_defined::iterator_base const&
            get(const_iterator const& it)
        {
            return boost::unordered_detail::iterator_access::get(it);
        }

    public:

        allocator_type get_allocator() const
        {
            return base.get_allocator();
        }

        // size and capacity

        bool empty() const
        {
            return base.empty();
        }

        size_type size() const
        {
            return base.size();
        }

        size_type max_size() const
        {
            return base.max_size();
        }

        // iterators

        iterator begin()
        {
            return iterator(base.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(base.begin());
        }

        iterator end()
        {
            return iterator(base.end());
        }

        const_iterator end() const
        {
            return const_iterator(base.end());
        }

        // modifiers

        std::pair<iterator, bool> insert(const value_type& obj)
        {
            return boost::unordered_detail::pair_cast<iterator, bool>(
                    base.insert_unique(obj));
        }

        const_iterator insert(const_iterator hint, const value_type& obj)
        {
            return const_iterator(base.insert_unique(get(hint), obj));
        }

        template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
        {
            base.insert(first, last);
        }

        const_iterator erase(const_iterator position)
        {
            return const_iterator(base.erase(get(position)));
        }

        size_type erase(const key_type& k)
        {
            return base.erase(k);
        }

        const_iterator erase(const_iterator first, const_iterator last)
        {
            return const_iterator(base.erase(get(first), get(last)));
        }

        void clear()
        {
            base.clear();
        }

        void swap(unordered_set& other)
        {
            base.swap(other.base);
        }

        // observers

        hasher hash_function() const
        {
            return base.hash_function();
        }

        key_equal key_eq() const
        {
            return base.key_eq();
        }

        // lookup

        const_iterator find(const key_type& k) const
        {
            return const_iterator(base.find(k));
        }

        size_type count(const key_type& k) const
        {
            return base.count(k);
        }

        std::pair<const_iterator, const_iterator>
            equal_range(const key_type& k) const
        {
            return boost::unordered_detail::pair_cast<const_iterator, const_iterator>(
                    base.equal_range(k));
        }

        // bucket interface

        size_type bucket_count() const
        {
            return base.bucket_count();
        }

        size_type max_bucket_count() const
        {
            return base.max_bucket_count();
        }

        size_type bucket_size(size_type n) const
        {
            return base.bucket_size(n);
        }

        size_type bucket(const key_type& k) const
        {
            return base.bucket(k);
        }

        local_iterator begin(size_type n)
        {
            return local_iterator(base.begin(n));
        }

        const_local_iterator begin(size_type n) const
        {
            return const_local_iterator(base.begin(n));
        }

        local_iterator end(size_type n)
        {
            return local_iterator(base.end(n));
        }

        const_local_iterator end(size_type n) const
        {
            return const_local_iterator(base.end(n));
        }


        // hash policy

        float load_factor() const
        {
            return base.load_factor();
        }

        float max_load_factor() const
        {
            return base.max_load_factor();
        }

        void max_load_factor(float m)
        {
            base.max_load_factor(m);
        }

        void rehash(size_type n)
        {
            base.rehash(n);
        }
    }; // class template unordered_set

    template <class T, class H, class P, class A>
    void swap(unordered_set<T, H, P, A> &m1,
            unordered_set<T, H, P, A> &m2)
    {
        m1.swap(m2);
    }

    //! An unordered associative container that stores equivalent values.
    /*! For full details see section 6.3.4.5 of the Technical report.
     *  http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1836.pdf
     */

    template <class Value,
        class Hash = hash<Value>,
        class Pred = std::equal_to<Value>,
        class Alloc = std::allocator<Value> >
    class unordered_multiset
    {
        // Named for the benefit of Doxygen.
        typedef boost::unordered_detail::hash_types<
            Value, Value, Hash,
            Pred, Alloc, true
        > implementation_defined;

        typename implementation_defined::hash_table base;

    public:
        //types
        typedef Value key_type;
        typedef Value value_type;
        typedef Hash hasher;
        typedef Pred key_equal;

        typedef Alloc allocator_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;

        typedef typename implementation_defined::size_type size_type;
        typedef typename implementation_defined::difference_type difference_type;

        typedef typename implementation_defined::const_iterator iterator;
        typedef typename implementation_defined::const_iterator const_iterator;
        typedef typename implementation_defined::const_local_iterator local_iterator;
        typedef typename implementation_defined::const_local_iterator const_local_iterator;

        // construct/destroy/copy

        explicit unordered_multiset(
                size_type n = boost::unordered_detail::default_initial_bucket_count,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
          : base(n, hf, eql, a)
        {
        }

        template <class InputIterator>
        unordered_multiset(InputIterator f, InputIterator l)
            : base(f, l, boost::unordered_detail::default_initial_bucket_count,
                hasher(), key_equal(), allocator_type())
        {
        }

        template <class InputIterator>
        unordered_multiset(InputIterator f, InputIterator l, size_type n,
                const hasher &hf = hasher(),
                const key_equal &eql = key_equal(),
                const allocator_type &a = allocator_type())
          : base(f, l, n, hf, eql, a)
        {
        }

    private:

        typename implementation_defined::iterator_base const&
            get(const_iterator const& it)
        {
            return boost::unordered_detail::iterator_access::get(it);
        }

    public:

        allocator_type get_allocator() const
        {
            return base.get_allocator();
        }

        // size and capacity

        bool empty() const
        {
            return base.empty();
        }

        size_type size() const
        {
            return base.size();
        }

        size_type max_size() const
        {
            return base.max_size();
        }

        // iterators

        iterator begin()
        {
            return iterator(base.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(base.begin());
        }

        iterator end()
        {
            return iterator(base.end());
        }

        const_iterator end() const
        {
            return const_iterator(base.end());
        }

        // modifiers

        iterator insert(const value_type& obj)
        {
            return iterator(base.insert_equivalent(obj));
        }

        const_iterator insert(const_iterator hint, const value_type& obj)
        {
            return const_iterator(base.insert_equivalent(get(hint), obj));
        }

        template <class InputIterator>
            void insert(InputIterator first, InputIterator last)
        {
            base.insert(first, last);
        }

        const_iterator erase(const_iterator position)
        {
            return const_iterator(base.erase(get(position)));
        }

        size_type erase(const key_type& k)
        {
            return base.erase(k);
        }

        const_iterator erase(const_iterator first, const_iterator last)
        {
            return const_iterator(base.erase(get(first), get(last)));
        }

        void clear()
        {
            base.clear();
        }

        void swap(unordered_multiset& other)
        {
            base.swap(other.base);
        }

        // observers

        hasher hash_function() const
        {
            return base.hash_function();
        }

        key_equal key_eq() const
        {
            return base.key_eq();
        }

        // lookup

        const_iterator find(const key_type& k) const
        {
            return const_iterator(base.find(k));
        }

        size_type count(const key_type& k) const
        {
            return base.count(k);
        }

        std::pair<const_iterator, const_iterator>
            equal_range(const key_type& k) const
        {
            return boost::unordered_detail::pair_cast<const_iterator, const_iterator>(
                    base.equal_range(k));
        }

        // bucket interface

        size_type bucket_count() const
        {
            return base.bucket_count();
        }

        size_type max_bucket_count() const
        {
            return base.max_bucket_count();
        }

        size_type bucket_size(size_type n) const
        {
            return base.bucket_size(n);
        }

        size_type bucket(const key_type& k) const
        {
            return base.bucket(k);
        }

        local_iterator begin(size_type n)
        {
            return local_iterator(base.begin(n));
        }

        const_local_iterator begin(size_type n) const
        {
            return const_local_iterator(base.begin(n));
        }

        local_iterator end(size_type n)
        {
            return local_iterator(base.end(n));
        }

        const_local_iterator end(size_type n) const
        {
            return const_local_iterator(base.end(n));
        }

        // hash policy

        float load_factor() const
        {
            return base.load_factor();
        }

        float max_load_factor() const
        {
            return base.max_load_factor();
        }

        void max_load_factor(float m)
        {
            base.max_load_factor(m);
        }

        void rehash(size_type n)
        {
            base.rehash(n);
        }
    }; // class template unordered_multiset

    template <class T, class H, class P, class A>
    void swap(unordered_multiset<T, H, P, A> &m1,
            unordered_multiset<T, H, P, A> &m2)
    {
        m1.swap(m2);
    }

} // namespace boost

#endif // BOOST_UNORDERED_SET_HPP_INCLUDED
