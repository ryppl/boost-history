/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_NODE_HPP
#define BOOST_INDEXED_SET_INDEX_NODE_HPP

#include <algorithm>
#include <cstddef>

namespace boost{

namespace indexed_sets{

namespace detail{

/* definition of red-black nodes for indexed_set::index */

enum index_color{red=false,black=true};

struct index_node_impl
{
  index_color&            color(){return color_;}
  const index_color&      color()const{return color_;}
  index_node_impl*&       parent(){return parent_;}
  index_node_impl*const & parent()const{return parent_;}
  index_node_impl*&       left(){return left_;}
  index_node_impl*const & left()const{return left_;}
  index_node_impl*&       right(){return right_;}
  index_node_impl*const & right()const{return right_;}

  /* interoperability with index_iterator */

  static void increment(index_node_impl*& x)
  {
    if(x->right()){
      x=x->right();
      while(x->left())x=x->left();
    }
    else{
      index_node_impl* y=x->parent();
      while(x==y->right()){
        x=y;
        y=y->parent();
      }
      if(x->right()!=y)x=y;
    }
  }

  static void decrement(index_node_impl*& x)
  {
    if(x->color()==red&&x->parent()->parent()==x){
      x=x->right();
    }
    else if(x->left()){
      index_node_impl* y=x->left();
      while(y->right())y=y->right();
      x=y;
    }else{
      index_node_impl* y=x->parent();
      while(x==y->left()){
        x=y;
        y=y->parent();
      }
      x=y;
    }
  }

  /* interoperability with index_proxy */

  static index_node_impl* begin(index_node_impl* header){return header->left();}
  static index_node_impl* end(index_node_impl* header){return header;}

  /* algorithmic stuff */

  static void rotate_left(index_node_impl* x,index_node_impl*& root)
  {
    index_node_impl* y=x->right();
    x->right()=y->left();
    if(y->left())y->left()->parent()=x;
    y->parent()=x->parent();
    
    if(x==root)                    root=y;
    else if(x==x->parent()->left())x->parent()->left()=y;
    else                           x->parent()->right()=y;
    y->left()=x;
    x->parent()=y;
  }

  static index_node_impl* minimum(index_node_impl* x)
  {
    while(x->left())x=x->left();
    return x;
  }

  static index_node_impl* maximum(index_node_impl* x)
  {
    while(x->right())x=x->right();
    return x;
  }

  static void rotate_right(index_node_impl* x,index_node_impl*& root)
  {
    index_node_impl* y=x->left();
    x->left()=y->right();
    if(y->right())y->right()->parent()=x;
    y->parent()=x->parent();

    if(x==root)                     root=y;
    else if(x==x->parent()->right())x->parent()->right()=y;
    else                            x->parent()->left()=y;
    y->right()=x;
    x->parent()=y;
  }

  static void rebalance(index_node_impl* x,index_node_impl*& root)
  {
    x->color()=red;
    while(x!=root&&x->parent()->color()==red){
      if(x->parent()==x->parent()->parent()->left()){
        index_node_impl* y=x->parent()->parent()->right();
        if(y&&y->color()==red){
          x->parent()->color()=black;
          y->color()=black;
          x->parent()->parent()->color()=red;
          x=x->parent()->parent();
        }
        else{
          if(x==x->parent()->right()){
            x=x->parent();
            rotate_left(x,root);
          }
          x->parent()->color()=black;
          x->parent()->parent()->color()=red;
          rotate_right(x->parent()->parent(),root);
        }
      }
      else{
        index_node_impl* y=x->parent()->parent()->left();
        if(y&&y->color()==red){
          x->parent()->color()=black;
          y->color()=black;
          x->parent()->parent()->color()=red;
          x=x->parent()->parent();
        }
        else{
          if(x==x->parent()->left()){
            x=x->parent();
            rotate_right(x,root);
          }
          x->parent()->color()=black;
          x->parent()->parent()->color()=red;
          rotate_left(x->parent()->parent(),root);
        }
      }
    }
    root->color()=black;
  }

  static index_node_impl* rebalance_for_erase(
    index_node_impl* z,index_node_impl*& root,
    index_node_impl*& leftmost,index_node_impl*& rightmost)
  {
    index_node_impl* y=z;
    index_node_impl* x=0;
    index_node_impl* x_parent=0;
    if(y->left()==0){        /* z has at most one non-null child. y==z. */
      x=y->right();          /* x might be null */
    }
    else{
      if(y->right()==0)  {     /* z has exactly one non-null child. y==z. */
        x=y->left();           /* x is not null */
      }
      else{                    /* z has two non-null children.  Set y to */
        y=y->right();          /*   z's successor.  x might be null.     */
        while(y->left())y=y->left();
        x=y->right();
      }
    }
    if(y!=z){
      z->left()->parent()=y;   /* relink y in place of z.  y is z's successor */
      y->left()=z->left();
      if(y!=z->right()){
        x_parent=y->parent();
        if(x) x->parent()=y->parent();
        y->parent()->left()=x; /* y must be a child of left */
        y->right()=z->right();
        z->right()->parent()=y;
      }
      else{
        x_parent=y;
      }

      if(root==z)                    root=y;
      else if(z->parent()->left()==z)z->parent()->left()=y;
      else                           z->parent()->right()=y;
      y->parent()=z->parent();
      std::swap(y->color(),z->color());
      y=z;                     /* y now points to node to be actually deleted */
    }
    else{                      /* y==z */
      x_parent=y->parent();
      if(x)x->parent()=y->parent();   
      if(root==z){
        root=x;
      }
      else{
        if(z->parent()->left()==z)z->parent()->left()=x;
        else                      z->parent()->right()=x;
      }
      if(leftmost==z){
        if(z->right()==0){      /* z->left() must be null also */
          leftmost=z->parent();
        }
        else{              
          leftmost=minimum(x);  /* makes leftmost==header if z==root */
        }
      }
      if(rightmost==z){
        if(z->left()==0){       /* z->right() must be null also */
          rightmost=z->parent();
        }
        else{                   /* x==z->left() */
          rightmost=maximum(x); /* makes rightmost==header if z==root */
        }
      }
    }
    if(y->color()!=red){
      while(x!=root&&(x==0 || x->color()==black)){
        if(x==x_parent->left()){
          index_node_impl* w=x_parent->right();
          if(w->color()==red){
            w->color()=black;
            x_parent->color()=red;
            rotate_left(x_parent,root);
            w=x_parent->right();
          }
          if((w->left()==0||w->left()->color()==black) &&
             (w->right()==0||w->right()->color()==black)){
            w->color()=red;
            x=x_parent;
            x_parent=x_parent->parent();
          } 
          else{
            if(w->right()==0 
                || w->right()->color()==black){
              if(w->left()) w->left()->color()=black;
              w->color()=red;
              rotate_right(w,root);
              w=x_parent->right();
            }
            w->color()=x_parent->color();
            x_parent->color()=black;
            if(w->right())w->right()->color()=black;
            rotate_left(x_parent,root);
            break;
          }
        } 
        else{                   /* same as above,with right <-> left */
          index_node_impl* w=x_parent->left();
          if(w->color()==red){
            w->color()=black;
            x_parent->color()=red;
            rotate_right(x_parent,root);
            w=x_parent->left();
          }
          if((w->right()==0||w->right()->color()==black) &&
             (w->left()==0||w->left()->color()==black)){
            w->color()=red;
            x=x_parent;
            x_parent=x_parent->parent();
          }
          else{
            if(w->left()==0||w->left()->color()==black){
              if(w->right())w->right()->color()=black;
              w->color()=red;
              rotate_left(w,root);
              w=x_parent->left();
            }
            w->color()=x_parent->color();
            x_parent->color()=black;
            if(w->left())w->left()->color()=black;
            rotate_right(x_parent,root);
            break;
          }
        }
      }
      if(x)x->color()=black;
    }
    return y;
  }

  static void restore(
    index_node_impl* x,index_node_impl* prior,index_node_impl* next,index_node_impl* header)
  {
    if(next==header){
      header->parent()=x;
      header->left()=x;
      header->right()=x;
      x->parent()=header;
    }
    else if(next->left()==0){
      next->left()=x;
      x->parent()=next;
      if(next==header->left()){
        header->left()=x;            /* maintain leftmost pointing to min node */
      }
    }
    else{                            /* prior->right() must be null */
      prior->right()=x;
      x->parent()=prior;
      if(prior==header->right()){
        header->right()=x;           /* maintain rightmost pointing to max node */
      }
    }
    x->left()=0;
    x->right()=0;
    rebalance(x,header->parent());
  }

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  static std::size_t black_count(index_node_impl* node,index_node_impl* root)
  {
    if(!node)return 0;
    std::size_t sum=0;
    for(;;){
      if(node->color()==black)++sum;
      if(node==root)break;
      node=node->parent();
    } 
    return sum;
  }
#endif

private:
  index_node_impl();

  index_color      color_; 
  index_node_impl* parent_;
  index_node_impl* left_;
  index_node_impl* right_;
};

template<typename Super> struct index_node_trampoline:index_node_impl{};

template<typename Super>
struct index_node:Super,index_node_trampoline<Super>
{
  index_color&            color(){return impl_type::color();}
  const index_color&      color()const{return impl_type::color();}
  index_node_impl*&       parent(){return impl_type::parent();}
  index_node_impl*const & parent()const{return impl_type::parent();}
  index_node_impl*&       left(){return impl_type::left();}
  index_node_impl*const & left()const{return impl_type::left();}
  index_node_impl*&       right(){return impl_type::right();}
  index_node_impl*const & right()const{return impl_type::right();}

  index_node_impl* impl(){return static_cast<impl_type*>(this);}
  const index_node_impl* impl()const{return static_cast<const impl_type*>(this);}

  static index_node* from_impl(index_node_impl *x)
    {return static_cast<index_node*>(static_cast<impl_type*>(x));}
  static const index_node* from_impl(const index_node_impl* x)
    {return static_cast<const index_node*>(static_cast<const impl_type*>(x));}

  /* interoperability with index_iterator */

  static void increment(index_node*& x)
  {
    index_node_impl* xi=x->impl();
    impl_type::increment(xi);
    x=from_impl(xi);
  }

  static void decrement(index_node*& x)
  {
    index_node_impl* xi=x->impl();
    impl_type::decrement(xi);
    x=from_impl(xi);
  }

  /* interoperability with index_proxy */

  static index_node* begin(index_node* header)
  {
    return from_impl(impl_type::begin(header->impl()));
  }

  static index_node* end(index_node* header)
  {
    return from_impl(impl_type::end(header->impl()));
  }

private:
  typedef index_node_trampoline<Super> impl_type;
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
