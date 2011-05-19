#include <sys/time.h>

#include <memory>
#include <iostream>
#include <boost/shared_ptr.hpp>
#define BOOST_BP_USE_RAW_POINTER
#include <boost/block_ptr.hpp>

using namespace std;
using namespace boost;

template <typename T, typename U>
	void worker()  
	{         
		T p;

	    for (int i = 0; i < 100000; ++ i)
	    	p.reset(new U);
	}  
   
timespec diff(timespec start, timespec end);

int main(int argc, char* argv[])  
{  
	timespec ts[2];
	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & ts[0]); 
	worker< auto_ptr<int>, int >();
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & ts[1]);
	cout << "auto_ptr:\t" << setw(numeric_limits<long>::digits10 + 2) << diff(ts[0], ts[1]).tv_nsec << " ns" << endl;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & ts[0]); 
	worker< shared_ptr<int>, int >();
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & ts[1]);
	cout << "shared_ptr:\t" << setw(numeric_limits<long>::digits10 + 2) << diff(ts[0], ts[1]).tv_nsec << " ns" << endl;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & ts[0]); 
	worker< block_ptr<int>, block<int> >();
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, & ts[1]);
	cout << "block_ptr:\t" << setw(numeric_limits<long>::digits10 + 2) << diff(ts[0], ts[1]).tv_nsec << " ns" << endl;

    return 0;  
}  

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
