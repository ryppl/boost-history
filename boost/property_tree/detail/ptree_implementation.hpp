// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_PTREE_IMPLEMENTATION_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_PTREE_IMPLEMENTATION_HPP_INCLUDED

#include <sstream>
#include <locale>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <functional>               // for std::less
#include <memory>                   // for std::auto_ptr
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>        // for boost::prior
#include <boost/property_tree/detail/ptree_utils.hpp>

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

    namespace detail
    {
        
        template<class T>
        struct array_to_pointer_decay
        {
            typedef T type;
        };

        template<class T, std::size_t N>
        struct array_to_pointer_decay<T[N]>
        {
            typedef const T *type;
        };

        ////////////////////////////////////////////////////////////////////////////
        // Extractor and inserter

        template<class Ch, class Type>
        struct extractor
        {
            inline bool operator()(const std::basic_string<Ch> &data, 
                                   Type &extracted,
                                   const std::locale &loc) const
            {
                std::basic_istringstream<Ch> stream(data);
                stream.imbue(loc);
                stream >> extracted >> std::ws;
                return stream.eof() && !stream.fail() && !stream.bad();
            }
        };

        template<class Ch>
        struct extractor<Ch, std::basic_string<Ch> >
        {
            inline bool operator()(const std::basic_string<Ch> &data, 
                                   std::basic_string<Ch> &extracted,
                                   const std::locale &loc) const
            {
                extracted = data;
                return true;
            }
        };

        template<class Ch, class Type>
        struct inserter
        {
            inline bool operator()(std::basic_string<Ch> &data, 
                                   const Type &to_insert,
                                   const std::locale &loc) const
            {
                typedef typename detail::array_to_pointer_decay<Type>::type Type2;
                std::basic_ostringstream<Ch> stream;
                stream.imbue(loc);
                if (std::numeric_limits<Type2>::is_specialized)
                    stream.precision(std::numeric_limits<Type2>::digits10 + 1);
                stream << to_insert;
                data = stream.str();
                return !stream.fail() && !stream.bad();
            }
        };

        template<class Ch>
        struct inserter<Ch, std::basic_string<Ch> >
        {
            inline bool operator()(std::basic_string<Ch> &data, 
                                   const std::basic_string<Ch> &to_insert,
                                   const std::locale &loc) const
            {
                data = to_insert;
                return true;
            }
        };

    }

    ///////////////////////////////////////////////////////////////////////////
    // Impl

    template<class Ch, class Tr>
    struct basic_ptree<Ch, Tr>::impl
    {
        data_type m_data;
        container_type m_container;
        index_type m_index;
    };

    ////////////////////////////////////////////////////////////////////////////
    // Traits

    template<class Ch>
    struct ptree_traits
    {
        typedef std::basic_string<Ch> data_type;
        template<class Type>
        struct extractor: public detail::extractor<Ch, Type> { };
        template<class Type>
        struct inserter: public detail::inserter<Ch, Type> { };
        inline bool operator()(const std::basic_string<Ch> &key1, 
                               const std::basic_string<Ch> &key2) const
        {
            return key1 < key2;
        }
    };

    template<class Ch>
    struct iptree_traits
    {
        std::locale loc;
        typedef std::basic_string<Ch> data_type;
        template<class Type>
        struct extractor: public detail::extractor<Ch, Type> { };
        template<class Type>
        struct inserter: public detail::inserter<Ch, Type> { };
        inline bool operator()(Ch c1, Ch c2) const      // Helper for comparing characters
        {
            return std::toupper(c1, loc) < std::toupper(c2, loc);
        }
        inline bool operator()(const std::basic_string<Ch> &key1, 
                               const std::basic_string<Ch> &key2) const
        {
            return std::lexicographical_compare(key1.begin(), key1.end(), key2.begin(), key2.end(), *this);
        }
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

    template<class Ch, class Tr>
    basic_ptree<Ch, Tr>::basic_ptree()
    {
        m_impl = new impl;
        BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT();
    }

    template<class Ch, class Tr>
    basic_ptree<Ch, Tr>::basic_ptree(const data_type &rhs)
    {
        std::auto_ptr<impl> tmp(new impl);
        tmp->m_data = rhs;
        m_impl = tmp.release();
        BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT();
    }

    template<class Ch, class Tr>
    basic_ptree<Ch, Tr>::basic_ptree(const basic_ptree<Ch, Tr> &rhs)
    {
        std::auto_ptr<impl> tmp(new impl);
        tmp->m_data = rhs.data();
        m_impl = tmp.get();
        insert(end(), rhs.begin(), rhs.end());
        tmp.release();
        BOOST_PROPERTY_TREE_DEBUG_INCREMENT_INSTANCES_COUNT();
    }

    template<class Ch, class Tr>
    basic_ptree<Ch, Tr>::~basic_ptree()
    {
        BOOST_PROPERTY_TREE_DEBUG_DECREMENT_INSTANCES_COUNT();
        delete m_impl;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Iterator access

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator 
        basic_ptree<Ch, Tr>::begin()
    {
        return m_impl->m_container.begin();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::const_iterator 
        basic_ptree<Ch, Tr>::begin() const
    {
        return m_impl->m_container.begin();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator 
        basic_ptree<Ch, Tr>::end()
    {
        return m_impl->m_container.end();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::const_iterator 
        basic_ptree<Ch, Tr>::end() const
    {
        return m_impl->m_container.end();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::reverse_iterator 
        basic_ptree<Ch, Tr>::rbegin()
    {
        return m_impl->m_container.rbegin();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::const_reverse_iterator 
        basic_ptree<Ch, Tr>::rbegin() const
    {
        return m_impl->m_container.rbegin();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::reverse_iterator 
        basic_ptree<Ch, Tr>::rend()
    {
        return m_impl->m_container.rend();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::const_reverse_iterator 
        basic_ptree<Ch, Tr>::rend() const
    {
        return m_impl->m_container.rend();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Data access

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::size_type 
        basic_ptree<Ch, Tr>::size() const
    {
        return m_impl->m_index.size();
    }

    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::empty() const
    {
        return m_impl->m_index.empty();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::data_type &
        basic_ptree<Ch, Tr>::data()
    {
        return m_impl->m_data;
    }

    template<class Ch, class Tr>
    const typename basic_ptree<Ch, Tr>::data_type &
        basic_ptree<Ch, Tr>::data() const
    {
        return m_impl->m_data;
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::value_type &
        basic_ptree<Ch, Tr>::front()
    {
        return m_impl->m_container.front();
    }
    
    template<class Ch, class Tr>
    const typename basic_ptree<Ch, Tr>::value_type &
        basic_ptree<Ch, Tr>::front() const
    {
        return m_impl->m_container.front();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::value_type &
        basic_ptree<Ch, Tr>::back()
    {
        return m_impl->m_container.back();
    }

    template<class Ch, class Tr>
    const typename basic_ptree<Ch, Tr>::value_type &
        basic_ptree<Ch, Tr>::back() const
    {
        return m_impl->m_container.back();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Operators

    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> &
        basic_ptree<Ch, Tr>::operator =(const basic_ptree<Ch, Tr> &rhs)
    {
        if (&rhs != this)
        {
            clear();
            data() = rhs.data();
            insert(end(), rhs.begin(), rhs.end());
        }
        return *this;
    }

    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::operator ==(const basic_ptree<Ch, Tr> &rhs) const
    {
        
        // Data and sizes must be equal
        if (size() != rhs.size() || data() != rhs.data())
            return false;

        // Keys and children must be equal
        Tr tr;
        const_iterator it = begin();
        const_iterator it_rhs = rhs.begin();
        for (; it != end(); ++it, ++it_rhs)
            if (tr(it->first, it_rhs->first) || 
                tr(it_rhs->first, it->first) || 
                it->second != it_rhs->second)
                return false;

        // Equal
        return true;

    }

    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::operator !=(const basic_ptree<Ch, Tr> &rhs) const
    {
        return !operator ==(rhs);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Container operations

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator 
        basic_ptree<Ch, Tr>::find(const key_type &key)
    {
        typename index_type::iterator it = m_impl->m_index.find(key);
        return it == m_impl->m_index.end() ? end() : it->second;
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::const_iterator 
        basic_ptree<Ch, Tr>::find(const key_type &key) const
    {
        typename index_type::const_iterator it = m_impl->m_index.find(key);
        return it == m_impl->m_index.end() ? end() : it->second;
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::size_type 
        basic_ptree<Ch, Tr>::count(const key_type &key) const
    {
        return m_impl->m_index.count(key);
    }

    template<class Ch, class Tr>
    void basic_ptree<Ch, Tr>::clear()
    {
        m_impl->m_data = data_type();
        m_impl->m_container.clear();
        m_impl->m_index.clear();
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator 
    basic_ptree<Ch, Tr>::insert(iterator where, 
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

    template<class Ch, class Tr>
    template<class It>
    void basic_ptree<Ch, Tr>::insert(iterator where, 
                                     It first, 
                                     It last)
    {
        for (; first != last; ++first, ++where)
            where = insert(where, value_type(first->first, first->second));
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator 
        basic_ptree<Ch, Tr>::erase(iterator where)
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

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::size_type 
        basic_ptree<Ch, Tr>::erase(const key_type &key)
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

    template<class Ch, class Tr>
    template<class It> 
    typename basic_ptree<Ch, Tr>::iterator 
        basic_ptree<Ch, Tr>::erase(It first, 
                                   It last)
    {
        while (first != last)
            first = erase(first);
        return first;
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator
        basic_ptree<Ch, Tr>::push_front(const value_type &value)
    {
        return insert(begin(), value);
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::iterator
        basic_ptree<Ch, Tr>::push_back(const value_type &value)
    {
        return insert(end(), value);
    }

    template<class Ch, class Tr>
    void basic_ptree<Ch, Tr>::pop_front()
    {
        erase(begin());
    }

    template<class Ch, class Tr>
    void basic_ptree<Ch, Tr>::pop_back()
    {
        erase(boost::prior(end()));
    }
        
    template<class Ch, class Tr>
    void basic_ptree<Ch, Tr>::swap(basic_ptree<Ch, Tr> &rhs)
    {
        std::swap(m_impl, rhs.m_impl);
    }

    template<class Ch, class Tr>
    void basic_ptree<Ch, Tr>::reverse()
    {
        m_impl->m_container.reverse();
    }
    
    template<class Ch, class Tr>
    template<class SortTr> 
    void basic_ptree<Ch, Tr>::sort(SortTr tr)
    {
        m_impl->m_container.sort(tr);
    }

    ///////////////////////////////////////////////////////////////////////////
    // ptree operations

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child(Ch separator, const key_type &path)
    {
        basic_ptree<Ch, Tr> *result;
        if (get_child_b(separator, path, &result))
            return result;
        else
            throw ptree_bad_path("key \"" + detail::narrow(path.c_str()) + "\" does not exist");
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    const basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child(Ch separator, const key_type &path) const
    {
        basic_ptree<Ch, Tr> *nc_this = const_cast<basic_ptree<Ch, Tr> *>(this);
        return nc_this->get_child(separator, path);
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::get_child_b(Ch separator, 
                                          const key_type &path, 
                                          basic_ptree<Ch, Tr> **result)
    {
        typename key_type::size_type n = path.find(separator);
        if (n != key_type::npos)
        {
            key_type head = path.substr(0, n);
            key_type tail = path.substr(n + 1, key_type::npos);
            iterator it = find(head);
            return it != end() ? it->second.get_child_b(separator, tail, result) : false;
        }
        else
        {
            iterator it = find(path);
            if (result && it != end())
                *result = &it->second;
            return it != end();
        }
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::get_child_b(Ch separator, 
                                          const key_type &path, 
                                          const basic_ptree<Ch, Tr> **result) const
    {
        basic_ptree<Ch, Tr> *nc_this = const_cast<basic_ptree<Ch, Tr> *>(this);
        basic_ptree<Ch, Tr> **nc_result = const_cast<basic_ptree<Ch, Tr> **>(result);
        return nc_this->get_child_b(separator, path, nc_result);
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child_d(Ch separator, 
                                         const key_type &path, 
                                         basic_ptree<Ch, Tr> *default_value)
    {
        basic_ptree<Ch, Tr> *result;
        if (get_child_b(separator, path, &result))
            return result;
        else
            return default_value;
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    const basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child_d(Ch separator, 
                                         const key_type &path, 
                                         const basic_ptree<Ch, Tr> *default_value) const
    {
        basic_ptree<Ch, Tr> *nc_this = const_cast<basic_ptree<Ch, Tr> *>(this);
        basic_ptree<Ch, Tr> *nc_default_value = const_cast<basic_ptree<Ch, Tr> *>(default_value);
        return nc_this->get_child_d(separator, path, nc_default_value);
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    optional<basic_ptree<Ch, Tr> *> 
        basic_ptree<Ch, Tr>::get_child_o(Ch separator, const key_type &path)
    {
        basic_ptree<Ch, Tr> *tmp;
        if (get_child_b(separator, path, &tmp))
            return optional<basic_ptree<Ch, Tr> *>(tmp);
        else
            return optional<basic_ptree<Ch, Tr> *>();
    }

    // Get child ptree with custom separator
    template<class Ch, class Tr>
    optional<const basic_ptree<Ch, Tr> *> 
        basic_ptree<Ch, Tr>::get_child_o(Ch separator, const key_type &path) const
    {
        const basic_ptree<Ch, Tr> *tmp;
        if (get_child_b(separator, path, &tmp))
            return optional<const basic_ptree<Ch, Tr> *>(tmp);
        else
            return optional<const basic_ptree<Ch, Tr> *>();
    }

    // Get child ptree with default separator
    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child(const key_type &path)
    {
        return get_child(Ch('.'), path);
    }

    // Get child ptree with default separator
    template<class Ch, class Tr>
    const basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child(const key_type &path) const
    {
        return get_child(Ch('.'), path);
    }

    // Get child ptree with default separator
    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::get_child_b(const key_type &path, 
                                          basic_ptree<Ch, Tr> **result)
    {
        return get_child_b(Ch('.'), path, result);
    }
    
    // Get child ptree with default separator
    template<class Ch, class Tr>
    bool basic_ptree<Ch, Tr>::get_child_b(const key_type &path, 
                                          const basic_ptree<Ch, Tr> **result) const
    {
        return get_child_b(Ch('.'), path, result);
    }

    // Get child ptree with default separator
    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child_d(const key_type &path, 
                                         basic_ptree<Ch, Tr> *default_value)
    {
        return get_child_d(Ch('.'), path, default_value);
    }
    
    // Get child ptree with default separator
    template<class Ch, class Tr>
    const basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::get_child_d(const key_type &path, 
                                         const basic_ptree<Ch, Tr> *default_value) const
    {
        return get_child_d(Ch('.'), path, default_value);
    }
    
    // Get child ptree with default separator
    template<class Ch, class Tr>
    optional<basic_ptree<Ch, Tr> *> 
        basic_ptree<Ch, Tr>::get_child_o(const key_type &path)
    {
        return get_child_o(Ch('.'), path);
    }

    // Get child ptree with default separator
    template<class Ch, class Tr>
    optional<const basic_ptree<Ch, Tr> *> 
        basic_ptree<Ch, Tr>::get_child_o(const key_type &path) const
    {
        return get_child_o(Ch('.'), path);
    }

    // Put child ptree with custom separator
    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::put_child(Ch separator, 
                                       const key_type &path, 
                                       const basic_ptree<Ch, Tr> *value)
    {
        typename key_type::size_type n = path.find(separator);
        if (n == key_type::npos)
        {
            iterator it = find(path);
            if (it == end())
                return &push_back(value_type(path, value ? *value : basic_ptree<Ch, Tr>()))->second;
            else
            {
                if (value)
                    it->second = *value;
                return &it->second;
            }
        }
        else
        {
            key_type head = path.substr(0, n);
            key_type tail = path.substr(n + 1, key_type::npos);
            iterator it = find(head);
            if (it == end())
                it = push_back(value_type(head, basic_ptree<Ch, Tr>()));
            return it->second.put_child(separator, tail, value);
        }
    }

    // Put child ptree with default separator
    template<class Ch, class Tr>
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::put_child(const key_type &path,
                                       const basic_ptree<Ch, Tr> *value)
    {
        return put_child(Ch('.'), path, value);
    }

    // Get value from data of ptree
    template<class Ch, class Tr>
    template<class Type>
    Type basic_ptree<Ch, Tr>::get_own(const std::locale &loc) const
    {
        Type tmp;
        if (get_own_b(&tmp, loc))
            return tmp;
        else
            throw ptree_bad_data(std::string("conversion of data into type '") + 
                                 typeid(Type).name() + "' failed");
    }

    // Get value from data of ptree
    template<class Ch, class Tr>
    template<class Type>
    bool basic_ptree<Ch, Tr>::get_own_b(Type *result, 
                                        const std::locale &loc) const
    {
        BOOST_STATIC_ASSERT(boost::is_pointer<Type>::value == false);
        Type tmp;
        if (typename traits_type::template extractor<Type>()(m_impl->m_data, tmp, loc))
        {
            if (result)
                *result = tmp;
            return true;
        }
        else
            return false;
    }

    // Get value from data of ptree
    template<class Ch, class Tr>
    template<class Type>
    Type basic_ptree<Ch, Tr>::get_own_d(const Type &default_value, 
                                        const std::locale &loc) const
    {
        Type tmp;
        if (get_own_b(&tmp, loc))
            return tmp;
        else
            return default_value;
    }

    // Get value from data of ptree
    template<class Ch, class Tr>
    template<class CharType>
    std::basic_string<CharType> 
        basic_ptree<Ch, Tr>::get_own_d(const CharType *default_value, 
                                       const std::locale &loc) const
    {
        BOOST_STATIC_ASSERT((boost::is_same<Ch, CharType>::value == true));
        return get_own_d(std::basic_string<CharType>(default_value), loc);
    }

    // Get value from data of ptree
    template<class Ch, class Tr>
    template<class Type>
    optional<Type> 
        basic_ptree<Ch, Tr>::get_own_o(const std::locale &loc) const
    {
        Type tmp;
        if (get_own_b(&tmp, loc))
            return optional<Type>(tmp);
        else
            return optional<Type>();
    }

    // Get value from data of child ptree (custom path separator)
    template<class Ch, class Tr>
    template<class Type>
    Type basic_ptree<Ch, Tr>::get(Ch separator,
                                  const key_type &path,
                                  const std::locale &loc) const
    {
        return get_child(separator, path)->get_own<Type>(loc);
    }

    // Get value from data of child ptree (custom path separator)
    template<class Ch, class Tr>
    template<class Type>
    bool basic_ptree<Ch, Tr>::get_b(Ch separator,
                                    const key_type &path, 
                                    Type *result, 
                                    const std::locale &loc) const
    {
        const basic_ptree<Ch, Tr> *child;
        if (get_child_b(separator, path, &child))
            return child->get_own_b(result, loc);
        else
            return false;
    }

    // Get value from data of child ptree (custom path separator)
    template<class Ch, class Tr>
    template<class Type>
    Type basic_ptree<Ch, Tr>::get_d(Ch separator,
                                    const key_type &path, 
                                    const Type &default_value, 
                                    const std::locale &loc) const
    {
        Type tmp;
        if (get_b(separator, path, &tmp, loc))
            return tmp;
        else
            return default_value;
    }

    // Get value from data of child ptree (custom path separator)
    template<class Ch, class Tr>
    template<class CharType>
    std::basic_string<CharType> 
        basic_ptree<Ch, Tr>::get_d(Ch separator,
                                   const key_type &path, 
                                   const CharType *default_value,
                                   const std::locale &loc) const
    {
        BOOST_STATIC_ASSERT((boost::is_same<Ch, CharType>::value == true));
        return get_d(separator, path, std::basic_string<CharType>(default_value), loc);
    }

    // Get value from data of child ptree (custom path separator)
    template<class Ch, class Tr>
    template<class Type>
    optional<Type> 
        basic_ptree<Ch, Tr>::get_o(Ch separator,
                                   const key_type &path, 
                                   const std::locale &loc) const
    {
        Type tmp;
        if (get_b(separator, path, &tmp, loc))
            return optional<Type>(tmp);
        else
            return optional<Type>();
    }

    // Get value from data of child ptree (default path separator)
    template<class Ch, class Tr>
    template<class Type>
    Type basic_ptree<Ch, Tr>::get(const key_type &path,
                                  const std::locale &loc) const
    {
        return get<Type>(Ch('.'), path, loc);
    }

    // Get value from data of child ptree (default path separator)
    template<class Ch, class Tr>
    template<class Type>
    bool basic_ptree<Ch, Tr>::get_b(const key_type &path, 
                                    Type *result,
                                    const std::locale &loc) const
    {
        return get_b(Ch('.'), path, result, loc);
    }

    // Get value from data of child ptree (default path separator)
    template<class Ch, class Tr>
    template<class Type>
    Type basic_ptree<Ch, Tr>::get_d(const key_type &path, 
                                    const Type &default_value,
                                    const std::locale &loc) const
    {
        return get_d(Ch('.'), path, default_value, loc);
    }

    // Get value from data of child ptree (default path separator)
    template<class Ch, class Tr>
    template<class CharType>
    std::basic_string<CharType> 
        basic_ptree<Ch, Tr>::get_d(const key_type &path, 
                                   const CharType *default_value,
                                   const std::locale &loc) const
    {
        return get_d(Ch('.'), path, default_value, loc);
    }

    // Get value from data of child ptree (default path separator)
    template<class Ch, class Tr>
    template<class Type>
    optional<Type> 
        basic_ptree<Ch, Tr>::get_o(const key_type &path, 
                                   const std::locale &loc) const
    {
        return get_o<Type>(Ch('.'), path, loc);
    }

    // Put value in data of ptree
    template<class Ch, class Tr>
    template<class Type> 
    void basic_ptree<Ch, Tr>::put_own(const Type &value, const std::locale &loc)
    {
        using namespace boost;
        BOOST_STATIC_ASSERT((is_pointer<Type>::value == false || 
                             is_same<Ch, typename remove_const<typename remove_pointer<Type>::type>::type>::value == true));
        typename traits_type::template inserter<Type>()(m_impl->m_data, value, loc);
    }

    // Put value in data of child ptree (custom path separator)
    template<class Ch, class Tr>
    template<class Type> 
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::put(Ch separator,
                                 const key_type &path, 
                                 const Type &value,
                                 const std::locale &loc)
    {
        basic_ptree<Ch, Tr> *child = put_child(separator, path, NULL);
        child->put_own(value, loc);
        return child;
    }

    // Put value in data of child ptree (default path separator)
    template<class Ch, class Tr>
    template<class Type> 
    basic_ptree<Ch, Tr> *
        basic_ptree<Ch, Tr>::put(const key_type &path, 
                                 const Type &value,
                                 const std::locale &loc)
    {
        return put(Ch('.'), path, value, loc);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Debugging

#ifdef BOOST_PROPERTY_TREE_DEBUG

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::size_type 
        basic_ptree<Ch, Tr>::debug_get_instances_count() 
    { 
        return debug_instances_count; 
    }

    template<class Ch, class Tr>
    typename basic_ptree<Ch, Tr>::size_type 
        basic_ptree<Ch, Tr>::debug_instances_count;

    template<class Ch, class Tr>
    boost::detail::lightweight_mutex 
        basic_ptree<Ch, Tr>::debug_mutex;

#endif

    ///////////////////////////////////////////////////////////////////////////
    // Free functions

    template<class Ptree> 
    inline const Ptree *empty_ptree()
    {
        static Ptree pt;
        return &pt;
    }

    template<class Ch, class Tr>
    inline void swap(basic_ptree<Ch, Tr> &pt1, basic_ptree<Ch, Tr> &pt2)
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
