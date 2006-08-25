// ----------------------------------------------------------------------------
// ******
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_PTREE_IMPLEMENTATION_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_PTREE_IMPLEMENTATION_HPP_INCLUDED

//////////////////////////////////////////////////////////////////////////////
// Debug macros

#ifdef BOOST_PROPERTY_TREE_DEBUG

    // Increment instances counter
    #define BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT()       \
        {                                                               \
            typedef boost::detail::lightweight_mutex::scoped_lock lock; \
            lock l(debug_mutex);                                        \
            ++debug_instances_count;                                    \
        }

    // Decrement instances counter
    #define BOOST_PROPERTY_TREE_DEBUG_DECREMENT_INSTANCES_COUNT()       \
        {                                                               \
            typedef boost::detail::lightweight_mutex::scoped_lock lock; \
            lock l(debug_mutex);                                        \
            BOOST_ASSERT(debug_instances_count > 0);                    \
            --debug_instances_count;                                    \
        }

#else // BOOST_PROPERTY_TREE_DEBUG

    #define BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT() static_cast<void>(0)
    #define BOOST_PROPERTY_TREE_DEBUG_DECREMENT_INSTANCES_COUNT() static_cast<void>(0)

#endif // BOOST_PROPERTY_TREE_DEBUG

namespace boost { namespace property_tree
{

    ///////////////////////////////////////////////////////////////////////////
    // Impl

    template<class C, class K, class P, class D, class X>
    struct basic_ptree<C, K, P, D, X>::impl
    {
        data_type m_data;
        container_type m_container;
        index_type m_index;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Exceptions

    class ptree_error: public std::runtime_error
    {
    public:
        ptree_error(const std::string &what): std::runtime_error(what) { }
        ~ptree_error() throw() { }
    };

    class ptree_bad_data: public ptree_error
    {
    public:
        ptree_bad_data(const std::string &what): ptree_error(what) { }
        ~ptree_bad_data() throw() { }
    };
    
    class ptree_bad_path: public ptree_error
    {
    public:
        ptree_bad_path(const std::string &what): ptree_error(what) { }
        ~ptree_bad_path() throw() { }
    };

    ///////////////////////////////////////////////////////////////////////////
    // Construction & destruction

    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X>::basic_ptree()
    {
        m_impl = new impl;
        BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT();
    }

    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X>::basic_ptree(const data_type &rhs)
    {
        std::auto_ptr<impl> tmp(new impl);
        tmp->m_data = rhs;
        m_impl = tmp.release();
        BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT();
    }

    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X>::basic_ptree(const basic_ptree<C, K, P, D, X> &rhs)
    {
        std::auto_ptr<impl> tmp(new impl);
        tmp->m_data = rhs.data();
        m_impl = tmp.get();
        insert(end(), rhs.begin(), rhs.end());
        tmp.release();
        BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT();
    }

    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X>::~basic_ptree()
    {
        BOOST_PROPERTY_TREE_DEBUG_DECREMENT_INSTANCES_COUNT();
        delete m_impl;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Iterator access

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator 
        basic_ptree<C, K, P, D, X>::begin()
    {
        return m_impl->m_container.begin();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::const_iterator 
        basic_ptree<C, K, P, D, X>::begin() const
    {
        return m_impl->m_container.begin();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator 
        basic_ptree<C, K, P, D, X>::end()
    {
        return m_impl->m_container.end();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::const_iterator 
        basic_ptree<C, K, P, D, X>::end() const
    {
        return m_impl->m_container.end();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::reverse_iterator 
        basic_ptree<C, K, P, D, X>::rbegin()
    {
        return m_impl->m_container.rbegin();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::const_reverse_iterator 
        basic_ptree<C, K, P, D, X>::rbegin() const
    {
        return m_impl->m_container.rbegin();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::reverse_iterator 
        basic_ptree<C, K, P, D, X>::rend()
    {
        return m_impl->m_container.rend();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::const_reverse_iterator 
        basic_ptree<C, K, P, D, X>::rend() const
    {
        return m_impl->m_container.rend();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Data access

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::size_type 
        basic_ptree<C, K, P, D, X>::size() const
    {
        return m_impl->m_index.size();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::size_type 
        basic_ptree<C, K, P, D, X>::max_size() const
    {
        return (std::min)(m_impl->m_container.max_size(), m_impl->m_index.max_size());
    }

    template<class C, class K, class P, class D, class X>
    bool basic_ptree<C, K, P, D, X>::empty() const
    {
        return m_impl->m_index.empty();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::data_type &
        basic_ptree<C, K, P, D, X>::data()
    {
        return m_impl->m_data;
    }

    template<class C, class K, class P, class D, class X>
    const typename basic_ptree<C, K, P, D, X>::data_type &
        basic_ptree<C, K, P, D, X>::data() const
    {
        return m_impl->m_data;
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::value_type &
        basic_ptree<C, K, P, D, X>::front()
    {
        return m_impl->m_container.front();
    }
    
    template<class C, class K, class P, class D, class X>
    const typename basic_ptree<C, K, P, D, X>::value_type &
        basic_ptree<C, K, P, D, X>::front() const
    {
        return m_impl->m_container.front();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::value_type &
        basic_ptree<C, K, P, D, X>::back()
    {
        return m_impl->m_container.back();
    }

    template<class C, class K, class P, class D, class X>
    const typename basic_ptree<C, K, P, D, X>::value_type &
        basic_ptree<C, K, P, D, X>::back() const
    {
        return m_impl->m_container.back();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Operators

    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::operator =(const basic_ptree<C, K, P, D, X> &rhs)
    {
        if (&rhs != this)
        {
            clear();
            data() = rhs.data();
            insert(end(), rhs.begin(), rhs.end());
        }
        return *this;
    }

    template<class C, class K, class P, class D, class X>
    bool basic_ptree<C, K, P, D, X>::operator ==(const basic_ptree<C, K, P, D, X> &rhs) const
    {
        
        // Data and sizes must be equal
        if (size() != rhs.size() || data() != rhs.data())
            return false;

        // Keys and children must be equal
        C comp;
        const_iterator it = begin();
        const_iterator it_rhs = rhs.begin();
        const_iterator it_end = end();
        for (; it != it_end; ++it, ++it_rhs)
            if (comp(it->first, it_rhs->first)
                || comp(it_rhs->first, it->first)
                || it->second != it_rhs->second)
            {
                return false;
            }

        // Equal
        return true;

    }

    template<class C, class K, class P, class D, class X>
    bool basic_ptree<C, K, P, D, X>::operator !=(const basic_ptree<C, K, P, D, X> &rhs) const
    {
        return !operator ==(rhs);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Container operations

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator 
        basic_ptree<C, K, P, D, X>::find(const key_type &key)
    {
        typename index_type::iterator it = m_impl->m_index.find(key);
        return it == m_impl->m_index.end() ? end() : it->second;
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::const_iterator 
        basic_ptree<C, K, P, D, X>::find(const key_type &key) const
    {
        typename index_type::const_iterator it = m_impl->m_index.find(key);
        return it == m_impl->m_index.end() ? end() : it->second;
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::size_type 
        basic_ptree<C, K, P, D, X>::count(const key_type &key) const
    {
        return m_impl->m_index.count(key);
    }

    template<class C, class K, class P, class D, class X>
    void basic_ptree<C, K, P, D, X>::clear()
    {
        m_impl->m_data = data_type();
        m_impl->m_container.clear();
        m_impl->m_index.clear();
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator 
    basic_ptree<C, K, P, D, X>::insert(iterator where, 
                                       const value_type &value)
    {

        // Insert new value into container. If that throws nothing needs to be rolled back
        where = m_impl->m_container.insert(where, value);

        // Update index. If that throws we need to rollback the insert
        try {
            m_impl->m_index.insert(typename index_type::value_type(where->first, where));
        } 
        catch (...) {
            m_impl->m_container.erase(where);   // rollback the insert
            throw;
        }

        return where;
    }

    template<class C, class K, class P, class D, class X>
    template<class It>
    void basic_ptree<C, K, P, D, X>::insert(iterator where, It first, It last)
    {
        for (; first != last; ++first, ++where)
            where = insert(where, value_type(first->first, first->second));
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator 
        basic_ptree<C, K, P, D, X>::erase(iterator where)
    {

        // Remove from index
        typename index_type::iterator lo = m_impl->m_index.lower_bound(where->first);
        typename index_type::iterator hi = m_impl->m_index.upper_bound(where->first);
        for (; lo != hi; ++lo)
            if (lo->second == where)
            {
                m_impl->m_index.erase(lo);
                break;
            }
        
        // Remove from container    
        return m_impl->m_container.erase(where);

    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::size_type 
        basic_ptree<C, K, P, D, X>::erase(const key_type &key)
    {
        size_type count = 0;
        typename index_type::iterator lo = m_impl->m_index.lower_bound(key);
        if (lo != m_impl->m_index.end())
        {
            typename index_type::iterator hi = m_impl->m_index.upper_bound(key);
            while (lo != hi)
            {
                typename index_type::iterator it = lo++;
                erase(it->second);
                ++count;
            }
        }
        return count;
    }

    template<class C, class K, class P, class D, class X>
    template<class It> 
    typename basic_ptree<C, K, P, D, X>::iterator 
        basic_ptree<C, K, P, D, X>::erase(It first, It last)
    {
        while (first != last)
            first = erase(first);
        return first;
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator
        basic_ptree<C, K, P, D, X>::push_front(const value_type &value)
    {
        return insert(begin(), value);
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::iterator
        basic_ptree<C, K, P, D, X>::push_back(const value_type &value)
    {
        return insert(end(), value);
    }

    template<class C, class K, class P, class D, class X>
    void basic_ptree<C, K, P, D, X>::pop_front()
    {
        erase(begin());
    }

    template<class C, class K, class P, class D, class X>
    void basic_ptree<C, K, P, D, X>::pop_back()
    {
        erase(boost::prior(end()));
    }
        
    template<class C, class K, class P, class D, class X>
    void basic_ptree<C, K, P, D, X>::swap(basic_ptree<C, K, P, D, X> &rhs)
    {
        std::swap(m_impl, rhs.m_impl);
    }

    template<class C, class K, class P, class D, class X>
    void basic_ptree<C, K, P, D, X>::reverse()
    {
        m_impl->m_container.reverse();
    }
    
    template<class C, class K, class P, class D, class X>
    template<class SortTr> 
    void basic_ptree<C, K, P, D, X>::sort(SortTr tr)
    {
        m_impl->m_container.sort(tr);
    }

    ///////////////////////////////////////////////////////////////////////////
    // ptree operations

    // Get child ptree
    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::get_child(const path_type &path)
    {
        self_type *child = path.get_child(*this);
        if (child)
            return *child;
        else
            /* throw ptree_bad_path("key \"" + detail::narrow(path.c_str()) + "\" does not exist"); */
            throw ptree_bad_path("");
    }

    // Get child ptree
    template<class C, class K, class P, class D, class X>
    const basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::get_child(const path_type &path) const
    {
        self_type *nc_this = const_cast<self_type *>(this);
        return nc_this->get_child(path);
    }

    // Get child ptree
    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::get_child(const path_type &path, 
                                              basic_ptree<C, K, P, D, X> &default_value)
    {
        self_type *child = path.get_child(*this);
        if (child)
            return *child;
        else
            return default_value;
    }

    // Get child ptree
    template<class C, class K, class P, class D, class X>
    const basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::get_child(const path_type &path, 
                                              const basic_ptree<C, K, P, D, X> &default_value) const
    {
        self_type *nc_this = const_cast<self_type *>(this);
        self_type &nc_default_value = const_cast<self_type &>(default_value);
        return nc_this->get_child(path, nc_default_value);
    }


    // Get child ptree
    template<class C, class K, class P, class D, class X>
    optional<basic_ptree<C, K, P, D, X> &> 
        basic_ptree<C, K, P, D, X>::get_child_optional(const path_type &path)
    {
        self_type *child = path.get_child(*this);
        if (child)
            return optional<self_type &>(*child);
        else
            return optional<self_type &>();
    }

    // Get child ptree
    template<class C, class K, class P, class D, class X>
    optional<const basic_ptree<C, K, P, D, X> &> 
        basic_ptree<C, K, P, D, X>::get_child_optional(const path_type &path) const
    {
        self_type *nc_this = const_cast<self_type *>(this);
        optional<self_type &> tmp = nc_this->get_child_optional(path);
        if (tmp)
            return optional<const self_type &>(tmp.get());
        else
            return optional<const self_type &>();
    }

    // Put child ptree
    template<class C, class K, class P, class D, class X>
    basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::put_child(const path_type &path, 
                                              const basic_ptree<C, K, P, D, X> &value,
                                              bool do_not_replace)
    {
        self_type *child = path.put_child(*this, value, do_not_replace);
        if (child)
            return *child;
        else
            throw ptree_bad_path("");
    }

    // Get value from data of ptree
    template<class C, class K, class P, class D, class X>
    template<class Type>
    Type basic_ptree<C, K, P, D, X>::get_value(const translator_type &x) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);   // Disallow pointer types, they are unsafe
        Type value;
        if (x.get_value(*this, value))
            return value;
        else
            throw ptree_bad_data(std::string("conversion of data into type \"") + 
                                 typeid(Type).name() + "\" failed");
    }

    // Get value from data of ptree
    template<class C, class K, class P, class D, class X>
    template<class Type>
    Type basic_ptree<C, K, P, D, X>::get_value(const Type &default_value, 
                                               const translator_type &x) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);   // Disallow pointer types, they are unsafe
        Type value;
        if (x.get_value(*this, value))
            return value;
        else
            throw ptree_bad_data(std::string("conversion of data into type \"") + 
                                 typeid(Type).name() + "\" failed");
    }

    // Get value from data of ptree
    template<class C, class K, class P, class D, class X>
    template<class CharType>
    std::basic_string<CharType> 
        basic_ptree<C, K, P, D, X>::get_value(const CharType *default_value, 
                                              const translator_type &x) const
    {
        return get_value(std::basic_string<CharType>(default_value), x);
    }

    // Get value from data of ptree
    template<class C, class K, class P, class D, class X>
    template<class Type>
    optional<Type> 
        basic_ptree<C, K, P, D, X>::get_value_optional(const translator_type &x) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);   // Disallow pointer types, they are unsafe
        Type value;
        if (x.get_value(*this, value))
            return optional<Type>(value);
        else
            return optional<Type>();
    }

    // Get value from data of child ptree
    template<class C, class K, class P, class D, class X>
    template<class Type>
    Type basic_ptree<C, K, P, D, X>::get(const path_type &path,
                                         const translator_type &x) const
    {
        return get_child(path).get_value<Type>(x);
    }

    // Get value from data of child ptree
    template<class C, class K, class P, class D, class X>
    template<class Type>
    Type basic_ptree<C, K, P, D, X>::get(const path_type &path, 
                                         const Type &default_value, 
                                         const translator_type &x) const
    {
        if (optional<Type> result = get_optional<Type>(path, x))
            return *result;
        else
            return default_value;
    }

    // Get value from data of child ptree
    template<class C, class K, class P, class D, class X>
    template<class CharType>
    std::basic_string<CharType> 
        basic_ptree<C, K, P, D, X>::get(const path_type &path, 
                                        const CharType *default_value,
                                        const translator_type &x) const
    {
        return get(path, std::basic_string<CharType>(default_value), x);
    }

    // Get value from data of child ptree
    template<class C, class K, class P, class D, class X>
    template<class Type>
    optional<Type> 
        basic_ptree<C, K, P, D, X>::get_optional(const path_type &path, 
                                                 const translator_type &x) const
    {
        if (optional<const basic_ptree<C, K, P, D, X> &> child = get_child_optional(path))
            return child.get().get_value_optional<Type>(x);
        else
            return optional<Type>();
    }

    // Put value in data of ptree
    template<class C, class K, class P, class D, class X>
    template<class Type> 
    void basic_ptree<C, K, P, D, X>::put_value(const Type &value, const translator_type &x)
    {
        if (!x.put_value(*this, value))
            throw ptree_bad_data(std::string("conversion of type \"") + typeid(Type).name() + 
                                 "\" into data failed");
    }

    // Put value in data of child ptree
    template<class C, class K, class P, class D, class X>
    template<class Type> 
    basic_ptree<C, K, P, D, X> &
        basic_ptree<C, K, P, D, X>::put(const path_type &path, 
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

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::size_type 
        basic_ptree<C, K, P, D, X>::debug_get_instances_count() 
    { 
        empty_ptree<basic_ptree<C, K, P, D, X> >();    // Make sure empty ptree exists
        return debug_instances_count - 1;              // Do not count empty ptree
    }

    template<class C, class K, class P, class D, class X>
    typename basic_ptree<C, K, P, D, X>::size_type 
        basic_ptree<C, K, P, D, X>::debug_instances_count;

    template<class C, class K, class P, class D, class X>
    boost::detail::lightweight_mutex 
        basic_ptree<C, K, P, D, X>::debug_mutex;

#endif

    ///////////////////////////////////////////////////////////////////////////
    // Free functions

    template<class Ptree> 
    inline const Ptree &empty_ptree()
    {
        static Ptree pt;
        return pt;
    }

    template<class C, class K, class P, class D, class X>
    inline void swap(basic_ptree<C, K, P, D, X> &pt1, basic_ptree<C, K, P, D, X> &pt2)
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
