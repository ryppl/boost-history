//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "named_creation_template.hpp"
#include "mutex_test_template.hpp"
#include <string>
#include "get_compiler_name.hpp"

using namespace boost::interprocess;

static const std::size_t SemCount      = 1;
static const std::size_t RecSemCount   = 100;
static const char *      SemName = test::get_compiler_name();

struct semaphore_deleter
{
   ~semaphore_deleter()
   {  named_semaphore::remove(SemName); }
};

//This wrapper is necessary to plug this class
//in named creation tests and interprocess_mutex tests
class named_semaphore_test_wrapper
   : public semaphore_deleter, public named_semaphore
{
   public:
   named_semaphore_test_wrapper()
      :  named_semaphore(open_or_create, SemName, SemCount)
   {}

   named_semaphore_test_wrapper(create_only_t)
      :  named_semaphore(create_only, SemName, SemCount)
   {}

   named_semaphore_test_wrapper(open_only_t)
      :  named_semaphore(open_only, SemName)
   {}

   named_semaphore_test_wrapper(open_or_create_t)
      :  named_semaphore(open_or_create, SemName, SemCount)
   {}

   ~named_semaphore_test_wrapper()
   {
      try{
         
      }
      catch(...){
      }
   }

   void lock()
   {  this->wait();  }

   bool try_lock()
   {  return this->try_wait();  }

   bool timed_lock(const boost::posix_time::ptime &pt)
   {  return this->timed_wait(pt);  }

   void unlock()
   {  this->post();  }

   protected:
   named_semaphore_test_wrapper(int initial_count)
      :  named_semaphore(create_only, SemName, initial_count)
   {}
};

//This wrapper is necessary to plug this class
//in recursive tests
class recursive_named_semaphore_test_wrapper
   :  public named_semaphore_test_wrapper
{
   public:
   recursive_named_semaphore_test_wrapper()
      :  named_semaphore_test_wrapper(RecSemCount)
   {}
};

int main ()
{
   try{
      named_semaphore::remove(SemName);
      test::test_named_creation<named_semaphore_test_wrapper>();
      test::test_all_lock<named_semaphore_test_wrapper>();
      test::test_all_recursive_lock<recursive_named_semaphore_test_wrapper>();
      test::test_all_mutex<false, named_semaphore_test_wrapper>();
   }
   catch(std::exception &ex){
      named_semaphore::remove(SemName);
      std::cout << ex.what() << std::endl;
      return 1;
   }
   named_semaphore::remove(SemName);
   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
