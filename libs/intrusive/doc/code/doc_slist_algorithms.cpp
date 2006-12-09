   #include <boost/intrusive/slist_algorithms.hpp>
   #include <cassert>

   struct my_node
   {
      my_node *next_;
      //other members
   };

   //Define our own slist_node_traits
   struct my_slist_node_traits
   {
      typedef my_node node;
      typedef void * void_pointer;

      static node *get_next(const node *n)
      {  return n->next_;  }  

      static void set_next(node *n, node *next)
      {  n->next_ = next;  }  
   };

   int main()
   {
      typedef boost::intrusive::slist_algorithms<my_slist_node_traits> algo;

      my_node one;
      my_node two;
      my_node three;

      //Create an empty singly linked list sequence:
      //"one" will be the first node of the circular sequence
      algo::init(&one);
      assert(algo::count(&one) == 1);

      //Now add a new node
      algo::link_after(&two, &one);
      assert(algo::count(&one) == 2);

      //Now add a new node after "one"
      algo::link_after(&three, &one);
      assert(algo::count(&one) == 3);

      //Now unlink the node after one
      algo::unlink_after(&one);
      assert(algo::count(&one) == 2);

      //Now unlink two
      algo::unlink(&two);
      assert(algo::count(&one) == 1);

      return 0;
   }
