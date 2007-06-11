export Singletons { 
public: 
  struct Factory { 
    // ... 
  private: 
    Factory(Factory const&);  // Disable copying 
  }; 
  Factory factory; 
} ;

