#ifndef BOOST_DBH_2002_07_19_TREE_HPP
#define BOOST_DBH_2002_07_19_TREE_HPP
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
// Red-black tree class, designed for use in implementing STL
// associative containers (set, multiset, map, and multimap). The
// insertion and deletion algorithms are based on those in Cormen,
// Leiserson, and Rivest, Introduction to Algorithms (MIT Press, 1990),
// except that
//
// (1) the header cell is maintained with links not only to the root
// but also to the leftmost node of the tree, to enable constant time
// begin(), and to the rightmost node of the tree, to enable linear time
// performance when used with the generic set algorithms (set_union,
// etc.);
//
// (2) when a node being deleted has two children its successor node is
// relinked into its place, rather than copied, so that the only
// iterators invalidated are those referring to the deleted node.
//----------------------------------------------------------------------------
#include <memory>                       // ???
#include <utility>                      // std::pair<>
#include <iterator>                     // std::reverse_iterator<>,
                                        //   std::bidirectional_iterator_tag,
                                        //   std::distance()
#include <algorithm>                    // std::swap(), std::equal(),
                                        //   std::lexicographical_compare()
#include <stdexcept>                    // std::domain_error
#include <cstddef>                      // std::size_t, std::ptrdiff_t
#include <cassert>                      // assert()
//----------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma warn -8027
#endif
//----------------------------------------------------------------------------
// Helper macros
#define TEMPLATE_PARAMETERS                                                  \
    template <                                                               \
        typename Key, typename Value, class KeyPolicy, class KeyOfValue,     \
        class NodePolicy, class Compare, class Alloc                         \
    >
#define RB_TREE                                                              \
    rb_tree<Key, Value, KeyPolicy, KeyOfValue, NodePolicy, Compare, Alloc>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    namespace detail
    {
        //--------------------------------------------------------------------
        // Non-templated base type reduces code bloat
        struct node_base
        {
        public:             // Types
            enum color_type { red, black };
        public:             // Structors
                            // We set color to red to distinguish header
                            // from root in decrement()
                            node_base(node_base* header)
                                : color_(red), parent_(0), left_(header),
                                right_(header)          { }

                            node_base(color_type color, node_base* parent)
                                : color_(color), parent_(parent), left_(0),
                                right_(0)               { }

            virtual        ~node_base(void)             { }
        public:             // Accessors
            node_base*      minimum(void)
            {
                node_base* n(this);
                while (n->left_ != 0) n = n->left_;
                return n;
            }
            node_base*      maximum(void)
            {
                node_base* n(this);
                while (n->right_ != 0) n = n->right_;
                return n;
            }

            int             black_count(node_base* header) const
            {
                int count = (color_ == black ? 1 : 0);
                if (this == header->parent_) return count;
                return count + parent_->black_count(header);
            }
        public:             // Interface
            node_base*      increment(void);
            node_base*      decrement(void);

            virtual void    rotate_left(node_base* header);
            virtual void    rotate_right(node_base* header);

            void            rebalance(node_base* header);
            node_base*      rebalance_for_erase(node_base* header);

            virtual void    update_insert(node_base*)   { }
            virtual void    update_erase(node_base*)    { }
        public:             // Implementation
            color_type      color_;
            node_base*      parent_;
            node_base*      left_;
            node_base*      right_;
        };
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
    struct NonIndexedNode
    {
        template <typename Value>
        struct impl : public detail::node_base
        {
        public:             // Types
            typedef Value value_type;
            typedef impl<Value> this_type;
            typedef detail::node_base base_type;
            typedef base_type::color_type color_type;
        public:             // Structors
                            impl(base_type* root)
                                : base_type(root)       { }

                            impl(color_type color, base_type* parent,
                                value_type const& value)
                                : base_type(color, parent), value_(value)
                                                        { }
        public:
            value_type      value_;
        };
    };
    //------------------------------------------------------------------------
    struct IndexedNode
    {
        template <typename Value>
        struct impl : public detail::node_base
        {
        public:             // Types
            typedef Value value_type;
            typedef impl<Value> this_type;
            typedef detail::node_base base_type;
            typedef base_type::color_type color_type;
        public:             // Structors
                            impl(base_type* root)
                                : base_type(root), index_(0)
                                                        { }

                            impl(color_type color, base_type* parent,
                                value_type const& value)
                                : base_type(color, parent), value_(value),
                                index_(0)               { }

        public:             // Methods
            virtual void    rotate_left(base_type* header)
            {
                base_type::rotate_left(header);
                static_cast<this_type*>(parent_)->index_ += index_ + 1;
            }
            virtual void    rotate_right(base_type* header)
            {
                base_type::rotate_right(header);
                index_ -= static_cast<this_type*>(parent_)->index_ + 1;
            }

            virtual void    update_insert(base_type* root)
            {
                base_type* p(parent_);
                for (base_type* n(this); n != root; n = p, p = p->parent_)
                {
                    if (n == p->left_) ++static_cast<this_type*>(p)->index_;
                }
            }
            virtual void    update_erase(base_type* root)
            {
                base_type* p(parent_);
                for (base_type* n(this); n != root; n = p, p = p->parent_)
                {
                    if (n == p->left_) --static_cast<this_type*>(p)->index_;
                }
            }
        public:             // Implementation
            value_type      value_;
            std::size_t     index_;
        };
    };
    //------------------------------------------------------------------------
    struct UniqueKey
    {
        template <class Tree>
        struct impl
        {
            typedef Tree                                tree_type;
            typedef typename Tree::value_type           value_type;
            typedef typename Tree::key_of_value         key_of_value;
            typedef typename Tree::key_compare          key_compare;
            typedef typename Tree::node_type            node_type;
            typedef typename detail::node_base          node_base;

            template <typename Insert>
            std::pair<node_type*, bool>
                            insert(value_type const& value, tree_type* tree,
                                node_base* header, Insert insert);
            template <typename Insert>
            node_type*      insert(node_type* hint, value_type const& value,
                                tree_type* tree, node_base* header,
                                Insert insert);
        public:
            key_of_value    key_of_value_;
            key_compare     key_compare_;
        };
    };
    //------------------------------------------------------------------------
    template <class Tree>
    template <typename Insert>
    std::pair<typename Tree::node_type*, bool>
    UniqueKey::impl<Tree>::insert(value_type const& value, tree_type* tree,
        node_base* header, Insert insert)
    {
        node_base* node(header->parent_);
        node_base* parent(header);
        bool less(true);                    // value < node->value
        // Defaults to true for the special case when the tree is empty

        // Locate insertion point
        while (node != 0)
        {
            parent = node;
            less = key_compare_(key_of_value_(value), key_of_value_(
                static_cast<node_type*>(node)->value_
            ));
            node = less ? node->left_ : node->right_;
        }
        node_type* parent_node(static_cast<node_type*>(parent));
        // Validate insertion point
        if (less)
        {
            if (parent == header->left_ || key_compare_(
                key_of_value_(
                    static_cast<node_type*>(parent->decrement())->value_
                ), key_of_value_(value)
            ))
            {
                // Insert at beginning or parent->left
                return std::pair<node_type*, bool>(
                    (tree->*insert)(Tree::left, parent_node, value), true
                );
            }
        }
        else
        {
            if (key_compare_(
                key_of_value_(parent_node->value_), key_of_value_(value)
            ))
            {
                // Insert at parent->right
                return std::pair<node_type*, bool>(
                    (tree->*insert)(Tree::right, parent_node, value), true
                );
            }
        }
        // Value exists
        return std::pair<node_type*, bool>(parent_node, false);
    }
    //----------------------------------------------------------------------------
    // Modifications CRP 7/10/00 as noted to improve conformance and
    // efficiency.
    template <class Tree>
    template <typename Insert>
    typename Tree::node_type*
    UniqueKey::impl<Tree>::insert(node_type* hint, value_type const& value,
        tree_type* tree, node_base* header, Insert insert)
    {
        // If the container is empty, fall back on default insert()
        if (tree->size() == 0)
        {
            return this->insert(value, tree, header, insert).first;
        }
        // hint == begin()
        if (hint == static_cast<node_type*>(header->left_))
        {
            if (key_compare_(key_of_value_(value), key_of_value_(hint->value_)))
            {
                // value < hint->value
                return (tree->*insert)(Tree::left, hint, value);
            }
            else
            {
                // hint->value < value
                bool hint_less(key_compare_(
                    key_of_value_(hint->value_), key_of_value_(value))
                );

                // hint->value == value
                if (!hint_less)
                {
                    return hint;
                }

                // Check for only one member -- in that case, hint points to
                // itself, and attempting to increment will cause an infinite
                // loop
                // hint == root
                if (static_cast<node_type*>(hint->left_) == hint)
                {
                    // Check guarantees exactly one member, so comparison was
                    // already performed and we know the result
                    return (tree->*insert)(Tree::right, hint, value);
                }

                // All other cases:
                // Standard-conformance: does the insertion point fall
                // immediately AFTER the hint?
                node_type* after(static_cast<node_type*>(hint->increment()));

                // Optimization to catch insert-equivalent -- save comparison
                // results, and we get this for free.
                if (key_compare_(key_of_value_(value), key_of_value_(after->value_)))
                {
                    // hint->value < value < after->value
                    if (hint->right_ == 0)
                    {
                        return (tree->*insert)(Tree::right, hint, value);
                    }
                    else
                    {
                        return (tree->*insert)(Tree::left, after, value);
                    }
                }
            }
        }
        // hint == end()
        else if (hint == static_cast<node_type*>(header))
        {
            node_type* right_child(static_cast<node_type*>(header->right_));
            if (key_compare_(
                key_of_value_(right_child->value_), key_of_value_(value))
            )
            {
                // rightmost->value < value
                return (tree->*insert)(Tree::right, right_child, value);
            }
        }
        else
        {
            node_type* before(static_cast<node_type*>(hint->decrement()));
            // value < hint->value
            bool value_less(key_compare_(
                key_of_value_(value), key_of_value_(hint->value_)
            ));

            if (value_less && key_compare_(
                key_of_value_(before->value_), key_of_value_(value)
            ))
            {
                // before->value < value < hint->value
                if (before->right_ == 0)
                {
                    return (tree->*insert)(Tree::right, before, value);
                }
                else
                {
                    return (tree->*insert)(Tree::left, hint, value);
                }
            }
            else
            {
                // Does the insertion point fall immediately AFTER the hint?
                node_type* after(static_cast<node_type*>(hint->increment()));

                // Optimization to catch equivalent cases and avoid unnecessary
                // comparisons
                bool hint_less(value_less ? false : key_compare_(
                    key_of_value_(hint->value_), key_of_value_(value)
                ));                     // Stored this result earlier
                // If the earlier comparison was true, this comparison doesn't
                // need to be performed because it must be false.  However, if
                // the earlier comparison was false, we need to perform this one
                // because in the equal case, both will be false.

                // value_less true implies hint_less false
                if (!value_less && hint_less && (after == header ||
                    key_compare_(key_of_value_(value), key_of_value_(after->value_))))
                {
                    // hint->value < value < after->value
                    if (hint->right_ == 0)
                    {
                        return (tree->*insert)(Tree::right, hint, value);
                    }
                    else
                    {
                        return (tree->*insert)(Tree::left, after, value);
                    }
                }
                else
                {
                    // Test for equivalent case
                    if (value_less == hint_less)
                    {
                        return hint;
                    }
                }
            }
        }
        return this->insert(value, tree, header, insert).first;
    }
    //------------------------------------------------------------------------
    struct NonUniqueKey
    {
        template <class Tree>
        struct impl
        {
            typedef Tree                                tree_type;
            typedef typename Tree::value_type           value_type;
            typedef typename Tree::key_of_value         key_of_value;
            typedef typename Tree::key_compare          key_compare;
            typedef typename Tree::node_type            node_type;
            typedef typename detail::node_base          node_base;

            template <typename Insert>
            std::pair<node_type*, bool>
                            insert(value_type const& value, tree_type* tree,
                                node_base* header, Insert insert);
            template <typename Insert>
            node_type*      insert(node_type* hint, value_type const& value,
                                tree_type* tree, node_base* header,
                                Insert insert);
        public:
            key_of_value    key_of_value_;
            key_compare     key_compare_;
        };
    };
    //----------------------------------------------------------------------------
    template <class Tree>
    template <typename Insert>
    std::pair<typename Tree::node_type*, bool>
    NonUniqueKey::impl<Tree>::insert(value_type const& value, tree_type* tree,
        node_base* header, Insert insert)
    {
        node_base* node(header->parent_);
        node_base* parent(header);

        while (node != 0)
        {
            parent = node;
            node = key_compare_(
                key_of_value_(value),
                key_of_value_(static_cast<node_type*>(node)->value_)
            ) ? node->left_ : node->right_;
        }
        return (tree->*insert)(
            Tree::compare, static_cast<node_type*>(parent), value
        );
    }
    //----------------------------------------------------------------------------
    template <class Tree>
    template <typename Insert>
    typename Tree::node_type*
    NonUniqueKey::impl<Tree>::insert(node_type* hint, value_type const& value,
        tree_type* tree, node_base* header, Insert insert)
    {
        // If the container is empty, fall back on default insert()
        if (tree->size() == 0)
        {
            return insert(value, tree, header);
        }
        // hint == begin()
        if (hint == static_cast<node_type*>(header->left_))
        {
            if (!key_compare_(key_of_value_(hint->value_), key_of_value_(value)))
            {
                // value <= hint->value
                return (tree->*insert)(Tree::left, hint, value);
            }
            else
            {
                // hint->value < value
                // Check for only one member
                if (static_cast<node_type*>(hint->left_) == hint)
                {
                    return (tree->*insert)(Tree::right, hint, value);
                }

                // All other cases:
                // Standard-conformance - does the insertion point fall immediately
                // AFTER the hint?
                node_type* after(static_cast<node_type*>(hint->increment()));

                // We want to know if value <= after; if not, invalid hint
                if (after == static_cast<node_type*>(header) || !key_compare_(
                    key_of_value_(after->value_), key_of_value_(value))
                )
                {
                    if (hint->right_ == 0)
                    {
                        return (tree->*insert)(Tree::right, hint, value);
                    }
                    else
                    {
                        return (tree->*insert)(Tree::left, after, value);
                    }
                }
            }
        }
        // hint == end()
        else if (hint == static_cast<node_type*>(header))
        {
            node_type* right_child(static_cast<node_type*>(header->right_));
            if (!key_compare_(
                key_of_value_(value), key_of_value_(right_child->value_))
            )
            {
                return (tree->*insert)(Tree::right, right_child, value);
            }
        }
        else
        {
            node_type* before(static_cast<node_type*>(hint->decrement()));
            // Store the result of the comparison between hint and value so that
            // we don't have to do it again later.  Note that this reverses the
            // shortcut on the if, possibly harming efficiency in comparisons;
            // I think the harm will be negligible, and to do what I want to do
            // (save the result of a comparison so that it can be re-used) there
            // is no alternative.  Test here is for before <= value <= hint.
            bool hint_less(key_compare_(
                key_of_value_(hint->value_), key_of_value_(value)
            ));
            if (!hint_less && !key_compare_(
                key_of_value_(value), key_of_value_(before->value_)
            ))
            {
                if (before->right_ == 0)
                {
                    return (tree->*insert)(Tree::right, before, value);
                }
                else
                {
                    return (tree->*insert)(Tree::left, hint, value);
                }
            }
            else
            {
                // Does the insertion point fall immediately AFTER the hint?
                // Test for hint < value <= after
                node_type* after(static_cast<node_type*>(hint->increment()));

                if (hint_less && (after == header || !key_compare_(
                    key_of_value_(after->value_), key_of_value_(value)))
                )
                {
                    if (hint->right_ == 0)
                    {
                        return (tree->*insert)(Tree::right, hint, value);
                    }
                    else
                    {
                        return (tree->*insert)(Tree::left, after, value);
                    }
                }
            }
        }
        return insert(value, tree, header);
    }
    //------------------------------------------------------------------------
    namespace detail
    {
        //--------------------------------------------------------------------
        template <typename Key, typename Value, class KeyPolicy,
            class KeyOfValue, class NodePolicy, class Compare, class Alloc
        >
        class rb_tree
        {
        public:             // Types
            typedef typename NodePolicy::template impl<Value>
                                                            node_type;
            //----------------------------------------------------------------
            struct nonconst_traits
            {
                typedef Value                               value_type;
                typedef Value*                              pointer;
                typedef Value&                              reference;
            };
            struct const_traits
            {
                typedef Value                               value_type;
                typedef Value const*                        pointer;
                typedef Value const&                        reference;
            };
            //----------------------------------------------------------------
            template <class Traits>
            struct rb_iterator
            {
            public:             // Types
                typedef std::bidirectional_iterator_tag     iterator_category;
                typedef std::ptrdiff_t                      difference_type;
                typedef typename Traits::value_type         value_type;
                typedef typename Traits::pointer            pointer;
                typedef typename Traits::reference          reference;
            public:             // Structors
                                rb_iterator(void)
                                    : node_(0)              { }
                                rb_iterator(node_type* n)
                                    : node_(n)              { }
                                rb_iterator(
                                    rb_iterator<nonconst_traits> const& i)
                                    : node_(i.node_)        { }
            public:             // Operators
                // bcc doesn't like friend functions here
                bool            operator==(rb_iterator const& rhs) const
                {
                    return node_ == rhs.node_;
                }
                bool            operator!=(rb_iterator const& rhs) const
                {
                    return node_ != rhs.node_;
                }

                reference       operator*()                 { return node_->value_; }
                pointer         operator->()                { return &(operator*()); }

                rb_iterator&    operator++()
                {
                    node_ = static_cast<node_type*>(node_->increment());
                    return *this;
                }
                rb_iterator     operator++(int)
                {
                    rb_iterator tmp(*this);
                    node_ = static_cast<node_type*>(node_->increment());
                    return tmp;
                }
                rb_iterator&    operator--()
                {
                    node_ = static_cast<node_type*>(node_->decrement());
                    return *this;
                }
                rb_iterator     operator--(int)
                {
                    rb_iterator tmp(*this);
                    node_ = static_cast<node_type*>(node_->decrement());
                    return tmp;
                }
            public:             // Implementation
                node_type*      node_;
            };
            //----------------------------------------------------------------
            enum hint_type { left, right, compare };
            // bcc 5.5 needs this typedef for key_policy
            typedef rb_tree<
                Key, Value, KeyPolicy, KeyOfValue, NodePolicy, Compare, Alloc
            >                                               this_type;
            // Template parameter types
            typedef Key                                     key_type;
            typedef Value                                   value_type;
            typedef KeyOfValue                              key_of_value;
            typedef Compare                                 key_compare;
            typedef typename KeyPolicy::template impl<this_type>
                                                            key_policy;
            typedef Alloc                                   allocator_type;

            // Iterator types
            typedef rb_iterator<nonconst_traits>            iterator;
            typedef rb_iterator<const_traits>               const_iterator;
            typedef std::reverse_iterator<iterator>         reverse_iterator;
            typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

            // Reference/pointer types
            typedef value_type&                             reference;
            typedef value_type const&                       const_reference;
            typedef value_type*                             pointer;
            typedef value_type const*                       const_pointer;
            typedef typename iterator::difference_type      difference_type;
            typedef std::size_t                             size_type;

            typedef typename node_type::color_type          color_type;
            typedef typename Alloc::template rebind<node_base>::other
                                                            header_alloc;
            typedef typename Alloc::template rebind<node_type>::other
                                                            node_alloc;
        public:             // Structors
                            rb_tree(void)
                                : header_(header_alloc().allocate(1))
                                , node_count_(0), key_of_value_(key_of_value())
                                , key_compare_(key_compare())
            {
                header_alloc().construct(header_, node_base(header_));
            }

                            rb_tree(Compare const& key_compare)
                                : header_(header_alloc().allocate(1))
                                , node_count_(0), key_of_value_(key_of_value())
                                , key_compare_(key_compare)
            {
                header_alloc().construct(header_, node_base(header_));
            }

                            rb_tree(this_type const& that)
                                : header_(header_alloc().allocate(1))
                                , node_count_(that.node_count_)
                                , key_of_value_(that.key_of_value_)
                                , key_compare_(that.key_compare_)
           {
               header_alloc().construct(header_, node_base(header_));
               if (that.root() != 0)
               {
                   header_->parent_ = copy(that.root(), static_cast<node_type*>(header_));
                   header_->left_ = root()->minimum();
                   header_->right_ = root()->maximum();
               }
           }
                           ~rb_tree(void)
           {
               clear();
               header_alloc().destroy(header_);
               header_alloc().deallocate(header_, 1);
           }
        public:             // Swap/Assignment
            void            swap(this_type& that)
            {
                using std::swap;
                swap(header_, that.header_);
                swap(node_count_, that.node_count_);
                swap(key_of_value_, that.key_of_value_);
                swap(key_compare_, that.key_compare_);
            }
            this_type&      operator=(this_type const& t);
        public:             // Accessors
            iterator        begin(void)                 { return iterator(leftmost()); }
            const_iterator  begin(void) const           { return const_iterator(leftmost()); }
            iterator        end(void)                   { return iterator(static_cast<node_type*>(header_)); }
            const_iterator  end(void) const             { return const_iterator(static_cast<node_type*>(header_)); }

            reverse_iterator
                            rbegin(void)                { return reverse_iterator(end()); }
            const_reverse_iterator
                            rbegin(void) const          { return const_reverse_iterator(end()); }
            reverse_iterator
                            rend(void)                  { return reverse_iterator(begin()); }
            const_reverse_iterator
                            rend(void) const            { return const_reverse_iterator(begin()); }

            bool            empty(void) const           { return node_count_ == 0; }
            size_type       size(void) const            { return node_count_; }
            size_type       max_size(void) const        { return size_type(-1); }
            key_compare     key_comp(void) const        { return key_compare_; }

            bool            valid(void) const;
            template <class OStream>
            void            dump(OStream& stream) const;
        public:             // Insert/Erase
            std::pair<iterator, bool>
                            insert(value_type const& value)
            {
                std::pair<node_type*, bool> result(key_policy().insert(
                    value, this, header_,
                    &rb_tree::insert_
                ));
                return std::pair<iterator, bool>(
                    iterator(result.first), result.second
                );
            }
            iterator        insert(iterator position, value_type const& value)
            {
                return iterator(key_policy().insert(
                    static_cast<node_type*>(position.node_), value, this,
                    header_, &rb_tree::insert_
                ));
            }
            template <class InsertIterator>
            void            insert(InsertIterator first, InsertIterator last)
            {
                while (first != last)
                {
                    key_policy().insert(
                        *first++, this, header_, &rb_tree::insert_
                    );
                }
            }

            void            erase(iterator position)
            {
                node_type* node(static_cast<node_type*>(
                    position.node_->rebalance_for_erase(header_)
                ));
                node_alloc().destroy(node);
                node_alloc().deallocate(node, 1);
                --node_count_;
            }
            size_type       erase(key_type const& key)
            {
                std::pair<iterator, iterator> range(equal_range(key));
                size_type count(std::distance(range.first, range.second));
                erase(range.first, range.second);
                return count;
            }
            void            erase(iterator first, iterator last)
            {
                if (first == begin() && last == end())
                {
                    clear();
                }
                else
                {
                    while (first != last) erase(first++);
                }
            }
            void            erase(key_type const* first, key_type const* last)
            {
                while (first != last) erase(*first++);
            }
        public:             // Map Operations
            iterator        find(key_type const& key)
            {
                return iterator(find_(key));
            }
            const_iterator  find(key_type const& key) const
            {
                return const_iterator(
                    const_cast<rb_tree*>(this)->find_(key)
                );
            }
            size_type       count(key_type const& key) const
            {
                std::pair<const_iterator, const_iterator> range(equal_range(key));
                return std::distance(range.first, range.second);
            }
            iterator        lower_bound(key_type const& key)
            {
                return iterator(lower_bound_(key));
            }
            const_iterator  lower_bound(key_type const& key) const
            {
                return const_iterator(
                    const_cast<rb_tree*>(this)->lower_bound_(key)
                );
            }
            iterator        upper_bound(key_type const& key)
            {
                return iterator(upper_bound_(key));
            }
            const_iterator  upper_bound(key_type const& key) const
            {
                return const_iterator(
                    const_cast<rb_tree*>(this)->upper_bound_(key)
                );
            }
            std::pair<iterator, iterator>
                            equal_range(key_type const& key)
            {
                return std::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
            }
            std::pair<const_iterator, const_iterator>
                            equal_range(key_type const& key) const
            {
                return std::pair<const_iterator, const_iterator>(
                    lower_bound(key), upper_bound(key)
                );
            }
        public:             // Indexed Map Operations
            iterator        at(size_type index)
            {
                return iterator(at_(index));
            }
            const_iterator  at(size_type index) const
            {
                return const_iterator(
                    const_cast<rb_tree*>(this)->at_(index)
                );
            }
            // Throws std::domain_error if key not found
            size_type       pos(key_type const& key) const;
        private:            // Structor Helpers
            static
            node_type*      create_node(node_type* parent,
                                value_type const& value,
                                color_type color = node_type::red)
            {
                node_type* node(node_alloc().allocate(1));
                try
                {
                    node_alloc().construct(
                        node, node_type(color, parent, value)
                    );
                }
                catch (...)
                {
                    node_alloc().deallocate(node, 1);
                    throw;
                }
                return node;
            }
            static
            node_type*      clone_node(node_type const* node, node_type* parent)
            {
                return create_node(parent, node->value_, node->color_);
            }
            static
            node_type*      copy(node_type* node, node_type* parent);
            void            clear(void)
            {
                if (node_count_ > 0)
                {
                    erase(root());
                    header_->parent_ = 0;
                    header_->left_ = header_;
                    header_->right_ = header_;
                    node_count_ = 0;
                }
            }
            static
            void            erase(node_type* node);
        private:            // Accessors
            node_type*      root(void) const            { return static_cast<node_type*>(header_->parent_); }
            node_type*      leftmost(void) const        { return static_cast<node_type*>(header_->left_); }
            node_type*      rightmost(void) const       { return static_cast<node_type*>(header_->right_); }
        private:            // Insert
            node_type*      insert_(hint_type hint, node_type* parent,
                                value_type const& value);
        private:            // Map Operations
            node_type*      find_(key_type const& key);
            node_type*      lower_bound_(key_type const& key);
            node_type*      upper_bound_(key_type const& key);
        private:            // Indexed Map Operations
            node_type*      at_(size_type index);
        private:            // Implementation
            node_base*      header_;
            size_type       node_count_;
            key_of_value    key_of_value_;
            key_compare     key_compare_;
        };
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        RB_TREE& RB_TREE::operator=(RB_TREE const& that)
        {
            if (&that == this) return *this;
            rb_tree(that).swap(*this);
            return *this;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        bool RB_TREE::valid(void) const
        {
            if (node_count_ == 0 || begin() == end())
            {
                return node_count_ == 0 && begin() == end() &&
                    header_->left_ == header_ && header_->right_ == header_;
            }
            if (leftmost() != root()->minimum()) return false;
            if (rightmost() != root()->maximum()) return false;

            int len(leftmost()->black_count(header_));
            for (const_iterator i(begin()); i != end(); ++i)
            {
                node_type* node(i.node_);
                node_type* left(static_cast<node_type*>(node->left_));
                node_type* right(static_cast<node_type*>(node->right_));

                if (node->color_ == node_type::red)
                {
                    if (left && left->color_ == node_type::red) return false;
                    if (right && right->color_ == node_type::red) return false;
                }
                if (left && key_compare_(
                    key_of_value_(node->value_), key_of_value_(left->value_)
                )) return false;
                if (right && key_compare_(
                    key_of_value_(right->value_), key_of_value_(node->value_)
                )) return false;
                if (!left && !right && node->black_count(header_) != len) return false;
            }
            return true;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        template <class OStream>
        void RB_TREE::dump(OStream& stream) const
        {
            stream
                << "size: " << node_count_ << "\n"
                << "header: " << (header_->color_ == node_type::red ? "red" : "black") << "\n"
                << "  root: ";
            if (header_->parent_ == 0)
                stream << "[null]\n";
            else
                stream << key_of_value_(
                    static_cast<node_type*>(header_->parent_)->value_) << "\n";
            stream << "  left: ";
            if (header_->left_ == header_)
                stream << "[null]\n";
            else
                stream << key_of_value_(
                    static_cast<node_type*>(header_->left_)->value_) << "\n";
            stream << "  right: ";
            if (header_->right_ == header_)
                stream << "[null]\n";
            else
                stream << key_of_value_(
                    static_cast<node_type*>(header_->right_)->value_) << "\n";
            for (node_type* node(static_cast<node_type*>(header_->left_));
                 node != header_;
                 node = static_cast<node_type*>(node->increment())
                )
            {
                stream
                    << "node [" << key_of_value_(node->value_) << "]: "
                        << (node->color_ == node_type::red ? "red" : "black") << "\n";
                stream << "  parent: ";
                if (node->parent_ == header_)
                    stream << "[header]\n";
                else
                    stream << key_of_value_(
                        static_cast<node_type*>(node->parent_)->value_) << "\n";
                stream << "  left: ";
                if (node->left_ == 0)
                    stream << "[null]\n";
                else
                    stream << key_of_value_(
                        static_cast<node_type*>(node->left_)->value_) << "\n";
                stream << "  right: ";
                if (node->right_ == 0)
                    stream << "[null]\n";
                else
                    stream << key_of_value_(
                        static_cast<node_type*>(node->right_)->value_) << "\n";
            }
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        typename RB_TREE::size_type RB_TREE::pos(key_type const& key) const
        {
            node_type* node(root());
            size_type base(0);
            while (node != 0)
            {
                if (key_compare_(key, key_of_value_(node->value_)))
                {
                    // key < node
                    node = static_cast<node_type*>(node->left_);
                }
                else
                {
                    // node <= key
                    if (key_compare_(key_of_value_(node->value_), key))
                    {
                        // node < key
                        base += node->index_ + 1;
                        node = static_cast<node_type*>(node->right_);
                    }
                    else
                    {
                        // key == node
                        return base + node->index_;
                    }
                }
            }
            throw std::domain_error("Key not found");
        }
        //--------------------------------------------------------------------
        // Deep copy.  node and parent must be non-null.
        TEMPLATE_PARAMETERS
        typename RB_TREE::node_type* RB_TREE::copy(
            RB_TREE::node_type* node, RB_TREE::node_type* parent
        )
        {
            node_type* top(clone_node(node, parent));
            try
            {
                if (node->right_)
                {
                    top->right_ = copy(static_cast<node_type*>(node->right_), top);
                }
                parent = top;
                node = static_cast<node_type*>(node->left_);
                while (node != 0)
                {
                    node_type* clone(clone_node(node, parent));
                    parent->left_ = clone;
                    if (node->right_)
                        clone->right_ = copy(static_cast<node_type*>(node->right_), clone);
                    parent = clone;
                    node = static_cast<node_type*>(node->left_);
                }
            }
            catch (...)
            {
                erase(top);
                throw;
            }
            return top;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        RB_TREE::node_type* RB_TREE::insert_(hint_type hint, node_type* parent,
            value_type const& value)
        {
            node_type* node(create_node(parent, value));

            if (parent == header_ || (hint != right && (hint == left ||
                key_compare_(key_of_value_(value), key_of_value_(parent->value_))
            )))
            {
                parent->left_ = node;           // Also makes leftmost() = node
                if (parent == header_)          //   when parent == header_
                {
                    header_->parent_ = node;
                    header_->right_ = node;
                }
                else if (parent == leftmost())
                {
                    header_->left_ = node;      // Maintain leftmost() pointing
                }                               //   to minimum node
            }
            else
            {
                parent->right_ = node;
                if (parent == rightmost())
                {
                    header_->right_ = node;     // Maintain rightmost() pointing
                }                               //   to maximum node
            }
            // Update indexes if necessary
            node->update_insert(static_cast<node_base*>(root()));
            node->rebalance(header_);
            ++node_count_;

            return node;
        }
        //--------------------------------------------------------------------
        // This has to stay out-of-line: it's recursive
        // erase without rebalancing
        TEMPLATE_PARAMETERS
        void RB_TREE::erase(node_type* node)
        {
            while (node != 0)
            {
                erase(static_cast<node_type*>(node->right_));
                node_type* child(static_cast<node_type*>(node->left_));
                node_alloc().destroy(node);
                node_alloc().deallocate(node, 1);
                node = child;
            }
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        typename RB_TREE::node_type* RB_TREE::find_(key_type const& key)
        {
            node_type* node(root());
            node_type* parent(static_cast<node_type*>(header_));
            while (node != 0)
            {
                if (!key_compare_(key_of_value_(node->value_), key))
                {
                    parent = node;
                    node = static_cast<node_type*>(node->left_);
                }
                else
                {
                    node = static_cast<node_type*>(node->right_);
                }
            }
            if (parent != header_ &&
                key_compare_(key, key_of_value_(parent->value_)))
            {
                parent = static_cast<node_type*>(header_);
            }
            // Allow RVO
            return parent;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        typename RB_TREE::node_type* RB_TREE::lower_bound_(key_type const& key)
        {
            node_type* node(root());
            node_type* parent(static_cast<node_type*>(header_));
            while (node != 0)
            {
                if (!key_compare_(key_of_value_(node->value_), key))
                {
                    parent = node;
                    node = static_cast<node_type*>(node->left_);
                }
                else
                {
                    node = static_cast<node_type*>(node->right_);
                }
            }
            return parent;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        typename RB_TREE::node_type* RB_TREE::upper_bound_(key_type const& key)
        {
            node_type* node(root());
            node_type* parent(static_cast<node_type*>(header_));
            while (node != 0)
            {
                if (key_compare_(key, key_of_value_(node->value_)))
                {
                    parent = node;
                    node = static_cast<node_type*>(node->left_);
                }
                else
                {
                    node = static_cast<node_type*>(node->right_);
                }
            }
            return parent;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        typename RB_TREE::node_type* RB_TREE::at_(size_type index)
        {
            node_type* node(root());
            if (index < node_count_)
            {
                size_type base(0);
                for (size_type i(node->index_); index != i; i = base + node->index_)
                {
                    if (index < i)
                    {
                        node = static_cast<node_type*>(node->left_);
                    }
                    else
                    {
                        base += node->index_ + 1;
                        node = static_cast<node_type*>(node->right_);
                    }
                }
            }
            else
            {    // Return end()
                node = static_cast<node_type*>(header_);
            }
            return node;
        }
        //--------------------------------------------------------------------
        // Non-member relational operators
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        inline bool         operator==(RB_TREE const& lhs, RB_TREE const& rhs)
        {
            return lhs.size() == rhs.size()
                && std::equal(lhs.begin(), lhs.end(), rhs.begin());
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        inline bool         operator!=(RB_TREE const& lhs, RB_TREE const& rhs)
        {
            return !(lhs == rhs);
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        inline bool         operator<(RB_TREE const& lhs, RB_TREE const& rhs)
        {
            return std::lexicographical_compare(
                lhs.begin(), lhs.end(), rhs.begin(), rhs.end()
            );
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        inline bool         operator>(RB_TREE const& lhs, RB_TREE const& rhs)
        {
            return rhs < lhs;
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        inline bool         operator<=(RB_TREE const& lhs, RB_TREE const& rhs)
        {
            return !(rhs < lhs);
        }
        //--------------------------------------------------------------------
        TEMPLATE_PARAMETERS
        inline bool         operator>=(RB_TREE const& lhs, RB_TREE const& rhs)
        {
            return !(lhs < rhs);
        }
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
namespace std
{
    // This overload violates the standard, but is useful anyway.
    TEMPLATE_PARAMETERS
    inline void         swap(boost::detail::RB_TREE& lhs, boost::detail::RB_TREE& rhs)
    {
        lhs.swap(rhs);
    }
}   // namespace std
//----------------------------------------------------------------------------
// Clear helper macros
#undef RB_TREE
#undef TEMPLATE_PARAMETERS
//----------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma warn -8027
#endif
//----------------------------------------------------------------------------
#endif // BOOST_DBH_2002_07_19_TREE_HPP

