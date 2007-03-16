#ifndef BOOST_DBH_2001_10_31_TREE_CPP
#define BOOST_DBH_2001_10_31_TREE_CPP
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
// Modified CRP 7/10/00 for improved conformance/efficiency on insert_unique/
// insert_equal with valid hint -- efficiency is improved all around, and it
// should now be standard-conforming for complexity on insert point immediately
// after hint (amortized constant time).
//----------------------------------------------------------------------------
#include <boost/detail/tree.hpp>
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    namespace detail
    {
        //--------------------------------------------------------------------
        node_base* node_base::increment(void)
        {
            node_base* n(this);

            if (n->right_ != 0)
            {
                n = n->right_;
                while (n->left_ != 0) n = n->left_;
            }
            else
            {
                node_base* p(parent_);
                while (n == p->right_)
                {
                    n = p;
                    p = p->parent_;
                }
                if (n->right_ != p)
                {
                    n = p;
                }
            }

            return n;
        }
        //--------------------------------------------------------------------
        node_base* node_base::decrement(void)
        {
            node_base* n(this);

            if (n->color_ == red && n->parent_->parent_ == n)
            {
                n = n->right_;
            }
            else if (n->left_ != 0)
            {
                n = n->left_;
                while (n->right_ != 0) n = n->right_;
            }
            else
            {
                node_base* p(n->parent_);
                while (n == p->left_)
                {
                    n = p;
                    p = p->parent_;
                }
                n = p;
            }
            return n;
        }
        //--------------------------------------------------------------------
        void node_base::rotate_left(node_base* header)
        {
            node_base* child(right_);           // Right child

            right_ = child->left_;              // Transfer right subtree
            if (child->left_ != 0)
            {
                child->left_->parent_ = this;
            }
            child->parent_ = parent_;

            if (this == header->parent_)        // Update parent
            {
                header->parent_ = child;
            }
            else if (this == parent_->left_)
            {
                parent_->left_ = child;
            }
            else
            {
                parent_->right_ = child;
            }
            child->left_ = this;
            parent_ = child;
        }
        //--------------------------------------------------------------------
        void node_base::rotate_right(node_base* header)
        {
            node_base* child(left_);            // Left child

            left_ = child->right_;              // Transfer left subtree
            if (child->right_ != 0)
            {
                child->right_->parent_ = this;
            }
            child->parent_ = parent_;

            if (this == header->parent_)        // Update parent
            {
                header->parent_ = child;
            }
            else if (this == parent_->right_)
            {
                parent_->right_ = child;
            }
            else
            {
                parent_->left_ = child;
            }
            child->right_ = this;
            parent_ = child;
        }
        //--------------------------------------------------------------------
        void node_base::rebalance(node_base* header)
        {
            node_base*& root(header->parent_);
            node_base* node(this);

            node->color_ = red;
            while (node != root && node->parent_->color_ == red)
            {
                node_base*& parent(node->parent_);
                node_base*& grandparent(parent->parent_);
                if (parent == grandparent->left_)
                {
                    node_base* uncle(grandparent->right_);
                    if (uncle && uncle->color_ == red)
                    {
                        parent->color_ = black;
                        uncle->color_ = black;
                        grandparent->color_ = red;
                        node = grandparent;
                    }
                    else
                    {
                        if (node == parent->right_)
                        {
                            node = parent;
                            node->rotate_left(header);
                        }
                        // Because node may have changed, we can no longer use
                        // the parent and grandparent aliases above
                        node->parent_->color_ = black;
                        node->parent_->parent_->color_ = red;
                        node->parent_->parent_->rotate_right(header);
                    }
                }
                else
                {
                    node_base* aunt(grandparent->left_);
                    if (aunt && aunt->color_ == red)
                    {
                        parent->color_ = black;
                        aunt->color_ = black;
                        grandparent->color_ = red;
                        node = grandparent;
                    }
                    else
                    {
                        if (node == parent->left_)
                        {
                            node = parent;
                            node->rotate_right(header);
                        }
                        // Same rationale as above
                        node->parent_->color_ = black;
                        node->parent_->parent_->color_ = red;
                        node->parent_->parent_->rotate_left(header);
                    }
                }
            }
            root->color_ = black;
        }
        //--------------------------------------------------------------------
        node_base* node_base::rebalance_for_erase(node_base* header)
        {
            node_base*& root(header->parent_);
            node_base*& leftmost(header->left_);
            node_base*& rightmost(header->right_);

            node_base* x(0);
            node_base* x_parent(0);
            node_base* y(this);

            if (y->left_ == 0)              // z has at most one non-null child; y == z
            {
                x = y->right_;              // x might be null
            }
            else if (y->right_ == 0)        // z has exactly one non-null child; y == z
            {
                x = y->left_;               // x is not null
            }
            else                            // z has two non-null children; set y to
            {
                y = y->right_;              //   z's successor; x might be null
                while (y->left_ != 0) y = y->left_;
                x = y->right_;
            }

            y->update_erase(root);          // Update indexes if necessary
            if (y != this)                  // relink y in place of z; y is z's successor
            {
                left_->parent_ = y;
                y->left_ = left_;
                if (y != right_)
                {
                    x_parent = y->parent_;
                    if (x) x->parent_ = y->parent_;
                    y->parent_->left_ = x;  // y must be a child of left_
                    y->right_ = right_;
                    right_->parent_ = y;
                }
                else
                {
                    x_parent = y;
                }
                if (root == this)
                {
                    root = y;
                }
                else if (parent_->left_ == this)
                {
                    parent_->left_ = y;
                }
                else
                {
                    parent_->right_ = y;
                }
                y->parent_ = parent_;
                std::swap(y->color_, color_);
                y = this;                   // y now points to node to be actually deleted
            }
            else                            // y == z
            {
                x_parent = y->parent_;
                if (x) x->parent_ = y->parent_;
                if (root == this)
                {
                    root = x;
                }
                else
                {
                    if (parent_->left_ == this)
                    {
                        parent_->left_ = x;
                    }
                    else
                    {
                        parent_->right_ = x;
                    }
                }
                if (leftmost == this)
                {
                    if (right_ == 0)     // left_ must be null also
                    {
                        assert(left_ == 0);
                        leftmost = parent_;
                        // makes leftmost == header if z == root
                    }
                    else
                    {
                        leftmost = x->minimum();
                    }
                }
                if (rightmost == this)
                {
                    if (left_ == 0)     // right_ must be null also
                    {
                        assert(right_ == 0);
                        rightmost = parent_;
                        // makes rightmost() == header_ if z == root()
                    }
                    else
                    {                // x == left_
                        rightmost = x->maximum();
                    }
                }
            }

            if (y->color_ != red)                // rebalance
            {
                while (x != root && (x == 0 || x->color_ == black))
                {
                    if (x == x_parent->left_)
                    {
                        node_base* w(x_parent->right_);
                        if (w->color_ == red)
                        {
                            w->color_ = black;
                            x_parent->color_ = red;
                            x_parent->rotate_left(header);
                            w = x_parent->right_;
                        }
                        if ((w->left_ == 0 || w->left_->color_ == black) &&
                            (w->right_ == 0 || w->right_->color_ == black))
                        {
                            w->color_ = red;
                            x = x_parent;
                            x_parent = x_parent->parent_;
                        }
                        else
                        {
                            if (w->right_ == 0 || w->right_->color_ == black)
                            {
                                if (w->left_) w->left_->color_ = black;
                                w->color_ = red;
                                w->rotate_right(header);
                                w = x_parent->right_;
                            }
                            w->color_ = x_parent->color_;
                            x_parent->color_ = black;
                            if (w->right_) w->right_->color_ = black;
                            x_parent->rotate_left(header);
                            break;
                        }
                    }
                    else            // same as above, with right_ <-> left_
                    {
                        node_base* w(x_parent->left_);
                        if (w->color_ == red)
                        {
                            w->color_ = black;
                            x_parent->color_ = red;
                            x_parent->rotate_right(header);
                            w = x_parent->left_;
                        }
                        if ((w->right_ == 0 || w->right_->color_ == black) &&
                            (w->left_ == 0 || w->left_->color_ == black))
                        {
                            w->color_ = red;
                            x = x_parent;
                            x_parent = x_parent->parent_;
                        }
                        else
                        {
                            if (w->left_ == 0 || w->left_->color_ == black)
                            {
                                if (w->right_) w->right_->color_ = black;
                                w->color_ = red;
                                w->rotate_left(header);
                                w = x_parent->left_;
                            }
                            w->color_ = x_parent->color_;
                            x_parent->color_ = black;
                            if (w->left_) w->left_->color_ = black;
                            x_parent->rotate_right(header);
                            break;
                        }
                    }
                }
                if (x) x->color_ = black;
            }

            return y;
        }
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // DBH_2001_10_31_TREE_HPP

