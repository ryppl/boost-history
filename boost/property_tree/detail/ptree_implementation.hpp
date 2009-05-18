// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_PTREE_IMPLEMENTATION_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_PTREE_IMPLEMENTATION_HPP_INCLUDED

namespace boost { namespace property_tree
{

    // Big four

    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A>::basic_ptree(allocator_type)
    {
        // FIXME: use allocator
    }

    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A>::basic_ptree(const data_type &data,
                                         allocator_type)
        : m_data(rhs)
    {
        // FIXME: use allocator
    }

    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A>::basic_ptree(const basic_ptree<K, D, C, A> &rhs)
    {
        m_data = rhs.m_data;
        insert(end(), rhs.begin(), rhs.end());
    }

    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::operator =(const basic_ptree<K, D, C, A> &rhs)
    {
        if (&rhs != this)
        {
            clear();
            data() = rhs.data();
            insert(end(), rhs.begin(), rhs.end());
        }
        return *this;
    }

    template<class K, class D, class C, class A>
    void basic_ptree<K, D, C, A>::swap(basic_ptree<K, D, C, A> &rhs)
    {
        m_data.swap(rhs.m_data);
        m_children.swap(rhs.m_children);
    }

    // Container view

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::size_type
        basic_ptree<K, D, C, A>::size() const
    {
        return m_children.size();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::size_type
        basic_ptree<K, D, C, A>::max_size() const
    {
        return m_children.max_size();
    }

    template<class K, class D, class C, class A>
    bool basic_ptree<K, D, C, A>::empty() const
    {
        return m_children.empty();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::begin()
    {
        return m_children.begin();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_iterator
        basic_ptree<K, D, C, A>::begin() const
    {
        return m_children.begin();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::end()
    {
        return m_children.end();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_iterator
        basic_ptree<K, D, C, A>::end() const
    {
        return m_children.end();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::reverse_iterator
        basic_ptree<K, D, C, A>::rbegin()
    {
        return m_children.rbegin();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_reverse_iterator
        basic_ptree<K, D, C, A>::rbegin() const
    {
        return m_children.rbegin();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::reverse_iterator
        basic_ptree<K, D, C, A>::rend()
    {
        return m_children.rend();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_reverse_iterator
        basic_ptree<K, D, C, A>::rend() const
    {
        return m_children.rend();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::value_type &
        basic_ptree<K, D, C, A>::front()
    {
        return m_children.front();
    }

    template<class K, class D, class C, class A>
    const typename basic_ptree<K, D, C, A>::value_type &
        basic_ptree<K, D, C, A>::front() const
    {
        return m_children.front();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::value_type &
        basic_ptree<K, D, C, A>::back()
    {
        return m_children.back();
    }

    template<class K, class D, class C, class A>
    const typename basic_ptree<K, D, C, A>::value_type &
        basic_ptree<K, D, C, A>::back() const
    {
        return m_children.back();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
    basic_ptree<K, D, C, A>::insert(iterator where, const value_type &value)
    {
        return m_children.insert(where, value).first;
    }

    template<class K, class D, class C, class A>
    template<class It>
    void basic_ptree<K, D, C, A>::insert(iterator where, It first, It last)
    {
        m_children.insert(where, first, last);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::erase(iterator where)
    {
        return m_children.erase(where);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::erase(iterator first, iterator last)
    {
        return m_children.erase(first, last);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::push_front(const value_type &value)
    {
        return m_children.push_front(value).first;
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::push_back(const value_type &value)
    {
        return m_children.push_back(value).first;
    }

    template<class K, class D, class C, class A>
    void basic_ptree<K, D, C, A>::pop_front()
    {
        m_children.pop_front();
    }

    template<class K, class D, class C, class A>
    void basic_ptree<K, D, C, A>::pop_back()
    {
        m_children.pop_back();
    }

    template<class K, class D, class C, class A>
    void basic_ptree<K, D, C, A>::reverse()
    {
        m_children.reverse();
    }

    template<class K, class D, class C, class A>
    void basic_ptree<K, D, C, A>::sort()
    {
        m_children.sort();
    }

    template<class K, class D, class C, class A>
    template<class Compare>
    void basic_ptree<K, D, C, A>::sort(Compare comp)
    {
        m_children.sort(comp);
    }

    // Equality

    template<class K, class D, class C, class A>
    bool basic_ptree<K, D, C, A>::operator ==(
                                  const basic_ptree<K, D, C, A> &rhs) const
    {
        // The size test is cheap, so add it as an optimization
        return size() == rhs.size() && data() == rhs.data() &&
            m_children == rhs.m_children;
    }

    template<class K, class D, class C, class A>
    bool basic_ptree<K, D, C, A>::operator !=(
                                  const basic_ptree<K, D, C, A> &rhs) const
    {
        return !(*this == rhs);
    }

    // Associative view

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::assoc_iterator not_found()
    {
        return assoc().end();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_assoc_iterator not_found() const
    {
        return assoc().end();
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::assoc_iterator
        basic_ptree<K, D, C, A>::find(const key_type &key)
    {
        return assoc().find(key);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_assoc_iterator
        basic_ptree<K, D, C, A>::find(const key_type &key) const
    {
        return assoc().find(key);
    }

    template<class K, class D, class C, class A>
    std::pair<
        typename basic_ptree<K, D, C, A>::assoc_iterator,
        typename basic_ptree<K, D, C, A>::assoc_iterator
    > basic_ptree<K, D, C, A>::equal_range(const key_type &key)
    {
        return assoc().equal_range(key);
    }

    template<class K, class D, class C, class A>
    std::pair<
        typename basic_ptree<K, D, C, A>::const_assoc_iterator,
        typename basic_ptree<K, D, C, A>::const_assoc_iterator
    > basic_ptree<K, D, C, A>::equal_range(const key_type &key) const
    {
        return assoc().equal_range(key);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::size_type
        basic_ptree<K, D, C, A>::count(const key_type &key) const
    {
        return assoc().count(key);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::size_type
        basic_ptree<K, D, C, A>::erase(const key_type &key)
    {
        return assoc().erase(key);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::iterator
        basic_ptree<K, D, C, A>::to_iterator(assoc_iterator ai)
    {
        return m_children.project<0>(ai);
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::const_iterator
        basic_ptree<K, D, C, A>::to_iterator(const_assoc_iterator ai) const
    {
        return m_children.project<0>(ai);
    }

        // Property tree view

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::data_type &
        basic_ptree<K, D, C, A>::data()
    {
        return m_data;
    }

    template<class K, class D, class C, class A>
    const typename basic_ptree<K, D, C, A>::data_type &
        basic_ptree<K, D, C, A>::data() const
    {
        return m_data;
    }

    template<class K, class D, class C, class A>
    void basic_ptree<K, D, C, A>::clear()
    {
        m_data = data_type();
        m_children.clear();
    }

    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::get_child(const path_type &path)
    {
        path_type p(path);
        self_type *n = walk_path(p);
        if (!n) {
            BOOST_PROPERTY_TREE_THROW(ptree_bad_path(path));
        }
        return *n;
    }

    // Get child ptree
    template<class K, class D, class C, class A>
    const basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::get_child(const path_type &path) const
    {
        return const_cast<self_type*>(this)->get_child(path);
    }

    // Get child ptree
    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::get_child(const path_type &path,
                                           self_type &default_value)
    {
        path_type p(path);
        self_type *n = walk_path(p);
        return n ? *n : default_value;
    }

    // Get child ptree
    template<class K, class D, class C, class A>
    const basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::get_child(const path_type &path,
                                           const self_type &default_value) const
    {
        return const_cast<self_type*>(this)->get_child(path,
            onst_cast<self_type&>(default_value));
    }


    // Get child ptree
    template<class K, class D, class C, class A>
    optional<basic_ptree<K, D, C, A> &>
        basic_ptree<K, D, C, A>::get_child_optional(const path_type &path)
    {
        path_type p(path);
        self_type *n = walk_path(p);
        if (!n) {
            return optional<self_type&>();
        }
        return *n;
    }

    // Get child ptree
    template<class K, class D, class C, class A>
    optional<const basic_ptree<K, D, C, A> &>
        basic_ptree<K, D, C, A>::get_child_optional(const path_type &path) const
    {
        path_type p(path);
        self_type *n = walk_path(p);
        if (!n) {
            return optional<const self_type&>();
        }
        return *n;
    }

    // Put child ptree
    template<class K, class D, class C, class A>
    basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::put_child(const path_type &path,
                                           const self_type &value)
    {
        path_type p(path);
        self_type &parent = force_path(p);
        // Got the parent. Now get the correct child.
        std::pair<key_type, std::size_t> fragment = p.reduce();
        std::pair<assoc_iterator, assoc_iterator> range =
            parent.equal_range(fragment.first);
        std::size_t n = fragment.second;
        // If the path specifies an index, walk the range.
        while(n > 0 && range.first != range.second) {
            --n;
            ++range.first;
        }
        // If there's still something of the index left, add default-constructed
        // elements.
        while(n > 0) {
            parent->push_back(self_type());
        }
        // We've finally reached the position for the new child
        parent->push_back(value);
    }

    // Get value from data of ptree
    template<class K, class D, class C, class A>
    template<class Type>
    Type basic_ptree<K, D, C, A>::get_value(const translator_type &x) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);   // Disallow pointer types, they are unsafe
        Type value;
        if (x.get_value(*this, value))
            return value;
        else
            BOOST_PROPERTY_TREE_THROW(ptree_bad_data(std::string("conversion of data into type \"") +
                                           typeid(Type).name() + "\" failed", data()));
    }

    // Get value from data of ptree
    template<class K, class D, class C, class A>
    template<class Type>
    Type basic_ptree<K, D, C, A>::get_value(const Type &default_value,
                                               const translator_type &x) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);   // Disallow pointer types, they are unsafe
        Type value;
        if (x.get_value(*this, value))
            return value;
        else
            return default_value;
    }

    // Get value from data of ptree
    template<class K, class D, class C, class A>
    template<class CharType>
    std::basic_string<CharType>
        basic_ptree<K, D, C, A>::get_value(const CharType *default_value,
                                              const translator_type &x) const
    {
        return get_value(std::basic_string<CharType>(default_value), x);
    }

    // Get value from data of ptree
    template<class K, class D, class C, class A>
    template<class Type>
    optional<Type>
        basic_ptree<K, D, C, A>::get_value_optional(const translator_type &x) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);   // Disallow pointer types, they are unsafe
        Type value;
        if (x.get_value(*this, value))
            return optional<Type>(value);
        else
            return optional<Type>();
    }

    // Get value from data of child ptree
    template<class K, class D, class C, class A>
    template<class Type>
    Type basic_ptree<K, D, C, A>::get(const path_type &path,
                                         const translator_type &x) const
    {
        return get_child(path).get_value<Type>(x);
    }

    // Get value from data of child ptree
    template<class K, class D, class C, class A>
    template<class Type>
    Type basic_ptree<K, D, C, A>::get(const path_type &path,
                                         const Type &default_value,
                                         const translator_type &x) const
    {
        if (optional<Type> result = get_optional<Type>(path, x))
            return *result;
        else
            return default_value;
    }

    // Get value from data of child ptree
    template<class K, class D, class C, class A>
    template<class CharType>
    std::basic_string<CharType>
        basic_ptree<K, D, C, A>::get(const path_type &path,
                                        const CharType *default_value,
                                        const translator_type &x) const
    {
        return get(path, std::basic_string<CharType>(default_value), x);
    }

    // Get value from data of child ptree
    template<class K, class D, class C, class A>
    template<class Type>
    optional<Type>
        basic_ptree<K, D, C, A>::get_optional(const path_type &path,
                                                 const translator_type &x) const
    {
        if (optional<const basic_ptree<K, D, C, A> &> child = get_child_optional(path))
            return child.get().get_value_optional<Type>(x);
        else
            return optional<Type>();
    }

    // Put value in data of ptree
    template<class K, class D, class C, class A>
    template<class Type>
    void basic_ptree<K, D, C, A>::put_value(const Type &value, const translator_type &x)
    {
        if (!x.put_value(*this, value))
            BOOST_PROPERTY_TREE_THROW(ptree_bad_data(std::string("conversion of type \"") + typeid(Type).name() +
                                                       "\" into data failed", boost::any()));
    }

    // Put value in data of child ptree
    template<class K, class D, class C, class A>
    template<class Type>
    basic_ptree<K, D, C, A> &
        basic_ptree<K, D, C, A>::put(const path_type &path,
                                        const Type &value,
                                        bool do_not_replace,
                                        const translator_type &x)
    {
        optional<self_type &> child;
        if (!do_not_replace && (child = get_child_optional(path)))
        {
            child.get().put_value(value, x);
            return *child;
        }
        else
        {
            self_type &child2 = put_child(path, empty_ptree<self_type>(), do_not_replace);
            child2.put_value(value, x);
            return child2;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Debugging

#ifdef BOOST_PROPERTY_TREE_DEBUG

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::size_type
        basic_ptree<K, D, C, A>::debug_get_instances_count()
    {
        empty_ptree<basic_ptree<K, D, C, A> >();    // Make sure empty ptree exists
        return debug_instances_count - 1;              // Do not count empty ptree
    }

    template<class K, class D, class C, class A>
    typename basic_ptree<K, D, C, A>::size_type
        basic_ptree<K, D, C, A>::debug_instances_count;

    template<class K, class D, class C, class A>
    boost::detail::lightweight_mutex
        basic_ptree<K, D, C, A>::debug_mutex;

#endif

    ///////////////////////////////////////////////////////////////////////////
    // Free functions

    template<class Ptree>
    inline const Ptree &empty_ptree()
    {
        static Ptree pt;
        return pt;
    }

    template<class K, class D, class C, class A>
    inline void swap(basic_ptree<K, D, C, A> &pt1, basic_ptree<K, D, C, A> &pt2)
    {
        pt1.swap(pt2);
    }

} }

// Undefine debug macros
#ifdef BOOST_PROPERTY_TREE_DEBUG
#   undef BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT
#   undef BOOST_PROPERTY_TREE_DEBUG_DECREMENT_INSTANCES_COUNT
#endif

#endif
