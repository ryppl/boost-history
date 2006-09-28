export Example2 { 
public: 
  template<typename T> struct S { 
    void f() {} 
  }; 
  class C { 
    friend void S<int>::f(); 
  }; 
};

export Lib {  // Module definition. 
  import std; 
public: 
  struct S { 
    S() { std::cout << "S()\n"; } 
  }; 
};

export MM { 
public: 
  import M1;  // Make exported names from M1 
                        // visible here and in code 
                        // that imports MM. 
private: 
  import M2;  // Make M2 visible here, but 
                        // not in clients. 
}; 

// File_1.cpp: 
export Lib { 
public: 
  struct S { void f() {} };  // Public f. 
  class C { void f() {} };   // Private f. 
}; 


