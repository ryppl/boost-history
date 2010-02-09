//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TRANSACT_DETAIL_MUTEX_HPP
#define BOOST_TRANSACT_DETAIL_MUTEX_HPP

#include <boost/thread/mutex.hpp>

namespace boost{
namespace transact{
namespace detail{

struct null_mutex{
	void lock(){}
	bool try_lock(){ return true; }
	void unlock(){}
};

}
}
}



#ifdef BOOST_ATOMIC
#include <boost/atomic.hpp>
#include <boost/assert.hpp>

namespace boost{
namespace transact{
namespace detail{

class combined_mutex{
public:
	combined_mutex() : count(-1){
		this->smutex.lock();
	}
	void lock(){
		int old=this->count.fetch_add(1,boost::memory_order_acquire);
		if(old >= 0){
			try{
				this->smutex.lock();
			 }catch(...){
				--this->count;
				throw;
			}
		}
	}
	void unlock(){
		unsigned int old=this->count.fetch_sub(1,boost::memory_order_release);
		BOOST_ASSERT(old >= 0);
		if(old > 0) this->smutex.unlock();
	}
	~combined_mutex(){
		BOOST_ASSERT(this->count.load() == -1);
		this->smutex.unlock();
	}
private:
	boost::mutex smutex;
	boost::atomic<int> count;
};

	
typedef combined_mutex mutex_type;


}
}
}

#else


namespace boost{
namespace transact{
namespace detail{

typedef boost::mutex mutex_type;

}
}
}


#endif




#endif

