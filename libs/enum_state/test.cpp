/*
  Test program to verify State<>.
*/

#include <iostream>
#include <cassert>
#include <boost/enum_state.hpp>
using namespace std;

enum myStates
    {
    read      = 3,
    parse     = 4,
    calculate = 5,
    write     = 6
    };
struct enum_traits<myStates> { enum { min = read, max = write, init = read }; };

int main()
try
    {
    // Construct bound-checked State instance.

    State<myStates> state;
    assert(state.is_bound_checked);
    assert(state == read);

    assert(++state == parse);

    assert(state++ == parse);
    assert(state == calculate);

    ++state;
    assert(state == write);

    try
        {
        ++state;
        assert(false);
        }
    catch(const out_of_range&) { }

    state = parse + 2;
    assert(state == write);

    try
        {
        state = parse + 4;
        assert(false);
        }
    catch(const out_of_range&) { }

    state = read;
    state += 2;
    assert(state == calculate);

    state -= 1;
    assert(state == parse);

    try
        {
        state -= 5;
        assert(false);
        }
    catch(const out_of_range&) { }

    cout << "All State<> tests succeeded." << endl;

    return 0;
    }
catch(const exception& e)
    {
    cerr << "Caught exception: " << e.what() << endl;
    return 1;
    }
