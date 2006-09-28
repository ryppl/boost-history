export MM { 
public: 
  import M1;  // Make exported names from M1 visible 
              // visible here and in code that imports 
              // module MM. 
private: 
  import M2;  // Make M2 visible here, but not in 
              // not in clients. 
} ;

