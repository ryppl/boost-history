//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <iostream>

using namespace std;

extern void conv_example();
extern void trim_example();
extern void predicate_example();
extern void find_example();
extern void replace_example();
extern void rle_example();

int main()
{
    conv_example();
    trim_example();
    predicate_example();
    find_example();
    replace_example();
    rle_example();

    cout << "Done." << endl;
    cin.get();

    return 0;
}
