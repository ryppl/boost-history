#ifndef DBH_2001_10_31_MAP_HPP
#define DBH_2001_10_31_MAP_HPP
//----------------------------------------------------------------------------
// Copyright (c) 1994
// Hewlett-Packard Company
//
// Copyright (c) 1996,1997
// Silicon Graphics Computer Systems, Inc.
//
// Copyright (c) 1997
// Moscow Center for SPARC Technology
//
// Copyright (c) 1999
// Boris Fomitchev
//
// Copyright (c) 2001-2002
// David B. Held
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
//----------------------------------------------------------------------------
#include <utility>                      // std::pair<>
#include <memory>                       // std::allocator<>
#include <functional>                   // std::less<>, std::binary_function<>
#include <algorithm>                    // std::lexicographical_compare()
#include <boost/detail/tree.hpp>        // boost::rb_tree<>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <typename Key, typename Value>
    struct StdKeyOfValue
    {
    public:             // Types
        typedef Key                                 key_type;
        typedef Value                               mapped_type;
        typedef std::pair<const Key, Value>         value_type;
    public:             // Accessors
        Key const&      operator()(value_type const& value) const
        {
            return value.first;
        }
        mapped_type&    value(value_type& value) const
        {
            return value.second;
        }
    public:             // Structors
        value_type      make_value(key_type const& key,
                            mapped_type const& value) const
        {
            return value_type(key, value);
        }
    };
    //------------------------------------------------------------------------
    template <typename Key, typename Value>
    struct CustomKeyOfValue
    {
    public:             // Types
        typedef Key                                 key_type;
        typedef Value                               mapped_type;
        typedef Value                               value_type;
    public:             // Accessors
        mapped_type&    value(value_type& value) const
        {
            return value;
        }
    public:             // Structors
        value_type      make_value(key_type const& key,
                            mapped_type const& value) const
        {
            return value;
        }
    };
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy = UniqueKey,
              class KeyOfValue = StdKeyOfValue<Key, Value>,
              class NodeType = NonIndexedNode, class Compare = std::less<Key>,
              class Alloc = std::allocator<Value> >
    class map
    {
    public:             // Types
        typedef typename KeyOfValue::key_type       key_type;
        typedef typename KeyOfValue::mapped_type    mapped_type;
        typedef typename KeyOfValue::value_type     value_type;
        typedef KeyOfValue                          key_of_value;
        typedef Compare                             key_compare;

        class value_compare
            : public std::binary_function<value_type, value_type, bool>
        {
        private:            // Types
            friend class map<
                Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc
            >;
        private:            // Structors
                            value_compare(Compare compare)
                                : compare_(compare) { }
        public:             // Interface
            bool            operator()(value_type const& lhs, value_type const& rhs) const
            {
                return compare_(KeyOfValue()(lhs), KeyOfValue()(rhs));
            }
        private:            // Implementation
            Compare         compare_;
        };
    private:            // Types
        typedef typename Alloc::template rebind<value_type>::other
                                                    value_alloc;
        typedef detail::rb_tree<
            key_type, value_type, KeyPolicy,
            KeyOfValue, NodeType, Compare, value_alloc
        >                                           map_type;
    public:             // Dependent Types
        typedef typename map_type::pointer          pointer;
        typedef typename map_type::const_pointer    const_pointer;
        typedef typename map_type::reference        reference;
        typedef typename map_type::const_reference  const_reference;
        typedef typename map_type::iterator         iterator;
        typedef typename map_type::const_iterator   const_iterator;
        typedef typename map_type::reverse_iterator reverse_iterator;
        typedef typename map_type::const_reverse_iterator
                                                    const_reverse_iterator;
        typedef typename map_type::size_type        size_type;
        typedef typename map_type::difference_type  difference_type;
        typedef typename map_type::allocator_type   allocator_type;
    public:             // Structors
        explicit        map(Compare const& compare = Compare())
                            : map_(compare)         { }
                        template <class InputIterator>
                        map(InputIterator first, InputIterator last,
                            Compare const& compare = Compare(),
                            allocator_type const& allocator = allocator_type())
                            : map_(compare, allocator)
                                                    { map_.insert(first, last); }
                        map(map const& that) : map_(that.map_)
                                                    { }
    public:             // Swap/Assignment
        void            swap(map& that)             { map_.swap(that.map_); }
        map&            operator=(map const& that)
        {
            map(that).swap(*this);
            return *this;
        }
    public:             // Accessors
        key_compare     key_comp(void) const        { return map_.key_comp(); }
        value_compare   value_comp(void) const      { return value_compare(map_.key_comp()); }
        allocator_type  get_allocator(void) const   { return map_.get_allocator(); }

        iterator        begin(void)                 { return map_.begin(); }
        const_iterator  begin(void) const           { return map_.begin(); }
        iterator        end(void)                   { return map_.end(); }
        const_iterator  end(void) const             { return map_.end(); }
        reverse_iterator
                        rbegin(void)                { return map_.rbegin(); }
        const_reverse_iterator
                        rbegin(void) const          { return map_.rbegin(); }
        reverse_iterator
                        rend(void)                  { return map_.rend(); }
        const_reverse_iterator
                        rend(void) const            { return map_.rend(); }

        bool            empty(void) const           { return map_.empty(); }
        size_type       size(void) const            { return map_.size(); }
        size_type       max_size(void) const        { return map_.max_size(); }

        mapped_type&    operator[](key_type const& key)
        {
            iterator i(lower_bound(key));
            if (i == map_.end() || key_comp()(key, key_of_value()(*i)))
            {
                // key < *i
                i = insert(i, key_of_value().make_value(key, mapped_type()));
            }

            return key_of_value().value(*i);
        }

        bool            valid(void) const           { return map_.valid(); }
        template <class OStream>
        void            dump(OStream& stream) const { map_.dump(stream); }
    public:             // Insert/Erase
        std::pair<iterator, bool>
                        insert(value_type const& value)
                                                    { return map_.insert(value); }
        iterator        insert(iterator position, value_type const& value)
                                                    { return map_.insert(position, value); }
        template <class InputIterator>
        void            insert(InputIterator first, InputIterator last)
                                                    { map_.insert(first, last); }

        void            erase(iterator position)    { map_.erase(position); }
        size_type       erase(key_type const& key)  { return map_.erase(key); }
        void            erase(iterator first, iterator last)
                                                    { map_.erase(first, last); }
        void            clear(void)                 { map_.clear(); }
    public:             // Map Operations
        iterator        find(key_type const& key)
        {
            return map_.find(key);
        }
        const_iterator  find(key_type const& key) const
        {
            return map_.find(key);
        }
        size_type       count(key_type const& key) const
        {
            return map_.find(key) == map_.end() ? 0 : 1;
        }
        iterator        lower_bound(key_type const& key)
        {
            return map_.lower_bound(key);
        }
        const_iterator  lower_bound(key_type const& key) const
        {
            return map_.lower_bound(key);
        }
        iterator        upper_bound(key_type const& key)
        {
            return map_.upper_bound(key);
        }
        const_iterator  upper_bound(key_type const& key) const
        {
            return map_.upper_bound(key);
        }
        std::pair<iterator, iterator>
                        equal_range(key_type const& key) {
            return map_.equal_range(key);
        }
        std::pair<const_iterator, const_iterator>
                        equal_range(key_type const& key) const
        {
            return map_.equal_range(key);
        }
    public:             // Indexed Map Operations
        iterator        at(size_type index)
        {
            return map_.at(index);
        }
        const_iterator  at(size_type index) const
        {
            return map_.at(index);
        }
        // Throws std::domain_error if key not found
        size_type       pos(key_type const& key) const
        {
            return map_.pos(key);
        }
    private:            // Implementation
        map_type        map_;
    };
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline bool         operator==(
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& lhs,
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& rhs)
    {
        return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
    }
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline bool         operator!=(
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& lhs,
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& rhs)
    {
        return !(lhs == rhs);
    }
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline bool         operator<(
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& lhs,
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& rhs)
    {
        return std::lexicographical_compare(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
        );
    }
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline bool         operator>(
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& lhs,
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& rhs)
    {
        return rhs < lhs;
    }
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline bool         operator<=(
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& lhs,
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& rhs)
    {
        return !(rhs < lhs);
    }
    //------------------------------------------------------------------------
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline bool         operator>=(
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& lhs,
        map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc> const& rhs)
    {
        return !(lhs < rhs);
    }
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
namespace std
{
    // This overload violates the standard, but is useful anyway.
    template <typename Key, typename Value, class KeyPolicy, class KeyOfValue,
              class NodeType, class Compare, class Alloc>
    inline void swap(
        boost::map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc>& lhs,
        boost::map<Key, Value, KeyPolicy, KeyOfValue, NodeType, Compare, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}   // namespace std
//----------------------------------------------------------------------------
#endif //DBH_2001_10_31_MAP_HPP

