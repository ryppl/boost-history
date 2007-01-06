
//[doc_ihashset_optimized_code_normal_find
   #include <boost/intrusive/ihashset.hpp>
   #include <boost/functional/hash.hpp>
   #include <string>

   class ExpensiveClass
      :  public boost::intrusive::ihashset_base_hook<0>
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

      friend bool operator == (const ExpensiveClass &a, const ExpensiveClass &b)
      {  return a.key_ == b.key_;  }

      //Other functions
   };

   //A hash function for c-strings
   struct CStringHasher
   {
      std::size_t operator()(const char *str) const
      {  
         std::size_t seed = 0;
         for(; *str; ++str)
            boost::hash_combine(seed, *str);
         return seed;
      }
   };

   //The hash function of ExpensiveClass (uses CStringHasher)
   //Now boost::hash<ExpensiveClass> will use this hash function
   std::size_t hash_value(const ExpensiveClass &object)
   {  return CStringHasher()(object.get_key().c_str());  }

   //A hashset that stores objects with expensive constructors
   typedef boost::intrusive::ihashset
      < boost::intrusive::ihashset_base_hook<0>::value_traits<ExpensiveClass> >
         ExpensiveClassHashSet;

   /* ...
      ...
      ... */

   //The normal get_object function
   ExpensiveClass *get_object
      (const char* key, ExpensiveClassHashSet &expensive_class_set)
   {
      //We must create an ExpensiveClass to find an equivalent one...
      ExpensiveClass find_this(key);
      ExpensiveClassHashSet::iterator it = expensive_class_set.find(find_this);
      
      if( it == expensive_class_set.end() ){
         return 0;
      }
      return &*it;
   }
   //]

   //[doc_ihashset_optimized_code_optimized_find

   //A functor to check if ExpensiveClass and a c-string are equal
   struct CStrExpensiveClassEqual
   {
      bool operator()(const char *str, const ExpensiveClass &c) const
      {  return std::strcmp(str, c.get_key().c_str()) == 0;  }

      bool operator()(const ExpensiveClass &c, const char *str) const
      {  return std::strcmp(c.get_key().c_str(), str) == 0;  }
   };

   //The optimized get_object function
   ExpensiveClass *get_object_optimized
      (const char* key, ExpensiveClassHashSet &expensive_class_set)
   {
      //Now find the value using just the key!
      ExpensiveClassHashSet::iterator it = 
         expensive_class_set.find
            ( key
            , CStringHasher()
            , CStrExpensiveClassEqual());
      
      if( it == expensive_class_set.end() ){
         return 0;
      }
      return &*it;
   }
   //]

   //[doc_ihashset_optimized_code_normal_insert

   //The usual way to insert a new object
   bool insert_unique_object
      (const char* key, ExpensiveClassHashSet &expensive_class_set)
   {
      //Construct an expensive object
      ExpensiveClass *pobject = new ExpensiveClass(key);

      std::pair<ExpensiveClassHashSet::iterator, bool> ret = 
         expensive_class_set.insert(*pobject);
      
      if(!ret.second){
         delete pobject;
      }
      return ret.second;
   }

   //]

   //[doc_ihashset_optimized_code_optimized_insert

   //The optimized way to insert a new object
   bool insert_unique_object_optimized
      (const char* key, ExpensiveClassHashSet &expensive_class_set)
   {
      ExpensiveClassHashSet::insert_commit_data insert_data;

      std::pair<ExpensiveClassHashSet::iterator, bool> ret = 
         expensive_class_set.insert_check
            ( key
            , CStringHasher()
            , CStrExpensiveClassEqual()
            , insert_data);
      
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
   const std::size_t BucketLen = 10;
   ExpensiveClassHashSet::bucket_type buckets[BucketLen];
   ExpensiveClassHashSet expensive_hashset(buckets, BucketLen);

   const char * const expensive_key
      = "A long string that avoids small string optimization";

   ExpensiveClass value(expensive_key);

   if(get_object(expensive_key, expensive_hashset)){
      return 1;
   }

   if(get_object_optimized(expensive_key, expensive_hashset)){
      return 1;
   }

   ExpensiveClassHashSet::iterator it =  expensive_hashset.insert(value).first;

   if(!get_object(expensive_key, expensive_hashset)){
      return 1;
   }

   if(!get_object_optimized(expensive_key, expensive_hashset)){
      return 1;
   }

   expensive_hashset.erase(it);

   if(!insert_unique_object(expensive_key, expensive_hashset)){
      return 1;
   }

   {
      ExpensiveClass *ptr = &*expensive_hashset.begin();
      expensive_hashset.clear();
      delete ptr;
   }

   if(!insert_unique_object_optimized(expensive_key, expensive_hashset)){
      return 1;
   }

   {
      ExpensiveClass *ptr = &*expensive_hashset.begin();
      expensive_hashset.clear();
      delete ptr;
   }

   it =  expensive_hashset.insert(value).first;

   if(insert_unique_object(expensive_key, expensive_hashset)){
      return 1;
   }

   if(insert_unique_object_optimized(expensive_key, expensive_hashset)){
      return 1;
   }

   expensive_hashset.erase(value);

   return 0;
}
