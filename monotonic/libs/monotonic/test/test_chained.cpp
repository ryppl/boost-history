#include <boost/monotonic/changed_storage.h>

void test_chained_storage()
{
	monotonic::chained_storage<10> store;
	void *p = store.allocate(20);
}

//EOF
