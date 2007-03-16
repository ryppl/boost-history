//[doc_iset_optimized_code_normal_find
   #include <boost/intrusive/iset.hpp>
   #include <string>

   class ExpensiveClass
      :  public boost::intrusive::iset_base_hook<0>
   {
      std::string key_;
      //Other members
      // ...
      
      public:
      ExpensiveClass(const char *key)
         :  key_(key)
      {
         //other expensive initializations
      }

      const std::string & get_key() const
      {  return key_;   }

      friend bool operator < (const ExpensiveClass &a, const ExpensiveClass &b)
      {  return a.key_ < b.key_;  }

      //Other functions
   };

   //A set that stores objects with expensive constructors
   typedef boost::intrusive::iset
      < boost::intrusive::iset_base_hook<0>::value_traits<ExpensiveClass> >
         ExpensiveClassSet;
   
   /* ...
      ...
      ... */

   //The normal get_object function
   ExpensiveClass *get_object
      (const char* key, ExpensiveClassSet &expensive_class_set)
   {
      //We must create an ExpensiveClass to find an equivalent one...
      ExpensiveClass find_this(key);
      ExpensiveClassSet::iterator it = expensive_class_set.find(find_this);
      
      if( it == expensive_class_set.end() ){
         return 0;
      }
      return &*it;
   }
   //]

   //[doc_iset_optimized_code_optimized_find

   //A functor to compare ExpensiveClass and a c-string
   struct CStrExpensiveClassCompare
   {
      bool operator()(const char *str, const ExpensiveClass &c) const
      {  return std::strcmp(str, c.get_key().c_str()) < 0;  }

      bool operator()(const ExpensiveClass &c, const char *str) const
      {  return std::strcmp(c.get_key().c_str(), str) < 0;  }
   };

   //The optimized get_object function
   ExpensiveClass *get_object_optimized
      (const char* key, ExpensiveClassSet &expensive_class_set)
   {
      //Now find the value using just the key!
      ExpensiveClassSet::iterator it = 
         expensive_class_set.find(key, CStrExpensiveClassCompare());
      
      if( it == expensive_class_set.end() ){
         return 0;
      }
      return &*it;
   }
   //]

   //[doc_iset_optimized_code_normal_insert

   //The usual way to insert a new object
   bool insert_unique_object
      (const char* key, ExpensiveClassSet &expensive_class_set)
   {
      //Construct an expensive object
      ExpensiveClass *pobject = new ExpensiveClass(key);

      std::pair<ExpensiveClassSet::iterator, bool> ret = 
         expensive_class_set.insert(*pobject);
      
      if(!ret.second){
         delete pobject;
      }
      return ret.second;
   }

   //]

   //[doc_iset_optimized_code_optimized_insert

   //The optimized way to insert a new object
   bool insert_unique_object_optimized
      (const char* key, ExpensiveClassSet &expensive_class_set)
   {
      ExpensiveClassSet::insert_commit_data insert_data;

      std::pair<ExpensiveClassSet::iterator, bool> ret = 
         expensive_class_set.insert_check(key, CStrExpensiveClassCompare(), insert_data);
      
      if(ret.second){
         //Construct an expensive object
         ExpensiveClass *pobject = new ExpensiveClass(key);
         expensive_class_set.insert_commit(*pobject, insert_data);
      }
      return ret.second;
   }

   //]

int main()
{
   ExpensiveClassSet expensive_set;

   const char * const expensive_key
      = "A long string that avoids small string optimization";

   ExpensiveClass value(expensive_key);

   if(get_object(expensive_key, expensive_set)){
      return 1;
   }

   if(get_object_optimized(expensive_key, expensive_set)){
      return 1;
   }

   ExpensiveClassSet::iterator it =  expensive_set.insert(value).first;

   if(!get_object(expensive_key, expensive_set)){
      return 1;
   }

   if(!get_object_optimized(expensive_key, expensive_set)){
      return 1;
   }

   expensive_set.erase(it);

   if(!insert_unique_object(expensive_key, expensive_set)){
      return 1;
   }

   {
      ExpensiveClass *ptr = &*expensive_set.begin();
      expensive_set.clear();
      delete ptr;
   }

   if(!insert_unique_object_optimized(expensive_key, expensive_set)){
      return 1;
   }

   {
      ExpensiveClass *ptr = &*expensive_set.begin();
      expensive_set.clear();
      delete ptr;
   }

   it =  expensive_set.insert(value).first;

   if(insert_unique_object(expensive_key, expensive_set)){
      return 1;
   }

   if(insert_unique_object_optimized(expensive_key, expensive_set)){
      return 1;
   }

   expensive_set.erase(value);

   return 0;
}
