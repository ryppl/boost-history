// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>

template<class T>
class scheduler :  public signals::filter<delay, void (double)>
{
public:
    void operator(double x)
    {
        out(x);
    }
};

int main()
{
    using namespace boost::signals;
    
    storage<void(double)> source;
    
    source >>= 

    return 0;
}