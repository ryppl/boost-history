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

    template<class Key> class basic_path;
    class translator;
    //template<class C, class K, class P, class D, class X> class basic_ptree;
    template <class Key, class Data,
              class KeyCompare = std::less<Key>,
              class Allocator = std::allocator<Data>,
              class Path = basic_path<Key>,
              class Translate = translator>
        class basic_ptree;

    class ptree_error;
    class ptree_bad_data;
    class ptree_bad_path;

    ///////////////////////////////////////////////////////////////////////////
    // Typedefs

    /** Implements a path using a std::string as the key. */
    typedef basic_path<std::string> path;

    /** Implements a path using a std::wstring as the key. */
    typedef basic_path<std::wstring> wpath;

    /**
     * A property tree that uses a path type based upon std::string.
     * Comparisons of keys are performed in a case-sensitive manner.
     */
    typedef basic_ptree<std::string, std::string> ptree;

    /**
     * A property tree that uses a path type based upon std::string.
     * Comparisons of keys are performed in a case-insensitive manner.
     */
    typedef basic_ptree<std::string, std::string,
                        detail::less_nocase<std::string> >
        iptree;

#ifndef BOOST_NO_CWCHAR
    /**
     * A property tree that uses a wide-character path type based upon std::wstring.
     * Comparisons of keys are performed in a case-sensitive manner.
     * @note The type only exists if the platform supports @c wchar_t.
     */
    typedef basic_ptree<std::wstring, std::wstring> wptree;

    /**
     * A property tree that uses a wide-character path type based upon std::wstring.
     * Comparisons of keys are performed in a case-insensitive manner.
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
     * @param pt1 Reference to first property tree involved in swap.
     * @param pt2 Reference to second property tree involved in swap.
     */
    template<class K, class D, class C, class A, class P, class X>
    void swap(basic_ptree<K, D, C, A, P, X> &pt1,
              basic_ptree<K, D, C, A, P, X> &pt2);

    /**
     * Reference to empty property tree. Can be used as a default value of
     * get_child. See empty_ptree_trick.cpp for example of usage.
     */
    template<class Ptree> const Ptree &empty_ptree();

    /** Join two path objects. */
    path operator /(const path &p1, const path &p2);

    /** Join two wide-path objects. */
    wpath operator /(const wpath &p1, const wpath &p2);

} }

#endif
