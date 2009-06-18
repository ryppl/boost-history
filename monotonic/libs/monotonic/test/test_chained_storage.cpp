
#include <boost/monotonic/storage.hpp>

void test_chained_storage()
{
	monotonic::local<monotonic::storage<0, 16> > store;
	{
		typedef std::vector<char, monotonic::allocator<char> > Vector;
		Vector vec;
		vec.resize(5);		// still on the stack
		vec.resize(32);		// now on the heap
		vec.resize(500);	// now on the heap
	}
}

//EOF
