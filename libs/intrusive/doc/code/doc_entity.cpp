//[doc_entity_code
#include <boost/intrusive/ilist.hpp>

//A class that can be inserted in an intrusive list
class entity 
   :  public boost::intrusive::ilist_base_hook<0, true>
{
   public:
   virtual ~entity();
   //...
};

//"some_entity" derives from "entity"
class some_entity  :  public entity
{/**/};

//Definition of the intrusive list
typedef boost::intrusive::ilist< entity::value_traits<entity> > entity_list;

//A global list
entity_list list;

//The destructor removes itself from the list
entity::~entity()
{  list.erase(entity_list::current(*this));  }

//Function to insert a new "some_entity" in the list
void insert_some_entity()
{  list.push_back (*new some_entity(/*...*/));  } 

//Funtion to clear an entity from the intrusive list
void clear_list ()
{
   // entity's destructor removes itself from the list implicitly
   while (!list.empty())
      delete &list.front();	
}

int main()
{
   //Insert some new entities
   insert_some_entity();
   insert_some_entity();
   //list's destructor will free objects
   return 0;
}

//]
