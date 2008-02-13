/*
 * Boost.Reflection / Hello World example - Tutorial 1
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#include <iostream>

#include <boost/reflection/reflection.hpp>
#include <boost/reflection/reflector.hpp>

using boost::reflections::reflection;
using boost::reflections::reflector;
using boost::reflections::instance;

reflection GetReflection();

int main(int argc, char ** argv) {
  reflection r = GetReflection();

  // Make sure this class supports a default constructor, and a function
  // identified as "print hello world" that has a void return value and
  // takes no arguments. If we wanted a constructor that took one int argument,
  // the call would become r.has_constructor<int>(). If we wanted a function
  // that took two float arguments, and returned a double, the call would be
  // r.has_function<void, float, float>("print hello world").
  if (r.get_constructor().valid() and
      r.get_function<void>("print hello world").valid()) {

    // Get and call the constructor to create an instance.
    instance i = r.get_constructor()();

    // Get and call the function called "print hello world".
    r.get_function<void>("print hello world")(i);
  } else {
    std::cerr << "Unable to find a required method.";
  }
}

class HelloWorld {
public:
  void printHelloWorld() {
    std::cout << "Hello World!" << std::endl;
  }
};

reflection GetReflection() {
  // This reflection will be returned
  reflection r;

  // Use a reflector<HelloWorld> which will only reflect out constructors
  // and functions for the HelloWorld class.
  reflector<HelloWorld> current_reflector(&r);

  // Reflect an argless constructor
  // To reflect a constructor that takes a "const string&" and an int,
  // call current_reflector.reflect_constructor<const string&, int>().
  current_reflector.reflect_constructor();

  // Reflect a member function returning void and having no parameters.
  // To reflect a member returning a float and taking a single int as
  // an argument, use: current_reflector.reflect<float, int>().
  current_reflector.reflect<void>(&HelloWorld::printHelloWorld,
                                  "print hello world");
  return r;
}