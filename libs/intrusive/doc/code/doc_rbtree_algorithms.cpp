//[doc_rbtree_algorithms_code
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <cassert>

struct my_node
{
   my_node(int i = 0)
      :  int_(i)
   {}
   my_node *parent_;
   my_node *left_;
   my_node *right_;
   int      color_;
   //other members
   int      int_;
};

//Define our own rbtree_node_traits
struct my_rbtree_node_traits
{
   typedef my_node node;
   typedef void * void_pointer;
   typedef int color;

   static node *get_parent(const node *n)
   {  return n->parent_;  }  

   static void set_parent(node *n, node *parent)
   {  n->parent_ = parent;  }  

   static node *get_left(const node *n)
   {  return n->left_;  }  

   static void set_left(node *n, node *left)
   {  n->left_ = left;  }  

   static node *get_right(const node *n)
   {  return n->right_;  }  

   static void set_right(node *n, node *right)
   {  n->right_ = right;  }  

   static color get_color(const node *n)
   {  return n->color_;  }  

   static void set_color(node *n, color c)
   {  n->color_ = c;  }

   static color black()
   {  return color(0);  }  

   static color red()
   {  return color(1);  }  
};

struct node_ptr_compare
{
   bool operator()(my_node *a, my_node *b)
   {  return a->int_ < b->int_;  }
};

int main()
{
   typedef boost::intrusive::rbtree_algorithms<my_rbtree_node_traits> algo;

   my_node header;
   my_node two(2);
   my_node three(3);

   //Create an empty rbtree sequence:
   //"header" will be the header node of the tree
   algo::init_header(&header);

   //Now insert node "two" in the tree using the sorting functor
   algo::insert_equal_upper_bound(&header, &two, node_ptr_compare());

   //Now insert node "three" in the tree using the sorting functor
   algo::insert_equal_lower_bound(&header, &three, node_ptr_compare());

   //Now take the first node (the left node of the header)
   my_node *n = header.left_;
   assert(n == &two);

   //Now go to the next node
   n = algo::next_node(n);
   assert(n == &three);      

   //Erase a node just using a pointer to it
   algo::unlink_and_rebalance(n);

   //Erase a node using also the header (faster)
   algo::erase(&header, n);
   return 0;
}
//]
