/* 
    This tests using some functions on the destructor of a global object.

    The problem with using some resources within a destructor OF A GLOBAL OBJECT,
    is that they might reference other resources that have already been destroyed.

    An example (which might or might not work on your machine), is using a std::ofstream on the destructor of a global object.
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

void nothing() {}

// the test we're running
void (*g_test)() = nothing;

void run_test() {
    g_test();
    // if this gets executed, we're successful
    printf("test successful");
}

void use_ofstream() {
    std::ofstream out("out.txt");
    out << "test";
}

void use_ifstream() {
    std::ifstream in("test.cpp"); // read this file
    std::string line;
    in >> line;
}

void use_stringstream() {
    std::ostringstream out;
    out << "hello ";
    if ( std::cout)
        out << "world";
    printf ( out.str().c_str() );
}

void use_cout() {
    std::cout << "test write to cout " << std::endl;
}

void use_string() {
    std::string str;
    str += "hello ";
    if ( std::cout)
        str += "world";
    printf ( str.c_str() );
}



struct run_test_in_destructor {
    ~run_test_in_destructor() {
        run_test();
    }
} g_runner;

int main(int argc, char * argv[]) {
    std::string test_name = "nothing";
    if ( argc > 1 )
        test_name = argv[1];

    if ( test_name == "ofstream")
        g_test = use_ofstream;
    else if ( test_name == "ifstream")
        g_test = use_ifstream;
    else if ( test_name == "cout")
        g_test = use_cout;
    else if ( test_name == "stringstream")
        g_test = use_stringstream;
    else if ( test_name == "string")
        g_test = use_string;

    std::cout << "running test '" << test_name << "'" << std::endl;

    // use this, so that if there are any globals, they will be initialized here
    {
    std::ofstream out("out.txt");
    out << "test";
    }
    {
    std::ostringstream out;
    std::ifstream in("out.txt");
    in >> out.rdbuf();
    }
}


