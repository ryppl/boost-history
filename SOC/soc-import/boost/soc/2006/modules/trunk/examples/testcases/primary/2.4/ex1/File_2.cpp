import Lib;  // Private members invisible. 
struct D: Lib::S, Lib::C { 
  void g() { 
    f();  // Not ambiguous: Calls S::f. 
  } 
}; 

