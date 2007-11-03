// test_ts_resource.cpp : Defines the entry point for the console application.
//

/* 
    I have a vector, that is modified by different threads.

    Every once in a while I take a snapshot of this vector. Different threads reading this, they will be equal to the last snapshot, or
    the snapshot took before. I repeat this several times, to see that what I write to the vector, really propagates.
    
*/

// make a scenario where every X seconds, we re-load a certain configuration;
// like a std::vector or something

/

int main()
{
	return 0;
}

