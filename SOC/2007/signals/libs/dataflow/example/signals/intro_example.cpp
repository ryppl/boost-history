// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/connection.hpp>

using namespace boost;

//[ signal_void_counter
class SignalVoidCounter : public signals::consumer<SignalVoidCounter>
{
    int cnt;
public:
    SignalVoidCounter() : cnt(0) {}
    void operator()()
    {
        cnt++; // whenever a void() signal is received, increase the counter
    }
    int GetCount()
    {
        return cnt;
    }
}; // end class SignalVoidCounter
//]

//[ fused_signal_void_counter
class FusedSignalVoidCounter : public signals::consumer<FusedSignalVoidCounter>
{
    int cnt;
public:    
    FusedSignalVoidCounter() : cnt(0) {}
    void operator()(const fusion::vector<> &)
    {
        cnt++; // whenever a void(const fusion::vector<> &) signal is received, increase the counter
    }
    int GetCount()
    {
        return cnt;
    }
}; // end class SignalVoidCounter
//]

int main(int, char* [])
{
{
//[ signal_void_counter_main
    SignalVoidCounter counter;
    signals::storage<void()> storage;

    storage >>= counter;
    for (int i=0; i<33; i++)
        invoke(storage);

    assert(counter.GetCount() == 33);
//]
}
{
//[ fused_signal_void_counter_main
    FusedSignalVoidCounter counter;
    signals::storage<void(), signals::output::fused> storage;

    storage >>= counter;
    for (int i=0; i<33; i++)
        invoke(storage);

    assert(counter.GetCount() == 33);
//]
}
    return 0;
}


