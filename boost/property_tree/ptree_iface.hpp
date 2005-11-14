// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_PTREE_IFACE_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_PTREE_IFACE_HPP_INCLUDED

#include <boost/config.hpp>
#include <string>
#include <list>
#include <map>
#include <utility>          // For std::pair
#include <locale>

#ifdef BOOST_PROPERTY_TREE_DEBUG
#   include <boost/detail/lightweight_mutex.hpp>   // For syncing debug instances counter
#endif

namespace boost { namespace property_tree
{

    ////////////////////////////////////////////////////////////////////////////
    // Traits

    template<class Ch> struct ptree_traits;
    template<class Ch> struct iptree_traits;

    ///////////////////////////////////////////////////////////////////////////
    // Errors

    class ptree_error;
    class bad_ptree_data;
    class bad_ptree_path;

    ///////////////////////////////////////////////////////////////////////////
    // Empty ptree helper

    template<class Ptree> const Ptree *empty_ptree();

    ///////////////////////////////////////////////////////////////////////////
    // basic_ptree class template

    template<class Ch, class Tr = ptree_traits<Ch> >
    class basic_ptree
    {

    public:

        // Basic types
        typedef Ch char_type;
        typedef Tr traits_type;
        typedef std::basic_string<Ch> key_type;
        typedef std::basic_string<Ch> data_type;
        
        // Container-related types
        typedef std::pair<key_type, basic_ptree<Ch, Tr> > value_type;
        typedef std::list<value_type> container_type;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::iterator iterator;
        typedef typename container_type::const_iterator const_iterator;
        typedef typename container_type::reverse_iterator reverse_iterator;
        typedef typename container_type::const_reverse_iterator const_reverse_iterator;
        
        ///////////////////////////////////////////////////////////////////////////
        // Construction & destruction

        basic_ptree();
        explicit basic_ptree(const data_type &data);
        basic_ptree(const basic_ptree<Ch, Tr> &rhs);
        ~basic_ptree();

        ///////////////////////////////////////////////////////////////////////////
        // Iterator access

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        
        ///////////////////////////////////////////////////////////////////////////
        // Data access

        size_type size() const;
        bool empty() const;
        
        data_type &data();
        const data_type &data() const;

        value_type &front();
        const value_type &front() const;
        value_type &back();
        const value_type &back() const;

        ///////////////////////////////////////////////////////////////////////////
        // Operators

        basic_ptree<Ch, Tr> &operator =(const basic_ptree<Ch, Tr> &rhs);

        bool operator ==(const basic_ptree<Ch, Tr> &rhs) const;
        bool operator !=(const basic_ptree<Ch, Tr> &rhs) const;

        ///////////////////////////////////////////////////////////////////////////
        // Container operations

        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;

        size_type count(const key_type &key) const;

        void clear();

        iterator insert(iterator where, const value_type &value);
        template<class It> void insert(iterator where, It first, It last);

        iterator erase(iterator where);
        size_type erase(const key_type &key);
        template<class It> iterator erase(It first, It last);

        iterator push_front(const value_type &value);
        iterator push_back(const value_type &value);

        void pop_front();
        void pop_back();

        void swap(basic_ptree<Ch, Tr> &rhs);

        void reverse();
        
        void sort();
        template<class SortTr> void sort(SortTr tr);

        ///////////////////////////////////////////////////////////////////////////
        // ptree operations

        // Get child ptree with custom separator
        basic_ptree<Ch, Tr> *get_child(Ch separator, const key_type &path);
        const basic_ptree<Ch, Tr> *get_child(Ch separator, const key_type &path) const;
        bool get_child_b(Ch separator, const key_type &path, basic_ptree<Ch, Tr> **result);
        bool get_child_b(Ch separator, const key_type &path, const basic_ptree<Ch, Tr> **result) const;
        basic_ptree<Ch, Tr> *get_child_d(Ch separator, const key_type &path, basic_ptree<Ch, Tr> *default_value);
        const basic_ptree<Ch, Tr> *get_child_d(Ch separator, const key_type &path, const basic_ptree<Ch, Tr> *default_value) const;

        // Get child ptree with default separator
        basic_ptree<Ch, Tr> *get_child(const key_type &path);
        const basic_ptree<Ch, Tr> *get_child(const key_type &path) const;
        bool get_child_b(const key_type &path, basic_ptree<Ch, Tr> **result);
        bool get_child_b(const key_type &path, const basic_ptree<Ch, Tr> **result) const;
        basic_ptree<Ch, Tr> *get_child_d(const key_type &path, basic_ptree<Ch, Tr> *default_value);
        const basic_ptree<Ch, Tr> *get_child_d(const key_type &path, const basic_ptree<Ch, Tr> *default_value) const;

        // Put child ptree with custom separator
        basic_ptree<Ch, Tr> *put_child(Ch separator, const key_type &path, basic_ptree<Ch, Tr> *value);

        // Put child ptree with default separator
        basic_ptree<Ch, Tr> *put_child(const key_type &path, basic_ptree<Ch, Tr> *value);

        // Get value from data of ptree
        template<class Type> Type get_own(const std::locale &loc = std::locale()) const;
        template<class Type> bool get_own_b(Type *result, const std::locale &loc = std::locale()) const;
        template<class Type> Type get_own_d(const Type &default_value, const std::locale &loc = std::locale()) const;
        template<class CharType> std::basic_string<CharType> get_own_d(const CharType *default_value, const std::locale &loc = std::locale()) const;

        // Get value from data of child ptree (custom path separator)
        template<class Type> Type get(Ch separator, const key_type &path, const std::locale &loc = std::locale()) const;
        template<class Type> bool get_b(Ch separator, const key_type &path, Type *result, const std::locale &loc = std::locale()) const;
        template<class Type> Type get_d(Ch separator, const key_type &path, const Type &default_value, const std::locale &loc = std::locale()) const;
        template<class CharType> std::basic_string<CharType> get_d(Ch separator, const key_type &path, const CharType *default_value, const std::locale &loc = std::locale()) const;

        // Get value from data of child ptree (default path separator)
        template<class Type> Type get(const key_type &path, const std::locale &loc = std::locale()) const;
        template<class Type> bool get_b(const key_type &path, Type *result, const std::locale &loc = std::locale()) const;
        template<class Type> Type get_d(const key_type &path, const Type &default_value, const std::locale &loc = std::locale()) const;
        template<class CharType> std::basic_string<CharType> get_d(const key_type &path, const CharType *default_value, const std::locale &loc = std::locale()) const;

        // Put value in data of ptree
        template<class Type> void put_own(const Type &value, const std::locale &loc = std::locale());

        // Put value in data of child ptree (custom path separator)
        template<class Type> basic_ptree<Ch, Tr> *put(Ch separator, const key_type &path, const Type &value, const std::locale &loc = std::locale());

        // Put value in data of child ptree (default path separator)
        template<class Type> basic_ptree<Ch, Tr> *put(const key_type &path, const Type &value, const std::locale &loc = std::locale());

    private:

        typedef std::multimap<key_type, iterator, Tr> index_type;
        
        struct impl;
        impl *m_impl;

        ////////////////////////////////////////////////////////////////////////////
        // Debugging

#ifdef BOOST_PROPERTY_TREE_DEBUG
    private:
        static boost::detail::lightweight_mutex debug_mutex;    // Mutex for syncing instances counter
        static size_type debug_instances_count;                 // Total number of instances of this ptree class
    public:
        static size_type debug_get_instances_count();
#endif

    };

    ////////////////////////////////////////////////////////////////////////////
    // Typedefs

    typedef basic_ptree<char, ptree_traits<char> > ptree;       // case sensitive, narrow char
    typedef basic_ptree<char, iptree_traits<char> > iptree;     // case insensitive, narrow char
#ifndef BOOST_NO_CWCHAR
    typedef basic_ptree<wchar_t, ptree_traits<wchar_t> > wptree;    // case sensitive, wide char
    typedef basic_ptree<wchar_t, iptree_traits<wchar_t> > wiptree;  // case insensitive, wide char
#endif

} }

#endif
