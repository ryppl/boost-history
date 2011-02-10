void BOOST_LOCAL_FUNCTION_PARAMS(
        (double& x) (auto int y)(default -1) (register int z)(default -2)
        (bind a) (bind& b) (const bind c) (const bind& d) ) {
    y = x;
} BOOST_LOCAL_FUNCTION_NAME(f)
 int x
double& y
 -1
1
1
 (bind a) (bind& b)
 (const bind c) (const bind& d)
