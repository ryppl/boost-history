struct frame { };
void nested( frame* ) { }
struct closure {
    void (*func)( frame* );
    frame* scope;
    void operator()() { func( scope ); }
};
