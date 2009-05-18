// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_PTREE_FWD_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_PTREE_FWD_HPP_INCLUDED

#include <boost/config.hpp>
#include <functional>           // for std::less
#include <memory>               // for std::allocator

namespace boost { namespace property_tree
{
    namespace detail {
        template <typename T> struct less_nocase;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Classes

    template <class Key, class Data,
              class KeyCompare = std::less<Key>,
              class Allocator = std::allocator<Data>>
    class basic_ptree;

    struct id_translator;

    template <typename String, typename Translator>
    class string_path;

    // We'll want to do this with concepts in C++0x.
#if 0
    concept PropertyTreePath<Path> {
      // The key type for which this path works.
      typedef key_type;
      // Return the key and index that the first segment of the path name.
      // Split the head off the state.
      std::pair<key_type, std::size_t> Path::reduce();

      // Return true if the path is empty.
      bool Path::empty();

      // Return true if the path contains a single element.
      bool Path::single();
    }
    concept PropertyTreeKey<Key> {
        PropertyTreePath path;
        requires SameType<Key, PropertyTreePath<path>::key_type>;
    }
#endif
    /// If you want to use a custom key type, specialize this struct for it
    /// and give it a 'type' typedef that specifies your path type. The path
    /// type must conform to the Path concept described in the documentation.
    /// This is already specialized for std::basic_string.
    template <typename Key>
    struct path_of;

    /// Specialize this struct to specify a default translator between the data
    /// in a tree whose data_type is TreeData, and the external data_type
    /// specified in a get_value, get, put_value or put operation.
    /// This is already specialized for TreeData being std::basic_string.
    template <typename TreeData, typename External>
    struct translator_between;

    class ptree_error;
    class ptree_bad_data;
    class ptree_bad_path;

    ///////////////////////////////////////////////////////////////////////////
    // Typedefs

    /** Implements a path using a std::string as the key. */
    typedef string_path<std::string, id_translator> path;

    /** Implements a path using a std::wstring as the key. */
    typedef string_path<std::wstring, id_translator> wpath;

    /**
     * A property tree with std::string for key and data, and default
     * comparison.
     */
    typedef basic_ptree<std::string, std::string> ptree;

    /**
     * A property tree with std::string for key and data, and case-insensitive
     * comparison.
     */
    typedef basic_ptree<std::string, std::string,
                        detail::less_nocase<std::string> >
        iptree;

#ifndef BOOST_NO_CWCHAR
    /**
     * A property tree with std::wstring for key and data, and default
     * comparison.
     * @note The type only exists if the platform supports @c wchar_t.
     */
    typedef basic_ptree<std::wstring, std::wstring> wptree;

    /**
     * A property tree with std::wstring for key and data, and case-insensitive
     * comparison.
     * @note The type only exists if the platform supports @c wchar_t.
     */
    typedef basic_ptree<std::wstring, std::wstring,
                        detail::less_nocase<std::wstring> >
        wiptree;
#endif

    ///////////////////////////////////////////////////////////////////////////
    // Free functions

    /**
     * Swap two property tree instances.
     */
    template<class K, class D, class C, class A>
    void swap(basic_ptree<K, D, C, A> &pt1,
              basic_ptree<K, D, C, A> &pt2);

    /**
     * Reference to empty property tree. Can be used as a default value of
     * get_child. See empty_ptree_trick.cpp for example of usage.
     */
    template<class Ptree> const Ptree &empty_ptree();

    /** Join two string_path objects. */
    template <typename String, typename Translator>
    string_path<String, Translator> operator /(
                                  const string_path<String, Translator> &p1,
                                  const string_path<String, Translator> &p2);

} }

#endif
