//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright David Abrahams, Vicente Botet, Ion Gaztanaga 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/move_semantics/move.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include "../example/movable.hpp"

int main()
{
   namespace ipc = ::boost::interprocess;
   //Default construct 10 movable objects
   ipc::vector<movable> v(10);

   //Test default constructed value
   if(v[0].moved()){
      return 1;
   }

   //Move values
   ipc::vector<movable> v2;
   std::copy(v.begin(), v.end(), boost::back_move_inserter(v2));

   //Test values have been moved
   if(!v[0].moved()){
      return 1;
   }

   if(v2.size() != 10){
      return 1;
   }

   if(v2[0].moved()){
      return 1;
   }

   return 0;
}
