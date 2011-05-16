#include <iostream>
#include <boost/thread.hpp>
#define BOOST_BP_RAW_POINTER
#include <boost/block_ptr.hpp>
//#include <boost/thread/mutex.hpp>

using namespace std;
using namespace boost;


block_ptr< pair<int, int> > p;


void worker(int id)  
{         
    std::cout << "Worker: running" << std::endl;  
       
    for (int i = 0; i < 100000; ++ i)
    {
       	cout << id << "-" << i << ", " << flush;
    	p = new block< pair<int, int> >(make_pair(id, i));
    }
    cout << endl;
       
    std::cout << "Worker: finished" << std::endl;  
}  
   
int main(int argc, char* argv[])  
{  
    std::cout << "main: startup" << std::endl;  
    
   	boost::thread t0(worker, 0);  
   	boost::thread t1(worker, 1);  
   	boost::thread t2(worker, 2);  
   	boost::thread t3(worker, 3);  
       
    std::cout << "main: waiting for thread" << std::endl;  
       
    t0.join();  
    t1.join();  
    t2.join();  
    t3.join();  
       
    std::cout << "main: done" << std::endl;  
       
    return 0;  
}  
