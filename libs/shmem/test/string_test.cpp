//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/containers/vector.hpp>
#include <boost/shmem/containers/string.hpp>
#include <boost/shmem/offset_ptr.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <cstddef>
#include <new>

using namespace boost::shmem;

//Explicit instantiation of a shmem::vector of shmem::strings
typedef allocator<char, named_shared_object::segment_manager> ShmemAllocatorChar;
typedef basic_string<char, std::char_traits<char>, ShmemAllocatorChar> ShmString;
typedef allocator<ShmString, named_shared_object::segment_manager> ShmemVectorAllocator;
typedef vector<ShmString, ShmemVectorAllocator> ShmStringVector;
//template class vector<ShmString, ShmemVectorAllocator>;

//Explicit instantiation of a std::vector of std::strings
typedef std::allocator<char>  StdAllocatorChar;
typedef std::basic_string<char, std::char_traits<char>, StdAllocatorChar> StdString;
typedef std::allocator<StdString> StdVectorAllocator;
typedef vector<StdString, StdVectorAllocator> StdStringVector;
//template class vector<StdString, StdVectorAllocator>;

struct StringEqual
{
   bool operator ()(const StdString &stdstring, const ShmString &shmstring) const
   {
      return strcmp(stdstring.c_str(), shmstring.c_str()) == 0; 
   }

   bool operator ()(const ShmString &shmstring, const StdString &stdstring) const
   {
      return strcmp(stdstring.c_str(), shmstring.c_str()) == 0; 
   }
};

//Function to check if both lists are equal
bool CheckEqual(ShmStringVector *shmvect, StdStringVector *stdvect)
{
   return std::equal(stdvect->begin(), stdvect->end(), 
                     shmvect->begin(), StringEqual() );
}

int main ()
{
   const int MaxSize = 100;

   //Create shared memory
   named_shared_object segment;
   segment.create("/MySharedMemory",//segment name
                  65536);           //segment size in bytes
   
   ShmemAllocatorChar shmallocator (segment.get_segment_manager());

   //Initialize vector with a range or iterators and allocator
   ShmStringVector *shmStringVect = 
      segment.find_or_construct<ShmStringVector>
                                ("ShmStringVector", std::nothrow)  //object name 
                                (shmallocator);

   StdStringVector *stdStringVect = new StdStringVector;

   ShmString auxShmString (segment.get_segment_manager());
   StdString auxStdString;

   char buffer [20];

   int i;
   for(i = 0; i < MaxSize; ++i){
      auxShmString = "String";
      auxStdString = "String";
      sprintf(buffer, "%i", i);
      auxShmString += buffer;
      auxStdString += buffer;
      shmStringVect->push_back(auxShmString);
      stdStringVect->push_back(auxStdString);
   }

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   std::sort(shmStringVect->begin(), shmStringVect->end());
   std::sort(stdStringVect->begin(), stdStringVect->end());

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   const char prefix []    = "Prefix";
   const int  prefix_size  = sizeof(prefix)/sizeof(prefix[0])-1;
   const char sufix []     = "Suffix";

   for(i = 0; i < MaxSize; ++i){
      (*shmStringVect)[i].append(sufix);
      (*stdStringVect)[i].append(sufix);
      (*shmStringVect)[i].insert((*shmStringVect)[i].begin(), 
                                 prefix, prefix + prefix_size);
      (*stdStringVect)[i].insert((*stdStringVect)[i].begin(), 
                                 prefix, prefix + prefix_size);
   }

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   for(i = 0; i < MaxSize; ++i){
      std::reverse((*shmStringVect)[i].begin(), (*shmStringVect)[i].end());
      std::reverse((*stdStringVect)[i].begin(), (*stdStringVect)[i].end());
   }

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   for(i = 0; i < MaxSize; ++i){
      std::reverse((*shmStringVect)[i].begin(), (*shmStringVect)[i].end());
      std::reverse((*stdStringVect)[i].begin(), (*stdStringVect)[i].end());
   }

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   for(i = 0; i < MaxSize; ++i){
      std::sort(shmStringVect->begin(), shmStringVect->end());
      std::sort(stdStringVect->begin(), stdStringVect->end());
   }

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   for(i = 0; i < MaxSize; ++i){
      (*shmStringVect)[i].replace((*shmStringVect)[i].begin(), 
                                  (*shmStringVect)[i].end(),
                                  "String");
      (*stdStringVect)[i].replace((*stdStringVect)[i].begin(), 
                                  (*stdStringVect)[i].end(),
                                  "String");
   }

   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   shmStringVect->erase(std::unique(shmStringVect->begin(), shmStringVect->end()),
                        shmStringVect->end());
   stdStringVect->erase(std::unique(stdStringVect->begin(), stdStringVect->end()),
                        stdStringVect->end());
   if(!CheckEqual(shmStringVect, stdStringVect)) return -1;

   //When done, delete vector
   segment.destroy<ShmStringVector>("ShmStringVector");
   delete stdStringVect;

   return 0;
}
