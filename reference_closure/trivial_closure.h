struct frame { };

inline void work( frame* )
{
}

#if REFERENCE_CLOSURE

void nested( frame* f ) { work(f); }

struct closure {
    void (*func)( frame* );
    frame* scope;
    void operator()() { func( scope ); }
};

#elif STD_FUNCTION

struct lambda
{
  frame* scope;
  void operator()() { work( scope ); }
};

#endif
