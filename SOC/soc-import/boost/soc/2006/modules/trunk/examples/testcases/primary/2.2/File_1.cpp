// File_1.cpp: 
export Lib { // Module definition header. 
             // Must precede all declarations. 
  import std; 
public: 
  struct S { 
    S() { std::cout << "S()\n"; } 
  }; 
} ;

