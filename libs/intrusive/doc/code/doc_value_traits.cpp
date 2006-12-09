   #include <boost/intrusive/value_traits_type.hpp>
   #include <boost/intrusive/ilist.hpp>
   #include <vector>
   #include <cassert>

   //This node contains the needed data
   //for a custom doubly linked list
   struct my_node
   {
      my_node *next_;
      my_node *prev_;
   };

   //This is the type to be inserted in the
   //intrusive ilist container
   struct my_value
      :  public my_node
   {
      my_value(int i = 0)
         :  int_(i)
      {}

      int int_;
   };

   //Define our own list_node_traits that
   //contains information about my_node node
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

   //This type will tell ilist container how to
   //transform our my_value to my_node
   struct my_value_traits
   {
      typedef my_list_node_traits   node_traits;

      typedef my_value              value_type;

      enum {   type = boost::intrusive::normal_value_traits   };

      static node_traits::node * to_node_ptr (value_type& v)
      {  return static_cast<node_traits::node *>(&v); }

      static const node_traits::node * to_node_ptr (const value_type& v)
      {  return static_cast<const node_traits::node *>(&v); }

      static value_type * to_value_ptr(node_traits::node *n)
      {  return static_cast<value_type*>(n); }

      static const value_type * to_value_ptr(const node_traits::node *n)
      {  return static_cast<const value_type*>(n); }
   };

   //Now define an intrusive list that will store my_value objects
   typedef boost::intrusive::ilist<my_value_traits> MyIntrusiveList;

   int main()
   {
      typedef std::vector<my_value> Vect;
      typedef Vect::iterator VectIt;
      typedef Vect::reverse_iterator VectRit;

      //Create several my_value objects, each one
      //with a different internal number
      Vect myclassvector;
      for(int i = 0; i < 100; ++i)
         myclassvector.push_back(my_value(i));

      MyIntrusiveList mylist;

      //Now insert them in the reverse order
      //in the our intrusive list
      for(VectIt it(myclassvector.begin()), itend(myclassvector.end())
         ; it != itend
         ; ++it){
         mylist.push_front(*it);
      }

      {
         //Now test the list
         MyIntrusiveList::reverse_iterator rbit(mylist.rbegin()), rbitend(mylist.rend());
         VectIt  it(myclassvector.begin()), itend(myclassvector.end());

         //Test the objects inserted in our list
         for(; it != itend; ++it, ++rbit){
            if(&*rbit != &*it)
               return 1;
         }
      }

      return 0;
   }
