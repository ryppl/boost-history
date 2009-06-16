
#include <boost/monotonic/storage.h>

void test_chained_storage()
{
	monotonic::storage<10> store;
	{
		typedef std::vector<char, monotonic::allocator<char> > Vector;
		Vector vec(store);
		vec.resize(5);		// still on the stack
		vec.resize(500);	// now on the heap
	}
}

//EOF
