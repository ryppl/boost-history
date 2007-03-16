//[doc_list_algorithms_code
#include <boost/intrusive/list_algorithms.hpp>
#include <cassert>

struct my_node
{
   my_node *next_;
   my_node *prev_;
   //other members
};

//Define our own list_node_traits
struct my_list_node_traits
{
   typedef my_node node;
   typedef void * void_pointer;

   static node *get_next(const node *n)
   {  return n->next_;  }  

   static void set_next(node *n, node *next)
   {  n->next_ = next;  }  

   static node *get_previous(const node *n)
   {  return n->prev_;  }  

   static void set_previous(node *n, node *prev)
   {  n->prev_ = prev;  }  
};

int main()
{
   typedef boost::intrusive::list_algorithms<my_list_node_traits> algo;

   my_node one;
   my_node two;
   my_node three;

   //Create an empty doubly linked list sequence:
   //"one" will be the first node of the circular sequence
   algo::init(&one);
   assert(algo::count(&one) == 1);

   //Now add a new node before "one"
   algo::link_before(&one, &two);
   assert(algo::count(&one) == 2);

   //Now add a new node after "two"
   algo::link_after(&two, &three);
   assert(algo::count(&one) == 3);

   //Now unlink the node after one
   algo::unlink(&three);
   assert(algo::count(&one) == 2);

   //Now unlink two
   algo::unlink(&two);
   assert(algo::count(&one) == 1);

   //Now unlink one
   algo::unlink(&one);
   assert(algo::count(&one) == 1);

   return 0;
}
//]
