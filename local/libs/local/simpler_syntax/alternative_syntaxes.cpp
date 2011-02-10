
Consider the following parenthesized syntax for local functions:

    (void) (f)( (int)(x) (double&)(y) (const bind)((a)(&b)) (bind)((&c)(d)) )

The followings are possible ways to simply this syntax.

DISCLAIMER: I still don't know if any of the following alternative syntaxes really works because I have not tried to implement them yet. This is just a paper excercise to write down possible options before I start looking into their implementation.


1) MERGING PARAM TYPE AND NAME (pure C++)

Parameter names can be passed to function types so I can transform `int x` into `int`:

    int x -[make function type]-> void (int x) -[get 1st parm type]-> int

Type should allow me to simplify the parenthesized syntax (*) to:
    
    (void) (f)( (int x) (double& y) (const bind)((a)(&b)) (bind)((&c)(d)) )

Note that the (unbound) parameter type and name are merged into a single sequence element.


2) 





