export Err { 
public: 
  struct S { int f() {} };  // Public f. 
  struct C { int f(); };    // Private f. 
  int C::f() {}  // C::f must be visible for parsing. 
  struct D: S, C { 
    void g() { 
      f();  // Error: Ambiguous. 
    } 
  }; 
} ;

