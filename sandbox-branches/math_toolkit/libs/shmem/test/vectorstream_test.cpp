#include <boost/shmem/containers/string.hpp>
#include <boost/shmem/streams/vectorstream.hpp>
#include <sstream>
#include <string.h>
#include <vector>

using namespace boost::shmem;

//Force instantiations to catch compile-time errors
typedef basic_vectorstream<basic_string<char> > my_stringstream_t;
typedef basic_string<char> my_string;
template class basic_vectorstream<my_string>;
template class basic_vectorstream<std::vector<char> >;

static int vectorstream_test()
{
   //Pre-resized string
   {
      my_stringstream_t my_stringstream;
      std::stringstream std_stringstream;
      std::string str1, str2, str3("testline:");
      int number1, number2;

      my_stringstream.resize(10000);
      for(int i = 0; i < 100; ++i){
         my_stringstream  << "testline: " << i << std::endl;
         std_stringstream << "testline: " << i << std::endl;
      }

      if(strcmp(my_stringstream.vector().c_str(), std_stringstream.str().c_str()) != 0){
         return -1;
      }

      for(int i = 0; i < 100; ++i){
         my_stringstream  >> str1 >> number1;
         std_stringstream >> str2 >> number2;
         if((str1 != str2) || (str1 != str3)){
            assert(0); return -1;  
         }
         if((number1 != number2) || (number1 != i)){
            assert(0); return -1;   
         }
      }
   }
   //Pre-resized vector
   {
      basic_vectorstream<std::vector<char> > my_vectorstream;
      std::vector<char> myvector;
      std::stringstream std_stringstream;
      std::string str1, str2, str3("testline:");
      int number1, number2;

      my_vectorstream.resize(10000);
      for(int i = 0; i < 100; ++i){
         my_vectorstream  << "testline: " << i << std::endl;
         std_stringstream << "testline: " << i << std::endl;
      }
      //Add final null to form a c string
      myvector.push_back(0);
      if(strcmp(&(my_vectorstream.vector()[0]), std_stringstream.str().c_str()) != 0){
         return -1;
      }
      myvector.pop_back();
      for(int i = 0; i < 100; ++i){
         my_vectorstream  >> str1 >> number1;
         std_stringstream >> str2 >> number2;
         if((str1 != str2) || (str1 != str3)){
            assert(0); return -1;  
         }
         if((number1 != number2) || (number1 != i)){
            assert(0); return -1;   
         }
      }
   }

   //No pre-resized or pre-reserved string
   {
      my_stringstream_t my_stringstream;
      std::stringstream std_stringstream;
      std::string str1, str2, str3("testline:");
      int number1, number2;

      for(int i = 0; i < 100; ++i){
         my_stringstream  << "testline: " << i << std::endl;
         std_stringstream << "testline: " << i << std::endl;
      }
      if(strcmp(my_stringstream.vector().c_str(), std_stringstream.str().c_str()) != 0){
         assert(0);   return -1;
      }
      for(int i = 0; i < 100; ++i){
         my_stringstream  >> str1 >> number1;
         std_stringstream >> str2 >> number2;
         if((str1 != str2) || (str1 != str3)){
            assert(0); return -1;  
         }
         if((number1 != number2) || (number1 != i)){
            assert(0); return -1;   
         }
      }
   }
   return 0;
}

int main ()
{
   if(vectorstream_test()==-1){
      return -1;
   }
   return 0;
}
